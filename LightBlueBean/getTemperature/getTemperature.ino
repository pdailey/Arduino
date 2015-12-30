void setup(){
  Serial.begin();
}
 
void loop(){
  // Returns the voltage with conversion of 0.01 V/unit
  uint16_t batteryReading =  Bean.getBatteryVoltage(); 
  
  // Get the current ambient temperature in degrees Celsius with a range of -40 C to 87 C.
  int temperature = Bean.getTemperature();
 
  // Format the output like so: "Battery voltage: 2.60 V"
  Serial.print(batteryReading/100);
  Serial.print(".");
  Serial.print(batteryReading%100);
  Serial.print(" V, ");
  
  Serial.print(temperature);
  Serial.print(" C");
  Serial.print("\n"); //Necessary for node-red to recognize that this is the end of input
//  Bean.sleep(60000);  //Sleep for 1 Min
  Bean.sleep(10000); //Sleep for 10 Sec
}
