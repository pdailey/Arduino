/*
 * Debugging script used to test uplaoding data to Dweet. 
 * Uses an HTTP GET command to post the data to the servers. 
 * 
 * Script also contains a really nice SetupWifi() function that 
 * could be expanded to incoperate the autoconnect script where 
 * WiFi connections are created on the fly.
 */
#include <ESP8266WiFi.h>
#include <Wire.h>

WiFiClientSecure client;

// WiFi
const char* ssid = "Hollow_Apollo";
const char* password =  "liftoff54321";

const char* host = "dweet.io";
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 1024;       // max size of the HTTP response


// Choose location by uncommenting one of the following
#define  US
//#define BJ
//#define SZ
#ifdef US
const char* thing_name = "US";
const char* thing_key  = "6DT6pu4gocsb9VEFEtHaqb";
#endif
#ifdef BJ
const char* thing_name = "BJ";
const char* thing_key  = "6Z72XzPplCx0rqHZfXXKCY";
#endif
#ifdef SZ
const char* thing_name = "SZ";
const char* thing_key  = "6GtSsbbgS23LRj0bpPLqud";
#endif

//// TEST /////////
float f [16] = {};
int n = 0;

void assignVals(float f[], int n) {
  for (int i; i < 16; i++) {
    f[i] = i + n * 1.5;
  }
}
//// TEST /////////

void setup() {
  Serial.begin(9600);
  delay(5000);

  // put your setup code here, to run once:
  setupWifi();

}

void loop() {
  assignVals(f, n);
  postToDweet();
  // put your main code here, to run repeatedly:
  delay(10000);
  n++;
}




String constructPostString() {
  String s = "GET https://dweet.io/dweet/for/";
  s += thing_name;
  s += "?key=";
  s += thing_key;
  s += "&peter=";
  s += f[1];
  s += " HTTP/1.1\r\n Host: " ;
  s += host;
  s += "\r\n Connection: close\r\n\r\n";
  return s;
}


bool postToDweet() {
  String post_string = constructPostString();
  // https://dweet.io/dweet/for/{my_locked_thing}?key={my_key}&hello=world&foo=bar
  //client.fastrprint(F("&temperature="));
  //client.print(temperature);

  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return false;
  }
  if (client.connected()) {
    Serial.print(F("Sending request... "));
    Serial.println(post_string);
    client.print(post_string);
    Serial.println(F("done."));
    return true;
  } else {
    Serial.println(F("Connection failed"));
    return false;
  }

  Serial.println(F("Reading answer..."));
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  return true;
}

bool setupWifi() {
  Serial.print("\n\Initiailzing wifi connection to network...");
  Serial.println(ssid);

  if (WiFi.status() != WL_CONNECTED) {  // FIX FOR USING 2.3.0 CORE (only .begin if not connected)
    WiFi.begin(ssid, password);       // connect to the network
  }

  byte count;
  for ( count = 0; count < 20 ; count++ )  {
    delay(500);
    Serial.print( "\t\t" ); Serial.print( count );
    Serial.print( " WifiStat: " ); Serial.println( WiFi.status() );
    if ( WiFi.status() == WL_CONNECTED ) {
      break;
    }
  }

  if ( count < 20 ) {
    Serial.println("\nWiFi connected");
    return true;
  } else {
    Serial.println("Client NOT connected!");

    // create a SoftAP for access
    //WiFi.softAP(ESPssid, ESPpassword);
    // etc..........
    return false;
  }
}
