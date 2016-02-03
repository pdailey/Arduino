#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

I2CSoilMoistureSensor sensor; // I2C Soil Sensor
static uint8_t powerPin = 5; // will be used to power soil sensor. In series with 470 Ohm Resistor


void setup() {
    // Rename the bean for node-red sketch
  String beanName = "Bean2";
  Bean.setBeanName(beanName);

  Wire.begin();
  Serial.begin();

  pinMode(powerPin, OUTPUT);
  
  // Start Soil Sensor
  sensor.begin(); // reset sensor
  
  // Configure Bean to wake up when a client connects
  Bean.enableWakeOnConnect(true);
}

int counter = 0;

// Loop runs once at program start.
// When a client connects, Bean wakes up and runs loop again.
// Loop continues to run as long as a client is connected.
void loop(){
  bool connected = Bean.getConnectionState();
  // Check if client has disconnected
  if (connected && counter == 0) {
    digitalWrite(powerPin, HIGH);
    
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
    counter = 1;
  } else {
    // Client disconnected
    // Sleep for a long time to conserve power.
    // 0xFFFFFFFF = 4,294,967,295 milliseconds = 49 days
    counter = 0;
    Bean.sleep(0xFFFFFFFF);
  }
}
