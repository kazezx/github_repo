//Libraries to be used by sketch.
#include <ESP8266WiFi.h>
#include <ThingerWifi.h>
#include <SoftwareSerial.h>

//defined constants
#define USERNAME "kazezx"
#define DEVICE_ID "nodemcu"
#define DEVICE_CREDENTIAL "nodemcu_lolin_v3"
#define SSID "Zuhayr"
#define SSID_PASSWORD "Zuhayr007"

//Pins Configurations and Variables
int humidity = 0;
int temperature = 0;
int luminosity = 0;
int moisture = 0;
int water_level = 0;
int pH = 0;

int light = D5;
int fan = D6;
int pump = D7;

float sensor_array[5];


//Thinger.io instantiation
ThingerWifi thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

SoftwareSerial NodeSerial(D2, D3); // RX | TX

void setup() {

  Serial.begin(9600);

  thing.add_wifi(SSID, SSID_PASSWORD);                        //Initiate WiFi connection to Thinger.io Server

  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);

  NodeSerial.begin(4800);

  pinMode(light, OUTPUT);                                     //Sets Pin mode as Output
  pinMode(fan, OUTPUT);
  pinMode(pump, OUTPUT);

  //Reads Sensorial Values from Sensors to Thinger.io
  thing["Temperature"] >> [](pson & out) {
    out = temperature ;
  };
  thing["Humidity"] >> [](pson & out) {
    out = humidity ;
  };
  thing["Luminosity"] >> [](pson & out) {
    out = luminosity ;
  };
  thing["pH"] >> [](pson & out) {
    out = pH ;
  };
  thing["Moisture"] >> [](pson & out) {
    out = moisture ;
  };
  thing["Water Level"] >> [](pson & out) {
    out = water_level ;
  };

  //Instantiate Buttons for Remote Controlled Devices
  thing["Light"] << digitalPin(light);
  thing["Fan"] << digitalPin(fan);
  thing["Pump"] << digitalPin(fan);



}
void loop() {
  //sends to thinger.io server
  thing.handle();
  //Receives Arduino transmitted data via Serial Port
  while (NodeSerial.available() > 0) {
    for (int i = 0; i < 6; i++) {
      sensor_array[i] = NodeSerial.parseFloat();
      if (NodeSerial.read() == '\n') {
        Serial.println(sensor_array[i]);
        i++;
      }
    }
    //Sends array values to variables
    humidity = sensor_array[0];
    temperature = sensor_array[1];
    luminosity = sensor_array[2];
    pH = sensor_array[3];
    moisture = sensor_array[4];
    water_level = sensor_array[5];
    delay(1000);
  }
}
