#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_9DOF                 dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/
void initSensors()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  
  /* Initialise the sensors */
  initSensors();
}

void rotateCoordinates(float *data, float *rotation) // angles have to be in radians
{
  // data:
  // data[0]: measured rotation around absolute x axis (roll)
  // data[1]: measured rotation around absolute y axis (pitch)
  // data[2]: measured rotation around absolute z axis (yaw/heading)

  // rotation:
  // rotation[0]: desired frame rotation around absolute x axis (roll)
  // rotation[1]: desired frame rotation around absolute y axis (pitch)
  // rotation[2]: desired frame rotation around absolute z axis (yaw/heading)
   
    data[0] = (cos(rotation[2])*cos(rotation[1]) + sin(rotation[2])*sin(rotation[1])*sin(rotation[0]))*data[0] + (sin(rotation[2])*cos(rotation[1]) - sin(rotation[1])*sin(rotation[0])*cos(rotation[2]))*data[1] + (-sin(rotation[1])*cos(rotation[0]))*data[2];
    data[1] = (-sin(rotation[2])*cos(rotation[0]))*data[0] + (cos(rotation[2])*cos(rotation[0]))*data[1] + (-sin(rotation[0]))*data[2];
    data[2] = (cos(rotation[2])*sin(rotation[1]) - sin(rotation[2])*cos(rotation[1])*sin(rotation[0]))*data[0] + (sin(rotation[2])*sin(rotation[1]) + sin(rotation[0])*cos(rotation[2])*cos(rotation[1]))*data[1] + (cos(rotation[1])*cos(rotation[0])) *data[2]; 
}


/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;

  float data[3];
  bool resultAccel;
  bool resultMag;
  bool resultCompensation;
  float rotation[3];

  accel.getEvent(&accel_event);
  mag.getEvent(&mag_event);

  float xyzAccelB[3] = {accel_event.acceleration.x, accel_event.acceleration.y, accel_event.acceleration.z};
  float xyzMagB[3] = {mag_event.magnetic.x, mag_event.magnetic.y, mag_event.magnetic.z};

   Serial.print(F("x: "));
  Serial.print(xyzAccelB[0]);
  Serial.print(F("; "));
  Serial.print(F("y: "));
  Serial.print(xyzAccelB[1]);
  Serial.print(F("; "));
  Serial.print(F("z: "));
  Serial.print(xyzAccelB[2]);
  Serial.print(F("; "));
  
  resultCompensation = dof.magTiltCompensation(SENSOR_AXIS_Z, &mag_event, &accel_event);
  resultAccel = dof.accelGetOrientation(&accel_event, &orientation);
  resultMag = dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation);
  
  rotation[0] = 0;
  rotation[1] = orientation.pitch/180.0*PI;
  rotation[2] = 0;

  rotateCoordinates(xyzAccelB, rotation);
  rotateCoordinates(xyzMagB, rotation);
  
  accel_event.acceleration.x = xyzAccelB[0];
  accel_event.acceleration.y = xyzAccelB[1];
  accel_event.acceleration.z = xyzAccelB[2];

  mag_event.magnetic.x = xyzMagB[0];
  mag_event.magnetic.y = xyzMagB[1];
  mag_event.magnetic.z = xyzMagB[2];

  resultCompensation = dof.magTiltCompensation(SENSOR_AXIS_Z, &mag_event, &accel_event);
  resultAccel = dof.accelGetOrientation(&accel_event, &orientation);
  resultMag = dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation);
  
  if (resultMag && resultAccel && resultCompensation)
  {
    data[0] = orientation.roll;
    data[1] = orientation.pitch;
    data[2] = orientation.heading;

//    rotateCoordinates(data, rotation);
//    Serial.print(F("Roll: "));
//    Serial.print(data[0]);
//    Serial.print(F("; "));
//    Serial.print(F("Pitch: "));
//    Serial.print(data[1]);
//    Serial.print(F("; "));
//    Serial.print(F("Heading: "));
//    Serial.print(data[2]);
//    Serial.print(F("; "));
  }
  

  Serial.println(F(""));
  delay(300);
}
