#include <SPI.h>              // The SD card uses SPI
#include <SD.h>               // SD Card
#include <Wire.h>             // Used to establish I2C connections
#include <EEPROM.h>           // Retain information in memory between power cycles
#include <stdio.h>            // This program uses the sprintf function
#include "RTClib.h"           // RTC
#include <Adafruit_ADS1015.h> // ADC for thermocouples
#include <Adafruit_INA219.h>  // current sensor for fan
#include <Adafruit_AM2315.h>  // outside T/RH sensor
#include "Adafruit_SHT31.h"   // inside T/RH sensor
#include <Adafruit_NeoPixel.h>// Status LEDs
#include <Automaton.h>        // Finite State Machine framework
#include "Atm_Relay.h"        // Controls relay timing

// WiFi can be used as a secondary backup for time adjustment
// on the RTC and to upload data to the cloud
// WARNING! Choose either WIFI or SOFT AP, not both!
//#define WIFI_ENABLED
#define SoftAP_ENABLED

#ifdef WIFI_ENABLED
#include <ESP8266WiFi.h>
#include <WiFiUdp.h> // For connection to NTP to sync time
#endif

#ifdef SoftAP_ENABLED
#include <ESP8266WiFi.h>
#endif

#ifdef SoftAP_ENABLED
const char APName[] = "Hollow Apollo - US";
//const char APName[] = "Hollow Apollo - SZ";
//const char APName[] = "Hollow Apollo - BJ";
const char APPass[] = "liftoff54321"; // Password
WiFiServer server(80); // Set port
#endif


// Enable debugging to speed up the timer intervals
#define DEBUGGING

// NEOPIXELS
// Strip order
// TODO: TEST
enum Pixels {RTC_PIXEL, SD_PIXEL, WIFI_PIXEL};

// Assign number of pixels, pin and pixel type
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(3, 2, NEO_RGB + NEO_KHZ800);

// Define Specific Colors
uint32_t red = pixels.Color(255, 0, 0);
uint32_t green = pixels.Color(0, 225, 0);
uint32_t blue = pixels.Color(0, 0, 225);


// RTC
RTC_PCF8523 rtc;
uint32_t unix_time;
DateTime lastSensorUpdate = DateTime (2020, 1, 1); // set time to 00:00:00
DateTime lastRelayUpdate = DateTime (2020, 1, 1);

// SD
const byte chipSelect = 15;

// File Name and header
File file;
char file_name[] = "00000.CSV";

// Define the header line for CSVs
#define QUOTE(...) #__VA_ARGS__
const char *file_headers = QUOTE(
                             datetime,
                             // thermocouples
                             TC_4, TC_3, TC_2, TC_1,
                             TC_8, TC_7, TC_6, TC_5,
                             // current sensors
                             Fan Current L, Fan Currrent R,
                             // Temperature and relative humidity
                             T_L, RH_L, T_R, RH_R,
                             T_Out, RH_Out
                           );


// SENSORS
// Store most recent sensor values to pass between functions
float sensor_values[16] = {0};

// Initialize and assign addresses to senors
Adafruit_ADS1115 ads_L(0x48);    // ADC for thermocouples
Adafruit_ADS1115 ads_R(0x49);    // ADC for thermocouples

Adafruit_INA219 ina219_L(0x40);   // Current sensors for left fan
Adafruit_INA219 ina219_R(0x41);   // Current sensors for right fan

//SHT31 address is specified in *.begin()
Adafruit_SHT31 sht31_L = Adafruit_SHT31(); // left inside T/RH
Adafruit_SHT31 sht31_R = Adafruit_SHT31(); // right inside T/RH

Adafruit_AM2315 am2315; // AM2315 - outside T/RH



// WIFI
#ifdef WIFI_ENABLED
//Hard-coded network and password
const char* ssid =      "Hollow_Apollo";
const char* password =  "liftoff54321";
#endif

