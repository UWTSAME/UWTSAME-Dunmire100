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
int loopCount = 1;
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

  
}

void loop(){
  alt.update();
  gps.update();

  dataString = formDataString(dataString);

  if(loopCount > 6){
    if(sd.write(dataString)){
      dataString = "";
      loopCount = 0;
    }
  } else {
    dataString += "\n";
  }
  
  loopCount++;
}

void bootUp(){

  //upon successful boot, displays successful LED
  alt.begin() ? flashLED(LED_VERIFY) : flashLED(LED_ALERT);
  theBno.begin() ? flashLED(LED_VERIFY) : flashLED(LED_ALERT);
  lora.begin() ? flashLED(LED_VERIFY) : flashLED(LED_ALERT);
  gps.begin() ? flashLED(LED_VERIFY) : flashLED(LED_ALERT);
  //If the SD card successfully sets up, displays succesful LED and writes the data headers.
  if(sd.begin()){
    flashLED(LED_VERIFY);
    sd.write("Time, Current Alt, Relative Alt, Temperature (C), Latitude,"
              "Longitude, GPS Altitude, distance, direction, Lora Status, X Orientation, "
              "Y Orientation, Z orientation");
  } else {
    flashLED(LED_ALERT);
  }
  
}

void flashLED(int pinNum){
  digitalWrite(pinNum, HIGH);
  delay(500);
  digitalWrite(pinNum, LOW);
  delay(500);
}

String formDataString(String thePrevData){
  String newData = 
        String(millis()) + "," + alt.getAlt() + "," + alt.getRelAlt() + "," + alt.getTempC() + "," + 
        String(gps.getLat(), 6) + "," + String(gps.getLon(), 6) + "," + String(gps.getAlt(),6) +
        "," + gps.getDistance() + "," + gps.getDirection() + "," + "lora.status()" + "," + 
        theBno.getOrientationX() + "," + theBno.getOrientationY() + "," + theBno.getOrientationZ();
  return thePrevData+newData;
}





















