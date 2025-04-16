#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#include <Dabble.h>
#include <Servo.h>

// Motor pin definitions
const int PIN_RF = A0;         // Right Motor Forward
const int PIN_RB = A1;         // Right Motor Backward
const int PIN_LF = A2;         // Left Motor Forward
const int PIN_LB = A3;         // Left Motor Backward

const int MOTOR_L_PWM = 3;     // Left Motor PWM Pin
const int MOTOR_R_PWM = 5;     // Right Motor PWM Pin

// Ultrasonic sensor pins
const int INPUT_PIN  = 13;     // Ultrasonic signal receiving pin
const int OUTPUT_PIN = 12;     // Ultrasonic signal emission pin

// Global constants and variables
const float SPEED_CORRECTION = 0.9;       // Left-to-right speed ratio correction
const unsigned long SERIAL_BAUD_RATE = 38400;
const int SERVO_DELAY = 200;              // Delay for servo stabilization (ms)

Servo servoMotor;        // Servo instance

// Variables to store sensor measurements
float frontDistance, leftDistance, rightDistance;
bool stopMotorsFlag = false;

// --- Motor Control Functions ---

// Stops all motors
void stopAllMotors() {
  analogWrite(MOTOR_R_PWM, 0);
  analogWrite(MOTOR_L_PWM, 0);
  digitalWrite(PIN_RF, LOW);
  digitalWrite(PIN_RB, LOW);
  digitalWrite(PIN_LF, LOW);
  digitalWrite(PIN_LB, LOW);
}

// Move forward at the given speed for the specified duration (milliseconds)
void advance(int speed, int durationMs) {
  digitalWrite(PIN_RF, HIGH);
  digitalWrite(PIN_RB, LOW);
  digitalWrite(PIN_LF, HIGH);
  digitalWrite(PIN_LB, LOW);
  analogWrite(MOTOR_L_PWM, speed * 0.92); // Adjusted for left motor compensation
  analogWrite(MOTOR_R_PWM, speed);
  delay(durationMs);
}

// Move backward at the given speed
void moveBackward(int speed) {
  digitalWrite(PIN_RF, LOW);
  digitalWrite(PIN_RB, HIGH);
  digitalWrite(PIN_LF, LOW);
  digitalWrite(PIN_LB, HIGH);
  analogWrite(MOTOR_R_PWM, speed);
  analogWrite(MOTOR_L_PWM, speed);
  delay(250);
}

// Turn left at the given speed
void turnLeft(int speed) {
  digitalWrite(PIN_RF, HIGH);
  digitalWrite(PIN_RB, LOW);
  digitalWrite(PIN_LF, LOW);
  digitalWrite(PIN_LB, HIGH);
  analogWrite(MOTOR_R_PWM, speed);
  analogWrite(MOTOR_L_PWM, speed);
  delay(100);
}

// Turn right at the given speed
void turnRight(int speed) {
  digitalWrite(PIN_RF, LOW);
  digitalWrite(PIN_RB, HIGH);
  digitalWrite(PIN_LF, HIGH);
  digitalWrite(PIN_LB, LOW);
  analogWrite(MOTOR_R_PWM, speed);
  analogWrite(MOTOR_L_PWM, speed);
  delay(100);
}

// Perform a 90-degree right turn
void turnRight90() {
  digitalWrite(PIN_RF, LOW);
  digitalWrite(PIN_RB, HIGH);
  digitalWrite(PIN_LF, HIGH);
  digitalWrite(PIN_LB, LOW);
  analogWrite(MOTOR_R_PWM, 200);
  analogWrite(MOTOR_L_PWM, 200);
  delay(700);   // Adjust for precise 90° turn
  stopAllMotors();
}

// Perform a 90-degree left turn
void turnLeft90() {
  digitalWrite(PIN_RF, HIGH);
  digitalWrite(PIN_RB, LOW);
  digitalWrite(PIN_LF, LOW);
  digitalWrite(PIN_LB, HIGH);
  analogWrite(MOTOR_R_PWM, 200);
  analogWrite(MOTOR_L_PWM, 200);
  delay(600);   // Adjust for precise 90° turn
  stopAllMotors();
}

// --- Ultrasonic Sensor Functions ---

