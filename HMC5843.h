/*
 -------------------------------------------------------------------------------
 HMC5843.h
 HMC5843 Project
 
 CLASS NAME:
    HMC5843();
 
 DESCRIPTION:
    This class is used to interface the Arduino with the HMC5843 digital
    magnetometer sensor.
 
 FILES:
    HMC5843.h
    HMC5843.cpp
 
 DEPENDENCIES:
    Arduino.h
    Wire.h
 
 CONSTRUCTORS:
    HMC5843();    
 
 PUBLIC METHODS:
    void initialize();
        Initializes the device. Must be done before device can be used.

    void raw(int& x, int& y, int& z);
        Get raw output values from the device.
            x - X channel raw value.
            y - Y channel raw value.
            z - Z channel raw value.
 
    void normalized(int& x, int& y, int& z);
        Get normalizied output values from the device. The raw values are
        normalized to -32768 and 32767 using minimum and maximum values
        obtained from calibration.
             x - X channel normalized value.
             y - Y channel normalized value.
             z - Z channel normalized value.
    
    double heading();
    double heading(double theta, double pitch);
        Get heading value in degrees.
            theta - Roll angle in radians. Default is 0.
            phi - Pitch angle in radians. Default is 0.
     
    Mode getMode();
        Get device mode. ([0-continuous], 1-single, 2-idle, 3-sleep).
    
    void setMode(Mode mode);
        Set device mode.
            mode - Desired mode. Valid values are 0, 1, 2, 3.
 
    double getSamplingRate();
        Get sampling rate of the device. Default is 4 = 10Hz.
 
    void setSamplingRate(int Ts);
        Set sampling rate. Valid values are
        (0=0.5, 1=1, 2=2, 3=5, [4=10], 5=20, 6=50)[Hz]
 
    double getGain();
        Get sensor gain. This controls the resolution of the sensor. The better
        the resolution the smaller the min/max values of the sensor.
 
    void setGain(int gain);
        Set gain value. Valid values are
        (0=+-0.7, [1=+-1.0], 2=+-1.5, 3=+-2.0, 4=+-3.2,
         5=+-3.8, 6=+-4.5, 7=+-6.5)[Ga]
 
 NOTES:
 
 
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

 -------------------------------------------------------------------------------
 */

#ifndef _HMC5843_h_
#define _HMC5843_h_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <Arduino.h>


class HMC5843 {
    
public:
    //--------------------------------------------------------------------------
    // Constants, Enums, and Types
    //--------------------------------------------------------------------------
	enum Mode {continuous, single, idle, sleep};
    
    //--------------------------------------------------------------------------
    // Lifecycle
    //--------------------------------------------------------------------------
    
    // Constructors
    HMC5843();
    
    // Destructor
    virtual ~HMC5843();
    
    // Copy constructor
    HMC5843(const HMC5843& srcObj);
    
    //--------------------------------------------------------------------------
    // Overloaded Operators
    //--------------------------------------------------------------------------
    // Assignment operator
    const HMC5843& operator=(const HMC5843& rhsObj);
    
    
    //--------------------------------------------------------------------------
    // Public Member Functions
    //--------------------------------------------------------------------------
    void initialize();
    void raw(int& x, int& y, int& z);
    void normalized(int& x, int& y, int& z);
    double heading();
    double heading(double theta, double pitch);
    void calibrate();
    
    Mode getMode() { return _mode;}
    void setMode(Mode mode);
    double getSamplingRate();
    void setSamplingRate(int Ts);
    double getGain();
    void setGain(int gain);
    
private:
	//--------------------------------------------------------------------------
    // Lifecycle
    //--------------------------------------------------------------------------
    void copyHelper(const HMC5843& srcObj);
    
    //--------------------------------------------------------------------------
    // Private Member Functions
    //--------------------------------------------------------------------------
    byte readByte(byte regAddress);
    void writeByte(byte regAddress, byte value);
    
    //--------------------------------------------------------------------------
    // Private Member Variables
    //--------------------------------------------------------------------------
    // Class constants
    static const double _rad2deg = 57.2957795131;
    
    // Device constants
    static const byte _HMC5843Address = 0x1E; // I2C address of HMC5843
    
    // Register addresses
    static const char _AReg = 0x00;
    static const char _BReg = 0x01;
    static const char _modeReg = 0x02;
    static const char _magneticReg = 0x03;
    static const char _statusReg = 0x09;
    static const char _idAReg = 0x10;
    static const char _idBReg = 0x11;
    
    // Device variables
    Mode _mode; // Sets device mode ([0-continuous], 1-single, 2-idle, 3-sleep)
    byte _Ts; // Samping rate (0=0.5, 1=1, 2=2, 3=5, [4=10], 5=20, 6=50)[Hz]
    byte _gain; // Device gain (0=+-0.7, 1=+-1.0, 2=+-1.5, 3=+-2.0, 4=+-3.2, 5=+-3.8, 6=+-4.5, 7=+-6.5)[Ga]
    
    int _minX; // Minimum raw x value. [-720]
    int _maxX; // Maximum raw x value. [380]
    int _minY; // Minimum raw y value. [-775]
    int _maxY; // Maximum raw y value. [145]
    int _minZ; // Minimum raw z value. [-37] 
    int _maxZ; // Maximum raw z value. [1025]
    
};

#endif