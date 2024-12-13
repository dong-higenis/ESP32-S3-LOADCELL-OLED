#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

void setup() {
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
  display.setTextSize(2);      // 글자 크기 설정 (2배)
  display.setTextColor(SSD1306_WHITE); // 흰색 텍스트
  display.setCursor(0, 0);     // 텍스트 시작 위치
  display.println("Hello,\nHiGenis!!!"); // 텍스트 출력
  display.display();           // 화면에 표시
}

void loop() {
  // 무한 루프에서 아무 작업도 하지 않음
}
