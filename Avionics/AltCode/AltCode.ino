/*!
 * @file bmp5xx_test.ino
 *
 * This is a comprehensive test sketch for the BMP5xx pressure and temperature sensor.
 * It demonstrates all settings with pretty-printed output and continuous mode operation.
 * 
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor "ladyada" Fried for Adafruit Industries.
 * BSD license, all text above must be included in any redistribution
 */
  
  //code modified by Ronan Wilson for the use of the SAME club's custom AV bay. particular sensor used in the BMP580 model

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
  BusyCheck();
  GetTempF();
  GetPressure();
  GetAlt();
  Debug();                      //prints the three sets of collected data
  Serial.println(F("---"));     //Makes a dividing line between cycles of data
  delay(10);                    // Short delay since we're checking dataReady()
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
  Serial.println(F("BMP5xx found!"));
  Serial.println();
  // Demonstrate all setter functions with range documentation
  Serial.println(F("=== Setting Up Sensor Configuration ==="));
  TempSetup();
  PressureSetup();
  IIRfilterSetup();
  DataRateSetup();
  PowerSetup();
  SensorSetup();
  SwitchSetup();
}
void TempSetup(){
  Serial.println(F("Setting temperature oversampling to 2X..."));
  bmp.setTemperatureOversampling(BMP5XX_OVERSAMPLING_2X);         //set temerature sampling
}
 
void PressureSetup(){
  Serial.println(F("Setting pressure oversampling to 16X..."));
  bmp.setPressureOversampling(BMP5XX_OVERSAMPLING_16X);           //set presure sampling
}
 
void IIRfilterSetup(){
  Serial.println(F("Setting IIR filter to coefficient 3..."));
  bmp.setIIRFilterCoeff(BMP5XX_IIR_FILTER_COEFF_3);               //set IIR sampling filter
}

void DataRateSetup(){
  Serial.println(F("Setting output data rate to 50 Hz..."));
  bmp.setOutputDataRate(BMP5XX_ODR_50_HZ);                        //set Data output rate in Hz
}
 
void PowerSetup(){
  Serial.println(F("Setting power mode to normal..."));
  desiredMode = BMP5XX_POWERMODE_NORMAL;
  bmp.setPowerMode(desiredMode);                                  //set the sensor's power mode
}

void SensorSetup(){
  Serial.println(F("Enabling pressure measurement..."));
  bmp.enablePressure(true);                                       //turns the pressure sensor on
  Serial.println(F("Configuring interrupt pin with data ready source..."));
  bmp.configureInterrupt(BMP5XX_INTERRUPT_LATCHED, BMP5XX_INTERRUPT_ACTIVE_HIGH, BMP5XX_INTERRUPT_PUSH_PULL, BMP5XX_INTERRUPT_DATA_READY, true);    
        //configures the data source with the interupt pin
  Serial.println();
  Serial.println(F("=== Current Sensor Configuration ==="));
}

void SwitchSetup(){
  TempSample();
  PressureSample();
  IIRfilterSample();
  DataRateSample();
  PowerInline();
  Serial.println();
}

