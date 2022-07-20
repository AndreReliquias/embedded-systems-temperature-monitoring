#include "DHT.h"
DHT dht2(D1, DHT11);

bool s = false;
float temperature = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(D1, INPUT);
  pinMode(D3, OUTPUT);
  dht2.begin();
}

void loop()
{
  temperature = dht2.readTemperature();
  Serial.println("Temperature in C:");
  Serial.println(temperature);
  Serial.println(s);

  if (temperature > 22)
  {
    s = true;
  }
  else if (temperature < 22 && s)
  {
    s = false;
  }

  digitalWrite(D3, s);
  delay(1000);
}