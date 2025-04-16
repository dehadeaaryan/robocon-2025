# Arduino Maze Navigation Robot

This project implements a maze navigation robot using an Arduino board. The code controls motor outputs, processes ultrasonic sensor readings via a servo-mounted sensor, and accepts inputs from a GamePad using the Dabble library. The robot is designed to autonomously navigate a maze by detecting obstacles and executing predefined maneuvers.

## Features

- **Motor Control:** Controls two motors for moving forward, backward, turning left/right, and executing 90° turns.
- **Ultrasonic Sensing:** Uses ultrasonic sensors (attached to a servo motor) to detect obstacles on the front, left, and right.
- **Maze Navigation:** Includes routines for hardcoded maze maneuvers as well as sensor-driven navigation.
- **GamePad Integration:** Processes input commands from a GamePad (using the Dabble library) to manually control the robot.

## Hardware Requirements

- **Arduino Board:** Compatible with most Arduino boards.
- **Motors:** Two DC motors with appropriate motor drivers.
- **Ultrasonic Sensor:** Connected to digital pins for pulse measurement.
- **Servo Motor:** Used to rotate the ultrasonic sensor.
- **GamePad Controller:** Integrated via the Dabble library.
- **Additional Components:** Wiring, breadboard, power supply, etc.

## Pin Configuration

- **Motor Control:**
  - Right Motor Forward: `A0`
  - Right Motor Backward: `A1`
  - Left Motor Forward: `A2`
  - Left Motor Backward: `A3`
  - Left Motor PWM: Pin `3`
  - Right Motor PWM: Pin `5`
  
- **Ultrasonic Sensor:**
  - Emission: Pin `12`
  - Reception: Pin `13`
  
- **Servo Motor:**
  - Attached to Pin `7`

## Software Setup

1. **Arduino IDE:**  
   Install the latest version of the Arduino IDE from the [Arduino website](https://www.arduino.cc/en/software).

2. **Dabble Library:**  
   Install the [Dabble library](https://github.com/DabbleLab/Dabble-Arduino-Library) in your Arduino IDE to enable GamePad integration.

3. **File Extension:**  
   Save the provided code as an `.ino` file (e.g., `MazeNavigationRobot.ino`).

## How to Use

1. **Wiring Setup:**  
   Connect all components as per the pin configuration mentioned above.

2. **Upload Code:**  
   Open the `.ino` file in the Arduino IDE, select the appropriate board and port, and upload the code.

3. **Operation Modes:**  
   - **Manual Mode:** Use the GamePad to control the robot’s movement. Press the directional buttons to move or turn the robot.
   - **Maze Navigation Mode:**  
     - Press the square button to execute a hardcoded maze routine.
     - Press the circle button to initiate sensor-driven maze navigation.
     
   The robot will also print sensor readings and control status messages to the Serial Monitor. Ensure the Serial Monitor baud rate is set to **38400**.

## Customization

- **Motor Speed and Delays:**  
  Adjust the motor speed variables, delays, or 90° turn duration as needed to accommodate your specific robot hardware.

- **Sensor Thresholds:**  
  Modify the obstacle distance thresholds in the sensor functions to optimize navigation for different environments.

## Troubleshooting

- **No Serial Output:**  
  Verify your board connection and ensure the baud rate in the Serial Monitor is set to **38400**.
- **Sensor Inaccuracies:**  
  Check the ultrasonic sensor wiring and ensure the servo is properly calibrated.

## License

This project is provided as-is, without any warranty. Feel free to modify and improve the code for your own purposes.
