#include <Arduino.h>
/*  // Código de prueba para el sensor MPU6050  
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
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
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>

const char* ssid = "Oms";
const char* password = "carranza1960";

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "5491169350147";
String apiKey = "9384670";

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  WiFiClient client;    
  HTTPClient http;
  http.begin(client, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // Send Message to WhatsAPP
  sendMessage("Hello from ESP8266!");
  delay(5000);
}