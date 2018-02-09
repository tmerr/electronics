#include <AccelStepper.h>
#include "StepperControl.hpp"
#include "WebServer.hpp"
#define SENSORPIN A0

StepperControl control;
WebServer server;

void setup() {
  control.setup();
  server.registerFeedCallback([&](){
    control.goDown();
  });
  server.registerUnfeedCallback([&](){
    control.goUp();
  });
  server.setup(
    "<SSID>",
    "<PASSWORD>"
  );
}

void loop() {
  int z = analogRead(SENSORPIN);
  control.setZ(z);
  server.setMode(control.getMode());
  server.setZ(z);
  control.loop();
  for (int i=0; i<1000; ++i) {
    server.loop();
  }
}
