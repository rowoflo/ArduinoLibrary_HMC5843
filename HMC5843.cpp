/*
 -------------------------------------------------------------------------------
 HMC5843.cpp
 HMC5843 Project
 
 Initially created by Rowland O'Flaherty (rowlandoflaherty.com) on 1/13/12.
 
 Version 1.0
 -------------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "HMC5843.h"
#include <Arduino.h>
#include <Wire.h>

//------------------------------------------------------------------------------
// Friends
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Friend Overloaded Operators
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Lifecycle
//------------------------------------------------------------------------------

// Constructors
HMC5843::HMC5843()
:
_mode(continuous),
_Ts(4),
_gain(1),
_minX(-720),
_maxX(380),
_minY(-775),
_maxY(145),
_minZ(-37),
_maxZ(1035) {
    
}


// Destructor
HMC5843::~HMC5843() {
    
}

// Copy constructor
HMC5843::HMC5843(const HMC5843& srcObj) {
    copyHelper(srcObj);
}

// Copy helper function
void HMC5843::copyHelper(const HMC5843& srcObj) {
    _mode = srcObj._mode;
    _Ts = srcObj._Ts;
    _gain = srcObj._gain;
    _minX = srcObj._minX ;
    _maxX = srcObj._maxX ;
    _minY = srcObj._minY ;
    _maxY = srcObj._maxY ;
    _minZ = srcObj._minZ ;
    _maxZ = srcObj._maxZ ;
}

//------------------------------------------------------------------------------
// Overloaded Operators
//------------------------------------------------------------------------------

// Assignment operator
const HMC5843& HMC5843::operator=(const HMC5843& rhsObj) {
	// Self-assignment check
    if (this == &rhsObj) {
        return (*this);
    }
    
    // Free old memory
    
    // Copy new memory
    copyHelper(rhsObj);
    
    return (*this);
}

//------------------------------------------------------------------------------
// Public Member Functions
//------------------------------------------------------------------------------
void HMC5843::initialize() {
    // Initialize device
    Wire.begin();
    
    // Configure device
    delay(5);
    setMode(_mode);
    delay(1000/getSamplingRate()); 
    
    // Calibrate device
    
}

void HMC5843::raw(int& x, int& y, int& z) {
    Wire.beginTransmission(_HMC5843Address);
    Wire.write(_magneticReg);
    Wire.endTransmission();
    Wire.requestFrom((int)_HMC5843Address, 6);
    if (6 <= Wire.available()) {
        x = Wire.read() << 8;
        x |= Wire.read();
        y = Wire.read() << 8;
        y |= Wire.read();
        z = Wire.read() << 8;
        z |= Wire.read();
    }
}

void HMC5843::normalized(int& x, int& y, int& z) {
    int xx, yy, zz;
    raw(xx, yy, zz);
    
    x = max(min(xx, _maxX), _minX);
    x = map(x,_minX,_maxX,-32768,32767);
    
    y = max(min(yy, _maxY), _minY);
    y = map(y,_minY,_maxY,-32768,32767);
    
    z = max(min(zz, _maxZ), _minZ);
    z = map(z,_minZ,_maxZ,-32768,32767);
    
}

double HMC5843::heading() {
    return heading(0.0,0.0);
}

double HMC5843::heading(double theta, double phi) {
    int x, y, z;
    normalized(x, y, z);
    
    double X, Y;
    X = x * cos(phi) + y * sin(theta) * cos(phi) - z * cos(theta) * sin(phi);
    Y = y * cos(theta) + z * sin(theta);
    
    return (atan2(Y, X) * _rad2deg);
}

void HMC5843::setMode(Mode mode) {
    if (mode < 0 || mode > 3) {
        mode = idle; // Default is idle mode
    }
    _mode = mode;
    writeByte(_modeReg, (int)_mode);
}

double HMC5843::getSamplingRate() {
    switch (_Ts) {
        case 0:
            return 0.5;
            break;
            
        case 1:
            return 1;
            break;
            
        case 2:
            return 2;
            break;
            
        case 3:
            return 5;
            break;
            
        case 4:
            return 10;
            break;
            
        case 5:
            return 20;
            break;
            
        case 6:
            return 50;
            break;
            
    }
}

void HMC5843::setSamplingRate(int Ts) {
    if (Ts < 0 || Ts > 6) {
        Ts = 4; // Default is 10Hz
    }
    _Ts = Ts;
    writeByte(_AReg, _Ts << 2);
}

double HMC5843::getGain() {
    switch (_gain) {
        case 0:
            return 0.7;
            break;
            
        case 1:
            return 1.0;
            break;
            
        case 2:
            return 1.5;
            break;
            
        case 3:
            return 2.0;
            break;
            
        case 4:
            return 3.2;
            break;
            
        case 5:
            return 3.8;
            break;
            
        case 6:
            return 4.5;
            break;
            
        case 7:
            return 6.5;
            break;
            
    }
}

void HMC5843::setGain(int gain) {
    if (gain < 0 || gain > 7) {
        gain = 1; // Default is 1.0Hz
    }
    _gain = gain;
    writeByte(_BReg, _gain << 5);
}

//------------------------------------------------------------------------------
// Private Member Functions
//------------------------------------------------------------------------------
byte HMC5843::readByte(byte regAddress) {
    // Read 8 bits or 1 byte from the given register adddress
    Wire.beginTransmission(_HMC5843Address);
    Wire.write(regAddress);
    Wire.endTransmission();
    
    Wire.requestFrom((int)_HMC5843Address, 1);
    if (Wire.available() >= 1) {
        return Wire.read();
    }
}

void HMC5843::writeByte(byte regAddress, byte value) {
    // Write 8 bits or 1 byte into E2PROM registers at the given address
    Wire.beginTransmission(_HMC5843Address);
    Wire.write(regAddress);
    Wire.write(value);
    Wire.endTransmission();
}
