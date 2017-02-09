/* 
 *  Script used to initialize an RTC, set the RTC time and check 
 *  that the time is accurate via Serial output
 */


#include <Wire.h>
#include "RTClib.h"

RTC_PCF8523 rtc;

void setup () {
  Serial.begin(9600);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  } else {
    Serial.println("RTC is running");
  }
}

void loop () {
    DateTime now = rtc.now();
    String _date = String(now.year(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.day(), DEC);
    String _time = String(now.hour(), DEC) + ":" + String(now.minute(), DEC) + ":" + String(now.second(), DEC);
    Serial.println(_date);
    Serial.println(_time);
    
    
    delay(10000);
}
