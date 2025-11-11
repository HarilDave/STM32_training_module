#include <SPI.h>

#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_SCK  18
#define PIN_CS   5

void setup() {
  Serial.begin(115200);
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);
  delay(100);
  Serial.println("ESP32 SPI Master ready");
}

void loop() {
  byte sendData = 0x55;
  byte recvData;

  digitalWrite(PIN_CS, LOW);
  recvData = SPI.transfer(sendData);
  digitalWrite(PIN_CS, HIGH);

  Serial.print("Sent: 0x");
  Serial.print(sendData, HEX);
  Serial.print("  Received: 0x");
  Serial.println(recvData, HEX);

  delay(1000);
}
