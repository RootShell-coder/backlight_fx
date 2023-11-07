#include "ambilight.h"

#define AMBILIGHT_PIN 3

static WS2812FX *ws2812fx;

ambilightNode::ambilightNode(const char *id, const char *name, const char *type) : HomieNode(id, name, type)
{
  ambilightLedCount = new HomieSetting<long>("ambilightLedCount", "Number of led WS2812 (range:1-500)");
  ambilightLedColor = new HomieSetting<const char *>("ambilightLedColor", "Color ambilight hex (range 0x000000:0xFFFFFF)");
  ambilightLedBrightness = new HomieSetting<long>("ambilightLedBrightness", "Brightness ambilight (range 0:255)");
  ambilightLedSpeed = new HomieSetting<long>("ambilightLedSpeed", "Speed FX ambilight (range 2:65000)");
  ambilightLedModeFX = new HomieSetting<long>("ambilightLedModeFX", "Mode FX ambilight (range 0:55)");
}

bool ambilightNode::handleInput(const HomieRange &range, const String &property, const String &value)
{
  if (property == "color")
  {
    unsigned long color = strtoul(value.c_str(), 0, 16);
    setProperty("color").send(String(value));
    ws2812fx->setColor(color);
  }
  if (property == "brightness")
  {
    uint8_t brightness = (uint8_t)value.toInt();
    if (brightness >= 0 && brightness <= 255)
    {
      setProperty("brightness").send(String(brightness));
      ws2812fx->setBrightness(brightness);
    }
  }
  if (property == "speed")
  {
    uint16_t speed = (uint16_t)value.toInt();
    if (speed >= 2 && speed <= 65000)
    {
      setProperty("speed").send(String(value));
      ws2812fx->setSpeed(speed);
    }
  }
  if (property == "modefx")
  {
    uint8_t modefx = (uint8_t)value.toInt();
    if (modefx >= 0 && modefx <= 54)
    {
      setProperty("modefx").send(String(modefx));
      ws2812fx->setMode(modefx);
    }
  }
  return true;
}

void ambilightNode::ambilightSetup()
{
  ambilightLedCount->setDefaultValue(1).setValidator([](long candidate)
                                                     { return (candidate > 0) && (candidate <= 500); });
  ambilightLedColor->setDefaultValue("0x000000").setValidator([](const char *candidate)
                                                              { return (strlen(candidate) == 8 && candidate[0] == '0' && candidate[1] == 'x'); });
  ambilightLedBrightness->setDefaultValue(128).setValidator([](long candidate)
                                                            { return (candidate >= 0 && candidate <= 255); });
  ambilightLedSpeed->setDefaultValue(1000).setValidator([](long candidate)
                                                        { return (candidate >= 2 && candidate <= 65000); });
  ambilightLedModeFX->setDefaultValue(0).setValidator([](long candidate)
                                                      { return (candidate >= 0 && candidate <= 54); });
}

void ambilightNode::setup()
{
  ws2812fx = new WS2812FX(ambilightLedCount->get(), AMBILIGHT_PIN, NEO_GRB + NEO_KHZ800);
  ws2812fx->init();
  String confColor = ambilightLedColor->get();
  ws2812fx->setColor(strtoul(confColor.c_str(), 0, 16));
  ws2812fx->setBrightness((uint8_t)ambilightLedBrightness->get());
  ws2812fx->setSpeed(ambilightLedSpeed->get());
  ws2812fx->setMode(ambilightLedModeFX->get());

  ws2812fx->start();
  advertise("color").setRetained(true).setFormat("0x000000:0xFFFFFF").settable();
  advertise("brightness").setRetained(true).setFormat("0:255").settable();
  advertise("speed").setRetained(true).setFormat("2:65000").settable();
  advertise("modefx").setRetained(true).setFormat("0:55").settable();
}

void ambilightNode::loop()
{
  ws2812fx->service();
}
