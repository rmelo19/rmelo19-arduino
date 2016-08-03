#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
//#include "vector.h"/

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

void rotateCoordinates(double *vector, double *rotation) // rotation angles have to be in radians
{
  // vector:
  // vector[0]: measured rotation around absolute x axis (roll)
  // vector[1]: measured rotation around absolute y axis (pitch)
  // vector[2]: measured rotation around absolute z axis (yaw/heading)

  // rotation:
  // rotation[0]: desired frame rotation around absolute x axis (roll)
  // rotation[1]: desired frame rotation around absolute y axis (pitch)
  // rotation[2]: desired frame rotation around absolute z axis (yaw/heading)

  float vectorAux[3] = {vector[0], vector[1], vector[2]};

  // This expression needs to be checked:
  //    vector[0] = (cos(rotation[2])*cos(rotation[1]) + sin(rotation[2])*sin(rotation[1])*sin(rotation[0]))*vectorAux[0] + (sin(rotation[2])*cos(rotation[1]) - sin(rotation[1])*sin(rotation[0])*cos(rotation[2]))*vectorAux[1] + (-sin(rotation[1])*cos(rotation[0]))*vectorAux[2];
  //    vector[1] = (-sin(rotation[2])*cos(rotation[0]))*vectorAux[0] + (cos(rotation[2])*cos(rotation[0]))*vectorAux[1] + (-sin(rotation[0]))*vectorAux[2];
  //    vector[2] = (cos(rotation[2])*sin(rotation[1]) - sin(rotation[2])*cos(rotation[1])*sin(rotation[0]))*vectorAux[0] + (sin(rotation[2])*sin(rotation[1]) + sin(rotation[0])*cos(rotation[2])*cos(rotation[1]))*vectorAux[1] + (cos(rotation[1])*cos(rotation[0])) *vectorAux[2]; 

  // Rotation z, then y, then x: link http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
  // alpha = rotation[0] = roll
  // beta = rotation[1] = pitch
  // gamma = rotation[2] = yaw

  // Rotation x, then y, then z: link http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
  // alpha = rotation[0] = roll
  // beta = rotation[1] = pitch
  // gamma = rotation[2] = yaw
  vector[0] = (cos(rotation[1])*cos(rotation[2]))*vectorAux[0] + (cos(rotation[2])*sin(rotation[0])*sin(rotation[1])-cos(rotation[0])*sin(rotation[2]))*vectorAux[1] + (cos(rotation[0])*cos(rotation[2])*sin(rotation[1]) + sin(rotation[0])*sin(rotation[2]))*vectorAux[2];
  vector[1] = (cos(rotation[1])*sin(rotation[2]))*vectorAux[0] + (cos(rotation[0])*cos(rotation[2])+sin(rotation[0])*sin(rotation[1])*sin(rotation[2]))*vectorAux[1] + (-cos(rotation[2])*sin(rotation[0])+cos(rotation[0])*sin(rotation[1])*sin(rotation[2]))*vectorAux[2];
  vector[2] = (-sin(rotation[1]))*vectorAux[0] + (cos(rotation[1])*sin(rotation[0]))*vectorAux[1] + (cos(rotation[0])*cos(rotation[1]))*vectorAux[2];
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

  bool resultAccel;
  bool resultMag;
  bool resultCompensation;
  double rotation[3];
  double rotationRef[3];
  double devAngle;
//  float yUnit[3] = {0, 1, 0};
  double xUnitRef[3] = {1, 0, 0};
  double yUnitRef[3] = {0, 1, 0};
  double zUnitRef[3] = {0, 0, 1};
  double zUnit[3] = {0, 0, 1};
  double projXYZ[3] = {0};
  double encoderAngle = 0;
  int len;
//  uint8_t buf[200] = {0};
  uint16_t buf = 0;
//  char aux;/
  

  accel.getEvent(&accel_event);
  mag.getEvent(&mag_event);
  resultCompensation = dof.magTiltCompensation(SENSOR_AXIS_Z, &mag_event, &accel_event);
  resultAccel = dof.accelGetOrientation(&accel_event, &orientation);
  resultMag = dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation);
  rotationRef[0] = orientation.roll/180.0*PI;
  rotationRef[1] = orientation.pitch/180.0*PI;
  rotationRef[2] = orientation.heading/180.0*PI;

