#pragma once

#define DEBUG_MODE true

// Motor pinout configuration
#define EN PD5
#define IN1 PD6
#define IN2 PD7

// Interuptions
#define HALL1 PD2
#define HALL2 PD3

// Stop time
#define STOP_TIME_MILLISECONDS 5000

// Mode
#define SWITCH 9

// Manual mode pot
#define POT A0
#define MIN_POT_VALUE 0
#define MAX_POT_VALUE 1024
#define ZERO_POT_VALUE 511

// Motor speed limit
#define MAX_MOTOR_SPEED 255
#define ZERO_MOTOR_SPEED 0

// Number of stops available in the system
#define STOPS 2
