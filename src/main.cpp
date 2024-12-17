#include <Arduino.h>
#include <stdlib.h>
#include <L298N.h>

#include "./config.h"
#include "../lib/state/StateMachine.h"

L298N *motor;
StateMachine *sm;

void setup()
{
  Serial.begin(9600);

  pinMode(SENSOR_1_PIN, INPUT);
  pinMode(SENSOR_2_PIN, INPUT);
  pinMode(INTERRUP_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUP_PIN), StateMachine::Callback, FALLING);

  motor = new L298N(EN, IN1, IN2);
  sm = new StateMachine();

  sm->Setup(motor);
}

void loop()
{
  sm->Run();
}
