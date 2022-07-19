#include "DHT.h"
DHT dht2(D1, DHT11);

bool s = false;

void setup()
{
  Serial.begin(9600);
  pinMode(D1, INPUT);
  pinMode(D3, OUTPUT);
  dht2.begin();
}

void loop()
{
  Serial.println("Temperature in C:");
  Serial.println((dht2.readTemperature()));
  Serial.println("Humidity in C:");
  Serial.println((dht2.readHumidity()));
  s = !s;
  digitalWrite(D3, s);
  delay(1000);
}