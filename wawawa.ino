#include <ESP8266WiFi.h>
#include <ThingerWifi.h>
#include <SoftwareSerial.h>
#include <elapsedMillis.h>

#define USERNAME "kazezx"
#define DEVICE_ID "nodemcu"
#define DEVICE_CREDENTIAL "nodemcu_lolin_v3"
#define SSID "MIC_LAB_Linksys"
#define SSID_PASSWORD "miclab123"

elapsedMillis timeElapsed;
unsigned int interval = 20000;

int humidity = 0;
int temperature = 0;
int luminosity = 0;
int moisture = 0;
int water_level = 0;

int light = D5;
int fan = D6;
int pump = D7;

String packet = "";
String notif = "";

int emg_pin = 16;
int val = 0;

String deviceId = "vBC342F299439410";
const char* logServer = "api.pushingbox.com";

const char* ssid = "MIC_LAB_Linksys";
const char* password = "miclab123";

ThingerWifi thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

SoftwareSerial NodeSerial(D2, D3); // RX | TX
WiFiClient client;

void setup() {

  WiFi.begin(ssid, password);

  pinMode(emg_pin, INPUT);

  Serial.begin(115200);
  NodeSerial.begin(4800);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  thing.add_wifi(SSID, SSID_PASSWORD);

  Serial.println("WiFi is operational.");

  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);

  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(pump, OUTPUT);

  thing["Temperature"] >> [](pson & out) {
    out = temperature ;
  };
  thing["Humidity"] >> [](pson & out) {
    out = humidity ;
  };
  thing["Luminosity"] >> [](pson & out) {
    out = luminosity ;
  };
  thing["Moisture"] >> [](pson & out) {
    out = moisture ;
  };
//  thing["Water Level"] >> [](pson & out) {
//    out = water_level ;
//  };

  thing["Light"] << digitalPin(light);
  thing["Fan"] << digitalPin(fan);
  thing["Pump"] << digitalPin(fan);

  sendNotification("Thinger Daily Report has Started,You will be notified of any Alerts.");
}
void loop() {

//  packet = "";

  thing.handle();

  delay(1500);

  while (NodeSerial.available() > 0) {
    char c = NodeSerial.read();
    packet += c;
  }

  Serial.println(packet);
  //delay(300);

  humidity = converter(packet, ':', 0);
  if (humidity < 60 || humidity > 70) {
    sendNotification("Humidity is wrong at " + String(humidity) + " ." + "Automatic callibration will be performed or you may remotely activate the Light.");
  }

  temperature = converter(packet, ':', 1);
  if (temperature < 20 || temperature > 28) {
    sendNotification("Temperature is high at " + String(temperature) + " ." + "Automatic callibration will be performed or you may remotely activate the Fan.");
  }

  luminosity = converter(packet, ':', 2);
  if (luminosity < 500 || luminosity > 1000) {
    sendNotification("Luminosity is high at " + String(luminosity) + " ." + "Automatic callibration will be performed or you may remotely activate the Light.");
  }

  moisture = converter(packet, ':', 3);
  if (moisture < 50 || moisture > 75) {
    sendNotification("Moisture is high at " + String(moisture) + " ." + "Automatic callibration will be performed or you may remotely activate the Pump.");
  }

//  water_level = converter(packet, ':', 4);
//  if (water_level < 100 || water_level > 200) {
//    sendNotification("Water Level is high at " + String(water_level) + " ." + "Automatic callibration will be performed or you may remotely activate the Pump.");
//  }

  notif = "Humidity : " + (String)humidity + "\n";
  notif += "Temperature : " + (String)temperature + "\n" ;
  notif += "Luminosity : " + (String)luminosity + "\n" ;
  notif += "Moisture : " + (String)moisture + "\n" ;
//  notif += "Water Level : " + (String)water_level + "\n" ;

  val = digitalRead(emg_pin);
  if (val == HIGH) {
    sendNotification(notif);
  }

  if (timeElapsed > interval) {
    Serial.println(notif);
    sendNotification(notif);
    timeElapsed = 0 ;
  }
  //delay(200);
}

int converter(String data, char separator, int index) {
  int counter = 0;
  int variable = 0;
  String dataPart = "";
  for (int i = 0; i < data.length(); i++) {
    if (data[i] == separator) {
      counter++;
    } else if (counter == index) {
      dataPart.concat(data[i]);
      variable = dataPart.toInt();
    } else if (counter > index) {
      return variable;
      break;
    }
  }
  return variable;
}

void sendNotification(String message) {

  if (client.connect(logServer, 80)) {
    String postStr = "devid=";
    postStr += String(deviceId);
    postStr += "&message_parameter=";
    postStr += String(message);
    postStr += "\r\n\r\n";

    client.print("POST /pushingbox HTTP/1.1\n");
    client.print("Host: api.pushingbox.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();
}
