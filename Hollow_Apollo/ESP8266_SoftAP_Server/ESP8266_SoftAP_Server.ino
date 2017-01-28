#include <ESP8266WiFi.h>

// NEW SHIT!





// WiFi Definitions
const char WiFiAPPSK[] = "liftoff54321"; // Password
const char LOCATION[] = "US";
WiFiServer server(80);

void setup()
{
  initHardware();
  setupWiFi();
  server.begin();

}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    handleClientConnection(client);
  }
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  String AP_NameString = "HOLLOW APOLLO ";
  AP_NameString.concat(LOCATION);

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, AP_NameString.length() + 1, 0);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  Serial.begin(9600);
}


void handleClientConnection(WiFiClient client)
{
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
  // request type (read/set) and value if set.
  if (req.indexOf("/read") != -1)
    val = 0;
  // other values will get an error page

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>";
  s += "\r\n";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />";

  if (val == 0)
  {
    // heading
    s += "<h1>Hollow Apollo</h1><hr />";

    // time
    s += "<h3>Current Time: TODO</h3><h3>Sensors Last Updated: TODO</h3>";

    // outside
    s += "<h3>Outside: TODOC, TODO%RH</h3>";

    // left chamber
    s += " <hr /><h3>Left Chamber</h3><p>TODOC, TODO%RH</p>";
    s += "<p>Fans TODO:ON/OFF</p>";
    s += "<table style=\"height: 70px; border-color: black;\" border=\"black\" width=\"226\"> <tbody> <tr>";
    s += "<td>Thermocouple</td> <td>4</td><td>3</td><td>2</td><td>1</td></tr><tr>";
    s += "<td>Temperature, C</td><td>44</td><td>33</td><td>22</td><td>11</td></tr></tbody></table>";
  }
  else
  {
    s += "Invalid Request.<br> Try /read or /set_time.";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
