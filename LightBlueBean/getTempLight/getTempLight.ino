#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 1);

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

  /* Setup the sensor gain and integration time */
  configureSensor();
}

//int seconds = 1000;
//int minutes = 60*seconds; 
//int sleepTime = 10*minutes; 

 
void loop(){
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  if (event.light)
  {
    Serial.print(event.light); 
    Serial.print(" lux, ");
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.print("Sensor overload, ");
  }
  
  // Returns the voltage with conversion of 0.01 V/unit
  uint16_t batteryReading =  Bean.getBatteryVoltage(); 
  
  // Get the current ambient temperature in degrees Celsius with a range of -40 C to 87 C.
  int temperature = Bean.getTemperature();
 
  // Format the output like "Battery voltage: 2.60 V"
  Serial.print(batteryReading/100);
  Serial.print(".");
  Serial.print(batteryReading%100);
  Serial.print(" V, ");
  
  //Serial.print(stringToPrint);
  Serial.print(temperature);
  Serial.print(" C");
  Serial.print("\n"); //necessary for node-red to recognize that this is the end of input
  Bean.sleep(10000);
}
