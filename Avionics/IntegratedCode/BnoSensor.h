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
    float mOX = 0;
    float mOY = 0;
    float mOZ = 0;
    float ax = 0;
    float ay = 0;
    float az = 0;
    float gx = 0;
    float gy = 0;
    float gz = 0;
    float dt = 0;
    float rawAccelMag = 0;

  public:
    BnoSensor();

    bool begin();
    bool loadCalibration();
    bool update();
    float getOrientationX();
    float getOrientationY();
    float getOrientationZ();
    uint8_t status();
};

#endif