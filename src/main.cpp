#include <Arduino.h>
#include <Homie.h>
#include "ambilight.h"
#include <LoggerNode.h>

#define BRAND "Ambilight"
#define FW_NAME "illuminated"
#define FW_VER "1.2.1"

void onHomieEvent(const HomieEvent &event)
{
  switch (event.type)
  {
  case HomieEventType::WIFI_CONNECTED:
    Serial << "Wi-Fi connected: " << (int8_t)event.wifiReason << endl;
    break;
  case HomieEventType::MQTT_READY:
    Serial << "MQTT connected: " << (int8_t)event.mqttReason << endl;
    break;
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
LoggerNode LN;

void setup()
{
  Serial.begin(115200);
  Serial << endl
         << endl;
  Serial.flush();
  Homie_setBrand(BRAND);
  Homie_setFirmware(FW_NAME, FW_VER);
  Homie.onEvent(onHomieEvent);
  AmbilightNode.ambilightSetup();
  Homie.setLoggingPrinter(&Serial);
  Homie.setup();
}

void loop()
{
  static uint32_t last = 0;
  Homie.loop();
  if (millis() - last > 10000)
  {
    LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG,
            "Free heap: %d bytes. Heap Fragmentation: %d", ESP.getFreeHeap(), ESP.getHeapFragmentation());
    last = millis();
  }
}
