
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
        //TODO: 
        
        void AVGSample(uint16_t numberToAvg); //1-1024 (values of 2)
        void TimeConversion(char input[], int convTime );
        void CurrentConversion(int convTime);
        void Mode(int mode);
        uint16_t calculateShuntCal(float Rshunt);

        float calculateCurrent(float voltage);
        float calculatePower(float current);

        // ability to read any register 
        uint16_t ReadReg(uint16_t RegAddr);
        void WriteReg(uint16_t RegAddr,uint16_t data);
        float calculateShuntResitance(float loadRes, float v_t, float v_sh, float shuntRes);
        void setShuntRes(float shutRes);

        bool getADCFlag();
    
    private:
        uint16_t addr;
        int checkTransmission(int value);
        uint16_t shunt_cal; 
        float shuntCal100 = 1.01 * pow(10,-7);
        float shuntCal1 = 0.00015625;
        float shuntCal01 = 1.9 * pow(10,-4);
        float currentLSB = 0;
        float voltageLSB = 0.000040;
        float busVoltageLSB = 25.6 * pow(10,-3);
        ConfigReg_t configReg;
        CalibrationReg_t calibrationReg;
        MaskEnableReg_t maskEnableReg;
        AlertLimit_t alertLimitReg;
        uint16_t powerTwo(uint16_t value);
       
        float uint16_bits(uint16_t in);
        uint16_t float_bits(float f);
        void resetRegValues();
        bool adcFlag = true;
        float shuntRes = 0;
        void setShuntOverLimit();
        void setShuntUnderLimit();
        void setLimitValue(bool high);
        //TODO: ADD MAP FOR ALL INT -> BINARY CONVERSIONS
     
};