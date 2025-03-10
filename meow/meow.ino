// Pin Definitions
const int ENA = 5;  // PWM pin for left motor (speed)
const int IN1 = 6;  // Direction pin for left motor
const int IN2 = 3;  // Direction pin for left motor
const int ENB = 4; // PWM pin for right motor (speed)
const int IN3 = 7;  // Direction pin for right motor
const int IN4 = 8;  // Direction pin for right motor

int speed_left = 0;  // Speed of the left motor (-255 to 255)
int speed_right = 0; // Speed of the right motor (-255 to 255)

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600); // Start serial communication
  Serial.println("Motor control ready. Send a, b, c, or d to change speeds.");
}

void loop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}
