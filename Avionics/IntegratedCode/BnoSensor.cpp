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

bool BnoSensor::doCalibration() // For doing sensor calibration
{
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  unsigned long startTime = millis();
  const unsigned long timeout = 90000;
  bool timedOut = false; 

  bool systemDone = false;
  bool gyroDone = false;
  bool accelDone = false;
  bool magDone = false;

  // Prints values until they reach 3
  while ((system < 3) || (gyro < 3) || (accel < 3) || (mag < 3)) {

    myBno.getCalibration(&system, &gyro, &accel, &mag); 

    Serial.print("System: "); // Depends on other factors
    Serial.println(system);
    Serial.print("Gyro: "); // Leave sensor still for a bit
    Serial.println(gyro);
    Serial.print("Accel: "); // Slowly tilt sensor in different directions
    Serial.println(accel);
    Serial.print("Mag: "); // Slowly rotate sensor in a figure-8
    Serial.println(mag);

    delay(100);

    // Blinks every time a value is fully calibrated
    if (system == 3 && !systemDone) {
      systemDone = true; 
    }
    if (gyro == 3 && !gyroDone) {
      gyroDone = true;
    }
    if (accel == 3 && !accelDone) {
      accelDone = true;
    }
    if (mag == 3 && !magDone) {
      magDone = true;
    }

    // Times out if calibration takes longer than 90 seconds
    if (millis() - startTime > timeout) {
      Serial.println("Calibration timeout reached.");
      timedOut = true;
      break;
    }
  }

  // Finishes the calibration and blinks
  if (!timedOut) {
    return true;
  }

  return false;
}

bool BnoSensor::saveCalibration() { // Reads and stores calibration values
  adafruit_bno055_offsets_t offsets;
  myBno.getSensorOffsets(offsets);
  myPrefs.begin("bno", false);
  myPrefs.putBytes("offsets", &offsets, sizeof(offsets));
  myPrefs.end();
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
