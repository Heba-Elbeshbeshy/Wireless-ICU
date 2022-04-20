
// Include the libraries
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <DallasTemperature.h>
#include <OneWire.h>

#define ssid "STUDBME2"
#define pass "BME2Stud"

int rate = 1;
byte counter = 0 ;
boolean Emergency = false;
String array_of_readings[6] = {0, 0, 0, 0, 0, 0};


// define bins for sensors
DHT dht1(2,DHT11); // D4, dht sensor
DHT dht2(2,DHT11); // D4, dht sensor
OneWire oneWire(0);  //D3, DS18B20 sensor
DallasTemperature DS18B20(&oneWire);


char get_state();
void connect_to_WiFi();
void get_data ( char data_to_focus);
void postreadings ( String arr[]);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  connect_to_WiFi();
}

void loop() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) 
  { 
    Server_return = get_state();
    get_data(Server_return);
    post_readings ( array_of_readings );  
  }
  else 
  {
    Serial.println("Error in WiFi connection");
  }
  //Send a request every 10 seconds
  delay(1000);  
}


void get_data (char data_to_focus)
{
     // dht sensor for reading humidity
    float Temp1 = dht1.readTemperature( );
    float Hum1 = dht1.readHumidity( );
    
    float Temp2 = dht2.readTemperature( );
    float Hum2 = dht2.readHumidity( );
  
    // DS18B20 sensor for reading Temperature
    DS18B20.requestTemperatures( );     
    float Temp3 = DS18B20.getTempCByIndex(0);

        // Checking dht sensor working  
    if (isnan(Temp1) || isnan(Hum1) || isnan(Temp2) || isnan(Hum2))     
    {                                   
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    //integer to string conversion
    String Temperature1 = String(Temp1) + String("°C"); 
    String Humidity1 = String(Hum1) + String("%");    
    String Temperature2 = String(Temp2) + String("°C"); 
    String Humidity2 = String(Hum2) + String("%");   
    String Temperature3 = String(Temp3) + String("°C");   
    
    Serial.print(" Temperature1: ");
    Serial.print(Temperature1);
    Serial.print(" Humidity1: ");
    Serial.print(Humidity1);
    Serial.print(" Temperature2: ");
    Serial.print(Temperature2);
    Serial.print(" Humidity2: ");
    Serial.print(Humidity2);
    Serial.print(" Temperature3: ");
    Serial.print(Temperature3);
    Serial.println();
   
    array_of_readings[0] = String(Temp1);
    array_of_readings[1] = String(Hum1);
    array_of_readings[2] = String(Temp2);
    array_of_readings[3] = String(Hum2);
    array_of_readings[4] = String(Temp3);
    
    if (Emergency)
    {
       switch (data_to_focus)
       {
        case 'A':
          float Temp1 = dht1.readTemperature( );
          array_of_readings[5] = String(Temp1);
        break;
        case 'B':
          float Hum1 = dht1.readHumidity( );
          array_of_readings[5] = String(Hum1);
        break;
        case 'C':
          float Temp2 = dht2.readTemperature( );
          array_of_readings[5] = String(Temp2);
        break;
        case 'D':
          float Hum2 = dht2.readHumidity( );     
          array_of_readings[5] = String(Hum2);
        break;
        case 'E':
          DS18B20.requestTemperatures( );     
          float Temp3 = DS18B20.getTempCByIndex(0); 
          array_of_readings[5] = String(Temp3);  
        break;
       }
    }
    else
    {
      array_of_reading[5] = 0;
    }
}

//----------------------------------------------------------//
// function to post values to server
void post_readings ( String arr[] )
{
// Set up the client objet
   WiFiClient client;
   HTTPClient http;
   
   http.begin(client,"http://toto3.pythonanywhere.com/test/");   //Specify request destination
   http.addHeader("Content-Type",  "application/json");  //Specify content-type header
  
   char json_str[100];
   sprintf(json_str, "{\"Readings\":[%s,%s,%s,%s,%s,%s]}", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
   int httpCode = http.POST(json_str);
   Serial.println("in post fun");
    
    if(httpCode > 0)
    {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
      {
          Serial.print("Response: ");
          Serial.println(httpCode);
          String payload = http.getString();
          Serial.println(payload);
       }
    }
    else{
         Serial.printf("[HTTP] GET... failed, error: %s", http.errorToString(httpCode).c_str());
    }
        http.end();  //Close connection
}

//----------------------------------------------------------//
char get_state()
{
    char server_return = 0 ;
    WiFiClient client; // Set up the client objet
    HTTPClient http;
    
    http.begin(client,"http://toto3.pythonanywhere.com/test/"); //Specify request destination
    int httpCode = http.GET(); //get the request
      
     if(httpCode > 0)
     {
       payload = http.getString();
       s.write(payload[1]);
       Serial.print("Response: ");
       Serial.println(payload);
       Emergency = true;
       server_return = payload[1];
       
      http.end(); 
     }
     return server_return;
}

//----------------------------------------------------------//
//function for scanning networks and conncting to one of them
void connect_to_WiFi() {
  
   WiFi.begin(ssid, pass);
   Serial.println(); 
   Serial.println("connecting");
   
   while (WiFi.status() != WL_CONNECTED)
   {
    Serial.print("..");
    delay(500); 
   }
    Serial.println();
    Serial.print("connected to:");
    Serial.println(ssid);
}