// NTP
#ifdef WIFI_ENABLED
unsigned int ntp_port = 2390;      // local port to listen for UDP packets
/* Don't hardwire the IP address or we won't get the benefits of the pool.
    Lookup the IP address for the host name instead */
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const byte NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
#endif


// RELAYS
Atm_Relay relay;
const char* Relay_State_Strings [] = {"HEAT LEFT", "HEAT RIGHT", "COOLING ALL"};


// Pins atached to the relay
const byte pin_p = 0;
const byte pin_v = 16;

// TODO: times will change
// time in ms for different cycles
#ifdef DEBUGGING
const unsigned long ms_off  = 60000;
const unsigned long ms_heat = 60000;
#else
const unsigned long ms_off  = 4500000;
const unsigned long ms_heat = 900000;
#endif


// Timers
// TODO: Changes sensor interval
Atm_timer sensor_timer;
Atm_timer file_timer;

#ifdef DEBUGGING
const uint8_t sensor_interval_seconds = 15; // Seconds between readings
const uint16_t file_interval_seconds = 600;  // seconds between file name changes
#else
const uint8_t sensor_interval_seconds = 240;   // 4 min
const uint32_t file_interval_seconds = 259200; // 3 days
#endif


#ifdef WIFI_ENABLED
Atm_timer rtc_timer; // RTC update using NTP
const uint16_t rtc_interval_seconds = 3600; // 1 hour
#endif


void setup() {
  // Open serial communications
  Serial.begin(9600);

  // Keep this delay to allow easy loading of sketches when reset is pressed
  delay(5000);

  // The following delay is used for dramatic effect.
#ifdef DEBUGGING
  int amt_delay = 1;
#else
  int amt_delay = 3000;
#endif

  Serial.print("\n\n\n\nInitializing Hollow Apollo...\n");
  Serial.print("=============================\n\n");

  // Setup the status LEDs
  setupNeopixels();
  delay(amt_delay);

  // set the system time
  bool _status = setupRTC();
  _status ? pixels.setPixelColor(RTC_PIXEL, red) : pixels.setPixelColor(RTC_PIXEL, green);
  pixels.show();
  delay(amt_delay);

  // set the file used to write to SD
  setFilename(file_name);
  delay(100);

  _status = setupSD();
  _status ? pixels.setPixelColor(SD_PIXEL, red) : pixels.setPixelColor(SD_PIXEL, green);
  pixels.show();
  delay(amt_delay);

  Serial.print("\tWriting file headers...\n\t\t");
  writeStringToSD(file_headers, file_name);

  setupSensors();
  delay(100);

#ifdef WIFI_ENABLED
  _status = setupWifi();
  _status ? (pixels.setPixelColor(WIFI_PIXEL, red)) : ((pixels.setPixelColor(WIFI_PIXEL, green)));
  pixels.show();
  if (wifi_online) {
    setupUDP();
  }
#endif

#ifdef SoftAP_ENABLED
  _status = setupAP();
  _status ? (pixels.setPixelColor(WIFI_PIXEL, red)) : ((pixels.setPixelColor(WIFI_PIXEL, green)));
  pixels.show();
#endif

  setupRelays();
  delay(100);

  setupTimers();
  delay(100);

  Serial.print("\n\n\n\nEntering Main Program\n");
  Serial.print("=============================\n\n");
  Serial.println(file_headers);
}

void loop() {

  // Handles the timers and relays using the automaton framework
  automaton.run();

  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    handleClientConnection(client);
  }
}

