#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

I2CSoilMoistureSensor sensor; // I2C Soil Sensor

void setup() {
  // Configure Bean to wake up when a client connects
  Bean.enableWakeOnConnect(true);
  
  Wire.begin();
  Serial.begin();

  // Rename the bean for node-red sketch
  String beanName = "Bean2";
  Bean.setBeanName(beanName);
  
  // Start Soil Sensor
  sensor.begin(); // reset sensor
}


// Loop runs once at program start.
// When a client connects, Bean wakes up and runs loop again.
// Loop continues to run as long as a client is connected.
void loop(){
  bool connected = Bean.getConnectionState();
  // Wait a moment for capacitance effects to settle
  if (connected) {
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
  //Bean.sleep(900000); // 15 min
  //Bean.sleep(10000);

  } else {
    // Client disconnected
    // This sketch runs only when a client is connected.
    // This might be useful for interactive installations, where you don't want
    // Bean to spend any power unless someone is actively connected.

    // Sleep for a long time to conserve power.
    // 0xFFFFFFFF = 4,294,967,295 milliseconds = 49 days
    Bean.sleep(0xFFFFFFFF);
  }
}
