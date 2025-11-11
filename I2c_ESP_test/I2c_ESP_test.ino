#include <Wire.h>

void setup() {
  Wire.begin(0x08); // Slave address = 0x08
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("ESP32 I2C Slave Ready");
}

void loop() {
  delay(100);
}

void receiveEvent(int len) {
  Serial.print("Received: ");
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}
