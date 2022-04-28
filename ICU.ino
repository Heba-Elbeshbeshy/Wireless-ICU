// Include the libraries
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>


#define ssid "STUDBME2"
#define pass "BME2Stud"

int rate = 1;
char Server_return;
boolean Emergency = false;
int array_of_readings[5] = {0, 0, 0, 0, 0};

int state=0;
int Temp1 = 0;
int Hum1 = 0;
int Temp2 = 0;
int Hum2 = 0;

char get_state();
void connect_to_WiFi();
void get_data ( char data_to_focus);
void postreadings ( int arr[]);

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
   
    array_of_readings[0] = Temp1;
    array_of_readings[1] = Hum1;
    array_of_readings[2] = Temp2;
    array_of_readings[3] = Hum2;
  }
//  if (Emergency)
//  {
//     switch (data_to_focus)
//     {
//      case 'a':
//        Temp1 = Serial.read();
//        array_of_readings[5] = int(Temp1);
//        break;
//      case 'b':
//        Hum1 = Serial.read();
//        array_of_readings[5] = int(Hum1);
//        break;
//      case 'c':
//        Temp2 = Serial.read();
//        array_of_readings[5] = int(Temp2);
//        break;
//      case 'd':
//        Hum2 = Serial.read();     
//        array_of_readings[5] = int(Hum2);
//        break;
//      default:
//        break;
//     }
//   }
//   else
//   {
//    array_of_readings[4] = 0;
//   }
}

//----------------------------------------------------------//
// function to post values to server
void post_readings ( int arr[] )
{
// Set up the client objet
   WiFiClient client;
   HTTPClient http;
   
   http.begin(client,"http://toto3.pythonanywhere.com/test/");   //Specify request destination
   http.addHeader("Content-Type",  "application/json");  //Specify content-type header
  
   char json_str[100];
//   sprintf(json_str, "{\"Readings\":[%s,%s,%s,%s,%s]}", arr[0], arr[1], arr[2], arr[3], arr[4]);
   sprintf(json_str, "{\"Readings\":[%d,%d,%d,%d,%d]}", arr[0], arr[1], arr[2], arr[3], arr[4]);
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
       String payload = http.getString();
       Serial.write(payload[1]);
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
