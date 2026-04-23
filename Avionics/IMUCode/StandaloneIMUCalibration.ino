#include <Wire.h> 
#include <Adafruit_Sensor.h> 
#include <Adafruit_BNO055.h> 
#include <utility/imumaths.h> 
#include <Preferences.h> 
#include <math.h>

#define LED_BUILTIN 2

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Preferences prefs;

void setup(void) // Starting conditions and sequence
{
  Serial.begin(115200);
  Wire.begin(); // Initiate I2C
  Wire.setClock(100000);  // Set 100kHz
  pinMode(LED_BUILTIN, OUTPUT); // LED output
  bnoSetup(); // Start the BNO055
}

void loop(void)
{
  // nothing
}


void ledblink(void) // Method for blinking the LED
{
  for (int i = 0; i < 10; i++) { // 10 blinks in succesion
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

void bnoSetup() {
  
  Serial.println("Stand-alone Calibration"); 
  Serial.println("");
  
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
  ledblink(); // ESP32 LED blinks when detected
    
  bno.setExtCrystalUse(true);

  delay(250);

// Always force calibration
  if (doCalibration()) {
    saveCalibration();
    Serial.println("Calibration complete. You can now power off.");
  } else {
    Serial.println("Calibration failed or timed out.");
  }
}

bool doCalibration() // For doing sensor calibration
{
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0; // Start values
  unsigned long startTime = millis(); // Start timer
  const unsigned long timeout = 150000; // Timeout limit
  bool timedOut = false; 

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
  bno.getSensorOffsets(offsets); // Get current offset
  prefs.begin("bno", false); // Open NVS write
  prefs.putBytes("offsets", &offsets, sizeof(offsets)); // Saves to ESP32 flash memory
  prefs.end(); // Close NVS write
  Serial.println("Calibration saved.");
}