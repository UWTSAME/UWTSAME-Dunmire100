#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

const int ARRAY_SIZE = 6;
double initial[3];
bool done = false;

void setup(void) 
{
  bnoSetup();


}

void saveOrientation() {  //Designed to be before setup

  sensors_event_t event;
  bno.getEvent(&event);

  Serial.println("Hold detector still.");
  double roll;
  double pitch;
  double yaw;

  for (int i = 0; i < 3; i++) {
    roll = event.orientation.x, 4;
    pitch = event.orientation.y, 4;
    yaw = event.orientation.z, 4;
    delay(20000);

  }

  initial[0] = roll;
  initial[1] = pitch;
  initial[2] = yaw;

}

void loop(void) 
{
  // if (done == true) {


  // }

  getVectors();
    
  delay(100);


}

void bnoSetup() {
  
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);

}

void getOrientation() {
  String orientation[ARRAY_SIZE];

  sensors_event_t event;
  bno.getEvent(&event);

  orientation[0] = "Roll:";
  orientation[1] = String((event.orientation.x, 4) - initial[0]);
  orientation[2] = "Pitch:";
  orientation[3] = String((event.orientation.y, 4) - initial[1]);
  orientation[4] = "Yaw:";
  orientation[5] = String((event.orientation.z, 4) - initial[2]);

  for (int i = 0; i < ARRAY_SIZE; i++) {
    Serial.print(orientation[i] + " ");
  }
  Serial.println();
}

void getVectors() {

  imu::Quaternion quat = bno.getQuat();
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  Serial.print("Acc"); 
  imu::Vector<3> gyr = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE); 
  // Serial.print("Gyr");      
  imu::Vector<3> gra = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY); 
  // Serial.print("Gra");       
  imu::Vector<3> linAccel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  // Serial.println("X: "); 
  // Serial.print(acc.x());
}





