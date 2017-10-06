
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ThingerWifi.h>
#include "DHT.h"
#define USERNAME "kazezx"
#define DEVICE_ID "nodemcu"
#define DEVICE_CREDENTIAL "nodemcu_lolin_v3"

#define SSID "Zuhayr"
#define SSID_PASSWORD "Zuhayr007"
// dht config
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int sensorValue = 0;
float voltage = 0 ;
int light = D4;
int fan = D5;
int pump = D6;
int moisture_pin =  D7;

ThingerWifi thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {

  Serial.begin(9600);

  thing.add_wifi(SSID, SSID_PASSWORD);

  dht.begin();

  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(pump, OUTPUT);

  thing["Temperature"] >> [](pson & out) {
    out = dht.readTemperature();
  };
  thing["Humidity"] >> [](pson & out) {
    out = dht.readHumidity();
  };
  thing["Intensity"] >> [](pson & out) {
    out = analogRead(A0);
  };
  thing["Moisture"] >> [] (pson & out) {
    out = digitalRead(moisture_pin);
  };
  thing["Light"] << digitalPin(light);
  thing["Fan"] << digitalPin(fan);
  thing["Pump"] << digitalPin(fan);
}

void loop() {
  thing.handle();
  Serial.print("Humidity: " );
  Serial.println(dht.readTemperature());
  Serial.print("Temperature: " );
  Serial.println(dht.readHumidity());
}
