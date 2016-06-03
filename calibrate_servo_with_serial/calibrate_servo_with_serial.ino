#include <Servo.h>

Servo servo;
const byte servo_pin = 10;

// set the servo to this position
int position = 0;
String input_str = "";


void setup() {
  Serial.begin(9600);
  servo.attach(servo_pin);
}


void loop() {
  // Read serial input:
  while (Serial.available() > 0) {
    position = read_int_from_serial();
    Serial.print("Position/Value:");
    Serial.println(position);
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
    }

    return position;
}
