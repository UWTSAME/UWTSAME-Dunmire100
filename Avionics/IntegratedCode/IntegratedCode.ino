#include "Altimeter.h"
#include "Gps.h"
#include "Imu.h"
#include "Lora.h"
#include "SdCard.h"

#define GpsRX 16
#define GpsTX 17
#define LoraRX 15
#define LoraTX 4

const float SEA_LEVEL_PRESSURE_HPA = 1019.4;

Altimeter alt(SEA_LEVEL_PRESSURE_HPA);
Gps gps(GspRX, GpsTX);
Imu imu;
Lora lora(LoraRX, LoraTX);
SdCard sd;

void setup(){
  Serial.begin(115200);

  if(!alt.setup()){
    Serial.println("Altimeter setup failed");
  }

  if(!gps.begin()){
    Serial.println("GPS setup failed");
  }

  if(!imu.begin()){
    Serial.println("IMU setup failed");
  }

  if(!lora.begin()){
    Serial.println("Lora setup failed");
  }

  if(!sdLog.begin()){
    Serial.println("SD card setup failed");
  }
}

void loop(){
  alt.update();
  gps.update();

  Serial.println(alt.getAlt());
  Serial.println(gps.getAlt());
  Serial.println(imu.getOrientationX());
  Serial.println(lora.status());
  Serial.println(sd.write("Hello World!"));

  delay(20);
}
























