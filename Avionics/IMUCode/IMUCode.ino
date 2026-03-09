#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Preferences.h>
#define LED_BUILTIN 2

  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Preferences prefs;

void setup(void) // Starting conditions and sequence
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  bnoSetup();
}

void loop(void) // Loop for getting orientation
{
  // Serial.print(getOrientation()); REMEMBER TO UNCOMMENT THIS ONCE DONE
  delay(100);
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

void calibrationblink(void) // LED Blinking for finishing calibration
{
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
  }
}

void bnoSetup() {
  
  Serial.println("Orientation Sensor Test: "); Serial.println("");
  
  // Initialize the sensor
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

    // Blinks every time a value is fully calibrated
    if (system == 3 && !systemDone) {
      calibrationblink();
      systemDone = true; 
    }
    if (gyro == 3 && !gyroDone) {
      calibrationblink();
      gyroDone = true;
    }
    if (accel == 3 && !accelDone) {
      calibrationblink();
      accelDone = true;
    }
    if (mag == 3 && !magDone) {
      calibrationblink();
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
  prefs.end();
  Serial.println("No saved calibration found.");
  return false;
}

void sensorHealth() { // TO-DO
  // detect if IC2 connection drops
  // detect if sensor is overheating
  // detect if sensor freezes
  // detect if calibration is still valid
  // detect if sudden impact/movements
  // detect if saved calibration is valid
  // sensor reset/reinitialize if invalid
}

void manualCalibration() {
  // Activation for manually calibrating the sensor if needed
}