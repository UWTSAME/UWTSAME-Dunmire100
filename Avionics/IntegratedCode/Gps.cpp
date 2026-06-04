#include "Gps.h"

Gps::Gps(int theRX, int theTX) : GPSSerial(2), myRX(theRX), myTX(theTX){
}

bool Gps::begin(){
  GPSSerial.begin(9600, SERIAL_8N1, myRX, myTX);
  delay(2000);
  return true
}

void Gps::setFirst(){
  myInitialLon = myLon;
  myInitialLat = myLat;
  myInitialAlt = myAlt;
  firstSet = true;
}

bool Gps::update(){
  bool updateSuccessful = false;
  while(GPSSerial.available()) {
    char gpsOutput = GPSSerial.read();
    if(myTinyGps.encode(gpsOutput)){
      if(myTinyGps.location.isValid() && myTinyGps.location.isUpdated()){
        myLon = myTinyGps.location.lng();
        myLat = myTinyGps.location.lat();
        myAlt = myTinyGps.altitude.meters();
        updateSuccessful = true;
        if(!firstSet){
          setFirst();
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
// put Leeah's code

double Gps::getDistance(){
  return getDistance(myLat, myLon, myInitialLat, myInitialLon);
}

double Gps::getDistance(double lat1, double lon1, double lat2, double lon2){
  const double R = 6371000; // Earth radius in meters

  double latRad1 = radians(lat1);
  double latRad2 = radians(lat2);
  double deltaLat = radians(lat2 - lat1);
  double deltaLon = radians(lon2 - lon1);

  double a = sin(deltaLat / 2) * sin(deltaLat / 2) +
             cos(latRad1) * cos(latRad2) *
             sin(deltaLon / 2) * sin(deltaLon / 2);

  double c = 2 * atan2(sqrt(a), sqrt(1-a));

  return R * c; //distance in meters
}
double Gps::getDirection(){
  return getDirection(myLat, myLon, myInitialLat, myInitialLon);
}

double Gps::getDirection(double lat1, double lon1, double lat2, double lon2){
  double latRad1 = radians(lat1);
  double latRad2 = radians(lat2);
  double deltaLon = radians(lon2 - lon1);

  double y = sin(deltaLon) * cos(latRad2);
  double x = cos(latRad1) * sin(latRad2) -
             sin(latRad1) * cos(latRad2) * cos(deltaLon);

  double bearing = atan2(y, x);
  bearing = degrees(bearing);

  // Normalize to 0–360°
  if (bearing < 0) bearing += 360;

  return bearing;
}
