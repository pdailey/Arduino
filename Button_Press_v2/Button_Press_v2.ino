#include <Button.h>

#define BAUD 57600
#define BUTTON_PIN 2       //Connect a tactile button switch (or something similar)
                           //from Arduino pin 2 to ground.
#define PULSE_PIN 11        //send pulses here
#define READ_PIN 9         //read pulses here


#define PULLUP true        //To keep things simple, we use the Arduino's internal pullup resistor.
#define INVERT true        //Since the pullup resistor will keep the pin high unless the
                           //switch is closed, this is negative logic, i.e. a high state
                           //means the button is NOT pressed. (Assuming a normally open switch.)
#define DEBOUNCE_MS 20     //A debounce time of 20 milliseconds usually works well for tactile button switches.
#define LED_PIN 13         //The standard Arduino "Pin 13" LED

#define nth 10    // Number of counts until pulse
byte count = 0;

Button myBtn(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button
Button pulse(READ_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button

boolean ledState;          //A variable that keeps the current LED status
boolean send_pulse;         //A variable that keeps the current send pin 


void setup (){
  pinMode(LED_PIN, OUTPUT);    
  pinMode(PULSE_PIN, OUTPUT); 
  
  Serial.begin(BAUD);
  Serial1.begin(BAUD);
  
  // DEBUGGING
  while(!Serial){}
}

void loop (){  
  if(count%nth == 0){
    //Serial.println("You've dones this for the nth time!");
    digitalWrite(PULSE_PIN, HIGH);
    
    // reset the counter
    count = 0;
  } else {
    digitalWrite(PULSE_PIN, LOW);
  }

  //Read the buttons
  myBtn.read();                  
  pulse.read();  

  if (myBtn.wasReleased()) {       //If the button was released, change the LED state
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.println("TRIGGERED!!!");
  }

  if (pulse.wasReleased()) {       //If the button was released, change the LED state
    Serial.println("You've dones this for the nth time!");
  } else {
    Serial.println(count);
  }

  count++;
  
} 
