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

const int fan = 5;
int fan_state = LOW;
const int pump = 6;
int pump_state = LOW;
const int light = 7;
int light_state = LOW;

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

void loop() {

  lcd.clear();

  humidity = round(dht.getHumidity());
  lcd.setCursor(0, 0);
  lcd.print("Humidity:" + String(humidity));
  packet = packet + String(humidity);

  temperature = round(dht.getTemperature());
  lcd.setCursor(0, 1);
  lcd.print("Temperature:" + String(temperature));
  packet = packet + ":" + String(temperature);

  luminosity = analogRead(A0);
  lcd.setCursor(4, 2);
  lcd.print("Luminosity:" + String(luminosity));
  packet = packet + ":" + String(luminosity);

  moisture = round(analogRead(A1));
  lcd.setCursor(4, 3);
  lcd.print("Moisture:" + String(moisture));
  packet = packet + ":" + String(moisture);

  Serial.println(packet);

  delay(500);
  writeString(packet);
  ArduinoSerial.print("\n");
  ArduinoSerial.flush();


  if (humidity < 49 || temperature < 30 ) {
    fan_state = HIGH;
    digitalWrite(fan, fan_state);
  }
  if (humidity > 55 ) {
    if (temperature < 30) {
      fan_state = HIGH;
      digitalWrite(fan, fan_state);
    }
    fan_state = LOW;
    digitalWrite(fan, fan_state);
  }
  if (moisture < 500) {
    pump_state = HIGH;
    digitalWrite(pump, pump_state);
  }
  if (moisture > 700) {
    pump_state = HIGH;
    digitalWrite(pump, pump_state);
  }

  if (luminosity < 140) {
    light_state = HIGH;
    digitalWrite(light, light_state);
  }
  if (luminosity > 600) {
    light_state = LOW;
    digitalWrite(light, light_state);
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

