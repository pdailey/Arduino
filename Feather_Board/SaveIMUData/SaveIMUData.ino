#include <SPI.h>
#include <SD.h>
#include <UM7.h>

UM7 imu;

// Convert Registers
#define ANGLE 91.02222
#define RATE 16.0

// Baudrate
#define BAUD 57600


// File Name Variables
File file;
String base_name = "log_";
String file_name;
const int MAX_FILES = 100;

// Serial Communication Variables
String inputString;

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
  }*/

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
  if (Serial1.available()) {
    if (imu.encode(Serial1.read())) {  // Reads byte from buffer.  Valid packet returns true.
        float data[7] = {imu.gps_time, \
                         imu.roll/ANGLE,     imu.pitch/ANGLE,     imu.yaw/ANGLE, \
                         imu.roll_rate/RATE, imu.pitch_rate/RATE, imu.yaw_rate/RATE};

        inputString = "$PPPPP, ";
        for(int i = 0; i<7; i++){
          inputString += String(data[i]);
          inputString += ", ";
        }
        writeDataToSD();
    }
  }
}

/*=== Create Save File ===*/
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
  Serial.println(inputString);

  // Open file for writing
  file = SD.open(file_name, FILE_WRITE);

  // If file sucessfully opened
  if (file) {
    file.println(inputString);
    file.close();
  } else {
    Serial.println("Error opening file");
  }
}
