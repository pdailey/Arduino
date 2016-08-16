/*
   Changes Based on:
   https://github.com/mike-rankin/ESP8266_OLED_Display
*/
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define SSID "IOT" // insert your SSID
#define PASS "20ElCapitan14" // insert your password
#define LOCATIONID "19806" // location id
#define HOST "api.wunderground.com"
#define YOUR_KEY "6b28a58b6de86bc9" // API Key


const int buffer_size = 300;    // length of json buffer

// Array of desired weather conditions
// These must be in the order received from wunderground!
//
// Also, watch out for repeating field names in returned json structures,
// and fields with embedded commas (used as delimiters)
char* conds[] = {
  "\"city\":",
  "\"weather\":",
  "\"temp_f\":",
  "\"relative_humidity\":",
  "\"wind_dir\":",
  "\"wind_mph\":",
  "\"pressure_in\":",
};

char* alerts[] = {
  "\"alerts\":",
  "\"type\":"
};

char* forecast[] = {
  "\"period\":",
  "\"pop\":"
  "\"icon\":"
};

unsigned long WMillis = 0;                           // temporary millis() register

void setup() {
  Serial.begin(115200);                              // baudrate of monitor
  WiFi.begin(SSID, PASS);                            // your WiFi Network's SSID & Password
  while (WiFi.status() != WL_CONNECTED) {            // DO until connected
    delay(500);                                      //
    Serial.print(".");                               // print a few dots
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  wunderground("/conditions/q/", conds, 7);
  // TODO
  //wunderground("/alerts/q/", alerts, 2);
  //wunderground("/forecast/q/", forecast, 3);
}


void loop() {
  // Only check weather every 5-15 minutes, so you don't go over quota on wunderground (for free api license)
  if (millis() - WMillis >= 600000) {              // 300 seconds interval
    //wunderground("/conditions/q/", conds, 8);
    //wunderground("/alerts/q/", alerts, 2);
    wunderground("/forecast/q/", forecast, 3);
    WMillis = millis();                            //
  }
  // enough time here to do other stuff, like a bar that is showing how long it takes to update.
}

void wunderground(String data_type, char* fields[], int num_fields) {
  Serial.print("Connecting to HOST: ");
  Serial.println(HOST);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(HOST, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // Build Request String
  // Requests
  //http://api.wunderground.com/api/6b28a58b6de86bc9/alerts/q/CA/San_Fransisco.json
  //http://api.wunderground.com/api/6b28a58b6de86bc9/conditions/q/CA/San_Francisco.json
  String cmd = "GET /api/";
  cmd += YOUR_KEY;
  cmd += data_type;
  cmd += LOCATIONID;
  cmd += ".json";
  //cmd += " HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";

  delay(500);
  client.print(cmd);                            // connect to api.wunderground.com with request_string
  // Debugging
  Serial.println(cmd);
  delay(500);

  // Read incoming data from server and save to string
  String answer;
  while (client.available()) {
    String line = client.readStringUntil('\r');
    answer += line;
  }

  client.stop();
  Serial.println();
  Serial.println("closing connection");

  // Debugging
  //Serial.println(answer);

  // Extract the JSON from the raw data. Primitive json parser.
  String jsonAnswer;
  int jsonIndex;

  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{') {
      jsonIndex = i;
      break;
    }
  }

  // Extract the JSON object and place it in a string
  jsonAnswer = answer.substring(jsonIndex);
  Serial.println();
  Serial.println("JSON answer: ");
  jsonAnswer.trim();
  Serial.println(jsonAnswer);

  parseJSON(jsonAnswer, num_fields);

  WMillis = millis();
}


void parseJSON(String json, int num_fields) {
  int start, end;
  
  if (num_fields == 7) {
    start = json.indexOf("city\":\"") + 7;
    end = start + 10;
    String city = json.substring(start, end);
    
    //int cityIndex = json.indexOf("city");
    //String city = json.substring(cityIndex, cityIndex + 14);
    //city.trim();
    

    
    /*const char* city         = root["city"];
    const char* weather      = root["weather"];
    double temp_f            = root["temp_f"];
    const char* humidity     = root["relative_humidity"];
    const char* wind_dir     = root["wind_dir"];
    double wind_mph          = root["wind_mph"];
    const char* pressure_in  = root["pressure_in"];
*/
    // location of conditions
    Serial.println(city);
    // Conditions: Sunny, Cloudy, Fog, Rain, etc.
    /*Serial.println(weather);
    // Temp
    Serial.println((int)temp_f);
    // Humidity
    Serial.println(humidity);
    // Wind Direction
    Serial.println(wind_dir);
    // Wind Speed
    Serial.println((int)wind_mph);
    // Barometric Pressure
    Serial.println(pressure_in);*/
  }

  /*if (num_fields == 2) {
    int num_alerts   = root["alerts"];
    //TODO: Type does not currently work. Problem with json
    const char* type = root["type"];

    Serial.print("No. Alerts: ");
    Serial.println(num_alerts);
    // Type of Alert
    Serial.println(type);
  }

  if (num_fields == 3) {
    int num_alerts   = root["alerts"];
    const char* type = root["type"];

    Serial.print("No. Alerts: ");
    Serial.println(num_alerts);
    // Type of Alert
    Serial.println(type);
  }*/
}