String readSensors(float f[]) {
  //  reset the array of values, marking unset values
  for (int i = 0; i < 16; i++) {
    f[i] = -404;
  }

  // read  sensors and update value in array
  // read thermocouples in left chamber
  for (byte i = 0; i < 4; i++) {
    double adc = ads_L.readADC_SingleEnded(i); // read the value on the pin
    double v = adc * 0.1875;                  // Convert value to voltage
    f[i] = float(( v / 1000 - 1.25 ) / 0.005);   // Convert voltage to deg C
  }
  delay(100);

  // read thermocouples in right chamber
  for (byte i = 0; i < 4; i++) {
    double adc = ads_R.readADC_SingleEnded(i); // read the value on the pin
    double v = adc * 0.1875;                  // Convert value to voltage
    f[i + 4] = float(( v / 1000 - 1.25 ) / 0.005); // Convert voltage to deg C
  }
  delay(100);

  // Read left fan current in mA
  f[8] = ina219_L.getCurrent_mA();
  delay(100);

  // Read right fan current in mA
  f[9] = ina219_R.getCurrent_mA();
  delay(100);

  // Read left inside T/RH
  f[10] = sht31_L.readTemperature();
  f[11] = sht31_L.readHumidity();
  delay(100);

  // Read right inside T/RH
  f[12] = sht31_R.readTemperature();
  f[13] = sht31_R.readHumidity();
  delay(100);

  //Read outside T/RH
  f[14] = am2315.readTemperature();
  delay(100); // KEEP THIS DELAY!
  f[15] = am2315.readHumidity();
  delay(100);

  // make a string for assembling the data to log:
  String str = "";

  for (byte i = 0; i < 16; i++) {
    str += String(f[i], DEC) + ", ";
  }

  return str;
}


bool writeStringToSD(String str, char *file_name) {
  File file = SD.open(file_name, FILE_WRITE);

  // if the file is available, write to it:
  if (file) {
    file.println(str);
    file.close();
    Serial.println(str);
    return true;
  } else {
    Serial.print("WARNING: error writing to file: ");
    Serial.println(file_name);
    return false;
  }
}


bool setupNeopixels() {
  Serial.print("\tInitializing status LEDs...");
  pixels.begin();
  pixels.setBrightness(64); // Sets the strip to 1/4 brightness
  for (byte i = 0; i < 3; i++) {
    pixels.setPixelColor(i, blue);
  }
  pixels.show(); // Initialize all pixels
  Serial.println("Status LEDs initialized.");
  return true;
}


bool setupSD() {
  Serial.print("\tInitializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return false;
  } else {
    Serial.println("Card initialized.");
    return true;
  }
}


bool setupRTC() {
  Serial.print("\tInitializing RTC...");
  delay(100);
  if (!rtc.begin()) {
    Serial.println("couldn't find RTC\n");
    return false;
  }
  if (! rtc.initialized()) {
    Serial.print("RTC is NOT running!\n");
    // set the time to Jan 1, 2020, 00:00:00
    // as this time will not conflict with any other
    rtc.adjust(DateTime(2020, 1, 1, 0, 0, 0));
    return false;
  } else {
    Serial.print("RTC is running.\n\t\t");
    Serial.println(getDateTimeString(rtc.now()));
    return true;
  }
}


bool setupSensors() {
  Serial.print("\tInitializing Sensors...");
  // Thermocouple sensors
  ads_L.begin();
  ads_R.begin();

  // Fan current sensor
  ina219_L.begin();
  ina219_L.setCalibration_32V_1A();
  ina219_R.begin();
  ina219_R.setCalibration_32V_1A();

  sht31_L.begin(0x44);
  sht31_R.begin(0x45);

  // Outside T/RH
  am2315.begin();
  Serial.print("sensors initialized.\n");
  return true;
}


bool setupTimers() {
  Serial.print("\tInitializing Timers...");
  char str [64];

  sensor_timer.begin()
  .interval_seconds(sensor_interval_seconds)
  .repeat( ATM_TIMER_OFF )      // Set timer to run continuously
  .onTimer( collectSensorData )
  .start();

  snprintf(str, 64, "\n\t\tSensor Timer set: readings taken every %d seconds", sensor_interval_seconds);
  Serial.print(str);


  file_timer.begin()
  .interval_seconds(file_interval_seconds)
  .repeat( ATM_TIMER_OFF )      // Set timer to run continuously
  .onTimer( file_timer_callback )
  .start();

  snprintf(str, 64, "\n\t\tFile Timer set: save file changed every %d days", file_interval_seconds / 86400 );
  Serial.print(str);


#ifdef WIFI_ENABLED
  rtc_timer.begin()
  .interval_seconds(rtc_interval_seconds)
  .repeat( ATM_TIMER_OFF )
  .onTimer( rtc_timer_callback )
  .start();

  snprintf(str, 64, "\n\t\tRTC Timer set: %d seconds between adjusting RTC using NTP servers\n", rtc_interval_seconds);
  Serial.print(str);
#endif

  Serial.print("\n\t\t...timers initialized.\n");
  return true;
}


