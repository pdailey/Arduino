#include <AnalogButtons.h>


// A call back function that you pass into the constructor of AnalogButtons, see example
// below. Alternativly you could extend the Button class and re-define the methods pressed() 
// or held() which are called 
void b1Click()
{  
    Serial.print("button 1 clicked"); 
}

void b2Click()
{  
    Serial.print("button 2 clicked"); 
}
void b2Hold()
{  
    Serial.print("button 2 held"); 
}

void b3Click()
{  
    Serial.print("button 3 clicked"); 
}
void b3Hold()
{  
    Serial.print("button 3 held"); 
}

void b4Click()
{  
    Serial.print("button 4 clicked"); 
}
void b4Hold()
{  
    Serial.print("button 4 held"); 
}

void b5Click()
{  
    Serial.print("button 5 clicked"); 
}
void b5Hold()
{  
    Serial.print("button 5 held"); 
}

AnalogButtons analogButtons(A0, INPUT);
Button b1 = Button(931, &b1Click);
Button b2 = Button(904, &b2Click);
Button b3 = Button(856, &b3Click);
Button b4 = Button(816, &b4Click);
Button b5 = Button(614, &b5Click);

void setup() 
{
  Serial.begin(9600); 
  Serial.println("Testing your Analog buttons");
  
  analogButtons.add(b1);
  analogButtons.add(b2);
  analogButtons.add(b3);
  analogButtons.add(b4);
  analogButtons.add(b5);  
 }
 
void loop() 
{  
  // To check values when button are pressed
  analogButtons.check();
  
  // To configure the MAX/Min values for each button: 
  // uncomment the following line and make sure you've called Serial.begin(9600) 
  // Then in turn hold down each button, noting down the max/min values
  
  //configure();
}

void configure() {
	unsigned int value = analogRead(A0);
	Serial.println(value);
	delay(250);
}
