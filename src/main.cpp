/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

// Import required libraries
#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "LittleFS.h"
#include "time.h"
#include "Adafruit_Sensor.h"
#include "DHT.h"
#include "DHT_U.h"

#define LEDPIN 2
#define DHTPIN 4
#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

// Replace with your network credentials
const char *ssid = "adb14045";
const char *password = "tidakada";

// const char *ssid = "Smugcat";
// const char *password = "tidakada";

// Stores LED state
String ledState;
int countState = 0;
String serverTime;
float temp;
float humd;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

StaticJsonDocument<250> json;
char buffer[250];

void createJson(const char *tag, int value)
{
  json.clear();
  json[tag] = value;
  serializeJson(json, buffer);
}

void addJsonObject(const char *tag, int value)
{
  JsonObject obj = json.createNestedObject();
  obj[tag] = value;
}

void getCount(AsyncWebServerRequest *request)
{
  Serial.println("Get Count");
  json.clear();
  json["temperature"] = temp;
  json["humidity"] = humd;
  json["serverTime"] = serverTime;
  serializeJson(json, buffer);
  request->send(200, "application/json", buffer);
}

void countUp(AsyncWebServerRequest *request)
{
  Serial.println("Count Up");
  countState += 1;
  json.clear();
  json["temperature"] = countState;
  json["humidity"] = countState;
  serializeJson(json, buffer);
  request->send(200, "application/json", buffer);
}

void notFound(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_OPTIONS)
  {
    request->send(200);
  }
  else
  {
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
  }
}

void blinkLED(const int &count)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(LEDPIN, HIGH);
    delay(100);
    digitalWrite(LEDPIN, LOW);
    delay(100);
  }
}

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%Y-%m-%dT%H:%M:%S");
  char iso8601[25];
  strftime(iso8601, 25, "%Y-%m-%dT%H:%M:%S%z", &timeinfo);
  serverTime = iso8601;
}

void connectToWifi()
{
  // Wifi mode station
  WiFi.mode(WIFI_STA);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address and blink led twice
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(WiFi.SSID());
    Serial.print("=> ");
    Serial.println(WiFi.localIP());
    blinkLED(2);
  }
}

void setupRouting()
{
  // These allow any URL, rather than the same URL (which is the default), to make API calls to the API endpoints
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  // Serve filesystem
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.serveStatic("/assets/", LittleFS, "/");

  server.on("/api/get-count", HTTP_GET, getCount);
  server.on("/api/countup", HTTP_GET, countUp);

  // Route not found
  server.onNotFound(notFound);
  // Start server
  server.begin();
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);

  if (!LittleFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  connectToWifi();

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" *C");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" *C");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" *C");
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println("%");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println("%");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println("%");
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  dht.begin();
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println("Error reading temperature!");
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    temp = event.temperature;
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println("Error reading humidity!");
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    humd = event.relative_humidity;
    Serial.println("%");
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  setupRouting();
}

void loop()
{
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println("Error reading temperature!");
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    temp = event.temperature;
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println("Error reading humidity!");
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    humd = event.relative_humidity;
    Serial.println("%");
  }
}