#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h> // I2C Light Sensor
#include "Adafruit_MCP9808.h" // I2C Temperature Sensor (default I2C address of 0x18)

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 1);
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();


// Light Sensor
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
}


void setup(){
  Serial.begin();

  // Rename the bean for node-red sketch
  String beanName = "Bean1";
  Bean.setBeanName(beanName);
  
  
  /* Setup the LUX sensor gain and integration time */
  configureSensor();
  
  /* Setup the Temperature sensor */
  tempsensor.begin();
}
 
void loop(){
  // Wait a moment for capacitance effects to settle
  delay(1000);

  //// LIGHT SENSOR /////
  // Get a new sensor event
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  Serial.print(event.light); 
  Serial.print(" lux, ");
 

  //// TEMPERATURE SENSOR /////
  // Get the current ambient temperature in degrees Celsius with a range of -40 C to 87 C.
  tempsensor.shutdown_wake(0);   // Don't remove this line! required before reading temp
  float c = tempsensor.readTempC(); // Read and print out the temperature
  Serial.print(c);
  Serial.print(" C, ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere

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
