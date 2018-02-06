#ifndef _STEPPERCONTROL_HPP
#define _STEPPERCONTROL_HPP
#include <AccelStepper.h>

class StepperControl {
public:
  StepperControl();
  void setup();
  void loop();
  void goUp();
  void goDown();

private:
  AccelStepper stepper;
};
#endif