// Reads the distance measured by the front sensor
void readFrontDistance() {
  servoMotor.write(90);
  digitalWrite(OUTPUT_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(OUTPUT_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(OUTPUT_PIN, LOW);
  frontDistance = pulseIn(INPUT_PIN, HIGH) / 5.8 / 10;  // Conversion to cm
  Serial.print("Front distance: ");
  Serial.println(frontDistance);
}

// Reads the distance measured by the left sensor
void readLeftDistance() {
  servoMotor.write(5);
  delay(SERVO_DELAY);
  digitalWrite(OUTPUT_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(OUTPUT_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(OUTPUT_PIN, LOW);
  leftDistance = pulseIn(INPUT_PIN, HIGH) / 5.8 / 10;  // Conversion to cm
  Serial.print("Left distance: ");
  Serial.println(leftDistance);
}

// Reads the distance measured by the right sensor
void readRightDistance() {
  servoMotor.write(177);
  delay(SERVO_DELAY);
  digitalWrite(OUTPUT_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(OUTPUT_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(OUTPUT_PIN, LOW);
  rightDistance = pulseIn(INPUT_PIN, HIGH) / 5.8 / 10;  // Conversion to cm
  Serial.print("Right distance: ");
  Serial.println(rightDistance);
}

// --- Maze Navigation Functions ---

enum Direction { FORWARD, LEFT, RIGHT, BACKWARD };
Direction decisionDirection;

// Uses sensor data to determine navigation decisions
void detection() {      
  readFrontDistance();
  
  if (frontDistance < 10) {
    stopAllMotors();
    moveBackward(2);
  } else if (frontDistance < 25) {
    stopAllMotors();
    readLeftDistance();
    delay(SERVO_DELAY);
    readRightDistance();
    delay(SERVO_DELAY);
    
    // Choose direction based on comparative distance
    if (leftDistance > rightDistance) {
      decisionDirection = LEFT;
    } else {
      decisionDirection = RIGHT;
    }
    
    // If both left and right distances are too close, move backward
    if (leftDistance < 15 && rightDistance < 15) {
      decisionDirection = BACKWARD;
    }
  } else {
    decisionDirection = FORWARD;
  }
}

// A predefined maze routine
void hardcodedMaze() {
  advance(200, 1500);
  stopAllMotors();
  turnLeft90();
  advance(200, 900);
  stopAllMotors();
}

// A maze navigation routine using sensor inputs
void startMaze() {
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT);

  long duration;
  float distance;
  int middleCheck = 0;
  bool turnSequence[] = {false, false, true, true, false};
  int numTurns = sizeof(turnSequence) / sizeof(turnSequence[0]);

  for (int i = 0; i < numTurns; i++) {
    // Move forward until an obstacle is detected within 10 cm
    while (true) {
      digitalWrite(OUTPUT_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(OUTPUT_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(OUTPUT_PIN, LOW);

      duration = pulseIn(INPUT_PIN, HIGH, 30000);
      distance = duration * 0.034 / 2;

      Serial.print("Distance: ");
      Serial.println(distance);

      if (distance <= 10.0 && distance > 0) {
        break;
      }
      
      advance(150, 150);
      // Middle correction logic placeholder (if needed)
      if (middleCheck > 2) {
        // Insert logic here if necessary
        middleCheck = 0;
      }
      delay(50);
      middleCheck++;
    }

    stopAllMotors();
    delay(200);

    // Execute turn based on predefined sequence
    if (turnSequence[i]) {
      turnRight90();
    } else {
      turnLeft90();
    }
    delay(300);
  }
}

// --- Arduino Setup and Loop ---

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Dabble.begin(9600, 11, 10);

  // Set motor control pins as OUTPUT
  pinMode(PIN_RF, OUTPUT);
  pinMode(PIN_RB, OUTPUT);
  pinMode(PIN_LF, OUTPUT);
  pinMode(PIN_LB, OUTPUT);
  pinMode(MOTOR_L_PWM, OUTPUT);
  pinMode(MOTOR_R_PWM, OUTPUT);

  // Attach servo to pin 7 and initialize ultrasonic sensor pins
  servoMotor.attach(7);
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);

  stopAllMotors();
}

void loop() {
  Dabble.processInput();
  Serial.print("KeyPressed: ");

  stopMotorsFlag = true;

  // Handle GamePad input
  if (GamePad.isUpPressed()) {
    Serial.println("UP");
    advance(200, 150);
    stopMotorsFlag = false;
  }
  if (GamePad.isDownPressed()) {
    Serial.println("DOWN");
    moveBackward(200);
    stopMotorsFlag = false;
  }
  if (GamePad.isLeftPressed()) {
    Serial.println("LEFT");
    turnLeft(200);
    stopMotorsFlag = false;
  }
  if (GamePad.isRightPressed()) {
    Serial.println("RIGHT");
    turnRight(200);
    stopMotorsFlag = false;
  }
  if (GamePad.isSquarePressed()) {
    Serial.println("SQUARE");
    hardcodedMaze();
    stopMotorsFlag = false;
  }
  if (GamePad.isCirclePressed()) {
    Serial.println("CIRCLE");
    startMaze();
    stopMotorsFlag = false;
  }
  if (GamePad.isCrossPressed()) {
    Serial.println("CROSS");
  }
  if (GamePad.isTrianglePressed()) {
    Serial.println("TRIANGLE");
  }
  if (GamePad.isStartPressed()) {
    Serial.println("START");
  }
  if (GamePad.isSelectPressed()) {
    Serial.println("SELECT");
  }
  if (stopMotorsFlag) {
    stopAllMotors();
  }

  // Log analog input from GamePad
  int angle = GamePad.getAngle();
  int radius = GamePad.getRadius();
  float xAxis = GamePad.getXaxisData();
  float yAxis = GamePad.getYaxisData();
  
  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print("\tRadius: ");
  Serial.print(radius);
  Serial.print("\tx_axis: ");
  Serial.print(xAxis);
  Serial.print("\ty_axis: ");
  Serial.println(yAxis);
  Serial.println();
}
