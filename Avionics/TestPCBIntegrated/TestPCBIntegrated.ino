//Libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP5xx.h"
#include <TinyGPSPlus.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Preferences.h>
#include <SD.h>
#include <SPI.h>

//Wire assignments:
#define LED_BUILTIN 2
//For the GPS
#define RX2 16
#define TX2 17
//For the SD card adapter
#define chip_select 5
//Assumed Wire assignments from libraries:
//For the BMP580 and BNO055
// SCL is set to 21 on ESP32
// SDA is set to 22 on ESP32


/*******Global variables*********/
//Gps
float gpsLat = 0;
float gpsLon = 0;
float gpsAlt = 0;
float firstGpsLon = 0;
float firstGpsLat = 0;
float firstGpsAlt = 0;
//Altimeter
const int SEALEVELPRESSURE_HPA (1019.4);
float firstAlt;
//SD card adapter
bool THISISDUMB = false;
int folderNum = 0;
String folderPath = "";

/*******Global constants*********/
//IMU
const int TEMP_WARNING_C = 70; 

/*******Object Creations*********/
//Altimeter
Adafruit_BMP5xx bmp; 
bmp5xx_powermode_t desiredMode = BMP5XX_POWERMODE_NORMAL; // Cache desired power mode
//GPS
HardwareSerial GPSSerial(2); 
TinyGPSPlus gps;
//IMU
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Preferences prefs;
//SD card adapter
File SD_card;

void setup() {
  Serial.begin(115200); //Sets up baud rate to 115200.
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial) delay(10);  // Wait for Serial Monitor to open
  AltSetup();
  gpsSetup();
  bnoSetup();
}

void gpsSetup(){
  GPSSerial.begin(9600, SERIAL_8N1, RX2, TX2);
}

void AltSetup(){
    //sets up the functions and presets for the sensor chip
  // Try to initialize the sensor
  // For I2C mode:
  if (!bmp.begin(BMP5XX_ALTERNATIVE_ADDRESS, &Wire)) {
    while (true) AlertLights();
  }
  VerifyLights();
  // Demonstrate all setter functions with range documentation
  bmp.setTemperatureOversampling(BMP5XX_OVERSAMPLING_2X);   //Setting temperature oversampling to 2X
  bmp.setPressureOversampling(BMP5XX_OVERSAMPLING_16X);     //Setting pressure oversampling to 16X
  bmp.setIIRFilterCoeff(BMP5XX_IIR_FILTER_COEFF_3);         //Setting IIR filter to coefficient 3
  bmp.setOutputDataRate(BMP5XX_ODR_50_HZ);                  //Setting output data rate to 50 Hz
  bmp.setPowerMode(BMP5XX_POWERMODE_NORMAL);                //Setting power mode to normal
  //Sets up the sensor
  bmp.enablePressure(true);
  bmp.configureInterrupt(BMP5XX_INTERRUPT_LATCHED, BMP5XX_INTERRUPT_ACTIVE_HIGH, BMP5XX_INTERRUPT_PUSH_PULL, BMP5XX_INTERRUPT_DATA_READY, true); 

  //Sets the first recorded altitude.
  firstAlt = GetAlt(); 
}

bool UpdateAltimeter(){
  bool returners = false;
  // Check if new data is ready before reading
  if(bmp.dataReady()){
    //If data is ready, updates the state fields of the bmp.
    returners = bmp.performReading();

  }
  return returners;
} 

float GetTempC(){
  return bmp.temperature;
}
float GetTempF(){
  return (GetTempC() * (9.0 / 5.0) + 32.0);
}

float GetPressure(){
  return bmp.pressure;
}

float GetAlt(){
   return bmp.readAltitude(SEALEVELPRESSURE_HPA) * 3.28;
}

float GetRelAlt(){
   return bmp.readAltitude(SEALEVELPRESSURE_HPA) * 3.28 - firstAlt;
}

void AlertLights(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void VerifyLights(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
}


//EDIT DEMO
//EDIT DEMO 1.5
//EDIT DEMO 2
//ORANGE
void AltDebug(){
  Serial.print("Data Ready:");
  Serial.print(UpdateAltimeter());
  Serial.print("  First Alt: ");
  Serial.print(firstAlt);
  Serial.print("  Current Alt: ");
  Serial.print(GetAlt());
  Serial.print("  Relative Displacement: ");
  Serial.print(GetRelAlt());
  Serial.print("  Current temp C");
  Serial.println(GetTempC());
  

}


void setFirstGps(){
  firstGpsLon = gpsLon;
  firstGpsLat = gpsLat;
  firstGpsAlt = gpsAlt;
}

bool updateGPS(){
  bool returner = false;
  while(GPSSerial.available() != 0){
    char gpsOutput = GPSSerial.read();
    if(gps.encode(gpsOutput)){
      gpsLat = gps.location.lat();
      gpsLon = gps.location.lng();
      gpsAlt = gps.altitude.meters();
      returner = true;
    }
    //If the first gps lon hasn't been set it updates all first values.
    //I guess it won't work if you are exactly on the prime meridian.
    if(firstGpsLon == 0) setFirstGps();
  }
  return returner;
}

void gpsDebug(){
  bool newInfo = updateGPS();
  Serial.print("Data Ready: ");
  Serial.println(newInfo);
  //Only displays information if we get new info
  if(newInfo){
    Serial.print(" First Alt: ");
    Serial.print(firstGpsAlt);
    Serial.print(" Current Alt: ");
    Serial.println(gpsAlt);
    Serial.print(" First Latitude: ");
    Serial.print(firstGpsLat);
    Serial.print(" First Longitude: ");
    Serial.print(firstGpsLon);
    Serial.print(" Current Lat: ");
    Serial.print(gpsLat);
    Serial.print(" Current Lon: ");
    Serial.println(gpsLon);
  }
  delay(1000);
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
  AlertLights();
    
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

void bnoDebug(){
  // Bare basics to see if the sensor is alive
  sensors_event_t event;
  bno.getEvent(&event);

  Serial.print("X: "); Serial.print(event.orientation.x);
  Serial.print(" Y: "); Serial.print(event.orientation.y);
  Serial.print(" Z: "); Serial.println(event.orientation.z);

  delay(100); 
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
      VerifyLights();
      systemDone = true; 
    }
    if (gyro == 3 && !gyroDone) {
      VerifyLights();
      gyroDone = true;
    }
    if (accel == 3 && !accelDone) {
      VerifyLights();
      accelDone = true;
    }
    if (mag == 3 && !magDone) {
      VerifyLights();
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
    AlertLights();
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

void folderMaker() {
  while (true) {
    folderPath = "/" + String(folderNum);

    if (!SD.exists(folderPath)) {
      SD.mkdir(folderPath);
      break;
    }
    folderNum++;
  }
}

void sdSetup(){
  SD.begin(chip_select);                                          // initializes SD card reader
  folderMaker();                                                  // calls the function to gernate a folder in the SD card
  SD_card = SD.open("/" + folderPath + "/Data.txt", FILE_WRITE);  // Creates a text file in the new folder where the data will be stored.
  Serial.print("GO");
}

void sdDebug(){
  SD_card.println("Hello World!");
}

void loop() {
  AltDebug();
  gpsDebug();
  bnoDebug();
  sdDebug();
}

