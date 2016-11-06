#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;       //Use this for the 16-bit version

int16_t adc0, adc1;
double v0, v1;  

void setup(void) 
{
  Serial.begin(9600);
  
  Serial.println("Getting single-ended readings from AIN0");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 0.1875mV/ADS1115)");
  Serial.println("Time (sec), Voltage 1 (mV), Voltage 2 (mV)");
  
  ads.begin();
  //pinMode(pin, OUTPUT);
}

void loop(void) 
{
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  v0 = adc0 * 0.1875;
  v1 = adc1 * 0.1875;
  Serial.print(millis()/1000);Serial.print(", ");
  Serial.print(v0);      Serial.print(", ");
  Serial.print(v1);    Serial.print(", \n");
  delay(1000);
}