//  rotateCoordinates(yUnit, rotation);
  // This vector is static
  
  rotateCoordinates(xUnitRef, rotationRef);
  rotateCoordinates(yUnitRef, rotationRef);
  rotateCoordinates(zUnitRef, rotationRef);

  while(1)
  {
      accel.getEvent(&accel_event);
      mag.getEvent(&mag_event);
      resultCompensation = dof.magTiltCompensation(SENSOR_AXIS_Z, &mag_event, &accel_event);
      resultAccel = dof.accelGetOrientation(&accel_event, &orientation);
      resultMag = dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation);
      rotation[0] = orientation.roll/180.0*PI;
      rotation[1] = orientation.pitch/180.0*PI;
      rotation[2] = orientation.heading/180.0*PI;
      zUnit[0] = 0;
      zUnit[1] = 0;
      zUnit[2] = 1;
      rotateCoordinates(zUnit, rotation);
  
      if (resultMag && resultAccel && resultCompensation)
      {
        // deviation angle:
//        devAngle = /AccelB[0]);

//        Serial.print("zUnit: ");
////        Serial.print(sqrt(zU/nit[0]*zUnit[0] + zUnit[1]*zUnit[1] + zUnit[2]*zUnit[2]));
//        Serial.print(" X: ");
//        Serial.print(zUnit[0], 3);
//        Serial.print(" Y: ");
//        Serial.print(zUnit[1], 3);
//        Serial.print(" Z: ");
//        Serial.print(zUnit[2], 3);
//        Serial.print(" zUnitRef: ");
////        Serial.print(sqrt(zUnitR/ef[0]*zUnitRef[0] + zUnitRef[1]*zUnitRef[1] + zUnitRef[2]*zUnitRef[2]));
//        Serial.print(" X: ");
//        Serial.print(zUnitRef[0], 3);
//        Serial.print(" Y: ");
//        Serial.print(zUnitRef[1], 3);
//        Serial.print(" Z: ");
//        Serial.print(zUnitRef[2], 3);

        projXYZ[0] = xUnitRef[0]*zUnit[0] + xUnitRef[1]*zUnit[1] + xUnitRef[2]*zUnit[2];
        projXYZ[1] = yUnitRef[0]*zUnit[0] + yUnitRef[1]*zUnit[1] + yUnitRef[2]*zUnit[2];
        projXYZ[2] = zUnitRef[0]*zUnit[0] + zUnitRef[1]*zUnit[1] + zUnitRef[2]*zUnit[2];
        
        Serial.print(" Proj: ");
        Serial.print(" XRef: ");
        Serial.print(projXYZ[0], 3);
        Serial.print(" YRef: ");
        Serial.print(projXYZ[1], 3);
        Serial.print(" ZRef: ");
        Serial.print(projXYZ[2], 3);
        Serial.print(" Enc: ");
        Serial.println(encoderAngle);

        len = Serial.available();
        if (len)
        {
          buf = Serial.parseInt(); 
          encoderAngle = (double) buf;

          rotationRef[1] = rotationRef[1] + encoderAngle/180*PI;
//          rotationRef[1] = orientation.pitch/180.0*PI;
//          rotationRef[2] = orientation.heading/180.0*PI;
          xUnitRef[0] = 1;
          xUnitRef[1] = 0;
          xUnitRef[2] = 0;
          yUnitRef[0] = 0;
          yUnitRef[1] = 1;
          yUnitRef[2] = 0;
          zUnitRef[0] = 0;
          zUnitRef[1] = 0;
          zUnitRef[2] = 1;
          rotateCoordinates(xUnitRef, rotationRef);
          rotateCoordinates(yUnitRef, rotationRef);
          rotateCoordinates(zUnitRef, rotationRef);

          while(Serial.available())
            Serial.read();
        }
      }
  delay(200);
  }

}

//          for(int i=0;i<len;i++)
//          {
//            buf[i] = Serial.read();
//          }

//          if (len >= 2)
//            buf = (uint16_t) (((uint16_t) aux << 8) + (uint16_t) Serial.read());
//          else
//            buf = (uint16_t) aux;
//          Serial.print("aux: "); //Serial.read(); input: 2 aux: 2 (int) aux: 50
//          Serial.print(aux);
//          Serial.print(" (int) aux: "); 
//          Serial.print((int) aux);

//          Serial.print(" len: ");
//          Serial.print(len);
//          Serial.print(" buf: ");
//          Serial.println(buf);

//Serial.print(F("x: "));
//Serial.print(xyzAccelB[0]);
//Serial.print(F("; "));
//Serial.print(F("y: "));
//Serial.print(xyzAccelB[1]);
//Serial.print(F("; "));
//Serial.print(F("z: "));
//Serial.print(xyzAccelB[2]);
//Serial.print(F("; "));
  
