#include <Servo.h>

struct PINS {
  int ENA = 3;
  int ENB = 6;
  int IN1 = 5;
  int IN2 = 4;
  int IN3 = 8;
  int IN4 = 7;
  int SERVO1 = 9;
  int SERVO2 = 10;
  int SERVO3 = 11;
  int CH1 = A0;
  int CH2 = A1;
  int CH3 = A2;
  int CH4 = A3;
  int CH5 = A4;
  int CH6 = A5;
} pins;

struct RAW_CHANNELS {
  int ch1 = 0;
  int ch2 = 0;
  int ch3 = 0;
  int ch4 = 0;
  int ch5 = 0;
  int ch6 = 0;
  int min_ch1 = 3000, min_ch2 = 3000, min_ch3 = 3000, min_ch4 = 3000, min_ch5 = 3000, min_ch6 = 3000;
  int max_ch1 = 0, max_ch2 = 0, max_ch3 = 0, max_ch4 = 0, max_ch5 = 0, max_ch6 = 0;
} raw;

Servo servo1, servo2;

int speed_left = 0;
int speed_right = 0;

const int basespeed = 50;
const int maxspeed = 255;
const float mixer_c = (maxspeed - basespeed) / 500.0;

const float gripperspeed = 0.5;
float gripperfloat = 90;
int val_servo1 = 180;
int val_servo2 = 90;

const float alpha = 0.2;  // Low pass filter constant, tune this for optimal smoothness

int new_ch2, new_ch3, new_ch4, new_ch6;
void read_channels() {
  // Read raw pulse data
  raw.ch1 = pulseIn(pins.CH1, HIGH);  // Direct mapping
  new_ch2 = pulseIn(pins.CH2, HIGH);
  new_ch3 = pulseIn(pins.CH3, HIGH);
  new_ch4 = pulseIn(pins.CH4, HIGH);
  raw.ch5 = pulseIn(pins.CH5, HIGH);  // Direct mapping
  new_ch6 = pulseIn(pins.CH6, HIGH);
  
  // Apply low pass filter to smooth input values for channels 2, 3, 4, and 6
  raw.ch2 = alpha * new_ch2 + (1 - alpha) * raw.ch2;
  raw.ch3 = alpha * new_ch3 + (1 - alpha) * raw.ch3;
  raw.ch4 = alpha * new_ch4 + (1 - alpha) * raw.ch4;
  raw.ch6 = alpha * new_ch6 + (1 - alpha) * raw.ch6;

  // Directly map channels 1 and 5 without filtering
  // raw.ch1 = new_ch1;
  // raw.ch5 = new_ch5;
}


void calibrate() {
  while (pulseIn(pins.CH5, HIGH) == 0) {
    Serial.println("Receiver not connected! ");
    delay(1000);
  }
  if (pulseIn(pins.CH5, HIGH) < 1800) {
    raw.min_ch1 = -510, raw.min_ch2 = -510, raw.min_ch3 = 990, raw.min_ch4 = 990, raw.min_ch5 = 1000, raw.min_ch6 = 1000;
    raw.max_ch1 = 485, raw.max_ch2 = 485, raw.max_ch3 = 1980, raw.max_ch4 = 1980, raw.max_ch5 = 1970, raw.max_ch6 = 1980;
    Serial.println("Default calibration values used");
    return;
  }

  Serial.println("Caliberating...");

  while (pulseIn(pins.CH5, HIGH) > 1800) {
    read_channels();
    print_channels();
    raw.min_ch1 = min(raw.min_ch1, raw.ch1);
    raw.max_ch1 = max(raw.max_ch1, raw.ch1);
    raw.min_ch2 = min(raw.min_ch2, raw.ch2);
    raw.max_ch2 = max(raw.max_ch2, raw.ch2);
    raw.min_ch3 = min(raw.min_ch3, raw.ch3);
    raw.max_ch3 = max(raw.max_ch3, raw.ch3);
    raw.min_ch4 = min(raw.min_ch4, raw.ch4);
    raw.max_ch4 = max(raw.max_ch4, raw.ch4);
    raw.min_ch5 = min(raw.min_ch5, raw.ch5);
    raw.max_ch5 = max(raw.max_ch5, raw.ch5);
  }


  raw.min_ch1 -= 1500;
  raw.max_ch1 -= 1500;
  raw.min_ch2 -= 1500;
  raw.max_ch2 -= 1500;
  raw.max_ch3 -= 10;
  raw.min_ch3 += 10;

  Serial.println("Caliberated Vales:");
  Serial.println("Min: " + String(raw.min_ch1) + " " + String(raw.min_ch2) + " " + String(raw.min_ch3) + " " + String(raw.min_ch4) + " " + String(raw.min_ch5) + " ");
  Serial.println("Max: " + String(raw.max_ch1) + " " + String(raw.max_ch2) + " " + String(raw.max_ch3) + " " + String(raw.max_ch4) + " " + String(raw.max_ch5) + " ");
}

