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

  //dt = (currentTime - lastTime) / 1000.0;
 //lastTime = millis();

  myOX = event.orientation.x, 4;
  myOY = event.orientation.y, 4;
  myOZ = event.orientation.z, 4;

  myAx = accel.x();
  myAy = accel.y();
  myAz = accel.z();

  myGx = gyro.x();
  myGy = gyro.y();
  myGz = gyro.z();

  myRawAccelMag = sqrt(rawAccel.x() * rawAccel.x() + 
  rawAccel.y() * rawAccel.y() + rawAccel.z() * rawAccel.z());

  //velX += ax * dt; 
  //velY += ay * dt;
  //velZ += az * dt;

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

<<<<<<< Updated upstream
  if(myBno.getTemp() > TEMP_WARNING_C) system_status = 7;
=======
  if(bno.getTemp > TEMP_WARNING_C) return 7;

  if (system_error != 0) return 1;
>>>>>>> Stashed changes

  return system_status;

}
