#include <DHT.h>

DHT dht(2, DHT11);
DHT dht2(4, DHT11);
int state=0;

void setup() {
  Serial.begin(9600);
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
  /*  Serial.println("Temperature:");
    Serial.println( temperature);    
    Serial.println("Humidity:");
    Serial.println(humidity); */
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
  /*  Serial.println("Temperature2:");
    Serial.println( temperature2);    
    Serial.println("Humidity2:");
    Serial.println(humidity2); */
    delay(3000);
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
 }
}
