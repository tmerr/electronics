#ifndef _STEPPERCONTROL_HPP
#define _STEPPERCONTROL_HPP
#include <AccelStepper.h>
#include "Mode.hpp"

class StepperControl {
public:
  StepperControl();
  void setup();
  void loop();
  void goUp();
  void goDown();
  void setZ(int val);
  Mode getMode();

private:
  AccelStepper stepper;
  int z;
  Mode mode;
};
#endif
