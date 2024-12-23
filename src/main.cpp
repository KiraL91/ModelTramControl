#include <Arduino.h>
#include <stdlib.h>
#include <L298N.h>

#include "./config.h"
#include "./state/StateMachine.h"
#include "./mode/OperatingModeHandler.h"
#include "./manual/ManualModeHandler.h"

L298N *motor;
Model::State::StateMachine *sm;
Model::Mode::OperatingModeHandler *omh;
Model::Manual::ManualModeHandler *mmh;

void setup()
{
  Serial.begin(9600);

  // Hall sensors
  pinMode(HALL1, INPUT_PULLUP);
  pinMode(HALL2, INPUT_PULLUP);

  // L298
  pinMode(EN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Pot
  pinMode(POT, INPUT);

  // Hall sensor interruptions
  attachInterrupt(digitalPinToInterrupt(HALL1), Model::State::StateMachine::Callback, CHANGE);
  attachInterrupt(digitalPinToInterrupt(HALL2), Model::State::StateMachine::Callback, CHANGE);

  // Mode sensors
  pinMode(SWITCH, INPUT_PULLUP);

  motor = new L298N(EN, IN1, IN2);

  sm = new Model::State::StateMachine();
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
