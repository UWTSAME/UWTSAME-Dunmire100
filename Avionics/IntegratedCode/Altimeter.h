#ifndef ALTIMETER_H 
#define ALTIMETER_H

#include <Adafruit_BMP5xx.h>
#include <Wire.h>

class Altimeter {
  private:
    Adafruit_BMP5xx myBmp;
    float mySeaPressure; //in hPa
    float myInitialAlt;

  public:
    Altimeter(float theSeaPressure);
    Altimeter();

    bool begin();
    bool update();
    float getTempC();
    float getTempF();
    float getPressure();
    float getAlt();
    float getRelAlt();

};

#endif