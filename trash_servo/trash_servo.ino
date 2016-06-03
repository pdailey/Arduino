#include <Servo.h>

Servo row_servo;
Servo col_servo;

const byte r_servo_pin = 9;
const byte c_servo_pin = 10;

//  0 1 2 3 4 5 6
//0 a b c d e f g
//1 h i j k l m n
//2 o p q r s t u
//3 v w x y z _ !

//                0         1         2
//                012345678901234567890123456789
char *keyboard = "abcdefghijklmnopqrstuvwxyz !";

const byte servo_row_pos[] = {120, 107, 89, 76};
const byte servo_col_pos[] = {130, 117, 100, 85, 65, 45, 30};

byte row = 0;
byte col = 0;

// String to write
char str[] = "mitch is trash! ";

void setup() {
  Serial.begin(9600);

  row_servo.attach(r_servo_pin);
  col_servo.attach(c_servo_pin);
}

void loop() {
  print_with_servo(str);
  delay(5000);
}



void print_with_servo(char str[]) {
  for (byte i = 0; i < strlen(str); i++) {
    char c = str[i];
    get_row_col_char(c, row, col);
    set_servo_position(row, col);
    delay(2000);
  }
}

void get_row_col_char(char target, byte &row, byte &col) {
  int index;

  // look for the location of the character in the keyboard string
  const char *result = strchr(keyboard, target);
  index = (int)(result - keyboard);
  
  // Map letter to keyboard layout
  col = index % 7;
  row = index / 7;

  Serial.print(target);
  Serial.print(row);
  Serial.print(", ");
  Serial.println(col);
}

void set_servo_position(byte row, byte col) {
  row_servo.write(servo_row_pos[row]);
  col_servo.write(servo_col_pos[col]);
}
