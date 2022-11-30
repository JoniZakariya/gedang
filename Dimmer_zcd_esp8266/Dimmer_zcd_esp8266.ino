#define LED D6
#define ZCD D7

int t_zcd = 5000;

ICACHE_RAM_ATTR void zcd()
{
  digitalWrite(LED, LOW);
  delayMicroseconds(t_zcd);
  digitalWrite(LED, HIGH);
  delayMicroseconds(10);
  digitalWrite(LED, LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  pinMode(LED, OUTPUT);
  //  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ZCD, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ZCD), zcd, RISING);
  //  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

}

void loop() {

}
