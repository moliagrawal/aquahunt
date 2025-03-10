// Pin Definitions
const int ENA = 5;
const int ENB = 6;
const int IN1 = 3;
const int IN2 = 4;
const int IN3 = 7;
const int IN4 = 8; 

int speed_left = 0;  // Speed of the left motor (-255 to 255)
int speed_right = 0; // Speed of the right motor (-255 to 255)

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600); // Start serial communication
  Serial.println("Motor control ready. Send a, b, c, or d to change speeds.");
}

void loop()
{
  if (Serial.available() > 0)
  {
    char command = Serial.read(); // Read the serial input

    switch (command)
    {
    case 'a': // Increment left motor speed
      speed_left = constrain(speed_left + 5, -255, 255);
      break;
    case 'b': // Decrement left motor speed
      speed_left = constrain(speed_left - 5, -255, 255);
      break;
    case 'c': // Increment right motor speed
      speed_right = constrain(speed_right + 5, -255, 255);
      break;
    case 'd': // Decrement right motor speed
      speed_right = constrain(speed_right - 5, -255, 255);
      break;
    }
  }

  // Output the new speeds
  Serial.print("Left Speed: ");
  Serial.print(speed_left);
  Serial.print(" | Right Speed: ");
  Serial.println(speed_right);

  // Set motor directions and speeds
  setMotor(ENA, IN1, IN2, speed_left);  // Left motor
  setMotor(ENB, IN3, IN4, speed_right); // Right motor
}

void setMotor(int EN, int IN1, int IN2, int speed)
{
  if (speed >= 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(EN, speed);
  }
  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(EN, -speed); // Reverse speed
  }
}
