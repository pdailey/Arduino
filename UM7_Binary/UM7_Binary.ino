#include <UM7.h>

UM7 imu;

#define ANGLE 91.02222
#define RATE 16.0

#define BAUD 57600

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD);
  Serial1.begin(BAUD);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available()) {
    if (imu.encode(Serial1.read())) {  // Reads byte from buffer.  Valid packet returns true.
        float data[7] = {imu.gps_time, \
                         imu.roll/ANGLE,     imu.pitch/ANGLE,     imu.yaw/ANGLE, \
                         imu.roll_rate/RATE, imu.pitch_rate/RATE, imu.yaw_rate/RATE};

        String str = "$PPPPP, ";
        for(int i = 0; i<7; i++){
          str += String(data[i]);
          str += ", ";
        }
        Serial.println(str);
    }
  }
}
