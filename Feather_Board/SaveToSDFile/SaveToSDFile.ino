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
const double IMU_BAUD = 115200;
//const double GPS_BAUD = 57600;

void setup() {
  /*=== USB and Board Serial===
   * If the Serial1 baud is not set correctly, the board will not 
   * recognize the newline character, and data will not be returned.
   */
  Serial.begin(115200);         // USB
  //Serial1.begin(GPS_BAUD);      // GPS
  Serial1.begin(IMU_BAUD);    // IMU

  // wait for USB serial port to connect.
  while (!Serial) {
    ;
  }

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);


  /*=== SD Card ===*/
  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("SD initialized.");


  /*=== Data Files ===
   * Select a fileneame to save data to. Look for files already on the SD card to prevent naming collisions.
   */
  for (int i = 1; i < MAX_FILES; i++) {
    file_name = base_name + i + ".csv";

    if (SD.exists(file_name)) { //check if file name already exists
      //Serial.print(file_name);
      //Serial.println(" exists.");
    } else {  // write to file
      file = SD.open(file_name, FILE_WRITE);
      Serial.print("Saving to ");
      Serial.println(file_name);
      file.close();
      break;
    }
  }
}

void loop() {
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

  /*=== Retrieve sentence from Serial1 === 
  */
  while (Serial1.available()) {
    // Get the new byte and add it to the input string
    char inChar = (char)Serial1.read();
    inputString += inChar;
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}



/*=== Write Data to SD Card ===*/
void writeDataToSD() {
  Serial.print(inputString);

  // Open file for writing
  file = SD.open(file_name, FILE_WRITE);

  // if file sucessfully opened
  if (file) {
    file.print(inputString);
    file.close();
  } else {
    Serial.println("Error opening file");
  }
}

