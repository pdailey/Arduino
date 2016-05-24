/*
   Set the RTC

   Upload this program to set and test the RTC Shield.

   Info on the  DS1307 RTC:  It can count leap-years and knows how many days are in
   a month, but it doesn't take care of Daylight Savings Time
*/
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

void setup () {
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
  }

  // The following line sets the RTC to the date & time this sketch was compiled
  RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop () {
  DateTime now = RTC.now();

  Serial.print("The current time is: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // wait 10 sec
  delay(10000);
}
