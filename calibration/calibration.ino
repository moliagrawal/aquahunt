struct PINS {
    int ENA = 5;
    int ENB = 6;
    int IN1 = 3;
    int IN2 = 4;
    int IN3 = 7;
    int IN4 = 8;
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
    int min_ch1 = 3000, min_ch2 = 3000, min_ch3 = 3000, min_ch4 = 3000, min_ch5 = 3000;
    int max_ch1 = 0, max_ch2 = 0, max_ch3 = 0, max_ch4 = 0, max_ch5 = 0;
} raw;

int speed_left = 0;
int speed_right = 0;

const int base_speed = 55;
const int max_speed = 255;
const float mixer_c = (max_speed - base_speed)/500.0;

void read_channels(){
    raw.ch1 = pulseIn(pins.CH1, HIGH); 
    raw.ch2 = pulseIn(pins.CH2, HIGH); 
    raw.ch3 = pulseIn(pins.CH3, HIGH); 
    raw.ch4 = pulseIn(pins.CH4, HIGH);
    raw.ch5 = pulseIn(pins.CH5, HIGH);
    raw.ch6 = pulseIn(pins.CH6, HIGH); 
}

void calibrate() {
  while(pulseIn(pins.CH6, HIGH) == 0) {
    Serial.println("Receiver not connected! ");
    delay(1000);
  }
  if(pulseIn(pins.CH6, HIGH) < 1800) {
    raw.min_ch1 = 1000, raw.min_ch2 = 1000, raw.min_ch3 = 1000, raw.min_ch4 = 1000, raw.min_ch5 = 1000;
    raw.max_ch1 = 2000, raw.max_ch2 = 2000, raw.max_ch3 = 2000, raw.max_ch4 = 2000, raw.max_ch5 = 2000;
    Serial.println("Default calibration values used");
    return;
  }

  Serial.println("Caliberating...");

  while(pulseIn(pins.CH6, HIGH) > 1800){
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

  Serial.println("Caliberated Vales:");
  Serial.println("Min: " + String(raw.min_ch1) + " " + String(raw.min_ch2) + " " + String(raw.min_ch3) + " " + String(raw.min_ch4) + " " + String(raw.min_ch5) + " ");
  Serial.println("Max: " + String(raw.max_ch1) + " " + String(raw.max_ch2) + " " + String(raw.max_ch3) + " " + String(raw.max_ch4) + " " + String(raw.max_ch5) + " ");
}

void print_channels(){
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
    Serial.println();
}

void map_data() {

  int ch1 = raw.ch1-1500;
  int ch2 = raw.ch2-1500;

  ch1 = (ch1 > 0) ? (ch1*500.0)/raw.max_ch1 : (ch1*(-500.0))/raw.min_ch1;
  ch2 = (ch2 > 0) ? (ch2*500.0)/raw.max_ch2 : (ch2*(-500.0))/raw.min_ch2;

  ch1 = max(-500, min(500, ch1));
  ch2 = max(-500, min(500, ch2));


  speed_left = mixer_c * ch1 + mixer_c * ch2 + base_speed;
  speed_right = -mixer_c * ch1 + mixer_c * ch2 + base_speed;
  
  Serial.println("Ch1: " + String(raw.ch1) + " " + String(ch1) + " " "Ch2: " + String(raw.ch2) + " " + String(ch2) + " mapped: " + String(speed_left) + String(speed_right));

    // speed_left = map(raw.ch1, 1000, 2000, -255, 255);
    // speed_right = map(raw.ch2, 1000, 2000, -255, 255);
    speed_left = max(-255, min(255, speed_left));
    speed_right = max(-255, min(255, speed_right));
    Serial.print("Speed Left: ");
    Serial.print(speed_left);
    Serial.print(" Speed Right: ");
    Serial.print(speed_right);
    Serial.println();
}

void write_data() {
    if(speed_left > 0) {
        digitalWrite(pins.IN1, HIGH);
        digitalWrite(pins.IN2, LOW);
        analogWrite(pins.ENA, speed_left);
    } else {
        digitalWrite(pins.IN1, LOW);
        digitalWrite(pins.IN2, HIGH);
        analogWrite(pins.ENA, -speed_left);
    }
    if(speed_right > 0) {
        digitalWrite(pins.IN3, HIGH);
        digitalWrite(pins.IN4, LOW);
        analogWrite(pins.ENB, speed_right);
    } else {
        digitalWrite(pins.IN3, LOW);
        digitalWrite(pins.IN4, HIGH);
        analogWrite(pins.ENB, -speed_right);
    }
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
    Serial.begin(9600);

    calibrate();
}

void loop() {
    read_channels();
    // print_channels();
    map_data();
    delay(500);
}