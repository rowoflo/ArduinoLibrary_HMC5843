HMC5843 Project

DESCRIPTION:
    This class is used to interface the Arduino with the HMC5843 digital
    magnetometer sensor.
 
USAGE:
    The pressure sensor only needs 4 pins hooked up to use: 3.3V, GND, I2C
    (SDA, SCL).
    
    See HMC5843.h header file for class information.

NOTES:
    If running on an Arduino that operates at 5V (e.g. UNO) it is recommended
    to use a logic level converter for the I2C interface because the sensor
    runs at 3.3V. However, it has been found that the sensor operates normally
    if it is hooked up directly to the Arduino.
 
EXAMPLES:
     Example 1: Initializing sensor and printing current heading.
     ----------------------------------------------------------------------------
    #include <Arduino.h>
    #include <HMC5843.h>
    HMC5843 hmc;
    void setup() {
        Serial.begin(9600);
        bmp.initialize();
        Serial.println(hmc.heading());
     }
     ----------------------------------------------------------------------------

VERSIONS:
    1.0 - 1/13/12 - Rowland O'Flaherty (rowlandoflaherty.com)
        Initial release.
    