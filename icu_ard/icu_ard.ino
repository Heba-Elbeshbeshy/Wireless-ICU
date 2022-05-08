//#include <SoftwareSerial.h>
#include <DHT.h>


//SoftwareSerial myserial (9, 10) ;
DHT dht(2, DHT11);
DHT dht2(4, DHT11);
int state=0;

void setup() {
  Serial.begin(38400);
  pinMode(8, OUTPUT);

 // Serial.println(F("DHT11 test!"));

  dht.begin();
  dht2.begin();
}

void loop() {

    delay(2000);
    // dht1 sensor 
    int Temp = dht.readTemperature( );
    int Hum = dht.readHumidity();  
    if (isnan(Temp) || isnan(Hum))     // Checking dht sensor working
  {                                   
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    Serial.write( Temp);
    Serial.write( Hum);
    delay(3000);

    // dht2 sensor 
    int Temp2 = dht2.readTemperature( );
    int Hum2 = dht2.readHumidity();  
    if (isnan(Temp2) || isnan(Hum2))     // Checking dht sensor working
  {                                   
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    Serial.write( Temp2);
    Serial.write( Hum2);
    delay(3000);

   // Checks whether data is comming from the serial port 
//  if(Serial.available() > 0)
//  { 
//    state = Serial.read(); // Reads the data from the serial port
//    if (state)
//    {
//         digitalWrite(8, HIGH);   
//         delay(1000); 
//    }
//    else
//    {
//         digitalWrite(8, LOW);   
//         delay(1000);   
//    }
// }
}
