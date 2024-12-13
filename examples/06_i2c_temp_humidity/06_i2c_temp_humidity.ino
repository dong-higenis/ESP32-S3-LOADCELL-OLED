#include <Wire.h>

#define SDA_PIN 4       // I2C SDA 핀
#define SCL_PIN 5       // I2C SCL 핀
#define HDC1080_ADDR 0x40 // HDC1080 I2C 주소 (기본값)

// HDC1080 레지스터 주소
#define TEMP_REGISTER 0x00
#define HUMIDITY_REGISTER 0x01

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN); // I2C 초기화 (사용자 정의 SDA, SCL)

  // HDC1080 초기화
  if (!initializeHDC1080()) {
    Serial.println("HDC1080 initialization failed!");
    while (1); // 오류 시 멈춤
  }
  Serial.println("HDC1080 initialized successfully.");
}

void loop() {
  float temperature = readTemperature();
  float humidity = readHumidity();

  if (temperature != -1 && humidity != -1) {
    Serial.printf("Temperature: %.2f °C, Humidity: %.2f %%\n", temperature, humidity);
  } else {
    Serial.println("Failed to read data from HDC1080.");
  }

  delay(2000); // 2초 대기
}

// HDC1080 초기화 함수
bool initializeHDC1080() {
  Wire.beginTransmission(HDC1080_ADDR);
  Wire.write(0x02); // Configuration 레지스터 주소
  Wire.write(0x10); // 14-bit resolution 설정
  Wire.write(0x00);
  return Wire.endTransmission() == 0;
}

// 온도 읽기 함수
float readTemperature() {
  Wire.beginTransmission(HDC1080_ADDR);
  Wire.write(TEMP_REGISTER);
  if (Wire.endTransmission() != 0) return -1; // 오류 발생 시 -1 반환

  delay(15); // 데이터 준비 시간

  Wire.requestFrom(HDC1080_ADDR, 2);
  if (Wire.available() == 2) {
    uint16_t rawTemperature = (Wire.read() << 8) | Wire.read();
    return (rawTemperature / 65536.0) * 165.0 - 40.0; // 온도 계산 공식
  }
  return -1; // 읽기 실패 시 -1 반환
}

// 습도 읽기 함수
float readHumidity() {
  Wire.beginTransmission(HDC1080_ADDR);
  Wire.write(HUMIDITY_REGISTER);
  if (Wire.endTransmission() != 0) return -1; // 오류 발생 시 -1 반환

  delay(15); // 데이터 준비 시간

  Wire.requestFrom(HDC1080_ADDR, 2);
  if (Wire.available() == 2) {
    uint16_t rawHumidity = (Wire.read() << 8) | Wire.read();
    return (rawHumidity / 65536.0) * 100.0; // 습도 계산 공식
  }
  return -1; // 읽기 실패 시 -1 반환
}
