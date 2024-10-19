#include <Servo.h>

#define in1 5     // Motor 1
#define in2 6
#define in3 10    // Motor 2
#define in4 11
#define ENA 9     // Enable for Motor 1
#define ENB 3     // Enable for Motor 2
#define LED 13    // Status LED

// Ultrasonic sensor pins
#define trigPin A1
#define echoPin A0

// Constants
const int obstacleThreshold = 10;  // Distance in cm to trigger obstacle avoidance

String command;      // To store Bluetooth command
int Speed = 150;     // Speed range (0 - 255)
bool isMoving = false;  // Flag to track if a movement command was received
String lastCommand;   // To store the last valid movement command

void setup() {
  // Motor pin setup
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(LED, OUTPUT);
  
  pinMode(ENA, OUTPUT);  // Set ENA for Motor 1
  pinMode(ENB, OUTPUT);  // Set ENB for Motor 2
  digitalWrite(ENA, HIGH);  // Enable Motor 1
  digitalWrite(ENB, HIGH);  // Enable Motor 2

  // Ultrasonic sensor pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  // Check for Bluetooth commands
  if (Serial.available() > 0) {
    long distance = measureDistance();
    Serial.println("Distance from obstacle: ");
    Serial.println(distance);
    command = Serial.readStringUntil('\n');  // Read the command as a string until a newline character
    command.trim();  // Remove any extra spaces or newline characters

    Serial.print("Command received: ");
    Serial.println(command);  // Print the received command to the Serial Monitor

    Stop();  // Always stop the motors before processing a new command
    isMoving = true;  // Set the flag to true once a valid command is received

    // Handle movement commands
    if (command.equalsIgnoreCase("f")) {
      moveForward();
      lastCommand = "f";  // Save the last command
    } else if (command.equalsIgnoreCase("b")) {
      moveBackward();
      lastCommand = "b";  // Save the last command
    } else if (command.equalsIgnoreCase("l")) {
      turnLeft();
      lastCommand = "l";  // Save the last command
    } else if (command.equalsIgnoreCase("r")) {
      turnRight();
      lastCommand = "r";  // Save the last command
    } else if (command.equalsIgnoreCase("s")) {
      Stop();
      isMoving = false;  // Stop the movement and disable the movement flag
    } 
    // Speed control commands
    else if (command.equals("0")) {
      Speed = 100;
    } else if (command.equals("1")) {
      Speed = 140;
    } else if (command.equals("2")) {
      Speed = 153;
    } else if (command.equals("3")) {
      Speed = 165;
    } else if (command.equals("4")) {
      Speed = 178;
    } else if (command.equals("5")) {
      Speed = 191;
    } else if (command.equals("6")) {
      Speed = 204;
    } else if (command.equals("7")) {
      Speed = 216;
    } else if (command.equals("8")) {
      Speed = 229;
    } else if (command.equals("9")) {
      Speed = 242;
    } else if (command.equalsIgnoreCase("q")) {
      Speed = 255;  // Max speed
    } else {
      Serial.println("Unrecognized command");
    }

    // Set the motor speed via the ENA and ENB pins
    analogWrite(ENA, Speed);  // Control speed of Motor 1 (Right motor)
    analogWrite(ENB, Speed);  // Control speed of Motor 2 (Left motor)
  }

  // Continuously check for obstacles, but only when the car is moving
  if (isMoving) {
    long distance = measureDistance();
    Serial.println("Distance from obstacle: ");
    Serial.println(distance);
    if (distance < obstacleThreshold) {
      Serial.println("Obstacle too close, moving backward");
      moveBackward();  // Move backward when an obstacle is detected
      delay(1000);     // Move back for 1 second
      Stop();          // Stop the car after moving backward
      isMoving = false; // Disable movement flag to stop moving after the obstacle is avoided

      // Recheck for obstacles and resume movement after avoiding
      delay(1000); // Pause before resuming
      distance = measureDistance(); // Measure distance again
      if (distance >= obstacleThreshold) {
        // Resume last command if no obstacle is detected
        if (lastCommand.equals("f")) {
          moveForward();
        } else if (lastCommand.equals("b")) {
          moveBackward();
        } else if (lastCommand.equals("l")) {
          turnLeft();
        } else if (lastCommand.equals("r")) {
          turnRight();
        }
        isMoving = true; // Set the movement flag to true again
      }
    }
  }
}

// Function to measure the distance using the ultrasonic sensor
long measureDistance() {
  // Send a 10 microsecond pulse to trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance based on the speed of sound
  long distance = duration * 0.034 / 2;  // Speed of sound is 0.034 cm/us

  return distance;
}

// Movement functions
void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveBackward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);   // Slow down one side
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);   // Slow down the other side
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
