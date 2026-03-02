
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP5xx.h"

const int SEALEVELPRESSURE_HPA (1000);
int first_alt;

Adafruit_BMP5xx bmp; // Create BMP5xx object
bmp5xx_powermode_t desiredMode = BMP5XX_POWERMODE_NORMAL; // Cache desired power mode

void setup() {

  Serial.begin(115200);
  while (!Serial) delay(10);  // Wait for Serial Monitor to open
  AltSetup();
  first_alt = FirstAlt();

}

void loop() {

  

}

