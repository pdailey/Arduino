void setup(){
Serial.begin();
}
 
void loop(){
Serial.print("Hi I'm a Bean!");
Serial.print("\n");
Bean.sleep(10000);
}
