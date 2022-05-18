#include <SoftwareSerial.h>
#include <DHT.h>


//SoftwareSerial mySerial (9, 10) ;
DHT dht(2, DHT11);
DHT dht2(4, DHT11);
char ledState;
char result[6]; // Buffer big enough for 7-character float
char result2[6]; // Buffer big enough for 7-character float
char result3[6]; // Buffer big enough for 7-character float
char result4[6]; // Buffer big enough for 7-character float

void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);

 // Serial.println(F("DHT11 test!"));

  dht.begin();
  dht2.begin();
}

void loop() {

    // dht1 sensor 
    float Temp = dht.readTemperature( );
    dtostrf(Temp, 2, 2, result);
    float Hum = dht.readHumidity();
    dtostrf(Hum, 2, 2, result2);  
    
    if (isnan(Temp) || isnan(Hum))     // Checking dht sensor working
    {                                   
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.write( result);
    Serial.write(',');
    Serial.write( result2);
    Serial.write(',');
    delay(2000);

    // dht2 sensor 
    float Temp2 = dht2.readTemperature( );
    dtostrf(Temp2, 2, 2, result3);
    float Hum2 = dht2.readHumidity();
     dtostrf(Hum2, 2, 2, result4);  
     
    if (isnan(Temp2) || isnan(Hum2))     // Checking dht sensor working
    {                                   
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.write( result3);
    Serial.write(',');
    Serial.write( result4);
    Serial.write(',');
    delay(3000);
   
  if(Serial.available() > 0)
  { 
    int ledState = Serial.read(); // Reads the data from the serial port
//    Serial.println("ledState");
    Serial.println(ledState);
    if (ledState)
    {
         digitalWrite(8, HIGH);  
         delay(1000); 
    }
    else 
    {
         digitalWrite(8, LOW);   
         delay(1000);   
    }
 }
}
