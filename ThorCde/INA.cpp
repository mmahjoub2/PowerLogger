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

float INA::readVoltage() {
    Wire.beginTransmission(this->addr);
    Wire.write(ShuntVoltageAddr);   
    if (checkTransmission(Wire.endTransmission() != 0)) {
        return 0;
    }

    Wire.requestFrom(this->addr,2, true);  
    uint8_t highByte = Wire.read();        // read that byte into 'slaveByte2' variable
    uint8_t lowByte = Wire.read();
    uint16_t voltage = (highByte <<8) | lowByte;
    // Serial.println(highByte, HEX);
    
    voltage = voltage >> 4;
    Serial.println(voltage, HEX);
    float dummy = static_cast<double>(voltage);
    Serial.println(dummy);
    float voltageValue = static_cast<float>(voltage) * voltageLSB;
    Serial.println(voltageValue * 1000);
    return voltageValue;
    
    
   
}

float INA::readCurrent() {
    Wire.beginTransmission(this->addr);
    Wire.write(CurrentRegAddr);   
    if (checkTransmission(Wire.endTransmission() != 0)) {
        return 0;
    }

    Wire.requestFrom(this->addr,2, true);  
    uint8_t highByte = Wire.read();        // read that byte into 'slaveByte2' variable
    uint8_t lowByte = Wire.read();
    uint16_t current = (highByte <<8) | lowByte;
                  //
    //Serial.println(current, HEX);
    current = current >> 4;
    float currentValue = static_cast<float>(current) * currentLSB;
    return currentValue;
}
float INA::readPower() {
    Wire.beginTransmission(this->addr);
    Wire.write(PowerRegAddr);   
    if (checkTransmission(Wire.endTransmission() != 0)) {
        return 0;
    }
    
    Wire.requestFrom(this->addr,2, true);  
    uint8_t highByte = Wire.read();        // read that byte into 'slaveByte2' variable
    uint8_t lowByte = Wire.read();
    uint16_t power = (highByte <<8) | lowByte;        
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