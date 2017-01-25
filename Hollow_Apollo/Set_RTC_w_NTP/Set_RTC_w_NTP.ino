/*
 * Debugging script used to set the RTC time using 
 * Network Time Protocol (NTP). Time is retrieved from 
 * a pool of NTP servers, and the RTC is adjusted to match 
 * the retreived time. 
 * 
 * Requires a wifi connection to function.
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "RTClib.h"

RTC_PCF8523 rtc;

// WiFi
const char* ssid = "Hollow_Apollo";
const char* password =  "liftoff54321";

// NTP
unsigned int ntp_port = 2390;      // local port to listen for UDP packets
/* Don't hardwire the IP address or we won't get the benefits of the pool.
    Lookup the IP address for the host name instead */
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const byte NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP

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

bool setupUDP() {
  Serial.println("Starting UDP");
  Udp.begin(ntp_port);
  Serial.print("NTP port: ");
  Serial.println(Udp.localPort());
  return true;
}

void setup() {
  Serial.begin(9600);
  delay(5000);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
  if ( ! rtc.initialized() ) {
    // It is Fine ine if the RTC is not initialized, 
    // the RTC will be set in the main loop
    Serial.println("RTC Not Configured");
  }

  if (setupWifi) {
    setupUDP();
  }
}

void loop() {
  adjustRTCusingNTP();
  // wait thirty seconds before asking for the time again
  delay(30000);
}

bool adjustRTCusingNTP() {
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  //get the NTP timestamp
  unsigned long epoch = getNTP();

  //set the RTC
  rtc.adjust(epoch);
  return true;
}

unsigned long getNTP() {
  sendNTPpacket(timeServerIP); // send an NTP packet to a time server

  // wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() ) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    Serial.println(epoch);
    return epoch;
  }
}

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
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
