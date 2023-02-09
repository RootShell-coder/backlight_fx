#include <Arduino.h>
#include <Homie.h>
#include "ambilight.h"

#define BRAND "DOOR"
#define FW_NAME "light"
#define FW_VER "1.0.0"

void onHomieEvent(const HomieEvent &event){
  switch (event.type){
  case HomieEventType::WIFI_DISCONNECTED:
    Serial << "Wi-Fi disconnected, reason: " << (int8_t)event.wifiReason << endl;
    break;
  case HomieEventType::MQTT_DISCONNECTED:
    Serial << "MQTT disconected, reason: " << (int8_t)event.mqttReason << endl;
    break;
  default:
    break;
  }
}

ambilightNode AmbilightNode("ambilight", "Ambilight", "WS2812B");

void setup() {
  Serial.begin(115200);
  Serial << endl
         << endl;
  Homie_setBrand(BRAND);
  Homie_setFirmware(FW_NAME, FW_VER);
  Homie.onEvent(onHomieEvent);
  AmbilightNode.ambilightSetup();
  Homie.setup();
}

void loop() {
  Homie.loop();
}
