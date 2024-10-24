#include <Arduino.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>


#define FALL_THRESHOLD 0.7

const char* ssid = "TCL 408";
const char* password = "apik6Vis";

bool flag;

int16_t ax = 0, ay = 0, az = 0;
float ax_g, ay_g, az_g;
float prevax_g, prevay_g, prevaz_g;

MPU6050 accelgyro(0x68); // <-- use for AD0 high


// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "5491169350147";
String apiKey = "9384670";

void getAccelerationG();
bool fallCheck();
void sendMessage(String message);

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Wire.begin();
  accelgyro.initialize();

  if (accelgyro.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  
  delay(1000);

  Serial.println("Setup done");
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected");

  accelgyro.setXAccelOffset(-4301);
  accelgyro.setYAccelOffset(-448);
  accelgyro.setZAccelOffset(2375);
  
  getAccelerationG();
  prevax_g = ax_g;
  prevay_g = ay_g;
  prevaz_g = az_g;
}


void loop() 
{
  digitalWrite(LED_BUILTIN, LOW);
  getAccelerationG();
  
  Serial.println("Actual:");

  Serial.print(ax_g);
  Serial.print(",");
  Serial.print(ay_g);
  Serial.print(",");
  Serial.println(az_g);

  Serial.println("Prev:");

  Serial.print(prevax_g);
  Serial.print(",");
  Serial.print(prevay_g);
  Serial.print(",");
  Serial.println(prevaz_g);

  if(fallCheck())
  {
    flag = true;
  }

  if(flag)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Detectado");
    String mensaje = "Ayuda";
    sendMessage(mensaje);
    flag = false;
    delay(3000);
  }

  delay(1000);
}


void getAccelerationG()
{
  accelgyro.getAcceleration(&ax,&ay,&az);
  ax_g = ax / 16384.0;
  ay_g = ay / 16384.0;
  az_g = az / 16384.0;
}

bool fallCheck()
{
  bool fall = false;
  if(abs(ax_g - prevax_g) > FALL_THRESHOLD)
    fall = true;
  else if (abs(ay_g - prevay_g) > FALL_THRESHOLD)
    fall = true;
  else if (abs(az_g - prevaz_g) > FALL_THRESHOLD)
    fall = true;
  else
    fall = false;

  prevax_g = ax_g;
  prevay_g = ay_g;
  prevaz_g = az_g;

  return fall;
}

void sendMessage(String message)
{
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