
const byte pirPin = 2; // Input for HC-S501
bool mvmt; // Place to store read PIR Value
bool lastMvmt;

void setup() {
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(pirPin, INPUT);
 
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  mvmt = digitalRead(pirPin);
  //digitalWrite(BUILTIN_LED, !pirVal);

  if(mvmt != lastMvmt){
    Serial.println(mvmt);    
    digitalWrite(BUILTIN_LED, !mvmt);
    
    lastMvmt = mvmt;
  }
}
