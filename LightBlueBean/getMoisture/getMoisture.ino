#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

I2CSoilMoistureSensor sensor; // I2C Soil Sensor

void setup() {
  Wire.begin();
  Serial.begin();

  // Rename the bean for node-red sketch
  String beanName = "Bean2";
  Bean.setBeanName(beanName);
  
  // Start Soil Sensor
  sensor.begin(); // reset sensor
}

 
void loop(){
  // Wait a moment for capacitance effects to settle
  delay(1000);

  //// SOIL SENSOR /////
  // Returns the moisture level of the soil by capacitance
  Serial.print(sensor.getCapacitance()); //read capacitance register
  Serial.print(" Soil_Moisture_Capacitance, ");

  //// TEMPERATURE SENSOR /////
  Serial.print(sensor.getTemperature()/(float)10); //temperature register
  Serial.print(" C, ");
  
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
  Bean.sleep(900000); // 15 min
  //Bean.sleep(10000);
}
