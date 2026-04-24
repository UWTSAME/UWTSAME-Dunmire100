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
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  //Gyroscope Vector
  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  //Raw acceleration
  imu::Vector<3> rawAccel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  myOX = event.orientation.x, 4;
  myOY = event.orientation.y, 4;
  myOZ = event.orientation.z, 4;

  ax = accel.x();
  ay = accel.y();
  az = accel.z();

  gx = gyro.x();
  gy = gyro.y();
  gz = gyro.z();

  rawAccelMag = sqrt(rawAccel.x() * rawAccel.x() + 
  rawAccel.y() * rawAccel.y() + rawAccel.z() * rawAccel.z());

  velX += ax * dt; 
  velY += ay * dt;
  velZ += az * dt;

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
