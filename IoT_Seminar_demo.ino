//Demo Sketch of IoT Seminar

//Libraries
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecureRedirect.h>

//Defining Constants and Values
const char* ssid     = "Zuhayr";// SSID of local network
const char* password = "Zuhayr007";// Password on network
char ssidx[] = "";
char pass[] = "";
char auth[] = "0fadbf23e89348499ef6111f344f9564"; //Blynk Authentification KSey
String APIKEY = "3e4a0763d0da4c27abb4894a7ecc7a9d";//Api Key
String CityID = "934169";//City ID
//Google Initiator Service
char const * const dstHost = "script.google.com";
char const * const dstPath = "/macros/s/AKfycbysZ7qLJtGzQpOC_DlBFJAvV67bvKPvAFwU8BI7PydnuV02AcW_/exec";  // ** UPDATE ME **
int const dstPort = 443;
int32_t const timeout = 5000;
char const * const dstFingerprint = "C7:4A:32:BC:A0:30:E6:A5:63:D1:8B:F4:2E:AC:19:89:81:20:96:BB";
char const * const redirFingerprint = "E6:88:19:5A:3B:53:09:43:DB:15:56:81:7C:43:30:6D:3E:9D:2F:DE";
//Openweathermap Service
WiFiClient client;
char servername[] = "api.openweathermap.org";// remote server
String result;
int  counter = 60;
String weatherDescription = "";
String weatherLocation = "";
String Country;
float Temperature;
float Humidity;
float Pressure;

//NodeMCU V3 LOLin Setup
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssidx, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connection to Internet has been established on IP : " + WiFi.localIP());
    Serial.println("Blynk Support Service is operational!");
  }
}

void loop() {
  Blynk.run();
  openweathermap();
  google_calendar();
  Blynk.syncAll();
}
//Weather Provider Method
void openweathermap() {
  
  if (counter == 60)//Get new data every 10 minutes
  {
    counter = 0;
    delay(500);
    getWeatherData(); // pull weather stats
  }
  else {
    counter++;
    Serial.println();
    displayWeather(weatherLocation, weatherDescription);
    displayConditions(Temperature, Humidity, Pressure);
    delay(500);
    Serial.println();
  }
}
//Pulling openweathermap stats
void getWeatherData()
{
  if (client.connect(servername, 80))
  { //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?id=" + CityID + "&units=metric&APPID=" + APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");//error message if no client connect
    Serial.println();
  }
  while (client.connected() && !client.available())
    delay(1);//waits for data
  while (client.connected() || client.available())
  { //connected or data available
    char c = client.read();//gets byte from ethernet buffer
    result = result + c;
  }
  client.stop();//stop client
  //Decoding json parsed data
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);
  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
  }
  //Parsing json data into variables
  String location = root["name"];
  String country = root["sys"]["country"];
  float temperature = root["main"]["temp"];
  float humidity = root["main"]["humidity"];
  String weather = root["weather"]["main"];
  String description = root["weather"]["description"];
  float pressure = root["main"]["pressure"];
  weatherDescription = description;
  weatherLocation = location;
  Country = country;
  Temperature = temperature;
  Humidity = humidity;
  Pressure = pressure;
}
//Display Regional Stats
void displayWeather(String location, String description)
{ //Location Details
  Serial.print(location);
  Serial.print(", ");
  Serial.print(Country);
  Serial.println();
  Serial.print(description);
  Blynk.virtualWrite(V1, location);
  Blynk.virtualWrite(V2, Country);
}
//Display Weather Stats
void displayConditions(float Temperature, float Humidity, float Pressure)
{ //Temperature
  Serial.println();
  Serial.print("Temperature :");
  Serial.print(Temperature, 1);
  Serial.print("*C");
  Blynk.virtualWrite(V3, Temperature);
  Serial.println();
  //Humidity
  Serial.print("Humidity:");
  Serial.print(Humidity, 0);
  Serial.print(" %");
  Blynk.virtualWrite(V4, Humidity);
  //Pressure
  Serial.println();
  Serial.print("Presure: ");
  Serial.print(Pressure, 1);
  Serial.print(" hPa");
  
}


//Google Calendar Method
void google_calendar() {
  
  bool error = true;
  WiFiClientSecureRedirect client;
  do {
    if (client.connect(dstHost, dstPort) != 1) {  // send connect request
      break;
    }
    while (!client.connected()) {  // wait until connected
      client.tick();
    }
    if (client.request(dstPath, dstHost, 2000, dstFingerprint, redirFingerprint) != 0) { // send alarm request
      break;
    }
    while (!client.response()) {  // wait until host responded
      client.tick();
    }
    while (client.available()) {  // read and print until end of data
      String line = client.readStringUntil('\n');
      Serial.println(line);
      Blynk.virtualWrite(V0, line);
      delay(1000);
    }
    client.stop();
    error = false;
  } while (0);
  delay(1000);
  
}
