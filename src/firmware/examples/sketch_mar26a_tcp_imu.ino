#include<ESP8266WiFi.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#if __has_include("tcp_config.private.h")
#include "tcp_config.private.h"
#else
#include "tcp_config.example.h"
#endif
WiFiClient client;
MPU6050 accelgyro;

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(TCP_WIFI_SSID,TCP_WIFI_PASSWORD);
  Serial.println("Connecting to WiFi... ");
  unsigned long start = millis();
  while(WiFi.status() != WL_CONNECTED && millis()-start < 15000){
    Serial.println("Failed!");
    delay(2000);
  }
  Serial.println("");
  Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "WiFi unavailable; connection will be retried");
  Serial.println("Local IP:"); 
  Serial.println(WiFi.localIP());
  // put your setup code here, to run once:

  Wire.begin();

  client.setTimeout(500);

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection()?"MPU6050 connection successful":"MPU6050 connection failed");
}

void loop() {
  Serial.println("Send The String to TCP Server :\"I am Esp8266\"");
  if (WiFi.status() != WL_CONNECTED || !client.connect(TCP_SERVER_HOST,TCP_SERVER_PORT)) { delay(2000); return; }
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
