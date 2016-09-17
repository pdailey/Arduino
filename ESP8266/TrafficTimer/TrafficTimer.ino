#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <Ticker.h>

WiFiClientSecure client;
Adafruit_7segment matrix = Adafruit_7segment();
Ticker ticker;

const unsigned long BAUD_RATE = 115200;    // serial connection speed
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 1024;       // max size of the HTTP response
const char* SSID = "IOT";                   // insert your SSID
const char* PASS = "20ElCapitan14";         // insert your password
const char* google_server = "maps.googleapis.com";
const char* home = "Wilmington+DE";
const char* work = "Elk+Mills+MD";
#define YOUR_KEY "AIzaSyCXMMVZ2ckFFsaA1XqGt0d_qfFfqFb1WvE" // API Key


int duration;
unsigned long WMillis = 60000;      // temporary millis() register
bool colonState = false;


void setup() {
  initSerial();
  initWifi();
  init7Segment();
  ticker.attach_ms(500, toggleState);
  //getTravelTime(ORIG, DEST);
}

void loop() {
  // Only check traffic time every 1 minutes, so you don't go over quota of requests (for free api license)
  if (millis() - WMillis >= 60000) {
    setDisplayToLoad();
    // If sucessfully connected
    if (connectHTTP(google_server)) { 
      if (sendRequest(server, home, work) && skipResponseHeaders()) {
        char response[MAX_CONTENT_SIZE];
        readReponseContent(response, sizeof(response));

        // If message sucessfully parsed
        if(parseDuration(response, duration)) { 
          printDuration(duration);
          displayDuration(duration);
        }
      }
      disconnect();
    }
    // reset update counter
    WMillis = millis();
  }

  // Update Time
  
  // Write the display to show blicking of colon
  matrix.writeDisplay();
}

// Initialize Serial port
void initSerial() {
  Serial.begin(BAUD_RATE);
  while (!Serial) ;  // wait for serial port to initialize
  Serial.println("Serial ready");
}

// Initialize Wifi library
void initWifi() {
  if (!WiFi.begin(SSID, PASS)) {                  // WiFi Network's SSID & Password
    Serial.println("Failed to configure Wifi");
    return;
  }

  while (WiFi.status() != WL_CONNECTED) {         // DO until connected
    delay(500);
    Serial.print(".");                            // print a few dots
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

// Initialize 7 Segment Display
void init7Segment() {
  matrix.begin(0x70);
  matrix.setBrightness(1);
  setDisplayToLoad();
}

// Open connection to the HTTP server
bool connectHTTP(const char* hostName) {
  Serial.print("Connecting to ");
  Serial.println(hostName);

  // bool ok = client.connect(hostName, 80);
  bool ok = client.connect(hostName, 443);

  Serial.println(ok ? "Connected" : "Connection Failed!");
  return ok;
}

// Send the HTTP GET request to the server
bool sendRequest(const char* host, const char* origin, const char* dest) {
  // Build Request String
  String cmd = "GET ";
  cmd += "https://maps.googleapis.com/maps/api/distancematrix/json?origins=";
  cmd += origin;
  cmd += "&destinations=";
  cmd += dest;
  cmd += "&mode=driving&departure_time=now";
  cmd += "&key=";
  cmd += YOUR_KEY;
  cmd += " HTTP/1.1\r\nHost: maps.googleapis.com\r\n\r\n";
  delay(500);

  Serial.println(cmd);
  client.print(cmd);
  return true;
}

// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  // char endOfHeaders[] = "\r\n\r\n";
  // TODO: May change with changing locations!
  char endOfHeaders[] = "27a\r\n"; // modified to match the output from the google api

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }
  return ok;
}

// Read the body of the response from the HTTP server
void readReponseContent(char* content, size_t maxSize) {
  size_t length = client.readBytes(content, maxSize);
  content[length] = 0;
  // DEBUGGING
  //Serial.println("Begin Response:");
  //Serial.println(content);
  //Serial.println("End Response");
}


// Parse the JSON from the input string and extract the interesting values
/*
  // Here is the JSON we need to parse
  // {
  //   "destination_addresses" : [ "Elk Mills, MD 21920, USA" ],
  //   "origin_addresses" : [ "Wilmington, DE, USA" ],
  //   "rows" : [
  //      {
  //         "elements" : [
  //            {
  //               "distance" : {
  //                  "text" : "34.3 km",
  //                  "value" : 34257
  //               },
  //               "duration" : {
  //                  "text" : "26 mins",
  //                  "value" : 1576
  //               },
  //                  "duration_in_traffic" : {
  //                  "text" : "30 mins",
  //                  "value" : 1804
  //               },
  //               "status" : "OK"
  //            }
  //         ]
  //      }
  //   ],
  //   "status" : "OK"
  //}
*/
bool parseDuration(char* content, int& duration) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(content);

  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }

  duration = root["rows"][0]["elements"][0]["duration_in_traffic"]["value"];
  return true;
}

// Print the data extracted from the JSON
void printDuration(int duration) {
  Serial.print("Time to desitnation (sec) = ");
  Serial.println(duration);
}

// Display the current commute time on the 7-segment display
void displayDuration(int duration) {
  uint16_t blinkcounter = 0;
  boolean drawDots = false;
  
  // duration is given in seconds, we want to display hrs:min
  matrix.writeDigitNum(0, 0, drawDots);
  matrix.writeDigitNum(1, (duration / 3600), drawDots);
  matrix.writeDigitNum(3, (duration / 60) / 10, drawDots);
  matrix.writeDigitNum(4, (duration / 60) % 10, drawDots);

  // Update
  matrix.writeDisplay();
}

// Close the connection with the HTTP server
void disconnect() {
  Serial.println("Disconnecting");
  client.stop();
}

void toggleState() {
  //toggle
  colonState = !colonState;
  matrix.drawColon(colonState);
}

void setDisplayToLoad() {
  // Number given is too large
  matrix.print(10000, DEC); 
  // Display reads "--:--" to show it is loading
  matrix.writeDisplay();
}
