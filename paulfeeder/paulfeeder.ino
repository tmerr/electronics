#include <AccelStepper.h>
#include "StepperControl.hpp"
#include "WebServer.hpp"

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
  control.loop();
  for (int i=0; i<1000; ++i) {
    server.loop();
  }
}
