#include <SPI.h>
#include <SD.h>

// File Name Variables
File file;
String base_name = "log_";
String file_name;
const int MAX_FILES = 100;

// Serial Communication Variables
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// Deafult Baudrates for Sensors
#define BAUD 57600

void setup() {  
  /*=== USB and Board Serial===
   * If the Serial1 baud is not set correctly, the board will not 
   * recognize the newline character, and data will not be returned.
   */
  Serial.begin(BAUD);
  Serial1.begin(BAUD);

  // Debugging: Arduino will not run without USB Serial connected  
  /*while (!Serial) {
    ;
  }

  // Debugging: Arduino will not run without Sensor connected  
  while (!Serial1) {
    ;
  }
  */

  /*=== SD Card ===*/
  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    return;
  } else {
    Serial.println("SD initialized.");
    createSaveFile();
  }

  // Reserve bytes for the inputString:
  inputString.reserve(512);
}



void loop() {
  /*=== Get sentence from Serial1 === 
  */
  while (Serial1.available()) {
    // Get the new byte and add it to the input string
    char c = (char) Serial1.read();

    // Check if NMEA string is incoming
    if(c == '$') {
      inputString += c;
    // if the string has started  
    } else if(inputString.length() != 0){
      inputString += c;
    }

    // String is complete
    if (c == '\n') {
        stringComplete = true;
    }
  }

    /*=== Retrieve Data from Serial1 ===
   * Print the string when a newline arrives
   */
  if (stringComplete) {
    //Serial.println(inputString);
    writeDataToSD();

    // Clear the string
    inputString = "";
    stringComplete = false;
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

/*=== Write Data to SD Card ===*/
void writeDataToSD() {
  Serial.print(inputString);

  // Open file for writing
  file = SD.open(file_name, FILE_WRITE);

  // If file sucessfully opened
  if (file) {
    file.print(inputString);
    file.close();
  } else {
    Serial.println("Error opening file");
  }
}
