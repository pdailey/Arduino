#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <DistanceSRF04.h>          // Handles the distance sensor
#include "Adafruit_LEDBackpack.h"   // Handles the display
#include "Adafruit_GFX.h" 

//BEAN
//#include <PinChangeInt.h> // Handles pin change interrupts

int interrupt_pin = 2;
int echo_pin = 4;
int trigger_pin = 3;

volatile boolean flag;

// Sensor Values
DistanceSRF04 Dist;
unsigned int distance;

Adafruit_7segment disp = Adafruit_7segment(); // Sets up the 7-segment display

void setup() {
  // Rename the bean for node-red sketch
  //String beanName = "BeanX";
  //Bean.setBeanName(beanName);
  
  disp.begin(0x70); // Initialize the 7-segment display

  Dist.begin(echo_pin, trigger_pin); //echo_pin, trigger_pin

  //BEAN run pinChanged when interruptPin is changed
  //pinMode(0, INPUT_PULLUP); // set digital pin number
  //attachPinChangeInterrupt(0, pinChanged, CHANGE);
  // make sure to check which pin is the interrupt and what pin number it is. 
  pinMode(interrupt_pin, INPUT_PULLUP);
  attachInterrupt(0, pinChanged, FALLING);
  Serial.begin(9600);
}


void loop() {
  Serial.print("Loop De Loop\n");
  delay(1000);

  if(flag == true){
       //  print a floating point
    //disp.print(77, DEC);
    //disp.writeDisplay();
    //Bean.sleep(10000);
    Serial.print("Button Pressed");
    distance = Dist.getDistanceCentimeter();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm\n");
    delay(1000);
    
    flag = false;
  }
}


// Interrupt service routine (ISR) needs to return void and accept no arguments
void pinChanged() {
  flag = true;
}
