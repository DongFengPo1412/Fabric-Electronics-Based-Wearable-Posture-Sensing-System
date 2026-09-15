#include<ESP8266WiFi.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#define AP_SSID "ACE"
#define AP_PSW  "66666666"
const uint16_t port = 8080;
const char * host = "127.0.0.1";
WiFiClient client;
MPU6050 accelgyro;

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID,AP_PSW);
  Serial.println("Connecting to WiFi... ");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Failed!");
    delay(2000);
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("Local IP:"); 
  Serial.println(WiFi.localIP());
  // put your setup code here, to run once:

  Wire.begin();

  Serial.begin(38400);

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection()?"MPU6050 connection successful":"MPU6050 connection failed");
}

void loop() {
  Serial.println("Send The String to TCP Server :\"I am Esp8266\"");
  client.println(String("I am Esp8266"));

  String line = client.readStringUntil('\n');
  Serial.println(line);

  delay(3000);
  Serial.println("");
  client.stop();

  Serial.println("wait 5 sec...");
  delay(5000);  
  // put your main code here, to run repeatedly:
}
