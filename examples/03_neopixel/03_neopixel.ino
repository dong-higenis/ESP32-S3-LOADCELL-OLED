#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN 38   // 버튼이 연결된 핀 번호
#define LED_PIN 36      // WS2812B 데이터 핀
#define NUM_LEDS 2      // 사용 중인 WS2812B LED의 개수

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 풀업 저항 활성화
  strip.begin();                    // NeoPixel 초기화
  strip.show();                     // LED를 초기 상태로 설정 (모두 끄기)
}

void loop() {
  // 버튼 상태 읽기
  int buttonState = digitalRead(BUTTON_PIN);

  // 버튼이 눌렸을 때
  if (buttonState == LOW) {
    changeColor(strip.Color(255, 0, 0)); // 빨간색으로 변경
    delay(500);                          // 버튼 디바운싱 및 대기
  } else {
    // changeColor(strip.Color(0, 0, 255)); // 파란색으로 변경
    changeColor(strip.Color(0, 0, 0));
  }
}

// WS2812B의 색상을 변경하는 함수
void changeColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color); // 각 LED에 색상을 설정
  }
  strip.show(); // 변경 사항 적용
}
