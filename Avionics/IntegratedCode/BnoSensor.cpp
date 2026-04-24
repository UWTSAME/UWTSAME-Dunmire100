#include "BnoSensor.h"

BnoSensor::BnoSensor(){
}

bool BnoSensor::begin(){
  if(!myBno.begin()) {
    return false;
  }
  delay(1000);

  myBno.setExtCrystalUse(true);

  delay(250);
  return true;

  loadCalibration();
}

bool BnoSensor::loadCalibration() { // Opens flash memory to load stored calibration values
  adafruit_bno055_offsets_t offsets;
  myPrefs.begin("bno", true);
  if (myPrefs.getBytesLength("offsets") == sizeof(offsets)) {
    myPrefs.getBytes("offsets", &offsets, sizeof(offsets));
    myPrefs.end();
    myBno.setSensorOffsets(offsets);
    Serial.println("Calibration restored.");
    return true;
  }
  myPrefs.end();
  return false;
}

bool BnoSensor::update(){
  sensors_event_t event;
  myBno.getEvent(&event);

  myOX = event.orientation.x, 4;
  myOY = event.orientation.y, 4;
  myOZ = event.orientation.z, 4;

  return true;
}

float BnoSensor::getOrientationX(){
  
  return myOX;
}

float BnoSensor::getOrientationY(){
  
  return myOY;
}

float BnoSensor::getOrientationZ(){
  
  return myOZ;
}
