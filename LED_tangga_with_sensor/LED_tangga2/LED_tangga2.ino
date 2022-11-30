#define PIN_SENSOR_A1 A0
#define PIN_SENSOR_A2 A1
#define PIN_SENSOR_B1 A2
#define PIN_SENSOR_B2 A3

#define NUM_L 10
#define INTERVAL 250
#define TIMEOUT 20000
#define NUM_RUN_LED 3

#define SENSOR1 1
#define SENSOR2 2

#define NAIK 1
#define TURUN 0
String encdir = "";
unsigned long lastTime;
int counter = 0;
bool currentStateCLK;
bool previousStateCLK = 0;
byte leds[NUM_L] = { 13, 12, 11, 10, 9, 8, 7, 6, 5, 4 };
//byte leds[NUM_L] = {
//  13, 12, 11, 10, 9, 8,
//  7, 6, 5, 4, 3, 2, 14,
//  15, 22, 24, 26, 28, 30,
//  32, 34, 36, 38, 40, 42,
//  44
//};

void setup() {
  Serial.begin (9600);
  // put your setup code here, to run once:
  pinMode(PIN_SENSOR_A1, INPUT);
  pinMode(PIN_SENSOR_A2, INPUT);
  pinMode(PIN_SENSOR_B1, INPUT_PULLUP);
  pinMode(PIN_SENSOR_B2, INPUT_PULLUP);
  for (byte i = 0; i < NUM_L; i++) {
    pinMode(leds[i], OUTPUT);
  }
}
byte pen = 0;
void loop() {
  //  for (int i = 0; i < NUM_L; i++) {
  //    stateLed(i, 1);
  //    delay(80);
  //    stateLed(i, 0);
  //  }
  bool state = 1;
  bool st = 0;
  while (digitalRead(PIN_SENSOR_A1)) {
    st = 1;
  }
  if (st == 1) {
    pen++;
  }
  if (pen >= 3 ) pen = 0;
  ledNaik(pen);
  Serial.println(pen);
  //  currentStateCLK = digitalRead(PIN_SENSOR_A1);
  //  if (currentStateCLK != previousStateCLK) {
  //    if (digitalRead(PIN_SENSOR_A2) != currentStateCLK) {
  //      encdir = "MASUK";
  //    } else {
  //      encdir = "KELUAR";
  //    }
  //    Serial.print("Direction: ");
  //    Serial.println(encdir);
  //  }
  //  previousStateCLK = currentStateCLK;

}
void ledNaik(byte in) {
  static int i = 0;
  static int _state = 0;
  static byte u[3] = {0};
  if (millis() - lastTime >= INTERVAL ) {
    lastTime = millis();
    if (_state == 0) {
      for (byte p = 0; p <= in; p++) {
        if (u[p] > NUM_L) {
          u[i] = 0;
          i++;
        }
        else if (i >= in) i = 0;
        stateLed(u[p] - NUM_RUN_LED, 0);
        stateLed(u[p] , 1);
        u[p]++;
      }
      i++;
      _state = 1;
    }
    else _state = 0;
  }
}

void ledTurun() {

}

void stateLed(int _i, byte l) {
  if (_i >= 0 && _i <= NUM_L) {
    digitalWrite(leds[_i], l);
  }
}
