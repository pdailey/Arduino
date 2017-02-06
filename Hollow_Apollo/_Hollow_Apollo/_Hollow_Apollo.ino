/******************************************************************************
HOLLOW APOLLO

A control system for remote thermal cycling of samples. Logs temperature
of heaters and T/RH inside and outside the chambers. This allows us to
remotely monitor the fixture and ensure that everything is running.


HARDWARE

Microcontroller: ESP8266 running C++ code. The code controls
relays that cycle heaters and fans. The code also handles the
collection of sensor data. Sensor data is logged to an SD and displayed
on a webpage.

Sensors: 8 thermocouples (one for each heater). Temperature and
relative humidity in each chamber. Outside temperature and relative
humidity. DC current, one for each bank of fans.

Data Storage: Sensor values are recorded to a CSV on a microSD card.
Timestamps for values are provided by an RTC.

Monitoring: 3 RGB LEDs provide the current status of RTC, SD & Wi-Fi.
Additionally, the ESP8266 creates a Wi-Fi access point, and host a
webpage locally showing the status and latest sensor readings.

 *****************************************************************************/

#include <SPI.h>              // The SD card uses SPI
#include <SD.h>               // SD Card
#include <Wire.h>             // Used to establish I2C connections
#include <EEPROM.h>           // Retain information in memory between power cycles
#include <stdio.h>            // This program uses the sprintf function
#include "RTClib.h"           // RTC
#include <Adafruit_ADS1015.h> // ADC for thermocouples
#include <Adafruit_INA219.h>  // current sensor for fans
#include <Adafruit_AM2315.h>  // outside T/RH sensor
#include "Adafruit_SHT31.h"   // inside T/RH sensor
#include <Adafruit_NeoPixel.h>// Status LEDs
#include <Automaton.h>        // Finite State Machine framework
#include "Atm_Relay.h"        // Controls relay timing

// Enable debugging to speed up the timer intervals for fast testing of functionality
#define DEBUGGING

// Enable SoftAP to set up a webserver to display system status
#define SoftAP_ENABLED

#ifdef SoftAP_ENABLED
#include <ESP8266WiFi.h>

const char APName[] = "Hollow Apollo"; // Wifi Network Name
const char APPass[] = "liftoff54321";  // Password
WiFiServer server(80); // Set port
#endif


/* NEOPIXELS
* The neopixels serve as status LEDs for different subsystems.
*/

// Pixel order, based upon physical connections
enum Pixel {RTC_PIXEL, SD_PIXEL, WIFI_PIXEL};

// Assign number of pixels, pin and pixel type
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(3, 2, NEO_RGB + NEO_KHZ800);

// Define Specific Colors (see comments for meaning of each color)
uint32_t blue = pixels.Color(0, 0, 225);  // Loading
uint32_t green = pixels.Color(0, 225, 0); // Ready
uint32_t red = pixels.Color(255, 0, 0);   // Error


/* Real Time Clock (RTC)
 *
 * The RTC provides time keeping functionality to the system. The time provided
 * by the RTC is used only to generate timestamps for sensor data, and
 * to display time to the webpage.
 *
 * Timekeeping for timers is governed by the microcontroller crystal.
 */
RTC_PCF8523 rtc;

DateTime lastSensorUpdate = DateTime (2020, 1, 1); // set to to 00:00:00
DateTime lastRelayUpdate = DateTime (2020, 1, 1);


/* SD
 * The SD stores all the senor readings to a CSV file.
 */

// SD file Name and header.
// Header row is appended each time the file name is changed.
char file_name[] = "00000.CSV";

// Define the header row
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
// Communication with sensors is over I2C.

// Stores most recent sensor values to pass between functions
float sensor_values[16] = {0};

// Initialize and assign I2C addresses to senors
Adafruit_ADS1115 ads_L(0x48);    // ADC for thermocouples
Adafruit_ADS1115 ads_R(0x49);    // ADC for thermocouples

Adafruit_INA219 ina219_L(0x40);   // Current sensors for left fan
Adafruit_INA219 ina219_R(0x41);   // Current sensors for right fan

//SHT31 address is specified in *.begin()
Adafruit_SHT31 sht31_L = Adafruit_SHT31(); // left inside T/RH
Adafruit_SHT31 sht31_R = Adafruit_SHT31(); // right inside T/RH

Adafruit_AM2315 am2315; // AM2315 - outside T/RH


// RELAYS
Atm_Relay relay;
const char* Relay_State_Strings [] = {"HEAT LEFT", "HEAT RIGHT", "COOLING ALL"};

// Pins attached to the relays
const byte pin_left = 0;
const byte pin_right = 16;

// time in ms for different cycles
// TODO: Check implementation in the Relay Library
#ifdef DEBUGGING
const unsigned long ms_off  = 60000;
const unsigned long ms_heat = 60000;
#else
const unsigned long ms_off  = 4500000;
const unsigned long ms_heat = 900000;
#endif


