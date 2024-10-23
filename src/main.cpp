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

#define INTERRUPT_PIN D7
#define THRESHOLD 20
#define DURATION 100

/************************************************************************************
 *                                  Prototypes                                      *
 ************************************************************************************/

void SensorCalibration();
void CheckFall();

/************************************************************************************
 *                               Global Variables                                   *
 ************************************************************************************/

int counter=0;
bool calibration_flag = false;
MPU6050 sensor(MPU6050_ADDRESS_AD0_LOW);
 

const uint8_t threshold = 20;
const uint8_t duration = 10;
 

/************************************************************************************
 *                                  Main                                            *
 ************************************************************************************/

void setup()
{
Wire.begin();
Serial.begin(115200);
Serial.println("Hello User");

// Calibration
SensorCalibration();

}
 
void loop()
{
  CheckFall();
}

/************************************************************************************
 *                               Local Functions                                    *
 ************************************************************************************/

void SensorCalibration() {

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int16_t ax = 0, ay = 0, az = 0;
int16_t gx = 0, gy = 0, gz = 0;

//Variables usadas por el filtro pasa bajos
long f_ax = 0,f_ay = 0, f_az = 0;
int p_ax = 0, p_ay = 0, p_az = 0;
long f_gx = 0,f_gy = 0, f_gz = 0;
int p_gx = 0, p_gy = 0, p_gz = 0;

//Valor de los offsets
int ax_o = 0, ay_o = 0, az_o = 0;
int gx_o = 0, gy_o = 0, gz_o = 0;

sensor.initialize();

// Printeo
Serial.println("Calibrando, no mover IMU");  

while(!calibration_flag)
{
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);

  // Filtrar las lecturas
  f_ax = f_ax-(f_ax>>5)+ax;
  p_ax = f_ax>>5;

  f_ay = f_ay-(f_ay>>5)+ay;
  p_ay = f_ay>>5;

  f_az = f_az-(f_az>>5)+az;
  p_az = f_az>>5;

  f_gx = f_gx-(f_gx>>3)+gx;
  p_gx = f_gx>>3;

  f_gy = f_gy-(f_gy>>3)+gy;
  p_gy = f_gy>>3;

  f_gz = f_gz-(f_gz>>3)+gz;
  p_gz = f_gz>>3;

  //Cada 100 lecturas corregir el offset
  if (counter==100)
  {
    //Mostrar las lecturas separadas por un [tab]
    Serial.print("promedio:"); Serial.print("t");
    Serial.print(p_ax); Serial.print("\t");
    Serial.print(p_ay); Serial.print("\t");
    Serial.print(p_az); Serial.print("\t");
    Serial.print(p_gx); Serial.print("\t");
    Serial.print(p_gy); Serial.print("\t");
    Serial.println(p_gz);

    //Calibrar el acelerometro a 1g en el eje z (ajustar el offset)
    if (p_ax>0) ax_o--;
    else {ax_o++;}
    if (p_ay>0) ay_o--;
    else {ay_o++;}
    if (p_az-16384>0) az_o--;
    else {az_o++;}
    
    sensor.setXAccelOffset(ax_o);
    sensor.setYAccelOffset(ay_o);
    sensor.setZAccelOffset(az_o);

    //Calibrar el giroscopio a 0º/s en todos los ejes (ajustar el offset)
    if (p_gx>0) gx_o--;
    else {gx_o++;}
    if (p_gy>0) gy_o--;
    else {gy_o++;}
    if (p_gz>0) gz_o--;
    else {gz_o++;}
    
    sensor.setXGyroOffset(gx_o);
    sensor.setYGyroOffset(gy_o);
    sensor.setZGyroOffset(gz_o);

    // Get out of loop
    calibration_flag = true;
  }
  counter++;
  }
}

void CheckFall()
{
  int16_t ax = 0, ay = 0, az = 0;
  int16_t gx = 0, gy = 0, gz = 0;

   // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);
  float ax_m_s2 = ax * (9.81/16384.0);
  float ay_m_s2 = ay * (9.81/16384.0);
  float az_m_s2 = az * (9.81/16384.0);
  float gx_deg_s = gx * (250.0/32768.0);
  float gy_deg_s = gy * (250.0/32768.0);
  float gz_deg_s = gz * (250.0/32768.0);
  //Mostrar las lecturas separadas por un [tab]
  Serial.print("a[x y z](m/s2) g[x y z](deg/s):\t");
  Serial.print(ax_m_s2); Serial.print("\t");
  Serial.print(ay_m_s2); Serial.print("\t");
  Serial.print(az_m_s2); Serial.print("\t");
  Serial.print(gx_deg_s); Serial.print("\t");
  Serial.print(gy_deg_s); Serial.print("\t");
  Serial.println(gz_deg_s);

  delay(100);
}