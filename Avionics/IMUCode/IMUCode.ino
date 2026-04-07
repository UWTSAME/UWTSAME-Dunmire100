#include <Wire.h> 
#include <Adafruit_Sensor.h> 
#include <Adafruit_BNO055.h> 
#include <utility/imumaths.h> 
#include <Preferences.h> 
#include <math.h>
#define LED_BUILTIN 2 // Built in LED pin

  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Preferences prefs;
const int TEMP_WARNING_C = 70; // Temperature threshold
float velX = 0, velY = 0, velZ = 0; // Velocity estimates
bool launched = false; // Launch detection flag
// Filtered acceleration values
float filteredAx = 0, filteredAy = 0, filteredAz = 0;

void setup(void) // Starting conditions and sequence
{
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);  // Runs 100 kHz instead of 400 kHz
  pinMode(LED_BUILTIN, OUTPUT);
  bnoSetup();
}

void loop(void) // Loop for getting orientation
{
  unsigned long start = millis();

  getOrientation(); // Runs and prints all orientation data
  checkSerialCommands(); // for manual calibration
  sensorHealth(); // Checks sensor health features

  // Runs a 20 ms loop 
  delay(20 - (millis() - start));
}

void ledblink(void) // Method for blinking the LED
{
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

void bnoSetup() {
  
  Serial.println("Orientation Sensor Test: "); Serial.println("");
  
  // Initializes the sensor
  if(!bno.begin())
  {
    // Message displayed when BNO055 is not connected properly
    Serial.print("BNO055 not detected. Check your wiring or I2C ADDR.");
    while(1);
  }
  
  delay(1000);

  // Message displayed when BNO055 is connected
  Serial.println("The BNO055 has been detected. ");
  ledblink();
    
  bno.setExtCrystalUse(true);

  delay(250);

  if (!loadCalibration()) {
        if (doCalibration()) {  // Return true if calibration completed
            saveCalibration();
        } else { // Otherwise say that calibration failed
            Serial.println("Calibration failed or timed out.");
        }
    }
}

bool doCalibration() // For doing sensor calibration
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

    bno.getCalibration(&system, &gyro, &accel, &mag); 

    Serial.print("System: "); // Depends on other factors
    Serial.println(system);
    Serial.print("Gyro: "); // Leave sensor still for a bit
    Serial.println(gyro);
    Serial.print("Accel: "); // Slowly tilt sensor in different directions
    Serial.println(accel);
    Serial.print("Mag: "); // Slowly rotate sensor in a figure-8
    Serial.println(mag);

    delay(100);

    // Blinks when all values are calibrated
    if (system == 3 && gyro == 3 && accel == 3 && mag == 3) {
            Serial.println("All calibration components complete.");
            ledblink();  
            return true;
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
    Serial.println("The calibration has finished.");
    ledblink();
    return true;
  }

  return false;
}

void saveCalibration() { // Reads and stores calibration values
  adafruit_bno055_offsets_t offsets;
  bno.getSensorOffsets(offsets);
  prefs.begin("bno", false);
  // Saves to ESP32 flash memory
  prefs.putBytes("offsets", &offsets, sizeof(offsets));
  prefs.end();
  Serial.println("Calibration saved.");
}

bool loadCalibration() { // Opens flash memory to load stored calibration values
  adafruit_bno055_offsets_t offsets;
  prefs.begin("bno", true);
  if (prefs.getBytesLength("offsets") == sizeof(offsets)) {
    prefs.getBytes("offsets", &offsets, sizeof(offsets));
    prefs.end();
    bno.setSensorOffsets(offsets);
    Serial.println("Calibration restored.");
    return true;
  }
  prefs.end(); // If there is no calibration saved
  Serial.println("No saved calibration found.");
  return false;
}

void sensorHealth() { // Sensor health features
  dangerSense();

  // Detect if high sensor temperature
  int tempC = bno.getTemp();
  if (tempC >= TEMP_WARNING_C) {
    Serial.print("WARNING: IMU temp high (C): ");
    Serial.println(tempC);
  }
}

void manualCalibration() { // For manually starting calibration
    Serial.println("Starting manual calibration...");
    if (doCalibration()) {  // Run normal calibration
        saveCalibration();  // Save offsets if successful
        Serial.println("Manual calibration complete and saved.");
    } else {
        Serial.println("Manual calibration failed or timed out.");
    }
}

