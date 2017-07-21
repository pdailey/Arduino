#define RELAY_ON 0
#define RELAY_OFF 1

const int no_chambers = 3;

// Pins atached to the relay
const int no_relays = 6;
const int starting_pin = 2;

// time entered in ms
// TODO: times will change
//const unsigned long ms_heat =  5000;
//const unsigned long ms_cool  = 45000;


long minutes = 60000;
const unsigned long ms_heat =  15 * minutes;
const unsigned long ms_cool  = 90 * minutes;

//const unsigned long ms_heat =  1 * minutes;
//const unsigned long ms_cool  = 5 * minutes;

const unsigned long ms_cool_adj = ms_cool - (ms_heat * no_chambers);


void setup() {
  Serial.begin( 9600 );
  setupRelays(no_relays, starting_pin);
}

void loop() {
  // run all heating cycles
  for (int i = 1; i < (no_chambers + 1); i++) {
    turnOnChamber(i);
    Serial.print(millis() / minutes);
    Serial.print("Chamber ");
    Serial.print(i);
    Serial.print(" ON");
    delay(ms_heat);
    allOff(no_relays, starting_pin);
  }
  // run cooling cycle
  Serial.print(millis() / minutes);
  Serial.print("Cooling for ");
  Serial.print(ms_cool_adj);
  Serial.print(" sec");
  delay(ms_cool_adj);
}

void setupRelays(int no_relays, int start) {
  for (int i = start; i < start + no_relays; i++) {
    digitalWrite( i, RELAY_OFF ); // Set the pin modes
    pinMode( i, OUTPUT );
    Serial.println(i);
  }
}

void allOff(int no_relays, int start) {
  for (int i = start; i < start + no_relays; i++) {
    digitalWrite( i, RELAY_OFF );
  }
  Serial.println("ALL OFF");
  delay(10);
}

// TODO: Remove hard coded values
void turnOnChamber(int chamber) {
  switch (chamber) {
    case 1:
      digitalWrite( 2, RELAY_ON );
      digitalWrite( 3, RELAY_ON );
      Serial.print(millis() / minutes);
      Serial.println("2,3 ON");
      break;
    case 2:
      digitalWrite( 4, RELAY_ON );
      digitalWrite( 5, RELAY_ON );
      Serial.print(millis() / minutes);
      Serial.println("4,5 ON");
      break;
    case 3:
      digitalWrite( 6, RELAY_ON );
      digitalWrite( 7, RELAY_ON );
      Serial.print(millis() / minutes);
      Serial.println("6,7 ON");
      break;
  }
}


