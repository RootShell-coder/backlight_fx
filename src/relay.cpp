#include "relay.h"

#define RELAY0 0

relayNode::relayNode(const char *id, const char *name, const char *type) : HomieNode(id, name, type)
{
  relay0 = new HomieSetting<long>("relay0", "State relay0 (range: 0,1)");
}

bool relayNode::handleInput(const HomieRange &range, const String &property, const String &value)
{
  if (property == "relay0")
  {
    uint8_t state = (uint8_t)value.toInt();
    if (state == 0 || state == 1)
    {
      digitalWrite(RELAY0, state ? HIGH : LOW);
      setProperty("relay0").send(value);
      return true;
    }
  }
  return false;
}

void relayNode::setup()
{
  pinMode(RELAY0, OUTPUT);
  digitalWrite(RELAY0, relay0->get() ? HIGH : LOW);
  advertise("relay0").setRetained(true).setFormat("0:1").settable();
}

void relayNode::relaySetup()
{
  relay0->setDefaultValue(1).setValidator([](long candidate)
                                          { return (candidate == 0 || candidate == 1); });
}

void relayNode::loop()
{
  // No implementation needed for now
}
