#include "StepperControl.hpp"

#define HALFSTEP 8
#define ZDOWN 350
#define ZUP 630


StepperControl::StepperControl() {
  stepper = AccelStepper(HALFSTEP, D1, D3, D2, D4);
  mode = Mode::idle;
  z = 0;
}

void StepperControl::setup() {
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(200);
}

void StepperControl::loop() {
  if (!stepper.run()) {
    mode = Mode::idle;
    stepper.disableOutputs();
  }
}

void StepperControl::goDown() {
  if (mode == Mode::idle && z < (ZDOWN + ZUP)/2) {
    mode = Mode::feeding;
    stepper.enableOutputs();
    stepper.move(-2048);
  }
}

void StepperControl::goUp() {
  if (mode == Mode::idle && z > (ZDOWN + ZUP)/2) {
    mode = Mode::unfeeding;
    stepper.enableOutputs();
    stepper.move(2048);
  }
}

void StepperControl::setZ(int val) {
  z = val;
}

Mode StepperControl::getMode() {
  return mode;
}
