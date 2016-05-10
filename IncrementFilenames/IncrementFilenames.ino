#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// set up file names
File file;
String base_name = "log_";
String file_name;
int max_files = 100;


void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for USB serial port to connect.
  }

  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("SD initialized.");

  /* Select a fileneame to save data to. 
   *  Look for files already on the SD card to prevent naming collisions. 
   *  Files are written in ascending order. If a name is already present, the number is increased by one. 
  */
  for (int i = 1; i < max_files; i++) {
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
  // nothing happens after setup finishes.
}
