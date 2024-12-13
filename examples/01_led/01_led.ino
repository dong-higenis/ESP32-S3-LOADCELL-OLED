#define LED_PIN 6

int pre_time = 0;
int led_state = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pre_time = millis();
}

void loop() {
  if (millis() - pre_time >= 500)
  {
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    pre_time = millis();

    // Serial.printf("LED: %d\n", led_state);
  }
}
