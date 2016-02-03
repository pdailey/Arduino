#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.

//#include "Adafruit_LEDBackpack.h"  // Handles the display
//#include "Adafruit_GFX.h" 

#include <PinChangeInt.h> // Handles pin change interrupts

//Adafruit_7segment disp = Adafruit_7segment(); // Sets up the 7-segment display

// set the interrupt pin at D4
int interruptPin = 4;

void setup() {
  // Rename the bean for node-red sketch
  String beanName = "BeanX";
  Bean.setBeanName(beanName);
  
  //disp.begin(0x70); // Initialize the 7-segment display

  pinMode(interruptPin, INPUT_PULLUP);
   // run pinChanged when something changes on D0 
   // This is an external library, so you don't need the prefix "Bean"
   attachPinChangeInterrupt(interruptPin, pinChanged, CHANGE);

  // Configure Bean to wake up when a client connects
  Bean.enableWakeOnConnect(true);
}

int counter = 0;


// Loop runs once at program start.
// When a client connects, Bean wakes up and runs loop again.
// Loop continues to run as long as a client is connected.
void loop() {
  // Check if client has disconnected
  bool connected = Bean.getConnectionState();

  // Sleep all the time when not handling an interrupt
  if (connected && counter == 0) {
    int c = Bean.getTemperature();
    Serial.print(c);
    Serial.print(" C, ");
    Bean.setLed(0, 255, 0);
    
    //// VOLATAGE SENSOR /////
    // Returns the voltage with conversion of 0.01 V/unit
    // Used to determine if sensor valeus above are valid
    uint16_t batteryReading =  Bean.getBatteryVoltage(); 
    // Format the output like "Battery voltage: 2.60 V"
    Serial.print(batteryReading/100);
    Serial.print(".");
    Serial.print(batteryReading%100);
    Serial.print(" V");
  
    Serial.print("\n"); //necessary for node-red to recognize that this is the end of input
    counter = 1;
  } else {
    Bean.setLed(0, 0, 0);
    // Client disconnected: 
    // Sleep for a long time to conserve power.
    // 0xFFFFFFFF = 4,294,967,295 milliseconds = 49 days
    counter = 0;
    Bean.sleep(0xFFFFFFFF);
  }
}


// Interrupt service routine (ISR) needs to return void and accept no arguments
void pinChanged() {
  // print a floating point
  //disp.print(25);
  //disp.writeDisplay();
  Serial.print("Button Pressed");
  //disp.writeDigitRaw(1,B00000001);
  // will display nothing
  //disp.writeDisplay();
}
