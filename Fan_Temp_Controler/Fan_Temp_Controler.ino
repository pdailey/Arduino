byte fanPin = 3;

byte pwmValue;

double currentTemp;
double desiredTemp = 27;

int i;

//Code written in setup() for pin configuration
//setup tempPin, fanPin, and Serial.begin
void setup()
{
  Serial.begin(9600);
  
  pinMode(fanPin, OUTPUT);
}

// Begin loop to run continous
void loop()
{  
  // Check if temp is above or below desired temperature
  // If measured Temp is less than desired temp set fan to off.
  if(readTemp() > desiredTemp)
    pwmValue = 225;
  else
    pwmValue = 0;
  analogWrite(fanPin, pwmValue);
  
  if(i==10){
    Serial.print("The Fan is currently at ");
    Serial.print(pwmValue * 100/225);
    Serial.print(" %. \n");
    Serial.print("The current temperature is ");
    Serial.print(readTemp());
    Serial.print(" *C \n\n");
    i=0;
  }
  
  i++;
  delay(100);
}

// User defined function readTemp() to take analogRead(tempPin), convert
// analog (0-1023) to temperature (C) and return value.
double readTemp()
{
  // declare local int measurement and take reading
  int sensorValue= analogRead(A0);
  // declare local double and cast int to double
  double voltage = sensorValue * (5.0 / 1023.0);
  // LM35 Conversion Factor: 10-mV/°C
  // convert analog measurement to deg C.
  double temp = voltage * (1 / 0.01);


  // return temp measured
  return (temp);
   
}
