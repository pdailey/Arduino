//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// EME 185 - Fill Gauge based on Potentiometer
// 
// Made by Peter Dailey
// License: Public Domain
// Downloaded from: https://123d.circuits.io/circuits/1620069-eme-185-fill-gauge-based-on-potentiometer

float v_table[10] = {.983, .852, .737, .631, .531, .437, .257, .171, .085};

void setup() {
  for(int i=0; i<10; i++){
  	pinMode(i, OUTPUT);
  }
}

void loop() {
  int v_read = analogRead(A5);
  float voltage= v_read * (5.0 / 1023.0);
  Serial.println(voltage);
  for(int i=0; i<10; i++){
    if(voltage < v_table[i])
    	digitalWrite(i, LOW);
    else
    	digitalWrite(i, HIGH);
  } 
  delay(1000);
}
