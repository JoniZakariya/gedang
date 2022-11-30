
#define KP 0
#define KI 0
#define KD 0
#define SP 150
#define DT 100

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

float PID(float setPoint, float input, float deltaTime) {
  static float integral;
  static float derivatif;
  static int previousErr;
  static unsigned long timeNow = 0;
  float interval = float (millis() - timeNow);
  if (interval >= deltaTime) {
    // ERROR
    float err = setPoint - input;

    // PROPORSIONAL
    float P = KP * err;

    // INTEGRAL
    integral += err * (deltaTime / 1000);
    float I = KI * integral;

    // DERIVATIF
    derivatif = err - previousErr;
    float D = KD * derivatif / (deltaTime / 1000);
    previousErr = err;
    
    return P + I + D;
  }
}
