#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <TinyGPSPlus.h>

class Gps {
  private:
    HardwareSerial GPSSerial;
    TinyGPSPlus myTinyGps;
    int myRX;
    int myTX;
    float myLat = 0.0;
    float myLon = 0.0;
    float myAlt = 0.0;
    float myInitialLon = 0.0;
    float myInitialLat = 0.0;
    float myInitialAlt = 0.0;
    const int BEGINNING_SAMPLE = 5;

  public:
    Gps(int theRX, int theTX);

    bool begin();
    void setFirst();
    bool update();
    float getLat();
    float getLon();
    float getAlt();
    float getDistance();
    float getDirection();
};

#endif