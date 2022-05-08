int state=0;
int Temp=0;
int Hum=0;
int Temp2=0;
int Hum2=0;

void setup() {
  Serial.begin(38400);
}

void loop() {
   if(Serial.available() > 0){ // Checks whether data is comming from the serial port
   // state = Serial.read(); // Reads the data from the serial port
   // dht1 sensor 
    Temp=Serial.read();
    String temperature = String(Temp) + String("°C");
    Serial.println("Temperature:");
    Serial.println( temperature);
    
    Hum=Serial.read();
    String humidity = String(Hum) + String("%");      //Humidity integer to string conversion
    Serial.println("Humidity:");
    Serial.println(humidity); 
    //delay(3000);

    // dht2 sensor   
    Temp2=Serial.read();
    String temperature2 = String(Temp2) + String("°C");
    Serial.println("Temperature2:");
    Serial.println( temperature2);
    
     Hum2=Serial.read();
    String humidity2 = String(Hum2) + String("%");      //Humidity integer to string conversion
    Serial.println("Humidity2:");
    Serial.println(humidity2); 
    //delay(3000);


   }
   delay(500);
}
