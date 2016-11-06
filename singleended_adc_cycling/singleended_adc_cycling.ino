#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;       //Use this for the 16-bit version


byte pin = 9;
byte brightness = 0;

int16_t adc0;
double voltage;   

void setup(void) 
{
  Serial.begin(9600);
  
  Serial.println("Getting single-ended readings from AIN0");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 0.1875mV/ADS1115)");
  
  ads.begin();
  pinMode(pin, OUTPUT);
}

void loop(void) 
{

  analogWrite(pin, brightness++);
  delay(500);
  
  
  adc0 = ads.readADC_SingleEnded(0);
  voltage = adc0 * 0.0001875;
  Serial.print(brightness); Serial.print(", ");
  Serial.print(adc0);       Serial.print(", ");
  Serial.print(voltage);    Serial.print(", \n");

  delay(500);
}
