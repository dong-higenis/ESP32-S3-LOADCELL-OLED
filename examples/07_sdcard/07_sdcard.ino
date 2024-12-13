#include <SD.h>
#include <SPI.h>

// Define the SD card chip select pin
#define SD_CS_PIN 5 // SD 카드 CS 핀 (ESP32 보드에 따라 핀이 다를 수 있음)

void setup() {
  Serial.begin(115200);
  
  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Create or open a file on the SD card
  File file = SD.open("/example.txt", FILE_WRITE);
  if (file) {
    Serial.println("Writing to file...");
    file.println("Hello, ESP32 SD card!");
    file.close(); // Close the file to save data
    Serial.println("Write complete.");
  } else {
    Serial.println("Failed to open file for writing.");
  }

  // Read the file content
  file = SD.open("/example.txt");
  if (file) {
    Serial.println("Reading from file:");
    while (file.available()) {
      Serial.write(file.read());
    }
    file.close();
  } else {
    Serial.println("Failed to open file for reading.");
  }
}

void loop() {
  // Nothing to do here
}
