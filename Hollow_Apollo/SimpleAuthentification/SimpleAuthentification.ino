#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Hollow Apollo - US";
const char* password = "liftoff54321";

ESP8266WebServer server(80);

//login page, also called for disconnect
void handleSetTime() {

  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>";
  s += "\r\n";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"; "<html><body>";
  s += "<form action=\"action_page.php\">";
  s += getOptionString(String("DAY"), 1, 31);
  s += getOptionString(String("MONTH"), 1, 12);
  s += getOptionString(String("YEAR"), 2017, 2020);
  s += getOptionString(String("HOUR"), 0, 23);
  s += getOptionString(String("MINUTE"), 0, 60);
  s += "</select>/";
  s += "<p><input type=\"submit\" /></p></form>";
  s += "</body></html>";
  server.sendContent(s);

  if (server.hasArg("YEAR") && server.hasArg("MONTH") && server.hasArg("DAY") &&
      server.hasArg("HOUR") && server.hasArg("MINUTE")) {
    // rtc.set(server.arg("YEAR"), server.arg("MONTH"), server.arg("DAY"), server.arg("HOUR"), server.arg("MINUTE"), 0)
    Serial.println("Time set was Successful");
    return;
  }
}

String getOptionString(String _name, int _min, int _max) {
  String s = "<select name=\"" + _name + "\">";
  for (int i = _min; i < (_max + 1) ; i++) {
    s += "<option value=\"";
    s += i;
    s += "\">";
    s += i;
    s += "</option>";
  }
  s += "</select>";
  return s;
}

//root page
void handleRoot() {
  Serial.println("Enter handleRoot");
  // TODO: hollow apollo main
  String content = "<html><body><H2>HEY!</H2><br></body></html>";
  server.send(200, "text/html", content);
}

void handleNotFound() {
  String message = "Page not found<br>";
  message += "Try http://192.168.4.1/ or http://192.168.4.1/set_time instead";
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Configuring Access point");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handleRoot);
  server.on("/set_time", handleSetTime);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
