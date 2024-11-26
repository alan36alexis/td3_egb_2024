#include <Arduino.h>
#include "Wire.h"

#define I2C_DEV_ADDR 0x15

#define RPI_MSG "00,p,1,1111,0,0,$"
#define TEST_MSG "ABCDEFGHIJ"

uint32_t i = 0;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Wire.begin();
  bool setClock(100000);

  //delay(1000);



}

void loop() {

  delay(100);
  
  Wire.beginTransmission(I2C_DEV_ADDR);
  Wire.write(RPI_MSG);
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
