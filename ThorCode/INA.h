
#include <Arduino.h>
#include "Wire.h"
#include "structs.h"
#include <math.h>
class INA {

    public: 
        INA(int InaNumber);
        void setConfig();
        void setCalibration(float shuntNumber);
        void setAlert(int AlertNumber);
        void readVoltage();
        void readCurrent();
        void readPower();
        void calculateShuntCal(int maxCurrent, int Rshunt);
    
    private:
        uint16_t addr;
        void checkTransmission(int value);
        uint16_t shunt_cal; 
        float shuntCal100 = 0.0000010;
        float shuntCal1 = 0.0001;
        float shuntCal01 = 0.01;
        ConfigReg_t configReg;
        CalibrationReg_t calibrationReg;
        MaskEnableReg_t maskEnableReg;
        AlertLimit_t alertLimitReg;

};