#include <TinyGPSPlus.h>
// Declaring GPIO pins 16 and 17 as the RX and TX of the Serial 2 line used by the GPS.
#define RX2 16
#define TX2 17

float gpsLat = 0;
float gpsLon = 0;
float gpsAlt = 0;
float firstGpsLon = 0;
float firstGpsLat = 0;
float firstGpsAlt = 0;

//Creates a HardwareSerial object. The number 2 declares it as a 2nd UART line.
HardwareSerial GPSSerial(2);

TinyGPSPlus gps;


void setup() {

  Serial.begin(115200);
  
  while (!Serial) delay(10);

  // GPS Baud rate. Can be different than the Microcontrollers Serial BAUD rate.
  GPSSerial.begin(9600, SERIAL_8N1, RX2, TX2);

}


void loop() {

  gpsDebug();

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

//For the purposes of distance and direction we might need a more advanced gpsDebug function.

// To-do
// Parsing GPS data into usable values
// If needed, unit conversions
// Calculations of distance based on GPS values
// If needed, filtering GPS data (related to parsing?)
// Finding GPS direction



