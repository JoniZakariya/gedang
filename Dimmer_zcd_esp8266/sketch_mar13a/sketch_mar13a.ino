#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define LED D6
#define ZCD D7
#define MAX 1500
#define MIN 8900
#define MEDIUM 5000
int t_zcd = MEDIUM;
// Replace with your network credentials
const char* ssid = "AAMIIIN";
const char* password = "--------";

bool state = 0;

const char* PARAM_INPUT = "value";
ICACHE_RAM_ATTR void zcd() {
  state = 1;
}
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>TERBAIK</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #00FFFF;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #7FFF00; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #7FFF00; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>ESP8266 Acces Point</h2>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="100" value="%SLIDERVALUE%" step="1" class="slider"></p>
<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  return String();
}

void setup(){
  pinMode(LED, OUTPUT);
  pinMode(ZCD, INPUT);
  attachInterrupt(digitalPinToInterrupt(ZCD), zcd, FALLING);
 
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
      inputMessage = request->getParam(PARAM_INPUT)->value();
      int m = map(inputMessage.toInt(), 0, 100, MIN, MAX);
      t_zcd = m;
    request->send(200, "text/plain", "OK");
  });
  server.begin();
  
}
  
void loop() {
  digitalWrite(LED, 0);
  for(state; state == 1; ){
    delayMicroseconds(t_zcd);
    digitalWrite(LED, 1);
    delayMicroseconds(15);
    state = 0;
    }
}
