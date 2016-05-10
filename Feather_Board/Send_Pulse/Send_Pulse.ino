#include <Button.h>

#define BAUD 57600
#define PULSE_PIN 11        //send pulses from here
#define LED_PIN 13         //The standard Arduino "Pin 13" LED
#define nth 5    // Number of counts until pulse

byte count = 0;


void setup (){
  pinMode(LED_PIN, OUTPUT);    
  pinMode(PULSE_PIN, OUTPUT); 
  
  Serial.begin(BAUD);
  
  // DEBUGGING
  //while(!Serial){}
}


void loop (){  
  if(count%nth == 0){
    Serial.println("You've dones this for the nth time!");
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(PULSE_PIN, HIGH);
    // reset the counter
    count = 0;
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(PULSE_PIN, HIGH);
  }
  
  count++;
  delay(1000);
} 
