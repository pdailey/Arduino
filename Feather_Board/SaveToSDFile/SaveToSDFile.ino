#include <SPI.h>
#include <SD.h>

// File Name Variables
File file;
String base_name = "log_";
String file_name;
const int MAX_FILES = 100;

// Serial Variables
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


// TODO
boolean written_already = false;

void setup() {
  /*=== USB and Board Serial===*/
  Serial.begin(9600);           // USB Serial
  Serial1.begin(115200);        // Board Serial

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
      Select a fileneame to save data to. Look for files already on the SD card to prevent naming collisions.
  */
  for (int i = 1; i < MAX_FILES; i++) {
    file_name = base_name + i + ".csv";

    if (SD.exists(file_name)) {
      Serial.print(file_name);
      Serial.println(" exists.");
    } else {
      file = SD.open(file_name, FILE_WRITE);
      Serial.print("Saving to ");
      Serial.println(file_name);
      //TODO: will need to open and close
      file.close();
      break;
    }
  }
}

void loop() {
  if (written_already == false) {
    inputString = "I have a big mouth!";

    // Open file for writing
    file = SD.open(file_name, FILE_WRITE);
    if (file) {

      Serial.print("Writing to ");
      Serial.print(file_name);

      file.println("testing 1, 2, 3.");
      file.println(inputString);

      // close the file
      file.close();
      Serial.println(" ...done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("Error opening test.txt");
    }

    written_already = true;
  }

  /*=== Write Data to SD Card ===*/


  /*=== Revieve Data from Serial1 ===*/
  /*
     // print the string when a newline arrives:
    if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
    }

    while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    //Serial.println(inChar);

    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    }
  */

}

