#define in1 5     // Motor 1
#define in2 6
#define in3 10    // Motor 2
#define in4 11
#define ENA 9     // Enable for Motor 1
#define ENB 3     // Enable for Motor 2
#define LED 13

String command;      // To store Bluetooth command
int Speed = 150;     // Speed range (0 - 255)

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(LED, OUTPUT);
  
  pinMode(ENA, OUTPUT);  // Set ENA for Motor 1
  pinMode(ENB, OUTPUT);  // Set ENB for Motor 2
  digitalWrite(ENA, HIGH);  // Enable Motor 1
  digitalWrite(ENB, HIGH);  // Enable Motor 2
  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');  // Read the command as a string until a newline character
    command.trim();  // Remove any extra spaces or newline characters

    Serial.print("Command received: ");
    Serial.println(command);  // Print the received command to the Serial Monitor
    Serial.print("Length: ");
    Serial.println(command.length());  // Print the length of the received command

    // Print each character of the command
    for (int i = 0; i < command.length(); i++) {
      Serial.print("Character ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(command[i]);  // Print each character
    }

    Stop();  // Always stop the motors before processing a new command

    // Compare the command string to the expected commands
    if (command.equalsIgnoreCase("f")) {
      forward();
    } else if (command.equalsIgnoreCase("b")) {
      back();
    } else if (command.equalsIgnoreCase("l")) {
      left();
    } else if (command.equalsIgnoreCase("r")) {
      right();
    } else if (command.equalsIgnoreCase("s")) {
      Stop();
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
}

void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void back() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);   // Slow down one side
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void right() {
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
