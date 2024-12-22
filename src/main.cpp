#include <Arduino.h>
#include <stdlib.h>
#include <L298N.h>

#include "./config.h"
#include "./state/StateMachine.h"

L298N *motor;
Model::StateMachine *sm;

void setup()
{
  Serial.begin(9600);

  pinMode(PD2, INPUT_PULLUP);
  pinMode(PD3, INPUT_PULLUP);

  pinMode(EN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PD2), Model::StateMachine::Callback, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PD3), Model::StateMachine::Callback, CHANGE);

  motor = new L298N(EN, IN1, IN2);
  sm = new Model::StateMachine();

  sm->Setup(motor);
}

void loop()
{
  sm->Run();
  delay(10);
}
