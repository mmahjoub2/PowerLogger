
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
        float readCurrent();
        float readPower();
        void calculateShuntCal(int maxCurrent, int Rshunt);
        
        void reset(); //clears all registers to default values 
        void ADCRange(int range);
        void AVGSample(int numberToAvg); //1-1024 (values of 2)
        void VoltageConversion(int convTime );
        void CurrentConversion(int convTime);
        void Mode(int mode);
        // ability to read any register 
        uint16_t ReadReg(uint16_t RegAddr);
        void WriteReg(uint16_t RegAddr,uint16_t data);
    
    private:
        uint16_t addr;
        int checkTransmission(int value);
        uint16_t shunt_cal; 
        float shuntCal100 = 0.0000010;
        float shuntCal1 = 0.0001;
        float shuntCal01 = 0.01;
        float currentLSB = 0.0000564;
        float voltageLSB = 0.000040;
        ConfigReg_t configReg;
        CalibrationReg_t calibrationReg;
        MaskEnableReg_t maskEnableReg;
        AlertLimit_t alertLimitReg;
        float uint16_bits(uint16_t in);
        uint16_t float_bits(float f);
};