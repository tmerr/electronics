#include "StepperControl.hpp"

#define HALFSTEP 8
#define SENSORPIN A0
#define ZDOWN 350
#define ZUP 630


StepperControl::StepperControl() {
  stepper = AccelStepper(HALFSTEP, D1, D3, D2, D4);
}

void StepperControl::setup() {
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(200);
}

void StepperControl::loop() {
  if (!stepper.run()) {
    stepper.disableOutputs();
  }
}

void StepperControl::goDown() {
  int zaxis = analogRead(SENSORPIN);
  if (zaxis < (ZDOWN + ZUP)/2) {
    stepper.enableOutputs();
    stepper.move(-2048);
  }
}

void StepperControl::goUp() {
  int zaxis = analogRead(SENSORPIN);
  if (zaxis > (ZDOWN + ZUP)/2) {
    stepper.enableOutputs();
    stepper.move(2048);
  }
}
