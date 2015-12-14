/* 
  This sketch shows you how to change the name of your Bean depending on the ambient temperature.
  
  If the name doesn't change, try turning your device's Bluetooth off and on to clear the cache. 
  
  This example code is in the public domain.
*/

void setup() {
}

void loop()
{
 String beanName = "TemperatureSensor";
 Bean.setBeanName(beanName);
 // Sleep for a minute 
 Bean.sleep(60000);
}
