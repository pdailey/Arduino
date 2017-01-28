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
// on the RTC and to upload data

//#define WIFI_ENABLED
#ifdef WIFI_ENABLED
#include <ESP8266WiFi.h>
#include <WiFiUdp.h> // For connection to NTP to sync time
#endif

// Enable debugging to speed up the timer intervals
#define DEBUGGING

// NEOPIXELS
// Strip order
const byte RTC_PIXEL = 0;
const byte SD_PIXEL = 1;
const byte WIFI_PIXEL = 2;

// Assign number of pixels, pin and pixel type
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(3, 2, NEO_RGB + NEO_KHZ800);

// Define Specific Colors
uint32_t red = pixels.Color(255, 0, 0);
uint32_t green = pixels.Color(0, 225, 0);
uint32_t blue = pixels.Color(0, 0, 225);


// RTC
RTC_PCF8523 rtc;
uint32_t unix_time;


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
float sensor_values[16] = {};

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
const char* ssid = "Hollow_Apollo";
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

#ifdef DEBUGGING
const int sensor_interval_seconds = 15; // Seconds between readings
#else
const int sensor_interval_seconds = 300; // Seconds between readings
#endif

Atm_timer file_timer;

#ifdef WIFI_ENABLED
Atm_timer rtc_timer; // RTC update to WiFi
#endif


void setup() {
  // Open serial communications
  Serial.begin(9600);

  // Keep this delay to allow easy loading of sketches when reset is pressed
  delay(5000);

  Serial.print("\n\n\n\nInitializing Hollow Apollo...\n");
  Serial.print("=============================\n\n");

  // Setup the status LEDs
  setupNeopixels();
  delay(3000); // delay for dramatic effect

  // set the system time
  bool RTC_setup = setupRTC();
  RTC_setup ? pixels.setPixelColor(RTC_PIXEL, red) : pixels.setPixelColor(RTC_PIXEL, green);
  pixels.show();
  delay(3000);

  // set the file used to write to SD
  setFilename(file_name);
  delay(100);

  bool SD_setup = setupSD();
  SD_setup ? pixels.setPixelColor(SD_PIXEL, red) : pixels.setPixelColor(SD_PIXEL, green);
  pixels.show();
  delay(3000);

  Serial.print("\tWriting file headers...\n\t\t");
  writeStringToSD(file_headers, file_name);

  setupSensors();
  delay(100);

#ifdef WIFI_ENABLED
  bool wifi_setup = setupWifi();
  wifi_setup ? (pixels.setPixelColor(WIFI_PIXEL, red)) : ((pixels.setPixelColor(WIFI_PIXEL, green)));
  pixels.show();
  if (wifi_online) {
    setupUDP();
  }
  delay(3000);
#else
  pixels.setPixelColor(WIFI_PIXEL, 0, 0, 0); // turn off the WiFi pixel
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

  for (int i = 0; i < 16; i++) {
  String str = "";
    str += String(f[i], DEC) + ", ";
  }

  #ifdef DEBUGGING
  Serial.println(str);
  #endif

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

  sensor_timer.begin()
  .interval_seconds(sensor_interval_seconds)
  .repeat( ATM_TIMER_OFF )      // Set timer to run continuously
  .onTimer( collectSensorData )
  .start();

  file_timer.begin()
  .interval_seconds(21600) // 0.25 days
  //.interval_seconds(604800)     // TODO: Timer goes once a week
  .repeat( ATM_TIMER_OFF )      // Set timer to run continuously
  .onTimer( file_timer_callback )
  .start();


#ifdef WIFI_ENABLED
  rtc_timer.begin()
  .interval_seconds(600) // hourly
  //.interval_seconds(604800)     // TODO: Timer goes once a week
  .repeat( ATM_TIMER_OFF )      // Set timer to run continuously
  .onTimer( rtc_timer_callback )
  .start();
#endif

  Serial.print("timers initialized.\n");
  return true;
}


void file_timer_callback( int idx, int v, int up ) {
  // Set the new filename
  setFilename(file_name);
}


#ifdef WIFI_ENABLED
void rtc_timer_callback( int idx, int v, int up ) {
  adjustRTCusingNTP();
}
#endif


bool setupRelays() {
  Serial.print("\tInitializing Relays...");

  relay.begin(pin_p, pin_v)   // Assign the pins to the relays
  .automatic(ms_heat, ms_off) // Set the time of the cooling and heating cycle.
  .trigger(relay.EVT_HEAT_P); // Run the heating cycles first

  Serial.print("relays initialized.\n");
  return true;
}


void collectSensorData( int idx, int v, int up ) {
  // read the sensor data and save it to a string
  String data = readSensors(sensor_values);
  String timeStamp = getUnixTime();
  String str = timeStamp + ", " + data;

  // save the data to the SD
  bool ok = writeStringToSD(str, file_name);
  ok ? (pixels.setPixelColor(SD_PIXEL, red)) : (pixels.setPixelColor(SD_PIXEL, green));
  pixels.show();
}


String getDateTimeString(DateTime t) {
  // Assemble Strings to log data
  String y = String(t.year(), DEC);
  String mn = String(t.month(), DEC);
  String d = String(t.day(), DEC);
  String hh = String(t.hour(), DEC);
  String mm = String(t.minute(), DEC);
  String ss = String(t.second(), DEC);
  //Put all the time and date strings into one String
  return String( y + mn + d + hh + mm + ss);
}


String getUnixTime() {
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



