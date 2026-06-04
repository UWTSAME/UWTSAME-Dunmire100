#include "Altimeter.h"
#include "Gps.h"
#include "BnoSensor.h"
#include "Lora.h"
#include "SdCard.h"

#define GpsRX 16
#define GpsTX 17
#define LoraRX 15
#define LoraTX 4
#define LED_VERIFY 12
#define LED_ALERT 26

String dataString = "";
int loopCount = 0;
const float SEA_LEVEL_PRESSURE_HPA = 1019.4;

Altimeter alt(SEA_LEVEL_PRESSURE_HPA);
Gps gps(GpsRX, GpsTX);
BnoSensor theBno;
Lora lora(LoraRX, LoraTX);
SdCard sd;

void setup(){
  Serial.begin(115200);

  pinMode(LED_VERIFY, OUTPUT);
  pinMode(LED_ALERT, OUTPUT);

  delay(2000); //Wait for Serial and Parts to boot up
  bootUp();
}

void loop(){
  alt.update(); // Read altimeter data
  gps.update(); // Read gps data
  theBno.update(); // Read imu data

  dataString = formDataString();

  loopCount++;

  if(loopCount > 6){
    lora.sendData(dataString); // Send packet over Lora
    if(sd.write(dataString)){ // Write packet to SD card
      dataString = "";
      loopCount = 0;
    }
  }
  delay(20); // 1000ms / 20ms = 50Hz rate
}

void bootUp(){

  //upon successful boot, displays successful LED
  alt.begin() ? flashLED(LED_VERIFY) : flashLED(LED_ALERT);       //starts altimiter
  theBno.begin() ? flashLED(LED_VERIFY) : flashLED(LED_ALERT);    //starts the bno
  lora.begin() ? flashLED(LED_VERIFY) : flashLED(LED_ALERT);      //starts the lora
  gps.begin() ? flashLED(LED_VERIFY) : flashLED(LED_ALERT);       //starts the GPS
  //If the SD card successfully sets up, displays succesful LED and writes the data headers.
  if(sd.begin()){                                                 //starts the SD card
    flashLED(LED_VERIFY);
    sd.write("Time, Current Alt, Relative Alt, Temperature (C), Latitude,"
              "Longitude, GPS Altitude, distance, direction, Lora Status, X Orientation, "
              "Y Orientation, Z orientation");
  } else {
    flashLED(LED_ALERT);                          //tells if the SD card started properly
  }
  
}

void flashLED(int pinNum){
  digitalWrite(pinNum, HIGH);
  delay(500);
  digitalWrite(pinNum, LOW);
  delay(500);
}

String formDataString(){ // Form message from all the data       
  String newData = 
        String(millis()) + "," +
        String(alt.getAlt()) + "," +
        String(alt.getRelAlt()) + "," +
        String(alt.getTempC()) + "," + 
        String(gps.getLat(), 6) + "," +
        String(gps.getLon(), 6) + "," +
        String(gps.getAlt(), 6) + "," +
        String(gps.getDistance()) + "," +
        String(gps.getDirection()) + "," +
        String(lora.status()) + "," + 
        String(theBno.getOrientationX()) + "," +
        String(theBno.getOrientationY()) + "," +
        String(theBno.getOrientationZ());
  return newData;
}





















