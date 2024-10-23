/**
 * @file main.cpp
 * @author Mariano Oms
 * @brief Este archivo contiene el código principal del proyecto
 * @date 2024-10-22
 */

/************************************************************************************
 *                               Includes                                           *
 ************************************************************************************/
#include <Arduino.h>
#include "MPU6050.h"


/************************************************************************************
 *                              Macros and Defines                                  *
 ************************************************************************************/

#define DELAY 20

/************************************************************************************
 *                                  Prototypes                                      *
 ************************************************************************************/


/************************************************************************************
 *                               Global Variables                                   *
 ************************************************************************************/

MPU6050 sensor(MPU6050_ADDRESS_AD0_LOW);
 
uint32_t now;
uint32_t then;
int16_t ax, ay, az;
float x, y, z;

 

/************************************************************************************
 *                                  Main                                            *
 ************************************************************************************/

void setup()
{
Wire.begin();
Serial.begin(115200);
Serial.println("Hello User");
sensor.initialize();

if (sensor.testConnection()) 
  Serial.println("Sensor iniciado correctamente");
else 
  Serial.println("Error al iniciar el sensor");

}
 
void loop()
{
  now = millis();
  if (now - then >= DELAY)
  {
    // update time
    then = now;

    // get RAW data
    sensor.getAcceleration(&ax, &ay, &az);

    // convert to g
    x = ax / 16384.0;
    y = ay / 16384.0;
    z = az / 16384.0;

    // print data
    Serial.print(x, 5);
    Serial.print(",");
    Serial.print(y, 5);
    Serial.print(",");
    Serial.println(z, 5);
  }
}

/************************************************************************************
 *                               Local Functions                                    *
 ************************************************************************************/
