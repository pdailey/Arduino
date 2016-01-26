#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include "Adafruit_MCP9808.h" // I2C Temperature Sensor

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 1);
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();


// Light Sensor
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}


void setup(){
  Serial.begin();
  
  String beanName = "Bean1";
  Bean.setBeanName(beanName);
  
  /* Setup the sensor gain and integration time */
  configureSensor();
}
 
void loop(){
  // Wait a moment for capacitance effects to settle
  delay(1000);
  
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
   /* If event.light = 0 lux the sensor is possibly saturated
       and no reliable data could be generated! */
  Serial.print(event.light); 
  Serial.print(" lux, ");
 
  
  // Get the current ambient temperature in degrees Celsius with a range of -40 C to 87 C.
   tempsensor.shutdown_wake(0);   // Don't remove this line! required before reading temp
  float c = tempsensor.readTempC(); // Read and print out the temperature
  //Serial.print(stringToPrint);
  Serial.print(c);
  Serial.print(" C, ");
  
  // Returns the voltage with conversion of 0.01 V/unit
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
