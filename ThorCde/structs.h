
#define ALERT0_PIN 16
#define ALERT1_PIN 15
#define ALERT2_PIN 7
#define ALERT3_PIN 5
#define CALBUTTON0_PIN 2
#define CALLBUTTON1_PIN 30
#define CALLBUTTON2_PIN 27
#define CALLBUTTON3_PIN 29
#define CONFIG_RESET 0x101F
#define CAL_SHUNT_100 0x07EA
#define CALL_SHUNT_1  0X020D
#define CALL_SHUNT_01 0x2492

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels 
#define SETCONFIG 0X4127
#define SETCAL 0x15D8
#define cardSelect 11

// typedef enum {
//   shunt100Resolution = .0001,
//   shunt1Resolution = .0001,
//   shunt001Resolution = .0001
// } shuntResolution; 

typedef enum {
  configDefault = 0x4127

} defaults;
typedef enum   {
    INA0_ADDR = 0x40,
    INA1_ADDR = 0x41,
    INA2_ADDR = 0X42,
    INA3_ADDR = 0X43, //current address 
    OLED_ADDR = 0x3C
} i2cAddy_e;

//INA Reg Addresses 
typedef enum  {
  ConfigAddr = 0x0, //R/W
  ShuntVoltageAddr = 0x1, //ReadOnly
  BusVoltageAddr = 0x2, //ReadOnly 
  PowerRegAddr = 0x3, //ReadOnly
  CurrentRegAddr = 0x4, //ReadOnly
  CalibrationRegAddr = 0x5, //R/W
  MaskEnableAddr = 0x6,
  AlertLimitAddr = 0x7,
  ManufacturerIDAddr = 0x3E,
  DeviceIDAddr = 0x3F
}INA234RegAddr_e;

//Config register 
typedef union  {
  uint16_t bits;
  struct  {
    uint16_t MODE     : 3; //TODO: put comments 
    uint16_t VSHCT    : 3;
    uint16_t VBUSCT   : 3;
    uint16_t AVG      : 3;
    uint16_t ADCRANGE : 1;
    uint16_t reserved : 2;
    uint16_t RST      : 1;
  } bitfield_t;
} ConfigReg_t;


//Calibration Register 
typedef union  {
  uint16_t bits;
  struct  {
    uint16_t reserved   : 1;
    uint16_t SHUNT_CAL : 15;
  } bitfield_t;
  
} CalibrationReg_t;

//MASK/Enable Register 
typedef union  {
	uint16_t bits;
	struct  {
		uint16_t LEN      : 1;             //Alert Latch Enable 
		uint16_t APOL     : 1;             //ALert Polarity
		uint16_t OVF      : 1;             //Math Over-Flow 
		uint16_t CVRF     : 1;             //Conversion flag ready 
		uint16_t AFF      : 1;             // Alert Function Flag
		uint16_t MemError : 1;             //CRC or ECC error
		uint16_t Reserved : 4;             //read only
		uint16_t CNVR     : 1;             //Conversion Read
		uint16_t POL      : 1;             //PowerOverLimit
		uint16_t BUL      : 1;             //BusUnderLimit
		uint16_t BOL      : 1;             //BusOverLimit
		uint16_t SUL      : 1;             //ShuntUnderLimit
		uint16_t SOL      : 1;             //ShuntOverLimit
	} bitfield_t;
} MaskEnableReg_t;

typedef union  {
	uint16_t bits;
	struct  {
		uint16_t LIMIT    : 16;            //AAlert Limit Register contains the value used to compare to the register selected in the Mask/Enable Registe
	} bitfield_t;
} AlertLimit_t;

typedef union { 
	struct {
		uint8_t LSB;
		uint8_t MSB;
	};
	uint16_t value;
} cu_t;

