struct PINS {
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
    // Serial.print(" CH4: ");
    // Serial.print(raw_channels.ch4);
    // Serial.print(" CH5: ");
    // Serial.print(raw_channels.ch5);
    // Serial.print(" CH6: ");
    // Serial.print(raw_channels.ch6);
    Serial.println();
}

void setup() {
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
    print_channels();
    delay(200);
}