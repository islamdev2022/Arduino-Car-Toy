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
const int humanFollowDistance = 30;  // Distance in cm to follow the human
const int obstacleThreshold = 10;     // Distance in cm to stop for obstacles

String command;      // To store Bluetooth command
int Speed = 150;     // Speed range (0 - 255)
bool isFollowing = false; // Flag to track if following mode is activated

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
  long distance = measureDistance(); // Measure distance using the ultrasonic sensor

  // Print distance for debugging
  Serial.print("Distance from obstacle: ");
  Serial.println(distance);

  // Check for obstacles
  if (distance < obstacleThreshold) {
    Serial.println("Obstacle too close, stopping.");
    Stop(); // Stop the car if an obstacle is detected
    isFollowing = false; // Disable following mode
  } else {
    // Check if a human is detected within following distance
    if (distance < humanFollowDistance) {
      Serial.println("Following the human.");
      moveForward(); // Move forward to follow the human
      isFollowing = true; // Enable following mode
    } else {
      Stop(); // Stop if the human is too far away
      isFollowing = false; // Disable following mode
    }
  }

  // Check for Bluetooth commands
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');  // Read the command as a string until a newline character
    command.trim();  // Remove any extra spaces or newline characters

    Serial.print("Command received: ");
    Serial.println(command);  // Print the received command to the Serial Monitor

    Stop();  // Always stop the motors before processing a new command

    // Handle movement commands
    if (command.equalsIgnoreCase("f")) {
      moveForward();
    } else if (command.equalsIgnoreCase("s")) {
      Stop();
      isFollowing = false;  // Stop following when the stop command is received
    } 

    // Speed control commands remain unchanged
    // Set the motor speed via the ENA and ENB pins
    analogWrite(ENA, Speed);  // Control speed of Motor 1 (Right motor)
    analogWrite(ENB, Speed);  // Control speed of Motor 2 (Left motor)
  }
}

// Function to measure the distance using the ultrasonic sensor
long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
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

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
