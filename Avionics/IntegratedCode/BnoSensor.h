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
    const float alpha = 0.5f;
    Adafruit_BNO055 myBno = Adafruit_BNO055(55);
    Preferences myPrefs;
    float myOX = 0;
    float myOY = 0;
    float myOZ = 0;
    float myAx = 0;
    float myAy = 0;
    float myAz = 0;
    float myGx = 0;
    float myGy = 0;
    float myGz = 0;

    float myVelX = 0;
    float myVelY = 0;
    float myVelZ = 0;
    float myFilteredAx = 0;
    float myFilteredAy = 0;
    float myFilteredAz = 0;
    float dt = 0;
    unsigned long currentTime = 0;
    unsigned long lastTime = 0;
    float myRawAccelMag = 0;

  public:
    BnoSensor();

    bool begin();
    bool loadCalibration();
    bool update();
    float getOrientationX();
    float getOrientationY();
    float getOrientationZ();
    float getRawAccelMag();
    uint8_t status();
};

#endif