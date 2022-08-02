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
    voltageLSB = 0.000040;
}
//Set Register Values
void INA::reset() {
    configReg.bitfield_t.RST = 0b1;
    WriteReg(ConfigAddr,configReg.bits);
    resetRegValues();
    
}

void INA::ADCRange(bool high) {
    adcFlag = high;
    if (high) {
        configReg.bitfield_t.ADCRANGE = 0b0;
        voltageLSB = .000040;
    }
    else {
        configReg.bitfield_t.ADCRANGE = 0b1;
        voltageLSB = .000010;

        
    }
    WriteReg(ConfigAddr,configReg.bits);
}

void INA::AVGSample(uint16_t numberToAvg) {
    uint16_t closeTwo = powerTwo(numberToAvg);
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

// void INA::TimeConversion( int convTime) {
//     bool voltageConv;
//     if (convTime == 140)

//     else if(convTime == 204)

//     else if(convTime == 332)

//     else if(convTime == 588)

//     else if(convTime == 1100)

//     else if(convTime == 2116)

//     else if(convTime == 4156)

//     else if (convTime ==)

   
// }
void INA::setCalibration(double shuntValue) {
    
    if (shuntValue == 100) {
        Serial.println("100");
        this->currentLSB = shuntCal100;
		WriteReg(CalibrationRegAddr, CAL_SHUNT_100);
        this->shuntRes = 100;
        ADCRange(false);
	}
	if (shuntValue == 1) {
        Serial.println("SET SHUNT CAL");
        this->currentLSB = shuntCal1;
        this->shuntRes = 1;
        WriteReg(CalibrationRegAddr, CALL_SHUNT_1);
        
	}

	if(shuntValue == .01) {
        this->currentLSB = shuntCal01;
        WriteReg(CalibrationRegAddr, CALL_SHUNT_01);
        this->shuntRes = 0.022;
        ADCRange(false);
        
	}
    
}

float INA::readVoltage() {
   
    uint16_t voltage = ReadReg(ShuntVoltageAddr);
    if(voltage == 0xFFFF || voltage == 0xFFF0) {
        return 0;
    }
    voltage = voltage >> 4;
    float voltageValue = static_cast<float>(voltage) * voltageLSB;
   
    return voltageValue;
    
}
float INA::readBusVoltage() {
   
    uint16_t busVoltage = ReadReg(BusVoltageAddr);
    if(busVoltage == 0xFFFF|| busVoltage == 0xFFF0) {
        return 0;
    }
    busVoltage = busVoltage >> 4;
    float voltageValue = static_cast<float>(busVoltage) * busVoltageLSB;
    return voltageValue;
    
}

float INA::readCurrent() {
    uint16_t current = ReadReg(CurrentRegAddr);
    if (current == 0xFFFF || current == 0xFFF0) {
        return 0;
    }
    //Serial.println(current, HEX);
    current = current >> 4;
    float currentValue = static_cast<float>(current) * currentLSB;
    return currentValue;
}

float INA::readPower() {
    uint16_t power = ReadReg(CurrentRegAddr);
    if (power == 0xFFFF ||power == 0xFFF0 ) {
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
        // Serial.println("data too long to fit in transmit buffer");
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

uint16_t INA::calculateShuntCal(float Rshunt) {
    return 0.08192/(this->currentLSB * Rshunt);

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

float INA::calculateShuntResitance(float loadRes, float v_t, float v_sh, float shuntRes) {

    float expectedCurrent = v_t /(loadRes+shuntRes);
    // Serial.print("resistance: ");
    // Serial.println((loadRes+shuntRes)) ;
    // Serial.print("v_t: ");
    // Serial.println((v_t)) ;
    // Serial.print("v_sh: ");
    // Serial.println((v_sh)) ;
    // delay(1000);
    return v_sh/expectedCurrent;

}

float INA::calculateCurrent(float voltage) {
    Serial.println(shuntRes);
    return voltage/shuntRes;
}

void INA::setShuntRes(float shutRes) {
    this->shuntRes = shutRes;
}

float INA::calculatePower(float current) {
    return this->readBusVoltage() * current;
}

void INA::setShuntOverLimit() {
    maskEnableReg.bitfield_t.SOL = 0b1;
    this->WriteReg(MaskEnableAddr,maskEnableReg.bits);
}

void INA::setShuntUnderLimit() {
    maskEnableReg.bitfield_t.SUL = 0b1;
    maskEnableReg.bitfield_t.SOL = 0b0;
    this->WriteReg(MaskEnableAddr,maskEnableReg.bits);
}

void INA::setLimitValue(bool high) {
    if (high) {
        Serial.println("set high");
        alertLimitReg.bits = 0x0010;
    }
}

bool INA::getADCFlag() {
    return this->adcFlag;
}