void print_channels() {
  Serial.print("CH1: ");
  Serial.print(raw.ch1);
  Serial.print(" CH2: ");
  Serial.print(raw.ch2);
  Serial.print(" CH3: ");
  Serial.print(raw.ch3);
  Serial.print(" CH4: ");
  Serial.print(raw.ch4);
  Serial.print(" CH5: ");
  Serial.print(raw.ch5);
  Serial.print(" CH6: ");
  Serial.print(raw.ch6);
  Serial.print(" SL: ");
  Serial.print(speed_left);
  Serial.print(" SR: ");
  Serial.print(speed_right);
  Serial.print(" Servo1: ");
  Serial.print(val_servo1);
  Serial.print(" Servo2: ");
  Serial.print(val_servo2);
  Serial.println();
}

void map_data() {
  // Mapping the motors
  int ch1 = raw.ch4 - 1500;
  int ch2 = raw.ch2 - 1500;

  if (ch1 > -20 and ch1 < 20 and ch2 > -20 and ch2 < 20) {
    speed_left = 0;
    speed_right = 0;
  } else {
    ch1 = (ch1 > 0) ? (ch1 * 500.0) / raw.max_ch1 : (ch1 * (-500.0)) / raw.min_ch1;
    ch2 = (ch2 > 0) ? (ch2 * 500.0) / raw.max_ch2 : (ch2 * (-500.0)) / raw.min_ch2;

    ch1 = max(-500, min(500, ch1));
    ch2 = max(-500, min(500, ch2));

    speed_left = mixer_c * ch1 + mixer_c * ch2;
    speed_right = -mixer_c * ch1 + mixer_c * ch2;

    speed_left += (speed_left > 0) ? basespeed : -basespeed;
    speed_right += (speed_right > 0) ? basespeed : -basespeed;
  }

  speed_left = max(-255, min(255, speed_left));
  speed_right = max(-255, min(255, speed_right));

  // Mapping arm servo
  val_servo1 = map(raw.ch3, raw.min_ch3, raw.max_ch3, 180, 0);
  val_servo1 = max(0, min(180, val_servo1));

  val_servo2 = map(raw.ch6, raw.min_ch6, raw.max_ch6, 0, 180);
  val_servo2 = max(0, min(180, val_servo2));

  // int factor = (raw.ch4-1500)/100;
  // gripperfloat += factor*gripperspeed;
  // gripperfloat = min(180, max(0, gripperfloat));

  // val_servo2 = gripperfloat;


  // Serial.println("Ch1: " + String(raw.ch1) + " " + String(ch1) + " " "Ch2: " + String(raw.ch2) + " " + String(ch2) + " mapped: " + String(speed_left) + " " + String(speed_right));
  // Serial.print("Speed Left: ");
  // Serial.print(speed_left);
  // Serial.print(" Speed Right: ");
  // Serial.print(speed_right);
  // Serial.println();
}

void write_data() {
  if (speed_left > 0) {
    digitalWrite(pins.IN1, HIGH);
    digitalWrite(pins.IN2, LOW);
    analogWrite(pins.ENA, speed_left);
  } else {
    digitalWrite(pins.IN1, LOW);
    digitalWrite(pins.IN2, HIGH);
    analogWrite(pins.ENA, -speed_left);
  }
  if (speed_right > 0) {
    digitalWrite(pins.IN3, HIGH);
    digitalWrite(pins.IN4, LOW);
    analogWrite(pins.ENB, speed_right);
  } else {
    digitalWrite(pins.IN3, LOW);
    digitalWrite(pins.IN4, HIGH);
  
  
    analogWrite(pins.ENB, -speed_right);
  }
  servo1.write(val_servo1);
  servo2.write(val_servo2);
}

void setup() {
  pinMode(pins.ENA, OUTPUT);
  pinMode(pins.ENB, OUTPUT);
  pinMode(pins.IN1, OUTPUT);
  pinMode(pins.IN2, OUTPUT);
  pinMode(pins.IN3, OUTPUT);
  pinMode(pins.IN4, OUTPUT);
  pinMode(pins.SERVO1, OUTPUT);
  pinMode(pins.SERVO2, OUTPUT);
  pinMode(pins.SERVO3, OUTPUT);
  pinMode(pins.CH1, INPUT);
  pinMode(pins.CH2, INPUT);
  pinMode(pins.CH3, INPUT);
  pinMode(pins.CH4, INPUT);
  pinMode(pins.CH5, INPUT);
  pinMode(pins.CH6, INPUT);
  Serial.begin(115200);

  servo1.attach(pins.SERVO1);
  servo2.attach(pins.SERVO2);

  calibrate();
}

void loop() {
  read_channels();
  map_data();
  // print_channels();
  write_data();
  // delay(500);

  // delay(5);
}

/*
ch1, 2
990



1985

ch3 985 - 1976
ch4 976 - 1985
*/