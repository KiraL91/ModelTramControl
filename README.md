# H0 Model Train Controller with Arduino Nano

This repository contains the code and setup instructions for controlling an H0 scale model train using an **Arduino Nano**. The project allows you to control the train's speed, direction, and operation modes (manual and automatic) using sensors and a motor controller.

## Project Features

- **Operation Modes**:
  - **Manual:**
    - The train's speed is controlled using a potentiometer.
    - The train stops when the potentiometer is in the center position.
    - The potentiometer's extremes control the minimum and maximum speeds.
  - **Automatic:**
    - The system uses Hall effect sensors to manage the train's states:
      - Undefined (on startup), first stop, last stop, forward, and reverse.
      - A timer determine the train's restart moments at stop points.
  - **Speed Control:**
    - Implemented using PWM (Pulse Width Modulation) signals.
  - **Direction Control:**
    - Automatically or manually managed, depending on the selected mode.

## Required Components
### Hardware

- Arduino Nano. 
  - Other microcontrollers can be used but the pinout would have to be adjusted
- L298N Motor Controller
- H0 Scale Model Train
  - For testing a DC motor can be used
- Two Hall Effect Sensors (A3144)
- Potentiometer
- Two-position Switch
- External power supply for the train (+12v)
- Wires and breadboard

### Software
- vscode IDE

## Hardware Setup

1. Connect the L298N outputs to the train's motor terminals.
2. Power the L298N module using an external power supply suitable for the train.
3. Connect the L298N control inputs to the Arduino Nano's digital pins as specified in config.h.
4. Connect the GND pin of the Arduino to the GND of the L298N and the Hall sensors to establish a common ground.
5. Reference the pin definitions in the `config.h` file for wiring details.

## Usage Instructions

1. **Download the repository** and open the project files in the vscode IDE. 
2. Review and modify the `config.h` file to ensure the pin configuration matches your setup.
3. **Upload the code** to the Arduino Nano via USB.
4. Set up the hardware according to the wiring instructions.
5. Power the system and select the operation mode using the switch.

## Operation Modes
### Manual Mode

- The potentiometer controls the train's speed.
- **Center position** of the potentiometer: the train stops.
- **Extremes** of the potentiometer:
  - Maximum speed forward.
  - Maximum speed in reverse.

### Automatic Mode

- The system transitions through different states managed by the Hall sensors:
  1. **Undefined:** Initial state on startup.
  2. **First Stop:** The train halts at the position detected by the first Hall sensor.
  3. **Last Stop:** The train halts at the position detected by the second Hall sensor.
  4. **Forward:** The train moves forward between the Hall sensor positions.
  5. **Reverse:** The train moves backward between the Hall sensor positions.
- **A timer** controls the restart timing at stop points.

## Physical Connections

- **Power Supply:**
  - The Arduino Nano must be powered with a stable 5V supply.
  - The L298N motor controller requires a 12V external power supply to drive the train motor. If more than 12V is supplied, the jumper on the converter that supplies 5V must be removed, otherwise the integrated circuit could be damaged.
- **Ground (GND):**
  - Ensure that the ground (GND) of the Arduino Nano and the 12V power supply are connected together to establish a common reference point.
- **Motor Connections:**
  - Connect the motor terminals to the output ports of the L298N module (OUT1 and OUT2).
- **Control Signals:**
  - Refer to the `config.h` file for the exact pin configuration of control signals, including PWM, direction, and sensor inputs.

## Configuration

All pin definitions and adjustable parameters are located in the `config.h` file. Modify this file to:

- Assign pin numbers for components such as the motor controller, Hall sensors, switch, and potentiometer.
- Adjust timers or thresholds to fit your setup.

## Possible Improvements

1. **Sound Control:** Add a sound module to simulate train horns or locomotive sounds.
2. **Lighting:** Control front and rear lights of the locomotive based on the direction.
3. **Remote Control:** Integrate a Bluetooth or Wi-Fi module for remote operation.
4. **State Monitoring:** Display the current system state using an LCD or OLED screen.

## Contributing

If you wish to contribute to this project, feel free to open an **issue** or submit a **pull request**. Improvements and optimizations are always welcome!
