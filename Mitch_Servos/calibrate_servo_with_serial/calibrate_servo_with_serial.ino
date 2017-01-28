/*
 * Calibrate Servo with Serial
 *
 * Allows servos to be calibrated through serial communication. Numbers are read
 * from serial, and if they are within a valid range, the servo's position is set
 * to the specified number. This position is held until another number is entered.
 *
 * Uses Arduino's standard servo library.
 *
 */


#include <Servo.h>

Servo servo;
const byte servo_pin = 10;

// set the servo to this position
int position = 0;
String input_str = "";


void setup() {
  servo.attach(servo_pin);

  // Arduino will not run without USB Serial connected
  Serial.begin(9600);
  while (!Serial) { ; }

  Serial.println("Enter a number to set the servo's position [0-180]:");
}


void loop() {
  // Read serial input:
  while (Serial.available() > 0) {
    position = read_int_from_serial();
  }

  // Set the servo to the requested position
  if(position >= 0 && position <= 180){
    servo.write(position);
  }
}

/*
 * Read from serial until a newline.
 * Returns the number received from serial.
 */
int read_int_from_serial(){
    int input_char = Serial.read();

    if (isDigit(input_char)) {
      // convert the incoming byte to a char and append to string
      input_str += (char)input_char;
    }

    if (input_char == '\n') {
      position = input_str.toInt();
      input_str = "";
      Serial.print("Position: ");
      Serial.println(position);
    }

    return position;
}
