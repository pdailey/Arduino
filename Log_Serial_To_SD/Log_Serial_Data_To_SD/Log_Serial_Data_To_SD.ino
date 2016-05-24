/***


      LLLLLLLLLLL
      L:::::::::L
      L:::::::::L
      LL:::::::LL
        L:::::L                  ooooooooooo      ggggggggg   ggggg
        L:::::L                oo:::::::::::oo   g:::::::::ggg::::g
        L:::::L               o:::::::::::::::o g:::::::::::::::::g
        L:::::L               o:::::ooooo:::::og::::::ggggg::::::gg
        L:::::L               o::::o     o::::og:::::g     g:::::g
        L:::::L               o::::o     o::::og:::::g     g:::::g
        L:::::L               o::::o     o::::og:::::g     g:::::g
        L:::::L         LLLLLLo::::o     o::::og::::::g    g:::::g
      LL:::::::LLLLLLLLL:::::Lo:::::ooooo:::::og:::::::ggggg:::::g
      L::::::::::::::::::::::Lo:::::::::::::::o g::::::::::::::::g
      L::::::::::::::::::::::L oo:::::::::::oo   gg::::::::::::::g
      LLLLLLLLLLLLLLLLLLLLLLLL   ooooooooooo       gggggggg::::::g
                                                           g:::::g
                                               gggggg      g:::::g
                                               g:::::gg   gg:::::g
         SSSSSSSSSSSSSSS                        g::::::ggg:::::::giiii                    lllllll
       SS:::::::::::::::S                        gg:::::::::::::gi::::i                   l:::::l
      S:::::SSSSSS::::::S                          ggg::::::ggg   iiii                    l:::::l
      S:::::S     SSSSSSS                             gggggg                              l:::::l
      S:::::S                eeeeeeeeeeee    rrrrr   rrrrrrrrr  iiiiiii   aaaaaaaaaaaaa    l::::l
      S:::::S              ee::::::::::::ee  r::::rrr:::::::::r i:::::i   a::::::::::::a   l::::l
       S::::SSSS          e::::::eeeee:::::eer:::::::::::::::::r i::::i   aaaaaaaaa:::::a  l::::l
        SS::::::SSSSS    e::::::e     e:::::err::::::rrrrr::::::ri::::i            a::::a  l::::l
          SSS::::::::SS  e:::::::eeeee::::::e r:::::r     r:::::ri::::i     aaaaaaa:::::a  l::::l
             SSSSSS::::S e:::::::::::::::::e  r:::::r     rrrrrrri::::i   aa::::::::::::a  l::::l
                  S:::::Se::::::eeeeeeeeeee   r:::::r            i::::i  a::::aaaa::::::a  l::::l
                  S:::::Se:::::::e            r:::::r            i::::i a::::a    a:::::a  l::::l
      SSSSSSS     S:::::Se::::::::e           r:::::r           i::::::ia::::a    a:::::a l::::::l
      S::::::SSSSSS:::::S e::::::::eeeeeeee   r:::::r           i::::::ia:::::aaaa::::::a l::::::l
      S:::::::::::::::SS   ee:::::::::::::e   r:::::r           i::::::i a::::::::::aa:::al::::::l
       SSSSSSSSSSSSSSS       eeeeeeeeeeeeee   rrrrrrr           iiiiiiii  aaaaaaaaaa  aaaallllllll
      DDDDDDDDDDDDD                                 tttt
      D::::::::::::DDD                           ttt:::t
      D:::::::::::::::DD                         t:::::t
      DDD:::::DDDDD:::::D                        t:::::t
        D:::::D    D:::::D  aaaaaaaaaaaaa  ttttttt:::::ttttttt      aaaaaaaaaaaaa
        D:::::D     D:::::D a::::::::::::a t:::::::::::::::::t      a::::::::::::a
        D:::::D     D:::::D aaaaaaaaa:::::at:::::::::::::::::t      aaaaaaaaa:::::a
        D:::::D     D:::::D          a::::atttttt:::::::tttttt               a::::a
        D:::::D     D:::::D   aaaaaaa:::::a      t:::::t              aaaaaaa:::::a
        D:::::D     D:::::D aa::::::::::::a      t:::::t            aa::::::::::::a
        D:::::D     D:::::Da::::aaaa::::::a      t:::::t           a::::aaaa::::::a
        D:::::D    D:::::Da::::a    a:::::a      t:::::t    tttttta::::a    a:::::a
      DDD:::::DDDDD:::::D a::::a    a:::::a      t::::::tttt:::::ta::::a    a:::::a
      D:::::::::::::::DD  a:::::aaaa::::::a      tt::::::::::::::ta:::::aaaa::::::a
      D::::::::::::DDD     a::::::::::aa:::a       tt:::::::::::tt a::::::::::aa:::a
      DDDDDDDDDDDDD         aaaaaaaaaa  aaaa         ttttttttttt    aaaaaaaaaa  aaaa
               tttt                                        SSSSSSSSSSSSSSS DDDDDDDDDDDDD
            ttt:::t                                      SS:::::::::::::::SD::::::::::::DDD
            t:::::t                                     S:::::SSSSSS::::::SD:::::::::::::::DD
            t:::::t                                     S:::::S     SSSSSSSDDD:::::DDDDD:::::D
      ttttttt:::::ttttttt       ooooooooooo             S:::::S              D:::::D    D:::::D
      t:::::::::::::::::t     oo:::::::::::oo           S:::::S              D:::::D     D:::::D
      t:::::::::::::::::t    o:::::::::::::::o           S::::SSSS           D:::::D     D:::::D
      tttttt:::::::tttttt    o:::::ooooo:::::o            SS::::::SSSSS      D:::::D     D:::::D
            t:::::t          o::::o     o::::o              SSS::::::::SS    D:::::D     D:::::D
            t:::::t          o::::o     o::::o                 SSSSSS::::S   D:::::D     D:::::D
            t:::::t          o::::o     o::::o                      S:::::S  D:::::D     D:::::D
            t:::::t    tttttto::::o     o::::o                      S:::::S  D:::::D    D:::::D
            t::::::tttt:::::to:::::ooooo:::::o          SSSSSSS     S:::::SDDD:::::DDDDD:::::D
            tt::::::::::::::to:::::::::::::::o          S::::::SSSSSS:::::SD:::::::::::::::DD
              tt:::::::::::tt oo:::::::::::oo           S:::::::::::::::SS D::::::::::::DDD
                ttttttttttt     ooooooooooo              SSSSSSSSSSSSSSS   DDDDDDDDDDDDD

   ============================================================================================
   ============================================================================================
   ============================================================================================
*/

