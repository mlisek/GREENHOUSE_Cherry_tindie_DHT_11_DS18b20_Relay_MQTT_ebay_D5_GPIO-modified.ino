#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include "Arduino.h"
#include <CayenneMQTTESP8266.h>
char ssid[] = "LudZbunjenNormalan";
char wifiPassword[] = "45984fd37HG97";
char username[] = "845f1240-abcc-11e6-bfa5-7b3dd1a0d34e";
char password[] = "2e8431d2180bad3bd84c1ce5fb5199eb7304a1e1";
char clientID[] = "195f24e0-255f-11ed-bf0a-bb4ba43bd3f6";
unsigned long lastMillis = 0;
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);
DeviceAddress tempSensor = {0x28, 0x28, 0x4D, 0x79, 0xA2, 0x15, 0x03, 0x7F}; //previosly detected adress DS18B20_address_reporter INO
float tempC;//18B20 sensor variable
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include "Adafruit_MCP23008.h"
Adafruit_MCP23008 mcp;
#include <I2CSoilMoistureSensor.h>
#include <dht.h>
dht12 DHT(0x5c);
I2CSoilMoistureSensor sensor;
const int address = TSL2561_ADDR_FLOAT;
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(address, 12345);
void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  Wire.begin(4, 5); // SDA, SDL
  mcp.begin(6);      // use default address 0
  for (uint8 count = 0; count < 8; count++)
  {
    mcp .pinMode(count, OUTPUT); // modify LOW or HIGH
  }
  for (uint8 pin = 0; pin < 8; pin++)
  {
    mcp .digitalWrite(pin, HIGH); // modify LOW or HIGH
  }
  sensor.begin();
  if (!tsl.begin())
  {
    CAYENNE_LOG("No TSL2561 detected");
    while (1);
  }
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  sensors.setResolution(tempSensor, 12); // DS18b20 resolution

}
void loop() {
  Cayenne.loop(60);
  DHT.read();
  while (sensor.isBusy()) delay(50); // available since FW 2.3
  Serial.print("Soil Moisture Capacitance: ");
  Serial.print(sensor.getCapacitance()); //read capacitance register
  Serial.print(", Temperature: ");
  Serial.print(sensor.getTemperature() / (float)10); //temperature register
  //Serial.print(", Light: ");
  //Serial.println(sensor.getLight(true)); //request light measurement, wait and read light register
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    Cayenne.virtualWrite(0, lastMillis);
    float soiltemperature = sensor.getTemperature() / 10.0;
    float soilhumidity = sensor.getCapacitance();
    float soillight = sensor.getLight(true);
    float temperature = DHT.getTemperature() / 10.0;
    float humidity = DHT.getHumidity() / 10.0;
    float dewPoint = DHT.dewPoint();
    sensors_event_t event;
    tsl.getEvent(&event);
    Cayenne.virtualWrite(3, event.light, TYPE_LUMINOSITY, UNIT_LUX);
    Cayenne.virtualWrite(5, soiltemperature);
    Cayenne.virtualWrite(6, soilhumidity);
    Cayenne.virtualWrite(7, soillight);
    Cayenne.celsiusWrite(1, temperature);
    Cayenne.virtualWrite(2, humidity, "rel_hum", "p");
    Cayenne.virtualWrite(4, dewPoint);
    sensors.requestTemperaturesByAddress(tempSensor);
    tempC = sensors.getTempC(tempSensor);
    Serial.print("Temp DS18B20 C: ");
    Serial.print("Sensor Resolution: ");
    Serial.println(sensors.getResolution(tempSensor), DEC);
    Serial.println(tempC, 2);
    Cayenne.virtualWrite(9, tempC);
  }
}
CAYENNE_IN_DEFAULT() {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
}
CAYENNE_IN(11) {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  if (request.channel = 11)
  {
    String value0;
    value0 = (String)getValue.asString();
    if (value0 == "1")
    {
      mcp.digitalWrite(0, LOW);
    }
    else
    {
      mcp.digitalWrite(0, HIGH);
    }
  }
}
CAYENNE_IN(12) {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  if (request.channel = 12)
  {
    String value1;
    value1 = (String)getValue.asString();
    if (value1 == "1")
    {
      mcp.digitalWrite(1, LOW);
    }
    else
    {
      mcp.digitalWrite(1, HIGH);
    }
  }
}
CAYENNE_IN(13) {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  if (request.channel = 13)
  {
    String value0;
    value0 = (String)getValue.asString();
    if (value0 == "1")
    {
      mcp.digitalWrite(2, LOW);
    }
    else
    {
      mcp.digitalWrite(2, HIGH);
    }
  }
}
CAYENNE_IN(14) {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  if (request.channel = 14)
  {
    String value0;
    value0 = (String)getValue.asString();
    if (value0 == "1")
    {
      mcp.digitalWrite(3, LOW);
    }
    else
    {
      mcp.digitalWrite(3, HIGH);
    }
  }
}
CAYENNE_IN(15) {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  if (request.channel = 15)
  {
    String value0;
    value0 = (String)getValue.asString();
    if (value0 == "1")
    {
      mcp.digitalWrite(4, LOW);
    }
    else
    {
      mcp.digitalWrite(4, HIGH);
    }
  }
}
CAYENNE_IN(16) {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  if (request.channel = 16)
  {
    String value0;
    value0 = (String)getValue.asString();
    if (value0 == "1")
    {
      mcp.digitalWrite(5, LOW);
    }
    else
    {
      mcp.digitalWrite(5, HIGH);
    }
  }
}
CAYENNE_IN(17) {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  if (request.channel = 17)
  {
    String value0;
    value0 = (String)getValue.asString();
    if (value0 == "1")
    {
      mcp.digitalWrite(6, LOW);
    }
    else
    {
      mcp.digitalWrite(6, HIGH);
    }
  }
}
CAYENNE_IN(18) {
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  if (request.channel = 18)
  {
    String value0;
    value0 = (String)getValue.asString();
    if (value0 == "1")
    {
      mcp.digitalWrite(7, LOW);
    }
    else
    {
      mcp.digitalWrite(7, HIGH);
    }
  }
}
