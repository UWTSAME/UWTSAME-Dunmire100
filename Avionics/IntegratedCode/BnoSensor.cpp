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

  loadCalibration();
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

bool BnoSensor::update(){
  //events
  sensors_event_t event;
  myBno.getEvent(&event);

  //Acceleration Vector
  imu::Vector<3> accel = myBno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  //Gyroscope Vector
  imu::Vector<3> gyro = myBno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  //Raw acceleration
  imu::Vector<3> rawAccel = myBno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  myOX = event.orientation.x;
  myOY = event.orientation.y;
  myOZ = event.orientation.z;

  myAx = accel.x();
  myAy = accel.y();
  myAz = accel.z();

  myGx = gyro.x();
  myGy = gyro.y();
  myGz = gyro.z();

  myRawAccelMag = sqrt(rawAccel.x() * rawAccel.x() + 
  rawAccel.y() * rawAccel.y() + rawAccel.z() * rawAccel.z());

  // For filtering linear acceleration values
  myFilteredAx = alpha * myAx + (1 - alpha) * myFilteredAx;
  myFilteredAy = alpha * myAy + (1 - alpha) * myFilteredAy;
  myFilteredAz = alpha * myAz + (1 - alpha) * myFilteredAz;

  myVelX += myAx * dt;
  myVelY += myAy * dt;
  myVelZ += myAz * dt;

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

float BnoSensor::getAccelX() {
 
 return myAx;
}

float BnoSensor::getAccelY() {
  
  return myAy;
}

float BnoSensor::getAccelZ() {
  
  return myAz;
}

float BnoSensor::getRawAccelMag() {
  return myRawAccelMag;
}

uint8_t BnoSensor::status(){
  /* System Status (see section 4.3.58)
     0 = Idle
     1 = System Error
     2 = Initializing Peripherals
     3 = System Iniitalization
     4 = Executing Self-Test
     5 = Sensor fusio algorithm running
     6 = System running without fusion algorithms
     7 = Temperature Problem
   */
  uint8_t system_status = 0, self_test = 0, system_error = 0; 
  myBno.getSystemStatus(&system_status, &self_test, &system_error);

  if (system_error != 0) return 1;

  if(myBno.getTemp() > TEMP_WARNING_C) return 7;

  return system_status;

}
