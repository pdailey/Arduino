#include <DistanceSRF04.h>  // Using Arduino Distance Library
#include <Wire.h>

#define  SLAVE_ADDRESS    0x29  //slave address,any number from 0x01 to 0x7F
#define  REG_MAP_SIZE     6     //size of array ot hold all register data 
#define  MAX_SENT_BYTES   3     // maximum bytes that can be recieved from master 
/*three bytes is the most amount of data that would ever need to be sent from the master.  
The first byte sent from the master is always the register address.  For our device, we only 
have two writable registers in our map and they are the configuration register and mode register.  
The rest of the registers are read only so they can’t be written to.
*/

/********* Global  Variables  ***********/
byte registerMap[REG_MAP_SIZE];
byte registerMapTemp[REG_MAP_SIZE - 1];
byte receivedCommands[MAX_SENT_BYTES];

byte newDataAvailable;
byte SensorStatus;
byte modeRegister;
byte configRegister;

DistanceSRF04 Dist;
unsigned int distance;



void setup()
{
  Wire.begin(SLAVE_ADDRESS); 
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);


  
  Serial.begin(9600);
  Dist.begin(2,3); //echo_pin, trigger_pin
}

void loop()
{
  distance = Dist.getDistanceCentimeter();
  storeData();
  newDataAvailable = 1;
  
  Serial.print("\nDistance in centimers: ");
  Serial.print(distance);
  delay(500);                                     //make it readable
}

void requestEvent(){
  Wire.write(registerMap, REG_MAP_SIZE);  //Set the buffer up to send all 14 bytes of data
}

void receiveEvent(int bytesReceived){
  for (int a = 0; a < bytesReceived; a++){
    if ( a < MAX_SENT_BYTES){
      receivedCommands[a] = Wire.read();
    } 
    else {
      Wire.read();  // if we receive more data then allowed just throw it away
    }
  }
}



void storeData(){
  byte * bytePointer;  //we declare a pointer as type byte
  byte arrayIndex = 1; //we need to keep track of where we are storing data in the array
  
  registerMapTemp[0] = SensorStatus;  //no need to use a pointer for Status
  bytePointer = (byte*)&distance;     //distance is 2 bytes

  for (int i = 1; i > -1; i--){
    registerMapTemp[arrayIndex] = bytePointer[i];  //increment pointer to store each byte
    arrayIndex++;
  }

  // Not used for this configuration, but left for future functionality
  registerMapTemp[arrayIndex] = modeRegister;
  arrayIndex++;
  registerMapTemp[arrayIndex] = configRegister;
  Serial.print("Register Map Temp: ");
  for(int n=0; n<6; n++){
    Serial.print(registerMapTemp[n]);
    Serial.print("-");
  }
}
