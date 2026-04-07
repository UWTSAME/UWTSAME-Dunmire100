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

bool firstSet = false; 
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
  firstSet = true; //Marks that starting GPS position has been saved (prevents resetting)
}

// read and parse gps data
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

    // Set first GPS fix once valid
    if(!firstSet && gps.location.isValid()) {
      setFirstGps(); 
    }
  } 

  return returner;
}

void gpsDebug(){
  bool newInfo = updateGPS();
  Serial.print("Data Ready: ");
  Serial.println(newInfo);
  //Only displays information if we get new info
  if(newInfo){
    double distance = calculateDistance(firstGpsLat, firstGpsLon, gpsLat, gpsLon);
    double bearing = calculateBearing(firstGpsLat, firstGpsLon, gpsLat, gpsLon);

    Serial.print(" Distance from start (m): ");
    Serial.println(distance);
    Serial.print(" Bearing (deg): ");
    Serial.println(bearing);
    Serial.print(" Direction: ");
    Serial.println(getCompassDirection(bearing));
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
    Serial.print("RECOVERY: Go ");
    Serial.print(distance);
    Serial.print(" meters toward ");
    Serial.println(getCompassDirection(bearing));
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

//calculates distance between two GPS points using Haversine formula
double calculateDistance(double lat1, double lon1, double lat2, double lon2){ 
  const double R = 6371000; // Earth radius in meters

  double latRad1 = radians(lat1);
  double latRad2 = radians(lat2);
  double deltaLat = radians(lat2 - lat1);
  double deltaLon = radians(lon2 - lon1);

  double a = sin(deltaLat / 2) * sin(deltaLat / 2) +
             cos(latRad1) * cos(latRad2) *
             sin(deltaLon / 2) * sin(deltaLon / 2);

  double c = 2 * atan2(sqrt(a), sqrt(1-a));

  return R * c; //distance in meters
}
// Calculates direction from start point to current point
double calculateBearing(double lat1, double lon1, double lat2, double lon2){
  double latRad1 = radians(lat1);
  double latRad2 = radians(lat2);
  double deltaLon = radians(lon2 - lon1);

  double y = sin(deltaLon) * cos(latRad2);
  double x = cos(latRad1) * sin(latRad2) -
             sin(latRad1) * cos(latRad2) * cos(deltaLon);

  double bearing = atan2(y, x);
  bearing = degrees(bearing);

  // Normalize to 0–360°
  if (bearing < 0) bearing += 360;

  return bearing;
}
// Converts bearing to compass direction
String getCompassDirection(double bearing) {
  if (bearing < 22.5) return "N";
  else if (bearing < 67.5) return "NE";
  else if (bearing < 112.5) return "E";
  else if (bearing < 157.5) return "SE";
  else if (bearing < 202.5) return "S";
  else if (bearing < 247.5) return "SW";
  else if (bearing < 292.5) return "W";
  else if (bearing < 337.5) return "NW";
  else return "N";
}