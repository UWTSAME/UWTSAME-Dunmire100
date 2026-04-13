#include "Gps.h"

Gps::Gps(int theRX, int theTX) : GPSSerial(2), myRX(theRX), myTX(theTX){
}

bool Gps::begin(){
  GPSSerial.begin(9600, SERIAL_8N1, myRX, myTX);
  delay(2000);
  return (GPSSerial.available() > 0);
}

void Gps::setFirst(){
  myInitialLon = myLon;
  myInitialLat = myLat;
  myInitialAlt = myAlt;
}

bool Gps::update(){
  bool updateSuccessful = false;
  while(GPSSerial.available() != 0) {
    char gpsOutput = GPSSerial.read();
    if(myTinyGps.encode(gpsOutput)){
      myAlt = myTinyGps.altitude.meters();
      updateSuccessful = true;
      if(myTinyGps.location.isValid() && myTinyGps.location.isUpdated()){
        myLon = myTinyGps.location.lng();
        myLat = myTinyGps.location.lat();
        myAlt = myTinyGps.altitude.meters();
        updateSuccessful = true;
        if(!firstSet){
          for(int i = 0; i < BEGINNING_SAMPLE; i++){
            setFirst();
          }
        }
      }
    }
  }
  return updateSuccessful;
}

float Gps::getLat(){
  return myLat;
}

float Gps::getLon(){
  return myLon;
}

float Gps::getAlt(){
  return myAlt;
}

//Need to bring in Leah's methods (hopefully I spelt her name right, prolly not)
float Gps::getDistance(){
  return 55.5;
}

float Gps::getDirection(){
  return 55.5;
}