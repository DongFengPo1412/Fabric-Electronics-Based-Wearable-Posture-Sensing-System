/**
 * @file wearable_posture_node.ino
 * @brief Fabric-Electronics-Based Wearable Posture Sensing & Biofeedback Firmware
 * @target Espressif ESP8266 (ESP-12F Wearable Module) + InvenSense MPU6050 6-Axis IMU
 * @interconnect Flexible Conductive Stainless/Silver Yarn Integrated on Textile Substrate
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <math.h>

// ================= Networking Configuration =================
const char* AP_SSID = "Posture_Garment_Node";
const char* AP_PASS = "12345678";
const char* STA_SSID = "ACE";
const char* STA_PASS = "66666666";

const char* TELEMETRY_HOST = "192.168.4.2";
const uint16_t TELEMETRY_PORT = 8080;

ESP8266WebServer server(80);
WiFiClient tcpClient;
MPU6050 mpu;

// ================= Pin Assignments =================
const int PIN_STATUS_LED = 2; // Onboard / E-Textile PWM Indicator (Active LOW)

// ================= Kinematic State Variables =================
int16_t raw_ax, raw_ay, raw_az;
int16_t raw_gx, raw_gy, raw_gz;

float pitch = 0.0f;
float roll  = 0.0f;
float accel_magnitude = 1.0f; // in g units

bool posture_abnormal = false;
float pitch_threshold = 30.0f; // degrees

// Respiration PWM parameters
int pwm_val = 0;
int pwm_dir = 5;
unsigned long last_pwm_update = 0;
unsigned long last_telemetry_send = 0;

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
    <div id="posture-badge" class="badge badge-ok">POSTURE: NORMAL</div>
  </div>
  <script>
    setInterval(() => {
      fetch('/api/data')
        .then(r => r.json())
        .then(d => {
          document.getElementById('pitch').innerText = d.pitch.toFixed(1) + '°';
          document.getElementById('roll').innerText = d.roll.toFixed(1) + '°';
          document.getElementById('accel').innerText = d.accel.toFixed(2) + ' g';
          const badge = document.getElementById('posture-badge');
          if (Math.abs(d.pitch) > 30.0 || Math.abs(d.roll) > 35.0) {
            badge.className = 'badge badge-alert';
            badge.innerText = 'ALERT: POSTURE SLUMP';
          } else {
            badge.className = 'badge badge-ok';
            badge.innerText = 'POSTURE: OPTIMAL';
          }
        }).catch(e => console.log(e));
    }, 200);
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleApiData() {
  String json = "{";
  json += "\"pitch\":" + String(pitch, 2) + ",";
  json += "\"roll\":" + String(roll, 2) + ",";
  json += "\"accel\":" + String(accel_magnitude, 2);
  json += "}";
  server.send(200, "application/json", json);
}

// ================= Kinematic Computation =================
void updateKinematics() {
  mpu.getMotion6(&raw_ax, &raw_ay, &raw_az, &raw_gx, &raw_gy, &raw_gz);

  // Convert raw acceleration to g-force (±2g range: 16384 LSB/g)
  float ax_g = (float)raw_ax / 16384.0f;
  float ay_g = (float)raw_ay / 16384.0f;
  float az_g = (float)raw_az / 16384.0f;

  accel_magnitude = sqrt(ax_g * ax_g + ay_g * ay_g + az_g * az_g);

  // Euler angle derivation via static accelerometer trigonometry
  pitch = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180.0f / M_PI;
  roll  = atan2(ay_g, az_g) * 180.0f / M_PI;

  posture_abnormal = (fabs(pitch) > pitch_threshold);
}

// ================= Biofeedback Controller =================
void updateBiofeedback() {
  if (millis() - last_pwm_update < 25) return;
  last_pwm_update = millis();

  if (posture_abnormal) {
    // Rapid stroboscopic alert flash (10 Hz alert warning)
    pwm_val = (pwm_val == 0) ? 1023 : 0;
    analogWrite(PIN_STATUS_LED, 1023 - pwm_val);
  } else {
    // Gentle rhythmic respiration pattern (healthy posture feedback)
    pwm_val += pwm_dir;
    if (pwm_val >= 1020 || pwm_val <= 5) {
      pwm_dir = -pwm_dir;
    }
    analogWrite(PIN_STATUS_LED, 1023 - pwm_val);
  }
}

// ================= Telemetry Transmission =================
void streamTelemetry() {
  if (millis() - last_telemetry_send < 100) return;
  last_telemetry_send = millis();

  if (!tcpClient.connected()) {
    tcpClient.connect(TELEMETRY_HOST, TELEMETRY_PORT);
  }
  if (tcpClient.connected()) {
    String packet = "PKT:P=" + String(pitch, 2) + ",R=" + String(roll, 2) + ",A=" + String(accel_magnitude, 2) + "\n";
    tcpClient.print(packet);
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

  Serial.println("[IMU] Initializing InvenSense MPU6050...");
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("[IMU] MPU6050 connection SUCCESSFUL");
  } else {
    Serial.println("[IMU] Warning: MPU6050 verification FAILED, retrying...");
  }

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
