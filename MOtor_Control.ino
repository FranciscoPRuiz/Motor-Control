#include <Arduino.h>
#include <WiFi.h>
#define RELAY_PIN_1 2
#include "fauxmoESP.h"


#define SERIAL_BAUDRATE 115200

#define WIFI_SSID "WIFI name"
#define WIFI_PASS "Password"

#define Motor_1 "Motor 1"
#define Motor_2 "Motor 2"

fauxmoESP fauxmo;

int motorPin = 4;
int motorPin2 = 6;



void wifiSetup() {

  WiFi.mode(WIFI_STA);

  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  wifiSetup();

  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, HIGH);

  fauxmo.createServer(true);
  fauxmo.setPort(80);

  fauxmo.enable(true);

  fauxmo.addDevice(Motor_1);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {


    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, Motor_1) == 0) ) {
      Serial.println("RELAY 1 switched by Alexa");
      if (state) {
        digitalWrite(motorPin, HIGH);
      } else {
        digitalWrite(motorPin, LOW);
      }
    }
  });
  fauxmo.addDevice(Motor_2);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {

    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, Motor_2) == 0) ) {
      Serial.println("RELAY 1 switched by Alexa");
      if (state) {
        digitalWrite(motorPin2, HIGH);
      } else {
        digitalWrite(motorPin2, LOW);
      }
    }
  });

}

void loop() {

  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

}
