#include <Arduino.h>
#include <ArduinoJson.h>
#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>

const char* ssid = "Mariano";
const char* password = "hola1234";

// unwiredlabs Hostname & Geolocation Endpoint url
const char* Host = "www.unwiredlabs.com";
String endpoint = "/v2/process.php";

// UnwiredLabs API_Token. Signup here to get a free token https://unwiredlabs.com/trial
String token = "pk.e3cd9d7c4f54963964e927d90a2ae160";

String jsonString = "{\n";

// Variables to store unwiredlabs response
double latitude = 0.0;
double longitude = 0.0;
double accuracy = 0.0;


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

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("Setup done");
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected");
}


void loop() {

  

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");

  if (n == 0 ) 
  {
    Serial.println("No networks available");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
  }

  // now build the jsonString...
  jsonString = "{\n";
  jsonString += "\"token\" : \"";
  jsonString += token;
  jsonString += "\",\n";
  jsonString += "\"id\" : \"saikirandevice01\",\n";
  jsonString += "\"wifi\": [\n";
  for (int j = 0; j < n; ++j) 
  {
    jsonString += "{\n";
    jsonString += "\"bssid\" : \"";
    jsonString += (WiFi.BSSIDstr(j));
    jsonString += "\",\n";
    jsonString += "\"signal\": ";
    jsonString += WiFi.RSSI(j);
    jsonString += "\n";
    if (j < n - 1)
    {
      jsonString += "},\n";
    }
    else
    {
      jsonString += "}\n";
    }
  }
  jsonString += ("]\n");
  jsonString += ("}\n");
  Serial.println(jsonString);

  WiFiClientSecure client;

    //Connect to the client and make the api call
    Serial.println("Requesting URL: https://" + (String)Host + endpoint);
    if (client.connect(Host, 443))
    {
      Serial.println("Connected");
      client.println("POST " + endpoint + " HTTP/1.1");
      client.println("Host: " + (String)Host);
      client.println("Connection: close");
      client.println("Content-Type: application/json");
      client.println("User-Agent: Arduino/1.0");
      client.print("Content-Length: ");
      client.println(jsonString.length());
      client.println();
      client.print(jsonString);
      delay(500);
    }

    //Read and parse all the lines of the reply from server
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      JsonObject& root = jsonBuffer.parseObject(line);
      if (root.success()) 
      {
        latitude    = root["lat"];
        longitude   = root["lon"];
        accuracy    = root["accuracy"];

        Serial.println();
        Serial.print("Latitude = ");
        Serial.println(latitude, 6);
        Serial.print("Longitude = ");
        Serial.println(longitude, 6);
        Serial.print("Accuracy = ");
        Serial.println(accuracy);
      }
    }

    Serial.println("closing connection");
    Serial.println();
    client.stop();

      delay(5000);
}