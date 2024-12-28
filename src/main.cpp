#include <Arduino.h>
#include <stdlib.h>
#include <L298N.h>

#include "./config.h"
#include "./state/IStateMachine.h"
#include "./state/dual/StateMachineDual.h"
#include "./state/sensorless/StateMachineSensorless.h"
#include "./state/single/StateMachineSingle.h"
#include "./state/StateMachineFactory.h"
#include "./manual/ManualModeHandler.h"
#include "./mode/OperatingModeHandler.h"

L298N *motor;
Model::State::IStateMachine *sm;
Model::Mode::OperatingModeHandler *omh;
Model::Manual::ManualModeHandler *mmh;

void setup()
{
  Serial.begin(9600);

  // L298
  pinMode(EN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Pot
  pinMode(POT, INPUT);

  // Mode sensors
  pinMode(SWITCH, INPUT_PULLUP);

  motor = new L298N(EN, IN1, IN2);

  sm = Model::State::StateMachineFactory::GetStateMachine();
  sm->Setup(motor);

  mmh = new Model::Manual::ManualModeHandler();
  mmh->Setup(motor);

  omh = new Model::Mode::OperatingModeHandler(sm, mmh);
}

void loop()
{
  digitalRead(SWITCH) == LOW
      ? omh->SetMode(Model::Mode::OperatingMode::Auto)
      : omh->SetMode(Model::Mode::OperatingMode::Manual);
  omh->Run();

  delay(10);
}
