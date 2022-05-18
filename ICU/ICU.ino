  // Include the libraries
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define serverPostURL "http://toto3.pythonanywhere.com/Post_4_Readings/" 
#define serverGetURL "http://meirr.pythonanywhere.com/GetRate/" 

#define ssid "STUDBME2"
#define pass "BME2Stud"

//#define ssid "Meirna" 
//#define pass "123456789" 

//#define ssid "khhh_h" 
//#define pass "01145450275"

//#define ssid "Walaa" 
//#define pass "riri7006"

//#define ssid "Rannnia" 
//#define pass "0111034249888"

String array_of_readings[4];

String Temp1; 
String Hum1 ;

String Temp2; 
String Hum2; 

String Emergency; 
int ledState;

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
//    httpGetRequest( ); 
  }
  else 
  {
    Serial.println("Error in WiFi connection");
  }
  
  if(Emergency == "1")
  {
    Serial.println("EMERGENCY");
     // turn on led
    ledState = 1;
    Serial.write(ledState);
  }
  else
  {
   // turn off led
    ledState = 0;
    Serial.write(ledState);
   }
  //Send a request every 10 seconds
  delay(1000);  
}

void getDataFromArduino ()
{
// Checks whether data is comming from the serial
  if(Serial.available() > 0){ 
    
    String readStr;
    while(Serial.available() > 0)
  {
    readStr += char(Serial.read());
//    Serial.println(readStr);
  }

    Temp1 = readStr.substring(0, 5);
    Hum1 = readStr.substring(6, 11);

    Temp2 = readStr.substring(12, 17);
    Hum2 = readStr.substring(18, 23);

    //integer to string conversion
    String Temperature1 = String(Temp1) + String("°C"); 
    String Humidity1 = String(Hum1) + String("%");    
    String Temperature2 = String(Temp2) + String("°C"); 
    String Humidity2 = String(Hum2) + String("%");   

    Serial.println(" Temperature1: "); 
    Serial.println(Temperature1); 
    Serial.println(" Humidity1: "); 
    Serial.println(Humidity1); 
    Serial.println(" Temperature2: "); 
    Serial.println(Temperature2); 
    Serial.println(" Humidity2: "); 
    Serial.println(Humidity2); 
    Serial.println(); 
   
    array_of_readings[0] = String(Temp1);
    array_of_readings[1] = String(Hum1);
    array_of_readings[2] = String(Temp2);
    array_of_readings[3] = String(Hum2);
    httpPostRequest ( array_of_readings );

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
   
   values["Temperature1"] = arr[0];
   values["Humidity1"] = arr[1];
   values["Temperature2"] = arr[2];
   values["Humidity2"] = arr[3];
   
    char json_str[500]; 
    values.prettyPrintTo(json_str, sizeof(json_str));
    int httpCode = http.POST(json_str);   //Send the request
    Serial.println("in post fun");
    Serial.println(httpCode); 
    Serial.println(json_str);
    
    if(httpCode > 0){
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.print("Response: ");
          Serial.println(payload);
        }
    }
    else{
         Serial.printf("[HTTP] GET... failed, error: %s", http.errorToString(httpCode).c_str());
    }
        http.end();  //Close connection
  
}

//----------------------------------------------------------//
void httpGetRequest()
{
    Serial.println("in GET Fun"); 
    WiFiClient client; // Set up the client objet
    HTTPClient http;
    
    http.begin( client, serverGetURL ); //Specify request destination
    int httpCode = http.GET(); //get the request
    Serial.println("httpCode2");
    Serial.println(httpCode);
    
     if(httpCode > 0)
     {
       String payload = http.getString();
//       Serial.write(payload[0]); 
//       Serial.print("Response: "); 
//       Serial.println(payload); 
         
       Emergency = payload;
        
       Serial.print("Emergency: "); 
       Serial.println(Emergency); 

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
