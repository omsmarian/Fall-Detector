#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MPU6050.h>
#include <Wire.h>
#include <I2Cdev.h>


MPU6050 sensor;
int16_t ax, ay, az;
int16_t gx, gy, gz;


void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  sensor.initialize();
  Serial.println(sensor.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}


void loop() 
{
  // Leer las aceleraciones y velocidades angulares
    sensor.getAcceleration(&ax, &ay, &az);
    sensor.getRotation(&gx, &gy, &gz);

    //Mostrar las lecturas separadas por un [tab]
    Serial.print("a[x y z] g[x y z]:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.println(gz);

    delay(100);
}
