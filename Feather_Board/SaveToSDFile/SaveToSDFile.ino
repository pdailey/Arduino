#include <Button.h>
#include <LedFlasher.h>
#include <SPI.h>
#include <SD.h>


// Button Variables
const byte BUTTON_PIN = 2;       // Connect a tactile button switch (or something similar)

const boolean PULLUP = true;     // Use Arduino's internal pullup resistor.
const boolean INVERT = true;     // Pullup resistor will keep the pin high unless the button is pressed
const byte DEBOUNCE_MS = 20;     // A debounce time of 20 milliseconds works well for tactile button switches.
const byte LED_PIN = 13;         // The standard Arduino "Pin 13" LED

Button button(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);

// State Variables
boolean ledState = false;       // Keeps the current LED status
boolean startLogging = false;   // Data logging occurs when true

// Pulse Variables
const byte PULSE_PIN = 11;        // Sends pulses
const byte nth = 10;    // Number of lines saved to the SD card between pulses
byte count = 0;         // count of lines written to SD


// File Name Variables
File file;
String base_name = "log_";
String file_name;
const byte MAX_FILES = 100;


// Serial Communication Variables
#define BAUD 57600
const byte MAX_INPUT = 200; // how much serial data we expect before a newline

LedFlasher statusLED (13, 500, 500); // pin, off for mS, on for mS

void setup() {
  // USB and Board Serial
  Serial.begin(BAUD);
  Serial1.begin(BAUD);

  // Debugging: Arduino will not run without USB Serial connected
  //while (!Serial) {;}

  // SD Card
  initializeSDCard();

  // Pins
  pinMode(LED_PIN, OUTPUT);    //Set the LED pin as an output
  pinMode(PULSE_PIN, OUTPUT);

  Serial.println("Waiting for button press...");
  statusLED.begin();
}


void loop() {
  //Read buttons
  button.read();

  // Process Button Press
  if (button.wasReleased()) {       //If the button was released, change the LED state and start logging data
    buttonRoutine();
  }

  // Log Data and send pulses
  if(startLogging){
    //Get data from Serial1
    while (Serial1.available () > 0){
      processIncomingByte (Serial1.read ());
    }
  } else {
    statusLED.update();
  }
}

void initializeSDCard(){
  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    return;
  } else {
    Serial.println("SD initialized.");
    createSaveFile();
  }
}

/*=== Data Files ===*/
// Select a fileneame to save data to. Look for files already on the SD card to prevent naming collisions.
void createSaveFile(){
  for (int i = 1; i < MAX_FILES; i++) {
    file_name = base_name + i + ".csv";
    if (SD.exists(file_name)) { //check if file name already exists
      Serial.print(file_name);
      Serial.println(" exists.");
    } else {  // write to file
      file = SD.open(file_name, FILE_WRITE);
      Serial.print("Saving to ");
      Serial.println(file_name);
      file.close();
      break;
    }
  }
}


void buttonRoutine(){
  startLogging = true;

  // DEBUGGING
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
  Serial.println("Triggered! (button pressed)");
}


void processIncomingByte (const byte inByte){
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;
  switch (inByte){
    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte

      // terminator reached! process input_line here ...
      writeDataToSD(input_line);

      // reset buffer for next time
      input_pos = 0;
      break;
    case '\r':   // discard carriage return
      break;
    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;
    }
}


void sendPulse(){
  if(count % nth == 0){
    digitalWrite(PULSE_PIN, HIGH);
    count = 0;
  } else {
    digitalWrite(PULSE_PIN, LOW);
  }
}


void writeDataToSD(const char * data) {
  // DEBUGGING
  Serial.println(data);
  
  // Open file for writing
  file = SD.open(file_name, FILE_WRITE);

  // If file successfully opened, write data and send a timing pulse
  if (file) {
    sendPulse();
    count++;
    file.println(data);
    file.close();
  } else {
    Serial.println("Error opening file");
  }
}
