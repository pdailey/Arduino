#User Guide for Arduino-Controlled IMU/GPS Logger
The IMU/GPS collects several key riding measurements, including euler angles, angular and linear accelerations, and global position. These measurements are sent to an Arduino, where they are stored to an SD card. The IMU/GPS Logger can be run independently, or in sync with the Beaglebone Black and its associated sensors.

Below is the basic layout of the system.

![System showing main components.](./docs/pics/overview.jpg )


### IMU
 The UM7 orientation sensor from CH Robotics is an Attitude and Heading Reference System (AHRS) that contains a three-axis accelerometer, rate gyroscope, and magnetometer. It combines this data using an Extended Kalman Filter to produce attitude and heading estimates.

The IMU outputs readings as soon as it has adequate power, signified by a green status LED.

As of this writing, the IMU is programmed to output the following information vis NMEA strings:

| Measurement                                                  | Sampling Rate (Hz) |
| ------------------------------------------------------       | :----------------: |
| 3-Axis  Raw Accelerometer, Gyroscope and Magnetometer        | 20                 |
| 3-Axis  Processed Accelerometer, Gyroscope and Magnetometer  | 20                 |
| Euler Angles (Pitch, Roll and Yaw)                           | 20                 |
| Euler Rates  (Pitch, Roll and Yaw Rate)                      | 20                 |
| Latitude, Longitude, and Velocity (from GPS)                 | 5                  |

The image below demonstrates the pin configuration used to reprogram the IMU and modify the packets it transmits or calibrate the magnetometer. Reprogramming the IMU is beyond the scope of this guide, but instructions can be found at https://www.pololu.com/product/2741 under the resources tab.

![Programming the IMU using the USB to TTL serial cable.](./docs/pics/program_imu.jpg)

### GPS
An external GPS provides longitude, latitude and velocity estimates. These are relayed to the IMU over TX2/RX2, which sends them to the Arduino via UART serial.

The GPS light will flash repeatedly when it has acquired 4+ satellite signals.  Once 4 satellites are acquired, the GPS outputs longitude and latitude.

The GPS operates most reliably when it acquires satellites within the first minute. For this reason, **power the GPS outside**. If a signal not acquired after 60 seconds (indicated by the red light flashing), restart the system and try again.

The image below demonstrates the pin configuration used to reprogram the GPS. Reprogramming the GPS is beyond the scope of this guide, but instructions can be found at https://www.pololu.com/product/2138 under the resources tab.

![Programming the GPS using the USB to TTL serial cable.](./docs/pics/program_gps.jpg)

## Arduino
The Arduino provides power to the system and saves sensor data to an SD card. In addition, the Arduino can rechage a 3.7V LiPo battery when it is connected to micro USB.

If the Arduino becomes unresponsive, try reflashing the program called `Log_Serial_Data_To_SD.ino`. You will also need to install the libraries located under `/lib/`

Programming the Arduino is beyond the scope of this guide, but instructions can be found at https://learn.adafruit.com/adafruit-feather-32u4-adalogger.


## Mounting on Bike
1. Insert the MicroSD into the MicroSD card holder.
- With the system outside, to allow the GPS to acquire a satellite fix, plug the battery into the Arudino. Attach the battery to the lid of the case.
- Mount the case to the bicycle frame underneath the bicycle seat, using the rails as a guide. Secure with zip ties.
- Check that the case is level in both lean and pitch. Adjust the bicycle saddle and seatpost if necessary.
- Plug the audio jack into the Beaglebone case into the port marked IMU/GPS.

## Operation
When the battery is plugged in, the red light on the GPS briefly flashes red. A green light labeled "ST" on the IMU will turn on. This indicates that the sensors have adequate power.

The Arduino has two modes, Standby and Data Logging, toggled by a signal from a button press or a pulse from the AUX cable.

In Standby Mode, the Arduino prepares to log data by creating a save file and then waits for input. In Data Logging Mode, the Arduino logs data to the newly created file. By toggling between these modes, new files are created. This allows multiple tests to be run in a single session and saved to incrementally named files, without resetting the entire system.

With a fully-charged 3.7V, 2000mAh battery, the system will run for 8+ hours.

**NOTE**: The red light will continue to flash if the SD card was not initialized properly or is missing. A flashing light indicates that data is not being saved to the SD card.

### Standby Mode
After the battery is plugged in, a red light on the Arduino will begin flashing. This indicates that the Arduino has power, and is in Standby Mode. The Arduino will continue flashing until it receives a signal to begin logging data from the sensors, at which point it will transition to Data Logging Mode.

### Data Logging Mode
 Once the Arduino receives a signal to begin logging data, the light turns solid red. The Arduino will log data until it receives a signal to stop, either from a button press or the AUX cable. A stop signal will cause the system to return to return to standby mode.

### Shutdown
At the end of testing, press the button to enter standby mode, and remove the battery. The Arduino and connected sensors will shut down.

## Retrieving Data
Retrieving data from the Arduino for processing is simple. Remove the SD card and connect it to a computer. The SD will contain files named `#_IMU.CSV`. A new file is created every time the Arduino enters Standby Mode, starting with `0_IMU.CSV`.

# TODO
- Make battery switch external

