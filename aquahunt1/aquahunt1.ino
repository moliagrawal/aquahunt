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
    int CH1 = A1;
    int CH2 = A2;
    int CH3 = A3;
    int CH4 = A4;
    int CH5 = A5;
    int CH6 = A0;
} pins;

struct RAW_CHANNELS {
    int ch1 = 0;
    int ch2 = 0;
    int ch3 = 0;
    int ch4 = 0;
    int ch5 = 0;
    int ch6 = 0;
} raw_channels;

int speed_left = 0;
int speed_right = 0;

void read_channels(){
    raw_channels.ch1 = pulseIn(pins.CH1, HIGH); 
    raw_channels.ch2 = pulseIn(pins.CH2, HIGH); 
    raw_channels.ch3 = pulseIn(pins.CH3, HIGH); 
    raw_channels.ch4 = pulseIn(pins.CH4, HIGH);
    // raw_channels.ch5 = pulseIn(pins.CH5, HIGH); 
    // raw_channels.ch6 = pulseIn(pins.CH6, HIGH); 
}

void print_channels(){
    Serial.print("CH1: ");
    Serial.print(raw_channels.ch1);
    Serial.print(" CH2: ");
    Serial.print(raw_channels.ch2);
    Serial.print(" CH3: ");
    Serial.print(raw_channels.ch3);
    Serial.print(" CH4: ");
    Serial.print(raw_channels.ch4);
    // Serial.print(" CH5: ");
    // Serial.print(raw_channels.ch5);
    // Serial.print(" CH6: ");
    // Serial.print(raw_channels.ch6);
    Serial.println();
}

void map_data() {
    speed_left = map(raw_channels.ch1, 1000, 2000, -255, 255);
    speed_right = map(raw_channels.ch2, 1000, 2000, -255, 255);
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
}

void loop() {
    read_channels();
    // print_channels();
    map_data();
    delay(100);
}