#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

I2CSoilMoistureSensor sensor; // I2C Soil Sensor
static uint8_t powerPin = 5; // will be used to power soil sensor. In series with 470 Ohm Resistor


void setup() {
    // Rename the bean for node-red sketch
  String beanName = "SoilBean";
  Bean.setBeanName(beanName);

  Wire.begin();
  Serial.begin();

  pinMode(powerPin, OUTPUT);
  
  // Start Soil Sensor
  sensor.begin(); // reset sensor
}

void loop(){
    digitalWrite(powerPin, HIGH); // Power up the Moisture Sensor
    delay(1000); // Wait a moment for capacitance effects to settle

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
    digitalWrite(powerPin, LOW);
}