void TempSample(){
  // Pretty print temperature oversampling inline
  Serial.print(F("Temperature Oversampling: "));
  switch(bmp.getTemperatureOversampling()) {
    case BMP5XX_OVERSAMPLING_1X:   Serial.println(F("1X")); break;
    case BMP5XX_OVERSAMPLING_2X:   Serial.println(F("2X")); break;
    case BMP5XX_OVERSAMPLING_4X:   Serial.println(F("4X")); break;
    case BMP5XX_OVERSAMPLING_8X:   Serial.println(F("8X")); break;
    case BMP5XX_OVERSAMPLING_16X:  Serial.println(F("16X")); break;
    case BMP5XX_OVERSAMPLING_32X:  Serial.println(F("32X")); break;
    case BMP5XX_OVERSAMPLING_64X:  Serial.println(F("64X")); break;
    case BMP5XX_OVERSAMPLING_128X: Serial.println(F("128X")); break;
    default: Serial.println(F("Unknown")); break;
  }
}
void PressureSample(){
  // Pretty print pressure oversampling inline
  Serial.print(F("Pressure Oversampling: "));
  switch(bmp.getPressureOversampling()) {
    case BMP5XX_OVERSAMPLING_1X:   Serial.println(F("1X")); break;
    case BMP5XX_OVERSAMPLING_2X:   Serial.println(F("2X")); break;
    case BMP5XX_OVERSAMPLING_4X:   Serial.println(F("4X")); break;
    case BMP5XX_OVERSAMPLING_8X:   Serial.println(F("8X")); break;
    case BMP5XX_OVERSAMPLING_16X:  Serial.println(F("16X")); break;
    case BMP5XX_OVERSAMPLING_32X:  Serial.println(F("32X")); break;
    case BMP5XX_OVERSAMPLING_64X:  Serial.println(F("64X")); break;
    case BMP5XX_OVERSAMPLING_128X: Serial.println(F("128X")); break;
    default: Serial.println(F("Unknown")); break;
  }
}
void IIRfilterSample(){
  // Pretty print IIR filter coefficient inline
  Serial.print(F("IIR Filter Coefficient: "));
  switch(bmp.getIIRFilterCoeff()) {
    case BMP5XX_IIR_FILTER_BYPASS:   Serial.println(F("Bypass (No filtering)")); break;
    case BMP5XX_IIR_FILTER_COEFF_1:  Serial.println(F("1 (Light filtering)")); break;
    case BMP5XX_IIR_FILTER_COEFF_3:  Serial.println(F("3 (Medium filtering)")); break;
    case BMP5XX_IIR_FILTER_COEFF_7:  Serial.println(F("7 (More filtering)")); break;
    case BMP5XX_IIR_FILTER_COEFF_15: Serial.println(F("15 (Heavy filtering)")); break;
    case BMP5XX_IIR_FILTER_COEFF_31: Serial.println(F("31 (Very heavy filtering)")); break;
    case BMP5XX_IIR_FILTER_COEFF_63: Serial.println(F("63 (Maximum filtering)")); break;
    case BMP5XX_IIR_FILTER_COEFF_127:Serial.println(F("127 (Maximum filtering)")); break;
    default: Serial.println(F("Unknown")); break;
  }
}
void DataRateSample(){
  // Pretty print output data rate inline
  Serial.print(F("Output Data Rate: "));
  switch(bmp.getOutputDataRate()) {
    case BMP5XX_ODR_240_HZ:   Serial.println(F("240 Hz")); break;
    case BMP5XX_ODR_218_5_HZ: Serial.println(F("218.5 Hz")); break;
    case BMP5XX_ODR_199_1_HZ: Serial.println(F("199.1 Hz")); break;
    case BMP5XX_ODR_179_2_HZ: Serial.println(F("179.2 Hz")); break;
    case BMP5XX_ODR_160_HZ:   Serial.println(F("160 Hz")); break;
    case BMP5XX_ODR_149_3_HZ: Serial.println(F("149.3 Hz")); break;
    case BMP5XX_ODR_140_HZ:   Serial.println(F("140 Hz")); break;
    case BMP5XX_ODR_129_8_HZ: Serial.println(F("129.8 Hz")); break;
    case BMP5XX_ODR_120_HZ:   Serial.println(F("120 Hz")); break;
    case BMP5XX_ODR_110_1_HZ: Serial.println(F("110.1 Hz")); break;
    case BMP5XX_ODR_100_2_HZ: Serial.println(F("100.2 Hz")); break;
    case BMP5XX_ODR_89_6_HZ:  Serial.println(F("89.6 Hz")); break;
    case BMP5XX_ODR_80_HZ:    Serial.println(F("80 Hz")); break;
    case BMP5XX_ODR_70_HZ:    Serial.println(F("70 Hz")); break;
    case BMP5XX_ODR_60_HZ:    Serial.println(F("60 Hz")); break;
    case BMP5XX_ODR_50_HZ:    Serial.println(F("50 Hz")); break;
    case BMP5XX_ODR_45_HZ:    Serial.println(F("45 Hz")); break;
    case BMP5XX_ODR_40_HZ:    Serial.println(F("40 Hz")); break;
    case BMP5XX_ODR_35_HZ:    Serial.println(F("35 Hz")); break;
    case BMP5XX_ODR_30_HZ:    Serial.println(F("30 Hz")); break;
    case BMP5XX_ODR_25_HZ:    Serial.println(F("25 Hz")); break;
    case BMP5XX_ODR_20_HZ:    Serial.println(F("20 Hz")); break;
    case BMP5XX_ODR_15_HZ:    Serial.println(F("15 Hz")); break;
    case BMP5XX_ODR_10_HZ:    Serial.println(F("10 Hz")); break;
    case BMP5XX_ODR_05_HZ:    Serial.println(F("5 Hz")); break;
    case BMP5XX_ODR_04_HZ:    Serial.println(F("4 Hz")); break;
    case BMP5XX_ODR_03_HZ:    Serial.println(F("3 Hz")); break;
    case BMP5XX_ODR_02_HZ:    Serial.println(F("2 Hz")); break;
    case BMP5XX_ODR_01_HZ:    Serial.println(F("1 Hz")); break;
    case BMP5XX_ODR_0_5_HZ:   Serial.println(F("0.5 Hz")); break;
    case BMP5XX_ODR_0_250_HZ: Serial.println(F("0.25 Hz")); break;
    case BMP5XX_ODR_0_125_HZ: Serial.println(F("0.125 Hz")); break;
    default: Serial.println(F("Unknown")); break;
  }
}
void PowerInline(){
  // Pretty print power mode inline
  Serial.print(F("Power Mode: "));
  switch(bmp.getPowerMode()) {
    case BMP5XX_POWERMODE_STANDBY:     Serial.println(F("Standby")); break;
    case BMP5XX_POWERMODE_NORMAL:      Serial.println(F("Normal")); break;
    case BMP5XX_POWERMODE_FORCED:      Serial.println(F("Forced")); break;
    case BMP5XX_POWERMODE_CONTINUOUS:  Serial.println(F("Continuous")); break;
    case BMP5XX_POWERMODE_DEEP_STANDBY:Serial.println(F("Deep Standby")); break;
    default: Serial.println(F("Unknown")); break;
  }
}
void BusyCheck(){
  // Check if new data is ready before reading
  if (!bmp.dataReady()) {
    return;
  }
  // Data is ready, perform reading
  if (!bmp.performReading()) {
    return;
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
float FirstAlt(){
   return bmp.readAltitude(SEALEVELPRESSURE_HPA) * 3.28;
}
float GetAlt(){
  return bmp.readAltitude(SEALEVELPRESSURE_HPA) * 3.28 + first_alt;
}
void Debug(){
Serial.print(GetTempF());
Serial.print("Â°F\n");
Serial.print(GetPressure());
Serial.print("Hpa\n");
Serial.print(GetAlt());
Serial.print("ft\n");
}