/*
   This program processes serial data from sensors attached to the Arduino's TX and RX pins.
   The data is saved to an SD card on the Arduino in a CSV file.

   The code breaks down into the following stages:
   1. *Wait* for a signal to begin processing data.
   2. *Process* incoming data on the serial line.
   3. *Store* the data on the SD card.
*/


#include <Button.h>
#include <LedFlasher.h>
#include <SPI.h>
#include <SD.h>


/* ============================================================================
                               GLOBAL VARIABLES!
                                    (/^▽^)/
   ==========================================================================*/

// Button Variables
const bool PULLUP = true;     // Use Arduino's internal pullup resistor.
const bool INVERT = true;     // Pullup resistor will keep the pin high unless the button is pressed!
const byte DEBOUNCE_MS = 20;     // A debounce time of 20 milliseconds works well for tactile button switches.

const byte BUTTON_PIN = 2;       // tactile button switch
const byte PULSE_PIN = 9;        // Receives timing pulses

// Input Pins
Button button(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS); // Tactile button
Button pulse_line(PULSE_PIN, PULLUP, INVERT, DEBOUNCE_MS); // Pulse Line

// Use the onboard LED to signal that the Arduino is waiting to start collecting data.
const byte RED_LED = 13;         // The standard "Pin 13" LED
LedFlasher redLed (RED_LED, 500, 500); // SYNTAX: pin, off for mS, on for mS


