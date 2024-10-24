#include <Arduino.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


MPU6050 mpu(0x68);

void setup() {
  Serial.begin(115200);
  

  
  Serial.println("I'm alive");

  ESP.deepSleep(20e6);
}

void loop() {
}

/*

PARA LO QUE QUIERO HACER DEBERIA TENER UNA ESTRUCTURA COMO:

void setup() {
  Serial.begin(115200);
  
  mpu.initialize();


  Serial.println("I'm alive");

  ESP.deepSleep(20e6);
}
*/