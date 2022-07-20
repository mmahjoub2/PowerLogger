#include "INA.h"

INA::INA(int INAnumber) {
    if (INAnumber == 0) {
        this->addr = INA0_ADDR;
    }
    if (INAnumber == 1) {
        this->addr = INA1_ADDR;
    }
    if (INAnumber == 2) {
        this->addr = INA2_ADDR;
    }
    if(INAnumber == 3) {
        this->addr = INA3_ADDR;
    }
    resetRegValues();
}
void INA::resetRegValues() {
    configReg.bits = configDefault;
    calibrationReg.bits = 0x0000;
    maskEnableReg.bits = 0x0000;
    alertLimitReg.bits = 0x0000;
}
//Set Register Values
void INA::reset() {
    configReg.bitfield_t.RST = 0b1;
    WriteReg(ConfigAddr,configReg.bits);
    resetRegValues();
    
}

void INA::ADCRange(bool high) {
    if (high) {
        configReg.bitfield_t.ADCRANGE = 0b0;
    }
    else {
        configReg.bitfield_t.ADCRANGE = 0b1;
    }
    WriteReg(ConfigAddr,configReg.bits);
}

void INA::AVGSample(uint16_t numberToAvg) {
    uint16_t closeTwo = powerTwo(numberToAvg);
    Serial.println(closeTwo, HEX);
    Serial.println(closeTwo, DEC);
    // if (closestPowTwo)
    if (closeTwo == 1) {
        configReg.bitfield_t.AVG = 0b000;
    }
    else if (closeTwo == 4) {
        configReg.bitfield_t.AVG = 0b001;
    }
    else if (closeTwo == 16) {
        configReg.bitfield_t.AVG = 0b010;
    }
    else if(closeTwo == 64) {
        configReg.bitfield_t.AVG = 0b011;
    }
    else if(closeTwo == 128) {
        configReg.bitfield_t.AVG = 0b100;
    }
    else if(closeTwo == 256) {
        configReg.bitfield_t.AVG = 0b101;
    }
    else if(closeTwo == 512) {
        configReg.bitfield_t.AVG = 0b110;
    }
    else if (closeTwo == 1024) {
        configReg.bitfield_t.AVG = 0b110;
    }
    else {
        Serial.println("No valid entry");
        return;
    }
    // Serial.println(configReg.bits, HEX);
    WriteReg(ConfigAddr, configReg.bits);
    
}

// void INA::TimeConversion(char input[], int convTime) {
//     bool voltageConv;
//     char sName[] = "Voltage"
//     if (convTime == 140)

//     else if(convTime == 204)

//     else if(convTime == 332)

//     else if(convTime == 588)

//     else if(convTime == 1100)

//     else if(convTime == 2116)

//     else if(convTime == 4156)

//     else if (convTime ==)

//     if( strcmp(sName,input) == 0 )  {
//         voltageConv = true;
//     }
//     else {

//     }
// }
void INA::setCalibration(float shuntValue) {
    
    if (shuntValue == 100) {
        Serial.println("100");
		WriteReg(CalibrationRegAddr, CAL_SHUNT_100);
	}
	if (shuntValue == 1) {
        WriteReg(CalibrationRegAddr, CALL_SHUNT_1);
	}

	if(shuntValue == .01) {
        WriteReg(CalibrationRegAddr, CALL_SHUNT_01);
	}
    
}

float INA::readVoltage() {
   
    uint16_t voltage = ReadReg(ShuntVoltageAddr);
    if(voltage == 0xFFFF) {
        return 0;
    }
    voltage = voltage >> 4;
    float voltageValue = static_cast<float>(voltage) * voltageLSB;
    return voltageValue;
    
    
   
}

float INA::readCurrent() {
    uint16_t current = ReadReg(CurrentRegAddr);
    if (current == 0xFFFF) {
        return 0;
    }
    //Serial.println(current, HEX);
    current = current >> 4;
    float currentValue = static_cast<float>(current) * currentLSB;
    return currentValue;
}
float INA::readPower() {
    uint16_t power = ReadReg(CurrentRegAddr);
    if (power == 0xFFFF) {
        return 0;
    }      
    // Serial.println(power, HEX);
    power = power >> 4;
    float powerValue = static_cast<float>(power) * currentLSB* 32;
    return powerValue;
}


int INA::checkTransmission(int value) {
    if (value == 0) {
        //Serial.println("transaction succesful");
        //Serial.println("data too long to fit in transmit buffer");
        return value;
    }
    else if (value == 1) {
        Serial.println("data too long to fit in transmit buffer");
        return value;
    }
    else if (value == 2) {
        Serial.println("received NACK on transmit of address.");
        return value;
    }
    else if (value == 3) {
        Serial.println("received NACK on transmit of data");
        return value;
    }
    else if (value == 4) {
        Serial.println("other error");
        return value;
    }
    else if (value == 5) {
        Serial.println("timeout");
        return value;
    }
    else {
        Serial.println("return value unknown");
        return value;
    }
}

void INA::calculateShuntCal(int maxCurrent, int Rshunt) {
    float maxCurr;
    if (Rshunt = 100) {
        maxCurr = .0008192;
    }

}

void INA::WriteReg(uint16_t RegAddr, uint16_t data) {
    cu_t v;
    v.value = data;

    Wire.beginTransmission(this->addr);
    Wire.write(RegAddr);
    Serial.println(v.MSB, HEX);
    Serial.println(v.LSB,HEX);
	Wire.write(v.MSB);
    Wire.write(v.LSB);
	checkTransmission(Wire.endTransmission());
    
}

uint16_t INA::ReadReg(uint16_t RegAddr) {
    Wire.beginTransmission(this->addr);
    Wire.write(RegAddr);   
    if (checkTransmission(Wire.endTransmission() != 0)) {
        return 0xFFFF;
    }
    Wire.requestFrom(this->addr,2, true);  
    uint8_t highByte = Wire.read();        // read that byte into 'slaveByte2' variable
    uint8_t lowByte = Wire.read();
    uint16_t value = (highByte <<8) | lowByte;

    return value;
}

uint16_t INA::powerTwo(uint16_t value) {
    uint16_t above = (value - 1); // handle case where input is a power of two
    above |= above >> 1;          // set all of the bits below the highest bit
    above |= above >> 2;
    above |= above >> 4;
    above |= above >> 8;
    above |= above >> 16;
    ++above;                      // add one, carrying all the way through
                                  // leaving only one bit set.

    uint16_t below = above >> 1;  // find the next lower power of two.

    return (above - value) < (value - below) ? above : below;
}


//UTILS functions 

// void setINAConfig() {
//   // Wire.beginTransmission(0x43);
//   // Wire.endTransmission();

//   Wire.beginTransmission(0x43); //target
//   Wire.write(0x0);  //reg addy
// 	Wire.write(0x41); //value
//   Wire.write(0x01);
//   Wire.endTransmission (false);
  
//   // Wire.write(CalibrationRegAddr);
//   // Wire.write(SETCAL);
// 	//Wire.endTransmission();
//   int test = Wire.endTransmission (false);
//   delay(50);
//    if (test == 0) {
//     Serial.println("made it");

//    }
//    else if (test == 1) {
//      Serial.println("too long");
//    }

//    else {
//     Serial.println("no made it");
//    }
  
// }