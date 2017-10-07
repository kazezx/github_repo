
#include "DHT.h"
#include <SoftwareSerial.h>

DHT dht;
SoftwareSerial ArduinoSerial(3, 2); // RX, TX
int humidity = 0;
int temperature = 0;
int luminosity = 0;
int moisture = 0;
int water_level = 0;
int sensors_data[5];

void setup()
{
  Serial.begin(9600);
  ArduinoSerial.begin(4800);
  dht.setup(2); // data pin 2
}
void loop()
{
  humidity = round(dht.getHumidity());
  sensors_data[0] = humidity;
  ArduinoSerial.print(sensors_data[0]); //sends humidity value
  ArduinoSerial.print("\n");
  if (humidity < 60 && humidity > 70) {
    fix_humidity(humidity);
  }

  temperature = round(dht.getTemperature());
  sensors_data[1] = temperature;
  ArduinoSerial.print(sensors_data[1]); //sends temperature value
  ArduinoSerial.print("\n");
  if (temperature < 20 && temperature > 28) {
    fix_temperature(temperature);
  }

  luminosity = round(analogRead(A1));
  sensors_data[2] = luminosity;
  ArduinoSerial.print(sensors_data[2]); //sends luminosity value
  ArduinoSerial.print("\n");
  if (luminosity < 0 && luminosity > 0) {
    fix_luminosity(luminosity);
  }

  moisture = round(analogRead(A2));
  sensors_data[3] = moisture;
  ArduinoSerial.print(sensors_data[3]); //sends moisture value
  ArduinoSerial.print("\n");
  if (moisture < 50 && moisture > 75) {
    fix_water_level(water_level);
  }

  water_level = round(analogRead(A3));
  sensors_data[4] = water_level;
  ArduinoSerial.print(sensors_data[4]); //sends water_level value
  ArduinoSerial.print("\n");
  if (water_level < 100 && water_level > 100) {
    fix_water_level(water_level);
  }
  delay(1000);
}
void fix_humidity(int val) {
  //manipulate fan
}
void fix_temperature(int val) {
  //manipulate fan
}
void fix_luminosity(int val) {
  // manipulate light
}
void fix_moisture(int val) {
  // manipulate water level
}
void fix_water_level(int val) {
  //manipulate water level
}