// State Variables
const bool DEBUGGING = false; // Set to true to see usb serial mesages
bool ledState = false;       // Keeps the current LED status
bool sdPresent = false;      // Was the SD properly enabled?
bool startLogging = false;   // Data logging occurs when true


// File Name Variables
// TODO: Change file name
File file;
String base_name = "imu_";
String file_name;
const int MAX_FILES = 1000;


// Serial Communication Variables
const unsigned int BAUD = 57600;
const byte MAX_INPUT = 200; // how much serial data we expect before a newline

/* ============================================================================
                                     SETUP!
                                 (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧
   ==========================================================================*/
void setup() {
  // USB and Board Serial
  Serial.begin(BAUD);
  Serial1.begin(BAUD);

  // Debugging: Arduino will not run without USB Serial connected
  if (DEBUGGING) {
    while (!Serial) {
      ;
    }
  }

  // Use the onboard LED to indicate the Arduino's state
  pinMode(RED_LED, OUTPUT);    //Set the LED pin as an output
  Serial.println("Waiting for signal to begin collecting data...");


  // SD Card
  if (initializeSDCard()) {
    sdPresent = true;
  }

  // Start Flashing
  redLed.begin();
}


/* ============================================================================
                                      LOOP!
                                     ᕕ( ᐛ )ᕗ
   ==========================================================================*/
void loop() {
  button.read();
  pulse_line.read();

  // Process a button press
  if (button.wasReleased()) {
    buttonRoutine();
  }

  // Process a timing pulse
  if (pulse_line.wasReleased()) {
    pulseRoutine();
  }

  // Log Data if a signal has been sent
  if (startLogging) {
    //Get data from Serial1
    while (Serial1.available () > 0) {
      processIncomingByte (Serial1.read ());
    }
  }

  // Flash the light if logging has not been triggered or the SD is not present
  // TODO: LED does not flash once the button is pressed
  if (!startLogging || !sdPresent) {
    redLed.update();
  }
}


/* =========================================================================================
   Look for an SD card and establish a connection.
   Returns true if the SD card is initialized.
   ========================================================================================*/
bool initializeSDCard() {
  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    return false;
  } else {
    Serial.println("SD initialized.");
    return true;
  }
}


/* =========================================================================================
   Create a CSV to save data.
   Look for files already on the SD card to prevent naming collisions.
   ========================================================================================*/
void createSaveFile() {
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


/* =========================================================================================
   Process a button press. A button press toggles data logging on and off.
   ========================================================================================*/
void buttonRoutine() {
  startLogging = !startLogging;
  if (startLogging) {
    // create a new save file
    createSaveFile();
    
    file = SD.open(file_name, FILE_WRITE);
  } else {
    file.close();
  }

  ledState = !ledState;
  digitalWrite(RED_LED, ledState);

  if (DEBUGGING) {
    Serial.println("Triggered! (button pressed)");
  }
}


/* =========================================================================================
   Process a pulse. A single pulse starts data logging.
   ========================================================================================*/
void pulseRoutine() {
  buttonRoutine();

  if (DEBUGGING) {
    Serial.println("Pulsed! (pulse recieved)");
  }
}


/* =========================================================================================
   Process bytes from the TX/RX Pins.

   Takes data from the serial buffer, scanning the input until a newline character is found.
   Once a newline character is found, the sentence is written to the SD card.
   ========================================================================================*/
void processIncomingByte (const byte inByte) {
  static char input_line[MAX_INPUT];
  static unsigned int input_pos = 0;
  switch (inByte) {
    case '\n':   // end of text
      input_line[input_pos] = 0;  // terminating null byte

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
        input_line[input_pos++] = inByte;
      break;
  }
}


/* =========================================================================================
 * Write the data to the SD Card.
 * ========================================================================================*/
void writeDataToSD(const char * data) {
  file.write(data);
  file.write('\n');

  if (DEBUGGING) {
    Serial.println(data);
  }
}
