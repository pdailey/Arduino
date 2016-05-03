#include <Button.h>

#define BAUD 57600
#define BUTTON_PIN 2       //Connect a tactile button switch (or something similar)
                           //from Arduino pin 2 to ground.
#define PULLUP true        //To keep things simple, we use the Arduino's internal pullup resistor.
#define INVERT true        //Since the pullup resistor will keep the pin high unless the
                           //switch is closed, this is negative logic, i.e. a high state
                           //means the button is NOT pressed. (Assuming a normally open switch.)
#define DEBOUNCE_MS 20     //A debounce time of 20 milliseconds usually works well for tactile button switches.
#define LED_PIN 13         //The standard Arduino "Pin 13" LED

Button myBtn(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button
boolean ledState;          //A variable that keeps the current LED status

void setup (){
  pinMode(LED_PIN, OUTPUT);    //Set the LED pin as an output
  
  Serial.begin(BAUD);
  Serial1.begin(BAUD);
  // DEBUGGING
  while(!Serial){}
  
  Serial.println("Waiting for button press...");
}

void loop (){  
  myBtn.read();                    //Read the button

  if (myBtn.wasReleased()) {       //If the button was released, change the LED state
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.println("Triggered!");
  }
} 
