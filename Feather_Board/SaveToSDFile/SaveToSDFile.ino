#include <Button.h>
#include <SPI.h>
#include <SD.h>


/* Button Variables */
#define BUTTON_PIN 2       // Connect a tactile button switch (or something similar)
#define PULLUP true        // Use Arduino's internal pullup resistor.
#define INVERT true        // Since the pullup resistor will keep the pin high unless the
                           // switch is closed, this is negative logic, i.e. a high state
                           // means the button is NOT pressed. (Assuming a normally open switch.)
#define DEBOUNCE_MS 20     // A debounce time of 20 milliseconds usually works well for tactile button switches.
#define LED_PIN 13         // The standard Arduino "Pin 13" LED

Button button(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);
boolean ledState;          //A variable that keeps the current LED status
boolean startLogging = false;

/* File Name Variables */
File file;
String base_name = "log_";
String file_name;
const int MAX_FILES = 100;


/* Serial Communication Variables */
// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 200;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

/* Deafult Baudrate */
#define BAUD 57600

byte counter = 0;

void setup() { 
  /*=== USB and Board Serial===
   * If the Serial1 baud is not set correctly, the board will not 
   * recognize the newline character, and data will not be returned.
   */
  Serial.begin(BAUD);
  Serial1.begin(BAUD);

  // Debugging: Arduino will not run without USB Serial connected  
  //while (!Serial) {;}

  /*=== SD Card ===*/
  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    return;
  } else {
    Serial.println("SD initialized.");
    createSaveFile();
  }

  /*=== LED Pin ===*/
  pinMode(LED_PIN, OUTPUT);    //Set the LED pin as an output

  // Reserve bytes for the inputString:
  inputString.reserve(512);

  Serial.println("Waiting for button press...");
}



void loop() {
  /*=== Get data from Serial1 === */
  button.read();                    //Read the button

  /*=== Handle Button Press === */
  if (button.wasReleased()) {       //If the button was released, change the LED state and start logging data
    buttonRoutine();
  }

  if(startLogging){
  /*=== Get data from Serial1 === */
    while (Serial1.available () > 0){
      processIncomingByte (Serial1.read ());
    }
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


/*=== Write Data to SD Card ===*/
void writeDataToSD(const char * data) {
  // DEBUGGING
  Serial.println(data);
  //Serial.println(counter);
  
  // Open file for writing
  file = SD.open(file_name, FILE_WRITE);

  // If file sucessfully opened
  if (file) {
    file.println(data);
    file.close();
    counter++;
  } else {
    Serial.println("Error opening file");
  }
}
