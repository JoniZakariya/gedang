/* Create a WiFi access point and provide a web server on it. */
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define APSSID "SEGO"
#define APPSK  "12345678"

bool state = 0;

ESP8266WebServer server(80);
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(APSSID, APPSK);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", response);
  server.on("/LEDON", handleLedOn);
  server.on("/LEDOff", handleLedOff);

  server.begin();
  Serial.println("HTTP server started");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  server.handleClient();
  //  digitalWrite(LED_BUILTIN, HIGH);
  //  delay(5000);
  //  digitalWrite(LED_BUILTIN, LOW);
  //  delay(500);
}

void response()
{
  String htmlRes = "<!DOCTYPE html><head><title>Mas Joni</title><meta name = \"viewport\" content = \"width=device-width, intitial-sclae=1\" /></head>"
  "<div class=\"rainbow-text\" style=\"text-align: center;\"> <span class=\"block-line\"><span><span style=\"color:#ff0000;\">C</span><span style=\"color:#cc0000;\">O</span><span style=\"color:#990000;\">N</span><span style=\"color:#660000;\">T</span><span style=\"color:#330000;\">R</span><span style=\"color:#000000;\">O</span><span style=\"color:#003300;\">L&nbsp;</span></span><span><span style=\"color:#006600;\">L</span><span style=\"color:#009900;\">E</span><span style=\"color:#00cc00;\">D</span></span></span> </div>"
  "<big>LED CONDITION = </big>";
  if (state)
    htmlRes += "<b><span style=\"color:Red;\">ON</b></span><br/>\n";
  else
    htmlRes += "<b>OFF</b><br/>\n";

  htmlRes += "<a href=\"LEDON\"><button style=\"display: block; color: red; width: 100%;\"><b>ON</b></button></a><br/>"
  "<a href=\"LEDOff\"><button style=\"display: block; color: brown; width: 100%;\"><b>OFF</b></button></a><br/>";
  htmlRes += "</html>";

  server.send(200, "text/html", htmlRes);
}
void handleLedOn() {
  digitalWrite(LED_BUILTIN, LOW);
  state = 1;
  response();
}

void handleLedOff() {
  digitalWrite(LED_BUILTIN, HIGH);
  state = 0;
  response();
}
