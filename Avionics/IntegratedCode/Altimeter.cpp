#include "Altimeter.h"

Altimeter::Altimeter(float theSeaPressure): mySeaPressure(theSeaPressure), myInitialAlt(0){
}

Altimeter::Altimeter(): mySeaPressure(1013.25), myInitialAlt(0){
}

bool Altimeter::begin(){
  bool setupSuccessful = false;
  if(!myBmp.begin_I2C(0x47, &Wire)) {
    return setupSuccessful;
  }
  setupSuccessful = true;

  // Demonstrate all setter functions with range documentation
  myBmp.setTemperatureOversampling(BMP5XX_OVERSAMPLING_2X);   //Setting temperature oversampling to 2X
  myBmp.setPressureOversampling(BMP5XX_OVERSAMPLING_16X);     //Setting pressure oversampling to 16X
  myBmp.setIIRFilterCoeff(BMP5XX_IIR_FILTER_COEFF_3);         //Setting IIR filter to coefficient 3
  myBmp.setOutputDataRate(BMP5XX_ODR_50_HZ);                  //Setting output data rate to 50 Hz
  myBmp.setPowerMode(BMP5XX_POWERMODE_NORMAL);                //Setting power mode to normal
  //Sets up the sensor
  myBmp.enablePressure(true);
  myBmp.configureInterrupt(BMP5XX_INTERRUPT_LATCHED, BMP5XX_INTERRUPT_ACTIVE_HIGH, BMP5XX_INTERRUPT_PUSH_PULL, BMP5XX_INTERRUPT_DATA_READY, true); 

  //Sets the first recorded altitude.
  myInitialAlt = getAlt();
  return setupSuccessful;
}

bool Altimeter::update(){
  bool updateSuccessful = false;
  //Check if new data is ready before reading
  if(myBmp.dataReady()){
    //If data is ready, updates the state fields of the bmp.
    updateSuccessful = myBmp.performReading();
  }
  return updateSuccessful;
}

float Altimeter::getTempC(){
  return myBmp.temperature;
}

float Altimeter::getTempF(){
  return (getTempC() * (9.0 / 5.0) + 32.0);
}

float Altimeter::getPressure(){
  return myBmp.pressure;
}

float Altimeter::getAlt(){
  return myBmp.readAltitude(mySeaPressure) * 3.28;
}

float Altimeter::getRelAlt(){
  return getAlt() - myInitialAlt;
}

