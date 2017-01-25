/* Debugging script to test SD Card initialization, 
 * file creation and writing a string to the created file. 
 */

// include the SD library:
#include <SPI.h>
#include <SD.h>

const byte chipSelect = 15;
bool sdPresent = false;      // Was the SD properly enabled?

// File Name Variables
File file;
String base_name = "_HMP_data";
String file_name;

/* =========================================================================================
   Create a CSV to save data.
   Look for files already on the SD card to prevent naming collisions.
   ========================================================================================*/
void createSaveFile() {
  // make i static so it is maintained between calls to function.
  static int i;

  while (true) {
    file_name = i + base_name + ".csv";

    if (SD.exists(file_name)) { //check if file name already exists
      i++;
      Serial.print(file_name);
      Serial.println(" exists.");
    } else {  // write to file
      file = SD.open(file_name, FILE_WRITE);
      Serial.print("Saving to ");
      Serial.println(file_name);
      file.close();
      return;
    }
  }
}


void setup() {
  Serial.begin(57600);

  // SD Card
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  sdPresent = true;
  Serial.println("card initialized.");

  createSaveFile();


  if (sdPresent) {
    String s = "I have the data\n";
    file = SD.open(file_name, FILE_WRITE);

    // if the file is available, write to it
    if (file) {
      file.println(s);
      file.close();
      Serial.println(s);
    } else {
      Serial.println("Error opening file");
    }
  }
}

void loop()
{
}
