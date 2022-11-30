#include <TimerOne.h>


int s = 1;
int t1 = 2000;
int t2 = 2000;
//int t2 = 2050;

void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  Timer1.initialize(t1);
  Timer1.attachInterrupt(led);
  attachInterrupt(digitalPinToInterrupt(2), run1, RISING);
}

void loop() {
  Serial.println("HELLO");
}
void run1()
{
  Timer1.setPeriod(t1);
}

void led()
{
  while (1) {
    Serial.println("zdc on\t");
    digitalWrite(4, HIGH);
    delayMicroseconds(5);
    Timer1.stop();
    digitalWrite(4, LOW);
  }

  //  test();
}

void run2()
{
  while (1) {
    Serial.println("zdc on\t");
    delayMicroseconds(t2);
    digitalWrite(4, HIGH);
    delayMicroseconds(5);
    digitalWrite(4, LOW);
  }

}
