
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
        void calculateShuntCal(int maxCurrent, int Rshunt);
        
        // ability to read any register 
        uint16_t ReadReg(uint16_t RegAddr);
        void WriteReg(uint16_t RegAddr,uint16_t data);
        float calculateShuntResitance(float loadRes, float v_t, float v_sh, float shuntRes);
    
    private:
        uint16_t addr;
        int checkTransmission(int value);
        uint16_t shunt_cal; 
        float shuntCal100 = 1.12 * pow(10,-8);
        float shuntCal1 = 0.00015625;
        float shuntCal01 = 0.001953125;
        float currentLSB = 0;
        float voltageLSB = 0.000040;
        ConfigReg_t configReg;
        CalibrationReg_t calibrationReg;
        MaskEnableReg_t maskEnableReg;
        AlertLimit_t alertLimitReg;
        uint16_t powerTwo(uint16_t value);
       
        float uint16_bits(uint16_t in);
        uint16_t float_bits(float f);
        void resetRegValues();
        bool adcFlag = true;
        //TODO: ADD MAP FOR ALL INT -> BINARY CONVERSIONS
     
};