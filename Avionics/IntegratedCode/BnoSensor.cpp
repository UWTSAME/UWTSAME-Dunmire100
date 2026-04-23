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

float BnoSensor::getOrientationX(){
  sensors_event_t event;
  myBno.getEvent(&event);

  return event.orientation.x;
}

float BnoSensor::getOrientationY(){
  sensors_event_t event;
  myBno.getEvent(&event);

  return event.orientation.y;
}

float BnoSensor::getOrientationZ(){
  sensors_event_t event;
  myBno.getEvent(&event);

  return event.orientation.z;
}