void file_timer_callback( int idx, int v, int up ) {
  // Set the new filename
  setFilename(file_name);

  // Write the headers to the new file
  writeStringToSD(file_headers, file_name);
}


#ifdef WIFI_ENABLED
void rtc_timer_callback( int idx, int v, int up ) {
  adjustRTCusingNTP();
#endif


bool setupRelays() {
  Serial.print("\tInitializing Relays...");

  relay.begin(pin_p, pin_v)   // Assign the pins to the relays
  .automatic(ms_heat, ms_off) // Set the time of the cooling and heating cycle.
  .onChange( relay_callback )
  .trigger(relay.EVT_HEAT_P); // Run the heating cycles first
  
  
  char str [64];
  byte min_heat = ms_heat / (60 * 1000);
  byte min_off  = ms_off  / (60 * 1000) + min_heat; // the 2 heater banks alternate to keep power consumption low. A complete cycle is 1 off + 2 heats.
  snprintf(str, 64, "\n\t\tRelays set: Heat %d min, Cool %d min.\n", min_heat, min_off);
  Serial.print(str);
  return true;
}

void relay_callback( int idx, int v, int up ) {
  lastRelayUpdate = rtc.now();
}

void collectSensorData( int idx, int v, int up ) {
  // read the sensor data and save it to a string
  String data = readSensors(sensor_values);
  String timeStamp = getUnixTimeFromRTC();
  String str = timeStamp + ", " + data;

  lastSensorUpdate = rtc.now();

  // save the data to the SD
  bool ok = writeStringToSD(str, file_name);
  ok ? (pixels.setPixelColor(SD_PIXEL, red)) : (pixels.setPixelColor(SD_PIXEL, green));
  pixels.show();
}


String getDateTimeString(DateTime t) {
  char str [19];
  snprintf(str, 19, "%04d/%02d/%02d %02d:%02d:%02d",
           t.year(), t.month(), t.day(), t.hour(), t.minute(), t.second());
  return str;
}

String getTimeString(DateTime t) {
  char str [12];
  snprintf(str, 12, "%02d:%02d:%02d", t.hour(), t.minute(), t.second());
  return str;
}

String getUnixTimeFromRTC() {
  DateTime now = rtc.now();
  return String(now.unixtime(), DEC);
}


void setFilename(char *filename) {
  Serial.print("\tSetting file name...");
  EEPROM.begin(12); // call EEPROM.begin(size) before you start reading or writing

  // A counter is saved to EEPROM as an unsigned bit (0-65535)
  // retrieve the counter value to set the file name
  byte high = EEPROM.read(0);
  byte low  = EEPROM.read(1);
  unsigned int cnt = (high << 8) + low; //reconstruct the integer

  // update the file name variable
  sprintf(filename, "%05d.CSV", cnt);

  Serial.print("file name set: \n\t\t");
  Serial.println(filename);

  // Increment the count and save to EEPROM
  cnt++;
  EEPROM.write(0, highByte(cnt)); //write the first half
  EEPROM.write(1, lowByte(cnt)); //write the second half
  EEPROM.end();
}

#ifdef SoftAP_ENABLED
bool setupAP() {
  Serial.print("\tInitiailzing Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APName, APPass);
  yield();

  server.begin();
  Serial.print("Access Point established. Connect at:");

  char str [64];
  snprintf(str, 64, "\n\t\tNetwork:  %s \n\t\tPassword: %s\n", APName, APPass);
  Serial.print(str);
  return true;
}
#endif

#ifdef SoftAP_ENABLED
void handleClientConnection(WiFiClient client)
{
  Serial.println("A client has connected.");
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
  // request type (read/set) and value if set.
  if (req.indexOf("/") != -1)
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
    s += "<h1>Hollow Apollo</h1><hr>";

    // time
    s += "<h3>Current Time: " + getTimeString(rtc.now()) + "</h3>";
    s += "<h3>Sensors Last Updated: " + getTimeString(lastSensorUpdate) + "</h3>";
    s += "<hr>";
    s += "<p>Relay Last Changed: " + getTimeString(lastRelayUpdate) + "</p>";
    s += "<p>System State: ";
    s += Relay_State_Strings[relay.state()];
    s += "</p>";
    yield();

    // outside
    char str [64];
    char t[8];
    char rh[8];
    char mA[8];
    dtostrf(sensor_values[14], 7, 2, t);
    dtostrf(sensor_values[15], 7, 2, rh);
    snprintf(str, 64, "<p>Outside T/RH: %sC / %sRH</p>", t, rh);
    s += str;
    yield();

    // left chamber
    s += " <hr><h3>Left Chamber</h3>";
    dtostrf(sensor_values[10], 7, 2, t);
    dtostrf(sensor_values[11], 7, 2, rh);
    snprintf(str, 64, "<p>Inside T/RH: %sC / %sRH</p>", t, rh);
    s += str;
    yield();
    dtostrf(sensor_values[8], 7, 1, mA);
    snprintf(str, 64, "<p>Fans: %s mA</p>", mA);
    s += str;
    yield();

    s += "<table border=\"black\" width=\"226\"><tbody><tr>";
    s += "<td>Thermocouple</td> <td>4</td><td>3</td><td>2</td><td>1</td></tr><tr>";
    s += "<td>Temperature, C</td>";
    yield();

    for (byte i = 0; i < 4; i++) {
      dtostrf(sensor_values[i], 7, 1, t);
      snprintf(str, 64, "<td>%s</td>", t);
      s += str;
    }
    s += "</tr></tbody></table>";

    // right chamber
    s += " <hr><h3>Right Chamber</h3>";
    dtostrf(sensor_values[12], 7, 2, t);
    dtostrf(sensor_values[13], 7, 2, rh);
    snprintf(str, 64, "<p>Inside T/RH: %sC / %sRH</p>", t, rh);
    s += str;
    yield();

    dtostrf(sensor_values[9], 7, 1, mA);
    snprintf(str, 64, "<p>Fans: %s mA</p>", mA);
    s += str;
    yield();
    
    s += "<table border=\"black\" width=\"226\"><tbody><tr>";
    s += "<td>Thermocouple</td> <td>8</td><td>7</td><td>6</td><td>5</td></tr><tr>";
    s += "<td>Temperature, C</td>";
    yield();

    for (byte i = 4; i < 8; i++) {
      dtostrf(sensor_values[i], 7, 1, t);
      snprintf(str, 64, "<td>%s</td>", t);
      s += str;
    }
    s += "</tr></tbody></table>";

  } else {
    s += "Invalid Request. RTFM...";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  Serial.println(s);
  yield();
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
#endif

#ifdef WIFI_ENABLED
bool adjustRTCusingNTP() {
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  unsigned long epoch = getNTP(); // get the NTP time stamp

  rtc.adjust(epoch);  // set the RTC
  return true;
}
#endif


#ifdef WIFI_ENABLED
unsigned long getNTP() {
  sendNTPpacket(timeServerIP); // send an NTP packet to a time server

  // wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() ) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // convert NTP time time to unix:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    Serial.print("Unix time = ");
    Serial.println(epoch);

    return epoch;
  }
}
#endif


#ifdef WIFI_ENABLED
// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress & address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a time stamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
#endif


#ifdef WIFI_ENABLED
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
#endif


#ifdef WIFI_ENABLED
bool setupUDP() {
  Serial.println("Starting UDP");
  Udp.begin(ntp_port);
  Serial.print("NTP port: ");
  Serial.println(Udp.localPort());
  return true;
}
#endif



