#include <Arduino.h>
#include "Wire.h"
#include <string.h>
#include <Print.h>

#define I2C_DEV_ADDR 0x15
#define txBUFFER_SIZE  128

#define RPI_MSG "00,p,1,xxxx1111,0,0,$"
#define TEST_MSG "ABCDEFGHIJ"

uint32_t i = 0;
char tx_buffer[txBUFFER_SIZE];

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Wire.begin();
  bool setClock(100000);

  //delay(1000);
  memset(tx_buffer, '\0', txBUFFER_SIZE);
  memcpy(tx_buffer, RPI_MSG, sizeof(RPI_MSG));

  // size_t length = sizeof(tx_buffer) / sizeof(tx_buffer[0]);

  // Serial.print("Buffer: ");
  // for (size_t i = 0; i < length; i++) {
  //     Serial.print(tx_buffer[i]);
  //     if (i < length - 1) {
  //         Serial.print(", "); // Separador entre elementos
  //     }
  // }
   // Salto de línea al final

}

void loop() {
  Serial.println(tx_buffer);
  delay(100);
  
  Wire.beginTransmission(I2C_DEV_ADDR);
  Wire.write(tx_buffer);
  uint8_t error = Wire.endTransmission();
  Serial.printf("endTransmission: %u\n", error); 

  delay(100);
  
  uint8_t bytesReceived = Wire.requestFrom(I2C_DEV_ADDR, 17);
  Serial.printf("requestFrom: %u\n", bytesReceived);
  if ((bool)bytesReceived) {  //If received more than zero bytes
    uint8_t temp[bytesReceived];
    Wire.readBytes(temp, bytesReceived);
    //log_print_buf(temp, bytesReceived);
    Serial.printf("%s\n", temp);
  }

  delay(2000); 
}
