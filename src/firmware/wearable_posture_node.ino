/**
 * @file wearable_posture_node.ino
 * @brief Fabric-Electronics-Based Wearable Posture Sensing & Biofeedback Firmware
 * @target ESP8266 + MPU6050; physical board details require hardware records.
 * @scope Accelerometer-derived tilt; firmware does not characterize textile materials.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <math.h>

// ================= Networking Configuration =================
#include "posture_logic.h"
#if __has_include("network_config.private.h")
#include "network_config.private.h"
#else
#include "network_config.example.h"
#endif

ESP8266WebServer server(80);
WiFiClient tcpClient;
constexpr uint8_t IMU_ADDRESS = MPU6050_DEFAULT_ADDRESS;
MPU6050 mpu(IMU_ADDRESS);

// ================= Pin Assignments =================
const int PIN_STATUS_LED = 2; // Onboard / E-Textile PWM Indicator (Active LOW)

// ================= Kinematic State Variables =================
int16_t raw_ax, raw_ay, raw_az;
int16_t raw_gx, raw_gy, raw_gz;

float pitch = 0.0f;
float roll  = 0.0f;
float accel_magnitude = 1.0f; // in g units

bool posture_abnormal = false;

Posture::Feedback feedback;
bool imu_available = false, sample_valid = false;
unsigned long last_imu_attempt = 0, last_sample_ms = 0;
unsigned long last_telemetry_send = 0, last_tcp_attempt = 0;
IPAddress telemetry_address;
bool telemetry_address_valid = false;

// ================= Web Interface HTML =================
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Wearable Posture Sensing Node</title>
  <style>
    body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; background: #0B0F19; color: #E2E8F0; margin: 0; padding: 20px; text-align: center; }
    .card { background: #1E293B; border-radius: 12px; padding: 24px; max-width: 480px; margin: 20px auto; border: 1px solid #334155; box-shadow: 0 4px 20px rgba(0,0,0,0.5); }
    h1 { font-size: 20px; color: #38BDF8; margin-bottom: 8px; }
    .status { font-size: 14px; color: #94A3B8; margin-bottom: 24px; }
    .metric { display: flex; justify-content: space-between; padding: 12px 16px; border-bottom: 1px solid #334155; font-size: 15px; }
    .metric:last-child { border-bottom: none; }
    .val { font-weight: bold; color: #34D399; }
    .warn { color: #EF4444 !important; }
    .badge { display: inline-block; padding: 6px 16px; border-radius: 20px; font-weight: bold; font-size: 14px; margin-top: 16px; }
    .badge-ok { background: rgba(52, 211, 153, 0.2); color: #34D399; border: 1px solid #34D399; }
    .badge-alert { background: rgba(239, 68, 68, 0.2); color: #EF4444; border: 1px solid #EF4444; animation: pulse 1s infinite; }
    @keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.5; } }
  </style>
</head>
<body>
  <div class="card">
    <h1>Smart E-Textile Posture Monitor</h1>
    <div class="status">Node ID: ESP8266-TEX-01 | IMU: MPU6050</div>
    <div class="metric"><span>Pitch Angle (Inclination):</span><span id="pitch" class="val">--&deg;</span></div>
    <div class="metric"><span>Roll Angle (Lateral Tilt):</span><span id="roll" class="val">--&deg;</span></div>
    <div class="metric"><span>Normalized Acceleration:</span><span id="accel" class="val">-- g</span></div>
    <div id="posture-badge" class="badge badge-alert">WAITING FOR DATA</div>
  </div>
  <script>
    function unavailable(message) {
      for (const id of ['pitch', 'roll', 'accel']) document.getElementById(id).innerText = '--';
      const badge = document.getElementById('posture-badge');
      badge.className = 'badge badge-alert'; badge.innerText = message;
    }
    async function poll() {
      const abort = new AbortController();
      const timeout = setTimeout(() => abort.abort(), 1500);
      try {
        const response = await fetch('/api/data', {signal: abort.signal});
        if (!response.ok) throw new Error('HTTP error');
        const d = await response.json();
        if (!d.valid || ![d.pitch,d.roll,d.accel].every(Number.isFinite) || d.age_ms > 1000) {
          unavailable('IMU: DATA UNAVAILABLE');
        } else {
          document.getElementById('pitch').innerText = d.pitch.toFixed(1) + '°';
          document.getElementById('roll').innerText = d.roll.toFixed(1) + '°';
          document.getElementById('accel').innerText = d.accel.toFixed(2) + ' g';
          const badge = document.getElementById('posture-badge');
          badge.className = d.abnormal ? 'badge badge-alert' : 'badge badge-ok';
          badge.innerText = d.abnormal ? 'ALERT: TILT THRESHOLD' : 'POSTURE: WITHIN THRESHOLD';
        }
      } catch (e) { unavailable('CONNECTION LOST'); }
      finally { clearTimeout(timeout); setTimeout(poll, 200); }
    }
    poll();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleApiData() {
  String json = "{\"valid\":";
  json += sample_valid ? "true" : "false";
  json += ",\"abnormal\":"; json += posture_abnormal && sample_valid ? "true" : "false";
  json += ",\"pitch_threshold\":" + String(Posture::kPitchThreshold, 1);
  json += ",\"roll_threshold\":" + String(Posture::kRollThreshold, 1);
  json += ",\"age_ms\":" + String(millis() - last_sample_ms);
  json += ",\"pitch\":"; json += sample_valid ? String(pitch, 2) : "null";
  json += ",\"roll\":"; json += sample_valid ? String(roll, 2) : "null";
  json += ",\"accel\":"; json += sample_valid ? String(accel_magnitude, 2) : "null";
  json += "}";
  server.send(200, "application/json", json);
}

// ================= Kinematic Computation =================
void updateKinematics() {
  if (!imu_available) {
    if (millis() - last_imu_attempt < 2000) return;
    last_imu_attempt = millis();
    mpu.initialize(); imu_available = mpu.testConnection();
    if (!imu_available) return;
  }
  uint8_t bytes[14];
  if (I2Cdev::readBytes(IMU_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 14, bytes, 20) != 14) {
    imu_available = sample_valid = posture_abnormal = false;
    last_imu_attempt = millis(); return;
  }
  raw_ax = (int16_t)(((uint16_t)bytes[0] << 8) | bytes[1]);
  raw_ay = (int16_t)(((uint16_t)bytes[2] << 8) | bytes[3]);
  raw_az = (int16_t)(((uint16_t)bytes[4] << 8) | bytes[5]);
  raw_gx = (int16_t)(((uint16_t)bytes[8] << 8) | bytes[9]);
  raw_gy = (int16_t)(((uint16_t)bytes[10] << 8) | bytes[11]);
  raw_gz = (int16_t)(((uint16_t)bytes[12] << 8) | bytes[13]);

  // Convert raw acceleration to g-force (±2g range: 16384 LSB/g)
  float ax_g = (float)raw_ax / 16384.0f;
  float ay_g = (float)raw_ay / 16384.0f;
  float az_g = (float)raw_az / 16384.0f;

  accel_magnitude = sqrt(ax_g * ax_g + ay_g * ay_g + az_g * az_g);

  // Euler angle derivation via static accelerometer trigonometry
  pitch = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180.0f / M_PI;
  roll  = atan2(ay_g, az_g) * 180.0f / M_PI;

  sample_valid = isfinite(pitch) && isfinite(roll) && isfinite(accel_magnitude) && accel_magnitude > 0.05f;
  posture_abnormal = sample_valid && Posture::IsAbnormal(pitch, roll);
  if (sample_valid) last_sample_ms = millis();
}

// ================= Biofeedback Controller =================
void updateBiofeedback() {
  analogWrite(PIN_STATUS_LED, feedback.Update(millis(), posture_abnormal, sample_valid));
}

// ================= Telemetry Transmission =================
void streamTelemetry() {
  if (!sample_valid || !telemetry_address_valid || millis() - last_telemetry_send < 100) return;
  last_telemetry_send = millis();

  if (!tcpClient.connected()) {
    if (millis() - last_tcp_attempt < 2000) return;
    last_tcp_attempt = millis();
    if (!tcpClient.connect(telemetry_address, TELEMETRY_PORT)) return;
    tcpClient.setNoDelay(true);
  }
  if (tcpClient.connected()) {
    String packet = "PKT:P=" + String(pitch, 2) + ",R=" + String(roll, 2) + ",A=" + String(accel_magnitude, 2) + "\n";
    if (tcpClient.availableForWrite() >= (int)packet.length()) tcpClient.print(packet);
  }
}

void setup() {
  pinMode(PIN_STATUS_LED, OUTPUT);
  analogWriteRange(1023);
  analogWrite(PIN_STATUS_LED, 1023); // Off initially

  Serial.begin(115200);
  delay(500);
  Serial.println("\n[INIT] E-Textile Wearable Posture Sensing Node Booting...");

  // I2C Fast Mode (400kHz) on default ESP8266 pins (SDA=GPIO4, SCL=GPIO5)
  Wire.begin(4, 5);
  Wire.setClock(400000);
  Wire.setClockStretchLimit(20000);
  I2Cdev::readTimeout = 20;

  Serial.println("[IMU] Initializing InvenSense MPU6050...");
  mpu.initialize();
  imu_available = mpu.testConnection();
  last_imu_attempt = millis();
  if (imu_available) {
    Serial.println("[IMU] MPU6050 connection SUCCESSFUL");
  } else {
    Serial.println("[IMU] Warning: MPU6050 verification FAILED, retrying...");
  }

  tcpClient.setTimeout(50);
  telemetry_address_valid = telemetry_address.fromString(TELEMETRY_HOST);

  // Configure SoftAP & STA Dual Mode
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);
  WiFi.begin(STA_SSID, STA_PASS);
  Serial.print("[WIFI] SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

  // Web Server Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/data", HTTP_GET, handleApiData);
  server.begin();
  Serial.println("[HTTP] Web telemetry server started on port 80");
}

void loop() {
  updateKinematics();
  updateBiofeedback();
  server.handleClient();
  streamTelemetry();
  delay(10);
}
