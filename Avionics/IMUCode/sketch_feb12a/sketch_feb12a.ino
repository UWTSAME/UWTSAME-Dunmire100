#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// BNO object. 
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Velocity Variables
float velX = 0, velY = 0, velZ = 0;

//Last loop iteration
unsigned long lastTime = 0;

//Function prototypes
//Introducing functions to declare them in scope
void initialize();
void orientation(float &roll, float &pitch, float &yaw);
void getLinAcc(float &ax, float &ay, float &az);
void getGyro(float &gx, float &gy, float &gz);
void velocityUpdate(float ax, float ay, float az, float dt);
void output(unsignted long time, float roll, float pitch, float yaw, float ax, float ay, float az, float gx, float gy, float gz);

void setup() {
  initialize();

}

void loop() {
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  //Variables
  float roll, pitch, yaw;
  float ax, ay, az;
  float gx, gy, gz;

  // Reading data
  getLinAcc(ax, ay, az);
  orientation(roll, pitch, yaw);
  getGryo(gx, gy, gz);

  //Updating velocity
  velocityUpdate(ax, ay, az, dt);

  //Printing data
  output(currentTime, roll, pitch, yaw, ax, ay, az, gx, gy, gz);
  delay(200);
}

void initialize() {
  Serial.begin(115200);   // bits / second

  //Searching for IMU
  while(!bno.begin()) {
    Serial.println("...Searching for IMU...");
    delay(1000);  //Waits 1 second
  }

  delay(1000);
  bno.setExtCrystalUse(true); //Sets the IMU to use it's external clock.

  lastTime = millis();  //The miliseconds since this starting running.
  Serial.println("IMU found.");
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

void orientation(float &roll, float &pitch, float &yaw) {
  sensors_event_t orientationData;
  bno.getEvent(&orientationData);

  roll = orientationData.orientation.x;
  pitch = orientationData.orientation.y;
  yaw = orientationData.orientation.z;
}

void velocityUpdate() {
  velX += ax * dt;
  velY += ay * dt;
  velZ += az * dt;

  if (abs(ax) < 0.05 && abs(ay) < 0.05 && abs(az) < 0.05) {
    velX = 0;
    velY = 0;
    velZ = 0;
  }
}

void output(unsigned long time,  float roll, float pitch, float yaw, float ax, float ay, float az, float gx, float gy, float gz) {
  Serial.println("======================================");
  Serial.print("Time: "); Serial.println(time);
  
  Serial.println("\nOrientation");
  Serial.print("Roll: "); Serial.println(roll);
  Serial.print("Pitch: "); Serial.println(pitch);
  Serial.print("Yaw: "); Serial.println(yaw);

  Serial.println("\nVelocity");
  Serial.print("vx: "); Serial.println(velX);
  Serial.print("vy: "); Serial.println(velY);
  Serial.print("vz: "); Serial.println(velZ);

  Serial.println("\nAcceleration");
  Serial.print("aX: "); Serial.println(ax);
  Serial.print("aY: "); Serial.println(ay);
  Serial.print("aZ: "); Serial.println(az);

  Serial.println("\nGryoscope");
  Serial.print("gX: "); Serial.println(gx);
  Serial.print("gY: "); Serial.println(gy);
  Serial.print("gZ: "); Serial.println(gz);
}