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
}

void INA::setConfig() {
    Wire.beginTransmission(INA3_ADDR);
    Wire.write(ConfigAddr);
	Wire.write(SETCONFIG);
    Wire.write(CalibrationRegAddr);
    Wire.write(SETCAL);
	checkTransmission(Wire.endTransmission());
}

void INA::setCalibration(float shuntValue) {
    uint8_t value[2];

    if (shuntValue == 100) {
        Serial.println("100");
		value[0] = CAL_SHUNT_100;
        value[1] = CAL_SHUNT_100 >> 8;
	}
	if (shuntValue == 1) {
        value[0] = CALL_SHUNT_1;
        value[1] = CALL_SHUNT_1 >> 8;
	}

	if(shuntValue == .01) {
        value[0] = CALL_SHUNT_01;
        value[1] = CALL_SHUNT_01 >> 8;
	}
    Serial.println(this->addr, HEX);
    Wire.beginTransmission(this->addr);
    Wire.write(CalibrationRegAddr);
    Wire.write(0x15);
    Wire.write(0xD8);
    
    checkTransmission(Wire.endTransmission());
    

}

void INA::readVoltage() {
    Wire.beginTransmission(this->addr);
    Wire.write(ShuntVoltageAddr);   
    checkTransmission(Wire.endTransmission());

    Wire.requestFrom(this->addr,2, true);  
    uint8_t highByte = Wire.read();        // read that byte into 'slaveByte2' variable
    uint8_t lowByte = Wire.read();
    uint16_t voltage = (highByte <<8) | lowByte;
    // Serial.println(highByte, HEX);
    // Serial.println(lowByte, HEX);
    Serial.println(voltage, HEX);
    
    
   
}

void INA::readCurrent() {
    Wire.beginTransmission(this->addr);
    Wire.write(CurrentRegAddr);   
    checkTransmission(Wire.endTransmission());

    Wire.requestFrom(this->addr,2, true);  
    uint8_t highByte = Wire.read();        // read that byte into 'slaveByte2' variable
    uint8_t lowByte = Wire.read();
    uint16_t current = (highByte <<8) | lowByte;
                  //
     Serial.println(current, HEX);
}
void INA::readPower() {
    Wire.beginTransmission(this->addr);
    Wire.write(PowerRegAddr);   
    checkTransmission(Wire.endTransmission());
    
    Wire.requestFrom(this->addr,2, true);  
    uint8_t highByte = Wire.read();        // read that byte into 'slaveByte2' variable
    uint8_t lowByte = Wire.read();
    uint16_t power = (highByte <<8) | lowByte;        
    Serial.println(power, HEX);
}


void INA::checkTransmission(int value) {
    if (value == 0) {
        Serial.println("transaction succesful");
    }
    else if (value == 1) {
        Serial.println("data too long to fit in transmit buffer");
    }
    else if (value == 2) {
        Serial.println("received NACK on transmit of address.");
    }
    else if (value == 3) {
        Serial.println("received NACK on transmit of data");
    }
    else if (value == 4) {
        Serial.println("other error");
    }
    else if (value == 5) {
        Serial.println("timeout");
    }
    else {
        Serial.println("return value unknown");
    }
}

void INA::calculateShuntCal(int maxCurrent, int Rshunt) {
    float maxCurr;
    if (Rshunt = 100) {
        maxCurr = .0008192;
    }

}