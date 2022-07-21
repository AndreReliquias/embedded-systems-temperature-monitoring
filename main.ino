#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define DHT_PIN D1
#define DHT_TYPE DHT11
#define COOLER_PIN D3

DHT dht2(DHT_PIN, DHT_TYPE);

bool coolerIsDisable = false;
float temperature = 0;
const char *ssid = "ssid-here";
const char *password = "pw-here";
String host = "host-here";
const char *fingerprint = "fingerprint-here";

void setup()
{
  Serial.begin(9600);
  pinMode(DHT_PIN, INPUT);
  pinMode(COOLER_PIN, OUTPUT);
  dht2.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  temperature = dht2.readTemperature();
  Serial.println("Temperature in C:");
  Serial.println(temperature);
  Serial.println("Is the cooler disable?");
  Serial.println(coolerIsDisable);

  if ((float)temperature > 22)
  {
    coolerIsDisable = false;
  }
  else if ((float)temperature < 22)
  {
    coolerIsDisable = true;
  }

  digitalWrite(COOLER_PIN, coolerIsDisable);

  if (WiFi.status() == WL_CONNECTED)
  {
    BearSSL::WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    String serverPath = "/prod/v1/datacenterlogs?temperature=" + (String)temperature;
    String url = serverPath.c_str();

    http.begin(client, host, 443, serverPath, fingerprint);

    int httpResponseCode = http.POST("");

    if (httpResponseCode > 0)
    {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }

  delay(1000);
}