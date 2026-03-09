#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP5xx.h"


const int SEALEVELPRESSURE_HPA (1019.4);
int firstAlt;

const int LED_BUILTIN = 2;

Adafruit_BMP5xx bmp; // Create BMP5xx object
//Uses the microcontroller SCL and SDA pins. (For Esp32, it will assume its wired GPIO 21 to SCL and GPIO 22 to SDA)
bmp5xx_powermode_t desiredMode = BMP5XX_POWERMODE_NORMAL; // Cache desired power mode

void setup() {
  Serial.begin(115200); //Sets up baud rate to 115200.
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial) delay(10);  // Wait for Serial Monitor to open
  AltSetup();
  //Sets the first recorded altitude.
  firstAlt = GetAlt(); 

}

void AltSetup(){
    //sets up the functions and presets for the sensor chip
  // Try to initialize the sensor
  // For I2C mode:
  if (!bmp.begin(BMP5XX_ALTERNATIVE_ADDRESS, &Wire)) {
    while (true) AlertLights();
  }
  VerifyLights();
  // Demonstrate all setter functions with range documentation
  bmp.setTemperatureOversampling(BMP5XX_OVERSAMPLING_2X);   //Setting temperature oversampling to 2X
  bmp.setPressureOversampling(BMP5XX_OVERSAMPLING_16X);     //Setting pressure oversampling to 16X
  bmp.setIIRFilterCoeff(BMP5XX_IIR_FILTER_COEFF_3);         //Setting IIR filter to coefficient 3
  bmp.setOutputDataRate(BMP5XX_ODR_50_HZ);                  //Setting output data rate to 50 Hz
  bmp.setPowerMode(BMP5XX_POWERMODE_NORMAL);                //Setting power mode to normal
  //Sets up the sensor
  bmp.enablePressure(true);
  bmp.configureInterrupt(BMP5XX_INTERRUPT_LATCHED, BMP5XX_INTERRUPT_ACTIVE_HIGH, BMP5XX_INTERRUPT_PUSH_PULL, BMP5XX_INTERRUPT_DATA_READY, true); 
}

void UpdateAltimeter(){
  // Check if new data is ready before reading
  if(bmp.dataReady()){
    //If data is ready, updates the state fields of the bmp.
    bmp.performReading();
  }
} 

float GetTempC(){
  return bmp.temperature;
}
float GetTempF(){
  return (GetTempC() * (9 / 5) + 32);
}

float GetPressure(){
  return bmp.pressure;
}

float GetAlt(){
   return bmp.readAltitude(SEALEVELPRESSURE_HPA) * 3.28;
}

float GetRelAlt(){
   return bmp.readAltitude(SEALEVELPRESSURE_HPA) * 3.28 - firstAlt;
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


//EDIT DEMO
//EDIT DEMO 2
//HELLO
void AltDebug(){
  Serial.print("Data Ready:");
  Serial.print(UpdateAltimeter());
  Serial.print("  First Alt: ");
  Serial.print(firstAlt);
  Serial.print("  Current Alt: ");
  Serial.print(getAlt());
  Serial.print("  Current temp C");
  Serial.print(getTempC());

}




void loop() {


}

