#include "DHT.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

SoftwareSerial ArduinoSerial(3, 2); // RX, TX
DHT dht;

int humidity = 0;
int temperature = 0;
int luminosity = 0;
int moisture = 0;
int water_level = 0;

int fan = 5;
int pump = 6;
int light = 7;

String packet = "";

void setup()
{
  Serial.begin(115200);
  ArduinoSerial.begin(4800);

  dht.setup(4);

  pinMode(fan, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(light, OUTPUT);

  Serial.println("Booting Arduino...");
  delay(100);
  
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 4);
  lcd.clear();
}

void loop()
{
  lcd.clear();

  humidity = round(dht.getHumidity());
  lcd.setCursor(0, 0);
  lcd.print("Humidity:" + String(humidity));
  packet = packet + String(humidity);

  temperature = round(dht.getTemperature());
  lcd.setCursor(0, 1);
  lcd.print("Temperature:" + String(temperature));
  packet = packet + ":" + String(temperature);

  luminosity = round(analogRead(A0));
  lcd.setCursor(4, 2);
  lcd.print("Luminosity:" + String(luminosity));
  packet = packet + ":" + String(luminosity);

  moisture = round(analogRead(A1));
  lcd.setCursor(4, 3);
  lcd.print("Moisture:" + String(moisture));
  packet = packet + ":" + String(moisture);

//  water_level = round(analogRead(A2));
//  lcd.setCursor(0, 1);
//  delay(1000);
//  lcd.clear();
//  lcd.print("Water Level:" + String(water_level));
//  packet = packet + ":" + String(water_level);

  //Serial.println(packet);
  //Serial.println();

  delay(500);
  writeString(packet);
  ArduinoSerial.print("\n");
  
  //delay(500);

  if (humidity < 60 || humidity > 70) {
    fix_humidity(humidity);
  }
  if (temperature < 20 || temperature > 28) {
    fix_temperature(temperature);
  }
 // if (water_level < 100 || water_level > 200) {
 //   fix_water_level(water_level);
 // }
  if (moisture < 50 || moisture > 75) {
    fix_moisture(moisture);
  }
  if (luminosity < 500 || luminosity > 1000) {
    fix_luminosity(luminosity);
  }

  delay(250);
  packet = "";
  delay(250);
}

void writeString(String stringData) {
  for (int i = 0; i < stringData.length(); i++)
  {
    ArduinoSerial.print(stringData[i]);
  }
}

void fix_humidity(int fix) {
  int timer = 0 ;
  if (fix < 60) {
    timer = 1000 * (60 - fix);
    digitalWrite(fan, HIGH);
    delay(timer);
    digitalWrite(fan, LOW);
  }
  else {
    timer = 1000 * (fix - 70);
    digitalWrite(light, HIGH);
    delay(timer);
    digitalWrite(light, LOW);
  }
}

void fix_temperature(int fix) {
  int timer = 0;
  if (fix < 20) {
    timer = 1000 * (20 - fix);
    digitalWrite(light, HIGH);
    delay(timer);
    digitalWrite(light, LOW);
  }
  else {
    timer = 1000 * (fix - 28);
    digitalWrite(fan, HIGH);
    delay(timer);
    digitalWrite(fan, LOW);
  }
}

void fix_luminosity(int fix) {
  int timer = 0;
  if (fix < 500) {
    timer = 1000 * (500 - fix) ;
    digitalWrite(light, HIGH);
    delay(timer);
    digitalWrite(light, LOW);
  }
  else {
    digitalWrite(light, LOW);
  }
}

void fix_moisture(int fix) {
  int timer = 0;
  if (fix < 50) {
    timer = 1000 * (fix - 50);
    digitalWrite(pump, HIGH);
    delay(timer);
    digitalWrite(pump, LOW);
  }
  else {
    timer = 1000 * (75 - fix);
    digitalWrite(light, HIGH);
    delay(timer);
    digitalWrite(light, LOW);
  }
}

void fix_water_level(int fix) {
  digitalWrite(pump, HIGH);
  delay(1000);
  digitalWrite(pump, LOW);
}


