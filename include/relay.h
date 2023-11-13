#ifndef RELAY_H
#define RELAY_H
#include <Homie.h>

class relayNode : public HomieNode
{
public:
  relayNode(const char *id, const char *name, const char *type);
  void relaySetup();

protected:
  virtual bool handleInput(const HomieRange &range, const String &property, const String &value) override;
  virtual void setup() override;
  virtual void loop() override;

private:
  HomieSetting<long> *relay0;
};

#endif
