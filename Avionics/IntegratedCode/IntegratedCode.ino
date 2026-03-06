#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP5xx.h"


const int SEALEVELPRESSURE_HPA (1000);
int firstAlt;



Adafruit_BMP5xx bmp; // Create BMP5xx object
//Uses the microcontroller SCL and SDA pins. (For Esp32, it will assume its wired GPIO 21 to SCL and GPIO 22 to SDA)
bmp5xx_powermode_t desiredMode = BMP5XX_POWERMODE_NORMAL; // Cache desired power mode

void setup() {
  Serial.begin(115200); //Sets up baud rate to 115200.
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial) delay(10);  // Wait for Serial Monitor to open
  AltSetup();
  first_alt = FirstAlt(); //Change the method name

}

void AltSetup(){
  if(!bmp.begin(BMP5XX_ALTERNATIVE_ADDRESS, &Wire)){
    while (true) AlertLights();
  }
}

void AltSetup(){
    //sets up the functions and presets for the sensor chip
  Serial.println(F("Adafruit BMP5xx Comprehensive Test!"));

  // Try to initialize the sensor
  // For I2C mode (default):
  if (!bmp.begin(BMP5XX_ALTERNATIVE_ADDRESS, &Wire)) {
  // For SPI mode (uncomment the line below and comment out the I2C line above):
  // if (!bmp.begin(BMP5XX_CS_PIN, &SPI)) {
    Serial.println(F("Could not find a valid BMP5xx sensor, check wiring!"));
    while (1) delay(10);
  }
  VerifyLights();
  // Demonstrate all setter functions with range documentation
  Serial.println(F("=== Setting Up Sensor Configuration ==="));
  bmp.setTemperatureOversampling(BMP5XX_OVERSAMPLING_2X);   //Setting temperature oversampling to 2X
  bmp.setPressureOversampling(BMP5XX_OVERSAMPLING_16X);     //Setting pressure oversampling to 16X
  bmp.setIIRFilterCoeff(BMP5XX_IIR_FILTER_COEFF_3);         //Setting IIR filter to coefficient 3
  bmp.setOutputDataRate(BMP5XX_ODR_50_HZ);                  //Setting output data rate to 50 Hz
  bmp.setPowerMode(BMP5XX_POWERMODE_NORMAL);                //Setting power mode to normal
  //Sets up the sensor
  bmp.enablePressure(true);
  bmp.configureInterrupt(BMP5XX_INTERRUPT_LATCHED, BMP5XX_INTERRUPT_ACTIVE_HIGH, BMP5XX_INTERRUPT_PUSH_PULL, BMP5XX_INTERRUPT_DATA_READY, true); 
}




void AlertLights(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void VerifyLights(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {

  

}

