#define RELAY_ON 0
#define RELAY_OFF 1


void setup() {
  // put your setup code here, to run once:

  pinMode(9, OUTPUT); // Set the pin modes
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(9, RELAY_OFF); // Set the pin modes
  digitalWrite(10, RELAY_OFF);
  digitalWrite(11, RELAY_OFF);
  digitalWrite(12, RELAY_OFF);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(9, RELAY_ON);
  delay(10000);
  digitalWrite(10, RELAY_ON );
  delay(10000);
  digitalWrite(11, RELAY_ON );
  delay(10000);
  digitalWrite(12, RELAY_ON );
  delay(10000);
  digitalWrite(9, RELAY_OFF);
  digitalWrite(10, RELAY_OFF);
  digitalWrite(11, RELAY_OFF);
  digitalWrite(12, RELAY_OFF);
}
