#define BUTTON0_PIN 48
#define BUTTON1_PIN 38
#define BUTTON2_PIN 42
#define LED_PIN 6

void setup() {
  pinMode(BUTTON0_PIN, INPUT_PULLUP);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  int button_state0 = digitalRead(BUTTON0_PIN);
  int button_state1 = digitalRead(BUTTON1_PIN);
  int button_state2 = digitalRead(BUTTON2_PIN);

  // 버튼이 눌렸을 때 (LOW 상태)
  if (button_state0 == HIGH) {
    Serial.println("Button0 Pressed!");
    digitalWrite(LED_PIN, HIGH); // LED 켜기
  } else {
    digitalWrite(LED_PIN, LOW);  // LED 끄기
  }

  // 버튼이 눌렸을 때 (LOW 상태)
  if (button_state1 == LOW) {
    Serial.println("Button1 Pressed!");
    digitalWrite(LED_PIN, HIGH); // LED 켜기
  } else {
    digitalWrite(LED_PIN, LOW);  // LED 끄기
  }

  // 버튼이 눌렸을 때 (LOW 상태)
  if (button_state2 == LOW) {
    Serial.println("Button2 Pressed!");
    digitalWrite(LED_PIN, HIGH); // LED 켜기
  } else {
    digitalWrite(LED_PIN, LOW);  // LED 끄기
  }

  delay(10); // 디바운싱을 위한 짧은 지연
}
