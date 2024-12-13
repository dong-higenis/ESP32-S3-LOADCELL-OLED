#define BUTTON_PIN 38 // 버튼이 연결된 핀 번호
#define BUZZER_PIN 10 // 부저가 연결된 핀 번호

void setup() {
  // 버튼 핀을 입력으로 설정 (풀업 저항 활성화)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // 부저 핀을 출력으로 설정
  pinMode(BUZZER_PIN, OUTPUT);

  // 시리얼 모니터 초기화
  Serial.begin(115200);
}

void loop() {
  // 버튼 상태 읽기
  int buttonState = digitalRead(BUTTON_PIN);

  // 버튼이 눌렸을 때
  if (buttonState == LOW) {
    Serial.println("Button Pressed! Playing sound...");
    playTone(BUZZER_PIN, 1000, 500); // 1000Hz 주파수로 500ms 동안 소리 재생
  }
}

// 부저를 통해 소리 재생
void playTone(int pin, int frequency, int duration) {
  int period = 1000000 / frequency; // 주기의 길이 (마이크로초 단위)
  int pulseWidth = period / 2;     // 듀티 사이클 (50%)
  
  unsigned long endTime = millis() + duration;
  while (millis() < endTime) {
    digitalWrite(pin, HIGH);       // 부저를 ON
    delayMicroseconds(pulseWidth); 
    digitalWrite(pin, LOW);        // 부저를 OFF
    delayMicroseconds(pulseWidth);
  }
}