// Timers
Atm_timer sensor_timer;
Atm_timer file_timer;

#ifdef DEBUGGING
const uint8_t sensor_interval_seconds = 15; // Seconds between readings
const uint16_t file_interval_seconds = 600;  // seconds between file name changes
#else
// TODO: Change sensor interval
const uint8_t sensor_interval_seconds = 240;   // 4 min
const uint32_t file_interval_seconds = 259200; // 3 days
#endif


void setup() {
  // Open serial communications
  Serial.begin(9600);

  // Keep this delay to allow easy loading of sketches when reset is pressed
  delay(5000);

  int long_delay = 2000;
  int short_delay = 100;

  Serial.print("\n\n\n\nInitializing Hollow Apollo...\n");
  Serial.print("=============================\n\n");

  // Setup the status LEDs
  setupNeopixels();
  delay(long_delay);

  // set the system time
  bool _status = setupRTC();
  updateStatusLED(RTC_PIXEL, _status);
  delay(long_delay);

  // set the file used to write to SD
  setFilename(file_name);
  delay(short_delay);

  _status = setupSD();
  updateStatusLED(SD_PIXEL, _status);
  delay(long_delay);

  Serial.print("\tWriting file headers...\n\t\t");
  writeStringToSD(file_headers, file_name);

  setupSensors();
  delay(short_delay);

#ifdef SoftAP_ENABLED
  _status = setupAP();
  updateStatusLED(WIFI_PIXEL, _status);
#endif

  setupRelays();
  delay(short_delay);

  setupTimers();
  delay(short_delay);

  Serial.print("\n\n\n\nEntering Main Program\n");
  Serial.print("=============================\n\n");
  Serial.println(file_headers);
}

void loop() {
  // Handles the timers and relays using the automaton framework
  automaton.run();

  // Check if a client has connected to the access point
  WiFiClient client = server.available();
  if (client) {
    handleClientConnection(client); // Serve a webpage
  }
}

void updateStatusLED(enum Pixel, bool status_ok){
  if( status_ok )
    pixels.setPixelColor(Pixel, green)
  else
    pixels.setPixelColor(Pixel,  red);

  // Send the updated color to the pixels
  pixels.show();
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


bool setupSD() {
  Serial.print("\tInitializing SD card...");

  byte chipSelectPin = 15;

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelectPin)) {
    Serial.println("Card failed, or not present");
    return false;
  } else {
    Serial.println("Card initialized.");
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


bool setupRelays() {
  Serial.print("\tInitializing Relays...");

  relay.begin(pin_left, pin_right)   // Assign the pins to the relays
  .automatic(ms_heat, ms_off) // Set the time of the cooling and heating cycle for a single chamber
  .onChange( relay_callback )
  .trigger(relay.EVT_HEAT_P); // Run the heating cycles first


  char str [64];
  byte min_heat = ms_heat / (60 * 1000);
  byte min_off  = ms_off  / (60 * 1000); // the 2 heater banks alternate to keep power consumption low. A complete cycle is 1 off + 2 heats.
  snprintf(str, 64, "\n\t\tRelays set: Heat %d min, Cool %d min.\n", min_heat, min_off);
  Serial.print(str);
  return true;
}

void relay_callback( int idx, int v, int up ) {
  lastRelayUpdate = rtc.now();
}


bool setupTimers() {
  Serial.print("\tInitializing Timers...");
  char str [64];

  file_timer.begin()
  .interval_seconds(file_interval_seconds)
  .repeat( ATM_TIMER_OFF )      // Set timer to run continuously
  .onTimer( file_timer_callback )
  .start();

  snprintf(str, 64, "\n\t\tFile Timer set: save file changed every %d days", file_interval_seconds / 86400 );
  Serial.print(str);


  sensor_timer.begin()
  .interval_seconds(sensor_interval_seconds)
  .repeat( ATM_TIMER_OFF )      // Set timer to run continuously
  .onTimer( collectSensorData )
  .start();

  snprintf(str, 64, "\n\t\tSensor Timer set: readings taken every %d seconds", sensor_interval_seconds);
  Serial.print(str);


  Serial.print("\n\t\t...timers initialized.\n");
  return true;
}


void file_timer_callback( int idx, int v, int up ) {
  // Set the new filename.
  setFilename(file_name);

  // Write the headers to the new file
  writeStringToSD(file_headers, file_name);
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


void collectSensorData( int idx, int v, int up ) {
  // read the sensor data and save it to a string
  String data = readSensors(sensor_values);
  String timeStamp = getUnixTimeFromRTC();
  String str = timeStamp + ", " + data;

  lastSensorUpdate = rtc.now();

  // save the data to the SD
  bool write_ok = writeStringToSD(str, file_name);
  updateStatusLED(SD_PIXEL, write_ok);
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
