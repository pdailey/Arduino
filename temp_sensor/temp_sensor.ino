const int pin = A5;  
int sensor_val;
double voltage;        
double temp;  

void setup() {
  // initialize the serial communications:
  Serial.begin(9600);
}

void loop() {
  // print the sensor values:
  sensor_val = analogRead(pin);
  Serial.println(sensor_val);
  voltage = sensor_val * (5.0 / 1023.0);
  Serial.println(voltage);
  temp = (voltage -1.25 ) / 0.005;
  Serial.print("Temperature is ");
  Serial.print(temp);
  Serial.print("*C\n");
  delay(1000);
}
