#ifndef BNOSENSOR_H
#define BNOSENSOR_H

#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Preferences.h>
#include <Wire.h>

class BnoSensor {
  private:
    const int TEMP_WARNING_C = 70;
    Adafruit_BNO055 myBno = Adafruit_BNO055(55);
    Preferences myPrefs;


  public:
    BnoSensor();

    bool begin();
    bool loadCalibration();
    bool doCalibration();
    bool saveCalibration();
    float getOrientationX();
    float getOrientationY();
    float getOrientationZ();
};

#endif