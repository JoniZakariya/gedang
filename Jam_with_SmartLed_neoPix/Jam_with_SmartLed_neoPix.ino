#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include "RTClib.h"

#define DATA_PIN D5
#define DATA_PIN1 D6
#define SEGMENT 6
#define NUM_LEDS (SEGMENT * 7)
#define NUM_LEDS1 4
#define PIN_ALARM


RTC_DS3231 rtc;
Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1(NUM_LEDS1, DATA_PIN1, NEO_GRB + NEO_KHZ800);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
byte hsv[3] = {0, 255, 255};
byte h[3] = {NULL};
int sec;
bool colorRGB;

const char* ssid = "Wall Clock";
const char* password = "digitalclock";
ESP8266WebServer server(80); //Server on port 80

const char MAIN[] PROGMEM = R"====(
<!DOCTYPE html>
<html lang = "en">
<title>setting datetime</title>
<head>
  <meta charset = "UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  .container {
  font-size: 5vw;
    width: 100%;
    min-height: 200px;
  }
</style>
</head>

<body>
  <div class = "container">
    <form class = "date" action = "/get">
    Date : <br>
    <input class = "date" type = "date" name = "date">
    <br>
    Time : <br>
    <input class = "time" type = "time" name = "time">
    <br>
    <input class = "in" type="submit" value="send">
    <br></form>
  </div>
</body>
</html>
)====";

void first(){
  server.send(200, "text/html", MAIN);
}
void last(){
  if(server.hasArg("date")){
    String dateTime = server.arg("date") + " " + server.arg("time");
    Serial.println(dateTime);
    int dataNum[5] = {dateTime.substring(0, 4).toInt(), 
                      dateTime.substring(5, 7).toInt(),
                      dateTime.substring(8, 10).toInt(),
                      dateTime.substring(11, 13).toInt(),
                      dateTime.substring(14).toInt()};
    Serial.println(dataNum[0]);
    Serial.println(dataNum[1]);
    Serial.println(dataNum[2]);
    Serial.println(dataNum[3]);
    Serial.println(dataNum[4]);
    rtc.adjust(DateTime(dataNum[0], dataNum[1], dataNum[2], dataNum[3], dataNum[4], 0));
  }
 server.send(200, "text/html", MAIN);
 strip.clear();
 strip.show();
 for(byte i = 0; i < 11; i++){
  for(byte j = 0; j < 4; j++){
    strip1.setPixelColor(j, (i % 2)*0xfdf4dc); //warm white
  }
  strip1.show();
  delay(50);
 }
 delay(1000);
 Serial.println("success send data");
}
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  strip.begin();
  strip1.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip1.show(); // Initialize all pixels to 'off'
  rtc.begin();
  delay(2000);
//  strip1.setPixelColor(0, strip1.ColorHSV(40000, 255, 255));
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // rtc.adjust(DateTime(year, montnt, date, hour, minute, second));
  // Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.mode(WIFI_AP_STA);
  IPAddress apIP(192, 168, 0, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/", first);//Which routine to handle at root location
  server.on("/get", last);//form action is handled here

  server.begin();//Start server
}
void loop() {
  server.handleClient();
  
  DateTime t = rtc.now();
  byte clk[6] = {t.hour() / 10, t.hour() % 10, t.minute() / 10, t.minute() % 10, t.second() / 10, t.second() % 10};
  
  if (t.second() != sec) { 
    sec = t.second();
    //==============BRIGHTNESS=============
    if(t.hour() >= 7 && t.hour() < 18) hsv[2] = 255;
    if(t.hour() >= 18 && t.hour() < 22) hsv[2] = 100;
    if(t.hour() >= 22 || t.hour() < 7) hsv[2] = 10;
    
    //=========CHANGE COLOR RGB SEGMENT============
    if ((t.second() % 10 == 0) && colorRGB) {
      for (byte i = 0; i < 3; i++) h[i] = random(0, 256);
      colorRGB = false;
    }
    if((t.second()%10) != 0) colorRGB = true;
    
    clkToSegment(clk, h, hsv);
    strip.show();
    ledPembatas(t.second(), hsv[2]);
    strip1.show();
    Serial.print(t.year(), DEC);
    Serial.print('/');
    Serial.print(t.month(), DEC);
    Serial.print('/');
    Serial.print(t.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[t.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(t.hour(), DEC);
    Serial.print(':');
    Serial.print(t.minute(), DEC);
    Serial.print(':');
    Serial.print(t.second(), DEC);
    Serial.println();
  }
  delay(20);
}

void alarm() {
	//masih malesss buat
}
void ledPembatas(int num, byte val) {
  byte r = random(0, 255);
  if (num % 2 == 0) {
    for (byte i = 0; i < NUM_LEDS1; i++) {
      uint16_t h = map(r, 0, 255, 0, 65535);
      strip1.setPixelColor(i, strip1.ColorHSV(h, 255, val * (i % 2)));
    }
  }
  else {
    for (byte i = 0; i < NUM_LEDS1; i++) {
      uint16_t h = map(r, 0, 255, 0, 65535);
      strip1.setPixelColor(i, strip1.ColorHSV(h, 255, val * (!(i % 2))));
    }
  }
}
void clkToSegment(byte *clk, byte *hue, byte *hsv) {
  byte bin[10] = {
    0b0111111,//0
    0b0000110,//1
    0b1011011,//2
    0b1001111,//3
    0b1100110,//4
    0b1101101,//5
    0b1111101,//6
    0b0000111,//7
    0b1111111,//8
    0b1101111,//9
  };
  for (byte i = 0; i < SEGMENT; i++) {
    *hsv = *hue;
    valueToSegment(bin[*(clk + i)], i, hsv);
    hue += (i % 2);
  }
}
void valueToSegment(byte bin, byte seg, byte *hsv) {
  for (byte i = 0; i < 7; i++) {
    byte state = (bin >> i) & 1;
    uint16_t h = map(*hsv, 0, 255, 0, 65535);
    strip.setPixelColor(i + (seg * 7), strip.ColorHSV(h, *(hsv + 1), state * (*(hsv + 2))));
  }
}
