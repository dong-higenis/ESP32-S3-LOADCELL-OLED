//////////////////// include hx711 /////////////////////
#include <Arduino.h>
#include "HX711.h"

//////////////////// include oled /////////////////////
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//////////////////// include web /////////////////////
#include <WiFi.h>
#include "ESPAsyncWebServer.h"

///////////////////// init hx711 //////////////////////
const int LOADCELL_DOUT_PIN = 21;
const int LOADCELL_SCK_PIN  = 35;

HX711 scale;

///////////////////// init oled //////////////////////
// OLED 디스플레이 설정
#define SCREEN_WIDTH 128  // 디스플레이 너비
#define SCREEN_HEIGHT 64  // 디스플레이 높이
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SSD1306_I2C_ADDRESS 0x3C

// I2C 핀 설정
#define SDA_PIN 13
#define SCL_PIN 14

// LCD 전원 제어 핀
#define LCD_POWER_PIN 45

//////////////////// init web /////////////////////
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

float gram;

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer


void getHX711Readings(){
    gram = scale.get_units(10);
}

String processor(const String& var){
  getHX711Readings();
  //Serial.println(var);
  if(var == "GRAM"){
    return String(gram);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>HX711 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v6.7.0/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #4B1D3F; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .card.gram { color: #0e7c7b; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>HX711 WEB SERVER</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card gram">
        <h4><i class="fa-solid fa-weight-scale"></i> Gram</h4><p><span class="reading"><span id="gram">%GRAM%</span> g</span></p>
      </div>
    </div>
  </div>
    <script>
    if (!!window.EventSource) {
        var source = new EventSource('/events');
        
        source.addEventListener('open', function(e) {
        console.log("Events Connected");
        }, false);
        source.addEventListener('error', function(e) {
        if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
        }
        }, false);
        
        source.addEventListener('message', function(e) {
        console.log("message", e.data);
        }, false);
        
        source.addEventListener('gram', function(e) {
        console.log("gram", e.data);
        document.getElementById("gram").innerHTML = e.data;
        }, false);
    }
    </script>
</body>
</html>)rawliteral";


/***********************************
 * Setup
 ***********************************/
void setup() {
  Serial.begin(115200);
  //////////////////////// setup hx711 /////////////////////////
  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(-419.855);
  scale.tare();               // reset the scale to 0


  ////////////////////// setup Async Web Server ///////////////////////
  WiFi.mode(WIFI_AP_STA);
  // WiFi.mode(WIFI_STA);

  Serial.print("Connecting to ");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }

    client->send("hello!", NULL, millis(), 1000);
  });

  server.addHandler(&events);
  server.begin();

  //////////////////////// setup OLED /////////////////////////
  Serial.println("=== Setup OLED ==="); 
    // LCD 전원 제어 핀 설정
  pinMode(LCD_POWER_PIN, OUTPUT);
  digitalWrite(LCD_POWER_PIN, HIGH); // LCD 전원 켜기
  
  // I2C 초기화
  Wire.begin(SDA_PIN, SCL_PIN);

  // 디스플레이 초기화
  if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS, false, false)) {
    Serial.println("SSD1306 allocation failed");
    while (true); // 실패 시 멈춤
  }

  // 화면 초기화
  display.clearDisplay();
  display.setTextSize(2);              // 글자 크기 설정 (2배)
  display.setTextColor(SSD1306_WHITE); // 흰색 텍스트
  display.setCursor(0, 0);             // 텍스트 시작 위치
  display.println("IP: ");             // 텍스트 출력
  display.println(WiFi.localIP());
  display.display();                   // 화면에 표시
  delay(3000);
}

/***********************************
 * Loop
 ***********************************/
void loop() {
  ////////////////////// loop scale ////////////////////////
  // Serial.println("=== Loop scale ===");
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 5);

  scale.power_down();             // put the ADC in sleep mode
  delay(2000);
  scale.power_up();

////////////////////// loop OLED ////////////////////////
  // Serial.println("=== Loop OLED ===");
  display.clearDisplay();
  display.setTextSize(2);                  // 글자 크기 설정
  display.setTextColor(SSD1306_WHITE);     // 흰색 텍스트
  display.setCursor(0, 0);                 // 텍스트 시작 위치
  display.println("scale: ");              // 텍스트 출력
  display.println(scale.get_units(10), 2); // 텍스트 출력
  display.display();                       // 화면에 표시

  ///////////////////// loop Async Web ///////////////////////
  if ((millis() - lastTime) > timerDelay) {
    getHX711Readings();
    
    // Send Events to the Web Server with the Sensor Readings
    events.send("ping", NULL, millis());
    events.send(String(gram).c_str(), "gram", millis());
    lastTime = millis();
  }
}