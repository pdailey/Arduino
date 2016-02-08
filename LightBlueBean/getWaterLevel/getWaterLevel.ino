#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.

//#include "Adafruit_LEDBackpack.h"  // Handles the display
//#include "Adafruit_GFX.h" 

#include <PinChangeInt.h> // Handles pin change interrupts

//Adafruit_7segment disp = Adafruit_7segment(); // Sets up the 7-segment display

void setup() {
  // Rename the bean for node-red sketch
  String beanName = "BeanX";
  Bean.setBeanName(beanName);
  
  //disp.begin(0x70); // Initialize the 7-segment display

  // run pinChanged when interruptPin is changed
  pinMode(0, INPUT_PULLUP); // set digital pin number
  attachPinChangeInterrupt(0, pinChanged, CHANGE);
}


void loop() {
  //disp.print(25);
  //disp.writeDisplay();
  Serial.print("Loop De Loop\n");

  Bean.sleep(10000);
}


// Interrupt service routine (ISR) needs to return void and accept no arguments
void pinChanged() {
//  print a floating point
  //disp.print(77);
  //disp.writeDisplay();
  Bean.sleep(10000);
  Serial.print("Button Pressed");
}
