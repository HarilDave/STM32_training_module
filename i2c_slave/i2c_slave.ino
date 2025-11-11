#include <Wire.h>

#define SLAVE_ADDR 0x28   // must match STM32's slave address

String receivedData = "";   // ✅ remove 'volatile'

void receiveEvent(int numBytes) {
  receivedData = "";   // clear string
  while (Wire.available()) {
    char c = Wire.read();
    receivedData += c;
  }
  Serial.print("Received: ");
  Serial.println(receivedData);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SLAVE_ADDR);       // ESP32 as I2C slave
  Wire.onReceive(receiveEvent); // attach handler
  Serial.println("ESP32 I2C Slave Ready");
}

void loop() {
  delay(100);
}
