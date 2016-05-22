#User Guide for Arduino-Controlled IMU/GPS Logger

## System Layout
 #PIC(red SD, LIPO, button, micro USB port, GPS, IMU, programming ports)


## Installation
1. Insert the MicroSD into the MicroSD card holder.
- With the system outside, to allow the GPS to acquire a satellite fix, plug the battery into the Arudino. Attach the battery to the lid of the case.
- Mount the case to the bicycle frame underneath the bicycle seat, using the rails as a guide. Secure with zip ties. #PIC
- Check that the case is level in both lean and pitch. Adjust the bicycle saddle and seatpost if necessary. #PIC
- Plug the audio jack into the Beaglebone case into the port marked IMU/GPS. #PIC

## Operation
When the battery is plugged in, the red light on the GPS briefly flashes red, and a green light labeled "ST" on the IMU will turn on. This indicates that the sensors have adequate power.

### GPS
The GPS light will flash repeatedly when it has acquired 4+ satellite signals.  Once 4 satellites are acquired, the GPS outputs longitude and latitude.

The GPS operates most reliably when it acquires satellites within the first minute. For this reason, power the GPS outside. If a signal not acquired after 60 seconds (indicated by the red light flashing), restart the GPS and try again.

### Standby Mode
After the battery is plugged in, a red light on the Arduino will begin flashing. This indicates that the Arduino has power, but it is not logging data. The Arduino will continue flashing until it receives a signal to begin logging data from the sensors.

**NOTE**: The red light will continue to flash if the SD card was not initialized properly or is missing. A flashing light indicates that data is not being saved to the SD card.


### Data Logging
The signal to begin logging data is sent via button press, or a pulse from the auxiliary cable. Once the Arduino receives a signal to begin logging data, the light turns solid red. The Arduino will log data until the battery is removed. With a fully-charged 3.7V, 2000mAh battery, the system will run for 8+ hours.

### Shutdown
At the end of testing, remove the battery. The Arduino and connected sensors will shut down.

## Retrieving Data
Retrieving data from the Arduino for processing is simple. Remove the SD card and connect it to a computer. The SD will contain files named `IMU_LOG_#.csv`. A new file is created every time the Arduino is turned on, starting with `IMU_LOG_0.csv`.


# TODO
- pictures
- Change the filename
- Make battery switch external
- calibrate the IMU