void checkSerialCommands() { // Check serial commands for manual calibration
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim(); // Remove whitespace/newline

        if (cmd.equalsIgnoreCase("calibration")) {
            manualCalibration(); // Manual call calibration
        }
    }
}

void dangerSense() { // Checking for system errors
  uint8_t system_status = 0, self_test = 0, system_error = 0; // Read system status
  bno.getSystemStatus(&system_status, &self_test, &system_error);

  if (system_error != 0) { // Note errors if there are any
    Serial.print("BNO055 system error: ");
    Serial.println(system_error);
    Serial.println("Attempting re-initialization");

    // Try a re-initialization attempt
    if (!bno.begin()) {
    Serial.println("Re-init failed.");
      } else {
    Serial.println("Re-init OK.");
    bno.setExtCrystalUse(true);
    loadCalibration();
    }
    }
}

void getOrientation() { // Getting all different datas for sensor orientation

  // For measuring loop time
  static unsigned long lastTime = millis(); 
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  // Float values of measured data
  float roll, pitch, yaw;
  float ax, ay, az;
  float gx, gy, gz;

  // Reading sensor data
  getLinAcc(ax, ay, az); // Linear acceleration
  orientation(roll, pitch, yaw); // Euler angles
  getGyro(gx, gy, gz); // Angular velocity

  // Read raw acceleration for launch detection
  imu::Vector<3> rawAccel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  float rawAccelMag = sqrt(rawAccel.x() * rawAccel.x() + 
  rawAccel.y() * rawAccel.y() + rawAccel.z() * rawAccel.z());

  filterAcceleration(ax, ay, az); // Filters linear acceleration

  detectLaunch(rawAccelMag); // Detects when it launches

  // Updates the velocity values
  velocityUpdate(filteredAx, filteredAy, filteredAz, dt);

  // Prints all measured values
  output(currentTime, roll, pitch, yaw, ax, ay, az, gx, gy, gz);
}

void filterAcceleration(float ax, float ay, float az) {
  // For filtering linear acceleration values
  float alpha = 0.5f;
  filteredAx = alpha * ax + (1 - alpha) * filteredAx;
  filteredAy = alpha * ay + (1 - alpha) * filteredAy;
  filteredAz = alpha * az + (1 - alpha) * filteredAz;
}

void detectLaunch(float rawAccelMag) { // For detecting rocket launch
  static uint8_t launchHits = 0;

  if (!launched) {
    if (rawAccelMag > 25.0f) {
      // Detect if raw acceleration magnitude is above value for launch
      launchHits++;
      if (launchHits >= 3) {
        launched = true;
        Serial.println("LAUNCH DETECTED");
      } // Print if above acceleration threshold for 3+ loops
    } else {
      launchHits = 0;
    }
  }
}

/**
* Reads gyroscope data. (rad/s)
*/
void getGyro(float &gx, float &gy, float &gz) {
  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  gx = gyro.x();
  gy = gyro.y();
  gz = gyro.z();

}

/**
* Reads linear acceleration.
*/
void getLinAcc(float &ax, float &ay, float &az) {
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  ax = accel.x();
  ay = accel.y();
  az = accel.z();
}

// Fuses orientation results
void orientation(float &roll, float &pitch, float &yaw) {
  sensors_event_t orientationData;
  bno.getEvent(&orientationData);

  roll = orientationData.orientation.x;
  pitch = orientationData.orientation.y;
  yaw = orientationData.orientation.z;
}

// Helps to integrate the velocity
void velocityUpdate(float ax, float ay, float az, float dt) {
  velX += ax * dt;
  velY += ay * dt;
  velZ += az * dt;

  if (!launched && fabsf(ax) < 0.05f && fabsf(ay) < 0.05f && fabsf(az) < 0.05f) {
    velX = 0;
    velY = 0;
    velZ = 0;
  }
}

// Output of orientation data printed
void output(unsigned long time,  float roll, float pitch, float yaw, float ax, float ay, float az, float gx, float gy, float gz) {
  Serial.print(time); Serial.print(",");
  Serial.print(roll); Serial.print(",");
  Serial.print(pitch); Serial.print(",");
  Serial.print(yaw); Serial.print(",");
  Serial.print(ax); Serial.print(",");
  Serial.print(ay); Serial.print(",");
  Serial.print(az); Serial.print(",");
  Serial.print(velX); Serial.print(",");
  Serial.print(velY); Serial.print(",");
  Serial.println(velZ);
}
