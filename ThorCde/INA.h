
#include <Arduino.h>
#include "Wire.h"
#include "structs.h"
#include <math.h>

class INA {
    public: 
        INA(int InaNumber);
        void setConfig();
        void setCalibration(double shuntNumber);
        void setAlert(int AlertNumber);
        float readVoltage();
        float readBusVoltage();
        float readCurrent();
        float readPower();
        void ADCRange(bool high);
        void reset(); //clears all registers to default values 
        void AVGSample(uint16_t numberToAvg); //1-1024 (values of 2)
        void TimeConversion(char input[], int convTime);
        void CurrentConversion(int convTime);
        void Mode(int mode);
        uint16_t calculateShuntCal(float Rshunt);
        float calculateCurrent(float voltage);
        float calculatePower(float current);
        uint16_t ReadReg(uint16_t RegAddr);
        void WriteReg(uint16_t RegAddr,uint16_t data);
        float calculateShuntResitance(float loadRes, float v_t, float v_sh, float shuntRes);
        void setShuntRes(float shutRes);
        bool getADCFlag();

    private:
        uint16_t addr;
        uint16_t shunt_cal; 
        float shuntCal100 = 1.01 * pow(10,-7);
        float shuntCal1 = 0.00015625;
        float shuntCal01 = 1.9 * pow(10,-4);
        float currentLSB = 0;
        float voltageLSB = voltageLSBHigh;
        float busVoltageLSB = 25.6 * pow(10,-3);
        bool adcFlag = true;
        float shuntRes = 0;

        ConfigReg_t configReg;
        CalibrationReg_t calibrationReg;
        MaskEnableReg_t maskEnableReg;
        AlertLimit_t alertLimitReg;

        int checkTransmission(int value);
        uint16_t powerTwo(uint16_t value);
        uint16_t float_bits(float f);
        void setShuntOverLimit();
        void setShuntUnderLimit();
        void setLimitValue(bool high);
        void resetRegValues();
        float uint16_bits(uint16_t in);
};