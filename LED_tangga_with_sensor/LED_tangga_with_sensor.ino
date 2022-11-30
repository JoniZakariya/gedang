#define PIN_SENSOR_A A0
#define PIN_SENSOR_B A1

#define NUM_L 26
#define INTERVAL 80
#define TIMEOUT 20000
#define NUM_RUN_LED 4

#define SENSOR1 1
#define SENSOR2 2

#define NAIK 1
#define TURUN 0

unsigned long lastTime;

byte leds[NUM_L] = {
13, 12, 11, 10, 9, 8,
7, 6, 5, 4, 3, 2, 14,
15, 22, 24, 26, 28, 30,
32, 34, 36, 38,40, 42,
44};

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  pinMode(PIN_SENSOR_A, INPUT_PULLUP);
  pinMode(PIN_SENSOR_B, INPUT_PULLUP);
  for (byte i = 0; i < NUM_L; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
//   put your main code here, to run repeatedly:
//  Serial.println("Hello World!");
  if (readState() == SENSOR1) light(0, NUM_L + NUM_RUN_LED, SENSOR2, NAIK);
  else if (readState() == SENSOR2) light(NUM_L, -NUM_RUN_LED, SENSOR1, TURUN);
  //  delay(5000);
}

void stateLed(byte _i, bool _logic) {
  if (_i >= 0 && _i < NUM_L)
    digitalWrite(leds[_i], _logic);
}

byte readState() {
  if (digitalRead(PIN_SENSOR_A) == 0) return SENSOR1;
  else if (digitalRead(PIN_SENSOR_B) == 0) return SENSOR2;
  else return 0;
}

void light(char  start, char end, byte sensor, bool login) {
  bool state = 0;
  bool logout = 0;
  unsigned long trigger = 0;
  char tukarA = (login) ? start : end; // Naik / 1
  char tukarB = (login) ? end : start; // Turun / 0
  while (1) {
    if (readState() == sensor) logout = 1;
    if (logout && tukarA > tukarB ) {
      while (readState() == sensor) delay(500);
      break;
    }
    else if (!logout && tukarA > tukarB ) {
      lastTime = millis();
      while (millis() - lastTime + trigger <=  TIMEOUT) {
        while (readState() == sensor) {
          trigger = 0;
          trigger += lastTime;
          delay(500);
        }
//        Serial.println("loop");
      }
      break;
    }
    if (millis() - lastTime >= INTERVAL ) {
      lastTime = millis();
      if (state == 0) {
        if (login) {
          stateLed(tukarA - NUM_RUN_LED, 0);
          stateLed(tukarA, 1);
          tukarA++;
        }
        else {
          stateLed(tukarB + NUM_RUN_LED, 0);
          stateLed(tukarB, 1);
          tukarB--;
        }
        state = 1;
      }
      else state = 0;
    }
  }
}
