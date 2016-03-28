#include <DistanceSRF04.h>  // Using Arduino Distance Library
#include <Wire.h>

#define  SLAVE_ADDRESS      0x29  //slave address,any number from 0x01 to 0x7F
#define  REG_MAP_SIZE       6     //size of array ot hold all register data 
#define  MAX_SENT_BYTES     3     // maximum bytes that can be recieved from master 
#define  INTERRUPT_PIN      2
#define  IDENTIFICATION     5
/*three bytes is the most amount of data that would ever need to be sent from the master.  
The first byte sent from the master is always the register address.  For our device, we only 
have two writable registers in our map and they are the configuration register and mode register.  
The rest of the registers are read only so they can’t be written to.
*/

/********* Global  Variables  ***********/
byte registerMap[REG_MAP_SIZE];
byte registerMapTemp[REG_MAP_SIZE - 1];
byte receivedCommands[MAX_SENT_BYTES];

byte useInterrupt = 0; // We are not using interrupt pins

byte newDataAvailable;
byte sensorStatus;
byte modeRegister;
byte configRegister;

byte zeroB;
byte zeroC;
byte zeroBData;
byte zeroCData;

// Sensor Values
DistanceSRF04 Dist;
unsigned int distance;



void setup()
{
  if(useInterrupt){
    pinMode(INTERRUPT_PIN,OUTPUT);
    digitalWrite(INTERRUPT_PIN,HIGH);
  }
  
  Wire.begin(SLAVE_ADDRESS); 
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  registerMapTemp[5] = IDENTIFICATION; // ID register


  
  Serial.begin(9600);
  Dist.begin(2,3); //echo_pin, trigger_pin
}


void loop()
{
  if(zeroB || zeroC){ // Add this loop between every colection of data
    newDataAvailable = 0;  //let the master know we don’t have any post correction data yet
    toggleInterrupt();
    changeModeConfig();  //call the function to make your changes
    return; //go back to the beginning and start collecting data from scratch
  }
  distance = Dist.getDistanceCentimeter();
  
  if(zeroB || zeroC){ // Add this loop between every colection of data
    newDataAvailable = 0;  //let the master know we don’t have any post correction data yet
    toggleInterrupt();
    changeModeConfig();  //call the function to make your changes
    return; //go back to the beginning and start collecting data from scratch
  }
  sensorStatus = getSignalStatus();     //returns status as a byte
  
  storeData();
  newDataAvailable = 1;
  toggleInterrupt();
  
  //Serial.print("\nDistance in centimers: ");
  //Serial.print(distance);
  delay(500);                                     //make it readable
}

void storeData(){
  byte * bytePointer;  //we declare a pointer as type byte
  byte arrayIndex = 1; //we need to keep track of where we are storing data in the array
  
  registerMapTemp[0] = sensorStatus;  //no need to use a pointer for Status
  bytePointer = (byte*)&distance;     //distance is 2 bytes

  for (int i = 1; i > -1; i--){
    registerMapTemp[arrayIndex] = bytePointer[i];  //increment pointer to store each byte
    arrayIndex++;
  }

  // Not used for this configuration, but left for future functionality
  registerMapTemp[arrayIndex] = modeRegister;
  arrayIndex++;
  registerMapTemp[arrayIndex] = configRegister;
  Serial.print("\n Register Map Temp: ");
  for(int n=0; n<6; n++){
    Serial.print(registerMapTemp[n]);
    Serial.print("-");
  }
}

void requestEvent(){
  if(newDataAvailable){
    for(int i = 0; i < REG_MAP_SIZE - 1; i++){
      registerMap[i] = registerMapTemp[i];
    }
  }

  newDataAvailable = 0;
  toggleInterrupt();
  
  //Set the buffer up to send all bytes of data
  Wire.write(registerMap+receivedCommands[0], REG_MAP_SIZE); 
}



void receiveEvent(int bytesReceived){
  for (int a = 0; a < bytesReceived; a++){
    if ( a < MAX_SENT_BYTES){
     receivedCommands[a] = Wire.read();
    }
    else{
      Wire.read();  // if we receive more data then allowed just throw it away
    }
  }

  if(bytesReceived == 1 && (receivedCommands[0] < REG_MAP_SIZE)){
    return; 
  }
  
  if(bytesReceived == 1 && (receivedCommands[0] >= REG_MAP_SIZE)){
    receivedCommands[0] = 0x00; //read only, so essentially rejected
    return;
  }
  
  switch(receivedCommands[0]){
    case 0x03: // TODO: set to MODE Register
     zeroB = 1; //status flag to let us know we have new data in register 0x0B
     zeroBData = receivedCommands[1]; //save the data to a separate variable
     bytesReceived--;
     
     if(bytesReceived == 1) { //only two bytes total were sent so we’re done
      return;
     }
     zeroC = 1;
     zeroCData = receivedCommands[2];
     return; //we simply return here because the most bytes we can receive is three anyway
     break;
  
    case 0x04:// TODO: set to CONFIG Register
      zeroC = 1;
      zeroCData = receivedCommands[1];
      return; //we simply return here because 0x04 is the last writable register
      break;
    default:
      return; // ignore the commands and return
  }
}


byte getSignalStatus(){
  return 1;  
}


void changeModeConfig(){
  /*Put your code here to evaluate which of the registers need changing
   And how to make the changes to the given device. 
   It could be issuing the commands to change the baud rate, update rate,
   Datum, etc…
   We just put some basic code below to copy the data straight to the registers*/
  
  if(zeroB){
    modeRegister = zeroBData;
    zeroB = 0;  //always make sure to reset the flags before returning from the function
    zeroBData = 0;
  }
  if(zeroC){
    configRegister = zeroCData;
    zeroC = 0;  //always make sure to reset the flags before returning from the function
    zeroCData = 0;
  }
}

void toggleInterrupt(){
  if(!useInterrupt)
  {return;} //first let’s make sure we’re using interrupts, if not just return from the function
  
  if(newDataAvailable){  // if new data is available set the interrupt low  
    digitalWrite(INTERRUPT_PIN,LOW);  //set pin low and return
    return;
  }

  //no new data available or data was just read so set interrupt pin high
  digitalWrite(INTERRUPT_PIN,HIGH); 
}

