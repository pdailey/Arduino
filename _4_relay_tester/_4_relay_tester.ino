#define RELAY_ON 0
#define RELAY_OFF 1


void setup() {
  // put your setup code here, to run once:

const byte pin_h1 = 9;
const byte pin_h2 = 10;
const byte pin_h3 = 11;
const byte pin_h4 = 12;

pinMode(pin_h1, OUTPUT); // Set the pin modes
pinMode(pin_h2, OUTPUT);
pinMode(pin_h3, OUTPUT);
pinMode(pin_h4, OUTPUT);

digitalWrite(pin_h1, RELAY_OFF); // Set the pin modes
digitalWrite(pin_h2, RELAY_OFF);
digitalWrite(pin_h3, RELAY_OFF);
digitalWrite(pin_h4, RELAY_OFF);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pin_h1, RELAY_ON);
  delay(10000);
  digitalWrite(pin_h2, RELAY_ON );
  delay(10000);
  digitalWrite(pin_h3, RELAY_ON );
  delay(10000);
  digitalWrite(pin_h4, RELAY_ON );
}
