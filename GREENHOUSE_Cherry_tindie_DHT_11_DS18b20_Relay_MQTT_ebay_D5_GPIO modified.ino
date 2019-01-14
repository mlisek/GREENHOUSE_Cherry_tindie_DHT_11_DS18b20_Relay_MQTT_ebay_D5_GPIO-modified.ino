// GREENHOUSE Cherry tindie DHT 11 DS18b20 Relay MQTT 
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(12);
DallasTemperature sensors(&oneWire);
DeviceAddress tempSensor = {0x28, 0x00, 0x09, 0x00, 0x93, 0xBA, 0x21, 0xB9}; //previosly detected adress DS18B20_address_reporter INO
float tempC;//18B20 sensor variable

int tindie_ADDR = 0x20;//I2c sensor address
float Temp = 0 ;//Tindie variable is set initially to zero
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11 // DHT 11
#include <CayenneMQTTESP8266.h>
//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = ".....................";
char wifiPassword[] = ".....................";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = ".....................";
char password[] = ".....................";
char clientID[] = ".....................";
int ledPin = 14;// Relay
unsigned long lastMillis = 0;
DHT dht(DHTPIN, DHTTYPE);

float h,tmp,f,hic,hif; //DHT built in variables
void setup() {
  Wire.begin();
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
pinMode(2,OUTPUT); // DHT
 pinMode(ledPin, OUTPUT); // Relay
sensors.setResolution(tempSensor, 10); // DS18b20 resolution
}
void loop() {
  Cayenne.loop();

  if (millis() - lastMillis > 10000) {
    lastMillis = millis();

    Cayenne.virtualWrite(1, Temp);
Cayenne.virtualWrite(4, h);
  Cayenne.virtualWrite(5, tmp);
  Cayenne.virtualWrite(6, hic);
  }
  h = dht.readHumidity();
  
  tmp = dht.readTemperature();
  
  
   f = dht.readTemperature(true);

  
  if (isnan(h) || isnan(tmp) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
}

  hif = dht.computeHeatIndex(f, h);
  
  hic = dht.computeHeatIndex(tmp, h, false);

Wire.beginTransmission(tindie_ADDR);
  Wire.write(5);
  Wire.endTransmission();
  Wire.requestFrom(tindie_ADDR, 2);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read(); // soil temperature variable picked from Tindie soli sensor
  Temp = (float) t / 10 ; // integers divided by number are always integers

  Wire.beginTransmission(tindie_ADDR);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(tindie_ADDR, 2);
  unsigned int c = Wire.read() << 8;
  c = c | Wire.read(); // soil capacity variable picked from Tindie soli sensor
  Cayenne.virtualWrite(0, c);

  Wire.beginTransmission(tindie_ADDR);
  Wire.write(3);
  Wire.write(4);
  Wire.endTransmission();
  Wire.requestFrom(tindie_ADDR, 2);
  unsigned int l = Wire.read() << 8;
  l = l | Wire.read(); // soil luminosity variable picked from Tindie soli sensor
  Cayenne.virtualWrite(2, l);
 sensors.requestTemperaturesByAddress(tempSensor);  
   tempC = sensors.getTempC(tempSensor);
  Serial.print("Temp DS18B20 C: ");
  Serial.println(tempC,2);
 Cayenne.virtualWrite(7, tempC);

  Serial.print("SOIL Temperature is ");
  Serial.print(Temp); //Reading GET TEMPERATURE register
  Serial.print(", ");
  Serial.print("SOIL Capacitance is ");
  Serial.print(c); //Reading GET Capacitance register
  Serial.print(", ");
  Serial.print("SOIL Lux is ");
  Serial.println(l); //Reading GET Lux register
  
Serial.print("Humidity: ");
  Serial.print(h);
  
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(tmp);
  
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
 Serial.print("Sensor Resolution: ");
  Serial.println(sensors.getResolution(tempSensor), DEC); 
  Serial.println();



//delay (10000);
}
CAYENNE_IN(8)
{ int value = getValue.asInt();
if (getValue.asInt() == 1) { digitalWrite(ledPin, HIGH);}
  else { digitalWrite(ledPin, LOW);}}



