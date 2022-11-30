#include "FastLED.h"

#define PIN_IN1 4
#define PIN_IN2 5
#define DATA_PIN 8
#define NUM_LEDS 18
#define LENGTH_SIGN 4
#define SPEED_SIGN 200
#define SPEED_OUT 1000

#define BLITZ 0
#define RIGHT 1
#define LEFT 2
#define HAZARD 3

CRGB leds[NUM_LEDS];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIN_IN1, INPUT_PULLUP);
  pinMode(PIN_IN2, INPUT_PULLUP);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (readIn() == HAZARD)
    hazard();
  else if (readIn() == RIGHT)
    rightSign();
  else if (readIn() == LEFT)
    leftSign();
  else
    blitz();
}
byte readIn() {
  if ((!digitalRead(PIN_IN1)) && (!digitalRead(PIN_IN2)))
    return HAZARD;
  else if (!digitalRead(PIN_IN1))
    return RIGHT;
  else if (!digitalRead(PIN_IN2))
    return LEFT;
  else
    return BLITZ;
}
void blitz() {
  byte chsv[3] = {random(0, 256), 255, 255};
  set_all_led(chsv);
  FastLED.show();
  for (int i = 0; i < 60; i++) {
    if (readIn() != BLITZ) {
      break;
    }
    delay(1);
  }
  chsv[2] = 0;
  set_all_led(chsv);
  FastLED.show();
  int r = random(1500, 4000);
  for (int i = 0; i < r; i++) {
    if (readIn() != BLITZ) {
      break;
    }
    delay(1);
  }
}
void hazard() {
  byte chsv[3] = {random(0, 256), 255, 255};
  int i = (NUM_LEDS / 2);
  int t = 0;
  int j = (NUM_LEDS / 2) - 1;
  while (1)
  {
    if (readIn() != HAZARD)
    {
      if (t > SPEED_OUT) {
        FastLED.clear();
        FastLED.show();
        break;
      }
    }
    else
    {
      t = 0;
    }
    chsv[2] = 0;
    set_led(j-LENGTH_SIGN, chsv);
    set_led(i+LENGTH_SIGN, chsv);
    chsv[2] = 255;
    for (byte side = (NUM_LEDS / 2) - (LENGTH_SIGN / 2); side < (NUM_LEDS / 2) + (LENGTH_SIGN / 2); side++) {
      set_led(side, chsv);
    }
    set_led(j, chsv);
    set_led(i, chsv);
    FastLED.show();
    for (int x = 0; x < 25; x++)
    {
      t++;
      delay(1);
    }
    j++;
    i--;
    if (j >= NUM_LEDS + LENGTH_SIGN) {
      chsv[0] = random(0, 256);
      j = (NUM_LEDS / 2) - 1;
      i = (NUM_LEDS / 2);
    }
  }

}
void rightSign() {
  byte chsv[3] = {random(0, 256), 255, 255};
  int t = 0;
  int i = 0;
  while (1) {
    if (readIn() != RIGHT)
    {
      if (t > SPEED_OUT) {
        FastLED.clear();
        FastLED.show();
        break;
      }
    }
    else
    {
      t = 0;
    }
    chsv[2] = 0;
    set_led(i - LENGTH_SIGN, chsv);
    chsv[2] = 255;
    set_led(i, chsv);
    FastLED.show();
    for (int x = 0; x < 15; x++)
    {
      t++;
      delay(1);
    }
    i++;
    if (i >= NUM_LEDS + LENGTH_SIGN) {
      chsv[0] = random(0, 256);
      i = 0;
    }
  }
}
void leftSign() {
  byte chsv[3] = {random(0, 256), 255, 255};
  int t = 0;
  int i = NUM_LEDS;
  while (1) {
    if (readIn() != LEFT)
    {
      if (t > SPEED_OUT) {
        FastLED.clear();
        FastLED.show();
        break;
      }
    }
    else
    {
      t = 0;
    }
    chsv[2] = 0;
    set_led(i + LENGTH_SIGN, chsv);
    chsv[2] = 255;
    set_led(i, chsv);
    FastLED.show();
    for (int x = 0; x < 15; x++)
    {
      t++;
      delay(1);
    }
    i--;
    if (i < -LENGTH_SIGN) {
      chsv[0] = random(0, 256);
      i = NUM_LEDS;;
    }
  }
}
void set_all_led(byte *chsv) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    set_led(i, chsv);
  }
}
void set_led(byte c, byte *chsv) {
  if (c >= 0 && c < NUM_LEDS) {
    leds[c] = CHSV(*chsv, *(chsv + 1), *(chsv + 2));
  }
}
