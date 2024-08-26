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

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

// Replace with your network credentials
const char *ssid = "adb14045";
const char *password = "tidakada";

// const char *ssid = "Smugcat";
// const char *password = "tidakada";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;
int countState = 0;
String serverTime;

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
  json["temperature"] = countState;
  json["humidity"] = countState;
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
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
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
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  if (!LittleFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  connectToWifi();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  setupRouting();
}

void loop() {}