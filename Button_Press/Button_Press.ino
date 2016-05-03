const byte interruptPin = 2;
#define BAUD 57600

/*volitle tells the compiler that such variables might change at any time, 
 * and thus the compiler must reload the variable whenever you reference 
 * it, rather than relying upon a copy it might have in a processor register.
 */
volatile boolean flag = false;

boolean looped = false;
int count = 0;

void setup ()
{
  Serial.begin(BAUD);
  Serial1.begin(BAUD);
  // DEBUGGING

  while(!Serial){}
  
  Serial.println("Waiting for button press...");
  
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(interruptPin, switchPressed, LOW);
  delay(1000);
  flag=false;

  // DO NOT REMOVE. Arduino always egisters an initial button press due to RC effects
  while(flag==false){
  }
  flag = false;
  detachInterrupt(interruptPin);

}

void loop ()
{  
  if(looped==false){
    Serial.println("TRIGGERED!");
    Serial.print("Seconds since start: ");
    Serial.println(millis()/1000);
    looped = true;
  }
  
  if(flag) {
    count++;
    String str1 = "Button Pressed: ";
    String str2 = str1 + count;
    Serial.println(str2);
    flag = false;
  }
} 

// Interrupt Service Routine (ISR)
void switchPressed (){
  flag = true;
} 
