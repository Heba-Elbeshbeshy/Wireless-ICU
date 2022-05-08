// Include the libraries
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>


#define serverPostURL "http://toto3.pythonanywhere.com/test/"
#define serverGetURL "http://toto3.pythonanywhere.com/test/"

#define ssid "STUDBME2"
#define pass "BME2Stud"

//#define ssid ""
//#define pass ""


boolean Emergency = false;
String array_of_readings[5];

int ledState=0;
int Temp1 = 0;
int Hum1 = 0;
int Temp2 = 0;
int Hum2 = 0;


void connectToWiFi();
void getDataFromArduino ( );
void httpGetRequest ( );
void httpPostRequest ( String arr[]);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  connectToWiFi();
}

void loop() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) 
  { 
    getDataFromArduino( );
    httpPostRequest ( array_of_readings ); 
    httpGetRequest( ); 
  }
  else 
  {
    Serial.println("Error in WiFi connection");
  }
  //Send a request every 10 seconds
  delay(1000);  
  if(Emergency)
  {
     //       turn on led
    ledState = 1;
    Serial.write(ledState);
  }
  else
  {
   // turn on led
    ledState = 0;
    Serial.write(ledState);
   }
}


void getDataFromArduino ()
{
// Checks whether data is comming from the serial
  if(Serial.available() > 0){  
     // dht sensor for reading humidity
    Temp1 = Serial.read();
    Hum1 = Serial.read();
    
    Temp2 = Serial.read();
    Hum2 = Serial.read();

    //integer to string conversion
    String Temperature1 = String(Temp1) + String("°C"); 
    String Humidity1 = String(Hum1) + String("%");    
    String Temperature2 = String(Temp2) + String("°C"); 
    String Humidity2 = String(Hum2) + String("%");   

    
    Serial.print(" Temperature1: ");
    Serial.print(Temperature1);
    Serial.print(" Humidity1: ");
    Serial.print(Humidity1);
    Serial.print(" Temperature2: ");
    Serial.print(Temperature2);
    Serial.print(" Humidity2: ");
    Serial.print(Humidity2);
    Serial.println();
   
    array_of_readings[0] = String(Temp1);
    array_of_readings[1] = String(Hum1);
    array_of_readings[2] = String(Temp2);
    array_of_readings[3] = String(Hum2);

    delay(3000);
  }

}

//----------------------------------------------------------//
// function to post values to server
void httpPostRequest ( String arr[] )
{
    // Set up the client objet
   WiFiClient client;
   HTTPClient http;
   http.begin( client, serverPostURL );   //Specify request destination
   http.addHeader("Content-Type",  "application/json");  //Specify content-type header
  
   StaticJsonBuffer<200> jsonBuffer;
   JsonObject& values = jsonBuffer.createObject();
   
   values["Temperature"] = arr[0];
   values["Humidity"] = arr[1];
   values["Temperature2"] = arr[2];
   values["Humidity2"] = arr[3];
   
    char json_str[500]; 
    values.prettyPrintTo(json_str, sizeof(json_str));
    int httpCode = http.POST(json_str);   //Send the request
    Serial.println("in post fun");
    
    if(httpCode > 0){
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.print("Response: ");Serial.println(payload);
        }
    }
    else{
         Serial.printf("[HTTP] GET... failed, error: %s", http.errorToString(httpCode).c_str());
    }
        http.end();  //Close connection
        
//   sprintf(json_str, "{\"Readings\":[%s,%s,%s,%s,%s]}", arr[0], arr[1], arr[2], arr[3], arr[4]);
//   sprintf(json_str, "{\"Readings\":[%d,%d,%d,%d,%d]}", arr[0], arr[1], arr[2], arr[3], arr[4]);

}

//----------------------------------------------------------//
void httpGetRequest()
{
    WiFiClient client; // Set up the client objet
    HTTPClient http;
    
    http.begin( client, serverGetURL ); //Specify request destination
    int httpCode = http.GET(); //get the request
      
     if(httpCode > 0)
     {
       String payload = http.getString();
       Serial.write(payload[1]);
       Serial.print("Response: ");
       Serial.println(payload);
       Emergency = payload[1];
       
      http.end(); 
     }
}

//----------------------------------------------------------//
//function for scanning networks and conncting to one of them
void connectToWiFi() {
  
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
