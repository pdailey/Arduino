
/*
  LiquidCrystal Library - Hello World

  This sketch prints "Hello World!" to the LCD.
  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  All of the buttons have a different value resistor tied to ground and analog pin A0, which should give you a different value if you analogRead() pin A0. This keeps the analog pin usage to a minimum, as well as allows you to push more than one button at a time (which may not be that useful with this button arrangement). You can also set the LED backlight brightness by writing via analogWrite() to pin 10 a value of 0 to 255 (this outputs a PWM signal at a duty cycle defined by your input value).

  http://www.arduino.cc/en/Tutorial/LiquidCrystal

*/

#define Rbase ((unsigned long)47)
#define Rup ((unsigned long)75)
#define Rdown  ((unsigned long)62)
#define Rleft  ((unsigned long)47)
#define Rright ((unsigned long)39)
#define Rselect  ((unsigned long)28)

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int sensorValue ;
int KeyTable[5];

void GenerateKeyTable(int vcc, int* array)
{
  float resistor;

  //////////////1key//////////////////////
  resistor = ((float)Rup) / (Rbase + Rup);
  *array++ = resistor * vcc;

  resistor = ((float)Rdown) / (Rbase + Rdown);
  *array++ = resistor * vcc;

  resistor = ((float)Rleft) / (Rbase + Rleft);
  *array++ = resistor * vcc;

  resistor = ((float)Rright) / (Rbase + Rright);
  *array++ = resistor * vcc;

  resistor = ((float)Rselect) / (Rbase + Rselect);
  *array++ = resistor * vcc;
}

unsigned char GetKey(int value)
{
  char tmpChar;
  unsigned int Rst;

  tmpChar = 0;
  do {
    if (value > KeyTable[tmpChar]) Rst = value - KeyTable[tmpChar];
    else  Rst = KeyTable[tmpChar] - value;
    tmpChar ++;
  } while (Rst > 1);

  return tmpChar--;

}

void setup()
{
  int tmpInt;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  sensorValue  = 1023;
  Serial.begin(9600);

  GenerateKeyTable(analogRead(A0), KeyTable);
  for (tmpInt = 0; tmpInt < 5; tmpInt++)
  {
    Serial.println(KeyTable[tmpInt]);
  }
}

void loop()
{
  unsigned char key;

  if (sensorValue != analogRead(A0))
  {
    sensorValue = analogRead(A0);
    lcd.setCursor(0, 1);
    lcd.print(sensorValue);
    key = GetKey(sensorValue);
    if (key < 31)
    {
      lcd.print("---");
      lcd.print(key);
      lcd.print(' ');
    }
    else  lcd.print("        ");

  }
}
