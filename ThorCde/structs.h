//I2C addresses 


#define ALERT0_PIN 16
#define ALERT1_PIN 15
#define ALERT2_PIN 7
#define ALERT3_PIN 5
#define CALBUTTON0_PIN 2
#define CALLBUTTON1_PIN 30
// #define CALLBUTTON2_PIN 30;
// #define CALLBUTTON3_PIN 27;
#define CONFIG_RESET 0x101F
#define CAL_SHUNT_100 0x3000
#define CALL_SHUNT_1  0X3000
#define CALL_SHUNT_01 0X3000

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels 
#define SETCONFIG 0X8327
#define SETCAL 0x15D8
#define cardSelect 11

typedef enum {
  shunt100Resolution = .0001,
  shunt1Resolution = .0001,
  shunt001Resolution = .0001
} shuntResolution; 

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
  struct bitfield_t {
    uint16_t MODE     : 3;
    uint16_t VSHCT    : 3;
    uint16_t VBUSCT   : 3;
    uint16_t avg      : 3;
    uint16_t ADCRANGE : 1;
    uint16_t reserved : 2;
    uint16_t RST      : 1;
  };
} ConfigReg_t;


//Calibration Register 
typedef union  {
  uint16_t bits;
  struct bitfield_t {
    uint16_t reserved   : 1;
    uint16_t SHUNT_CAL : 15;
  } ;
  
} CalibrationReg_t;

//MASK/Enable Register 
typedef union  {
  uint16_t bits;
  struct bitfield_t {
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
  };
} MaskEnableReg_t;

typedef union  {
  uint16_t bits;
  struct bitfield_t {
    uint16_t LIMIT    : 16;            //AAlert Limit Register contains the value used to compare to the register selected in the Mask/Enable Registe
  };
}AlertLimit_t;



// [[//I2C addresses 
// typedef enum {
//     INA0_ADDR = 0x40,
//     INA1_ADDR = 0x41,
//     INA2_ADDR = 0X42,
//     INA3_ADDR = 0X43,
//     OLED_ADDR = 0x3C
// } i2cAddy_e;

// //INA Reg Addresses 
// typedef enum  {
//   ConfigAddr = 0x00, //R/W
//   ShuntVoltageAddr = 0x01, //ReadOnly
//   BusVoltageAddr = 0x02, //ReadOnly 
//   PowerRegAddr = 0x03, //ReadOnly
//   CurrentRegAddr = 0x04, //ReadOnly
//   CalibrationRegAddr = 0x05, //R/W
//   MaskEnableAddr = 0x06,
//   AlertLimitAddr = 0x07,
//   ManufacturerIDAddr = 0x3E,
//   DeviceIDAddr = 0x3F
// } INA234RegAddr_e;

// //Config register 
// typedef union  {
//   uint16_t bits;
//    struct {
//     uint16_t MODE     : 3;
//     uint16_t VSHCT    : 3;
//     uint16_t VBUSCT   : 3;
//     uint16_t avg      : 3;
//     uint16_t ADCRANGE : 1;
//     uint16_t reserved : 2;
//     uint16_t RST      : 1;
//   }  bitfield_s;
  
// } ConfigReg_t;

// //Calibration Register 
// typedef union  {
//   uint16_t bits;
//   struct  {
//     uint16_t reserved   : 1;
//     uint16_t SHUNT_CAL : 15;
//   } bitfield_s;
  
// } CalibrationReg_t;

// //MASK/Enable Register 
// typedef union  {
//   uint16_t bits;
//   struct  {
//     uint16_t LEN      : 1;             //Alert Latch Enable 
//     uint16_t APOL     : 1;             //ALert Polarity
//     uint16_t OVF      : 1;             //Math Over-Flow 
//     uint16_t CVRF     : 1;             //Conversion flag ready 
//     uint16_t AFF      : 1;             // Alert Function Flag
//     uint16_t MemError : 1;             //CRC or ECC error
//     uint16_t Reserved : 4;             //read only
//     uint16_t CNVR     : 1;             //Conversion Read
//     uint16_t POL      : 1;             //PowerOverLimit
//     uint16_t BUL      : 1;             //BusUnderLimit
//     uint16_t BOL      : 1;             //BusOverLimit
//     uint16_t SUL      : 1;             //ShuntUnderLimit
//     uint16_t SOL      : 1;             //ShuntOverLimit
//   } bitfield_s;
// } MaskEnableReg_t;

// typedef union  {
//   uint16_t bits;
//   struct  {
//     uint16_t LIMIT    : 16;            //AAlert Limit Register contains the value used to compare to the register selected in the Mask/Enable Registe
//   } bitfield_s;
  
// } AlertLimit_t;]]

////DEVICE ADDRESSES 
//#define INA0_ADDR 0x40
//#define INA1_ADDR 0x41
//#define INA2_ADDR 0X42
//#define INA3_ADDR 0X43


//REG ADDY TO WRITE
// #define CONFIG_REG 0x00
// #define CAL_REG 0x5
// PIN DEFINITIONS
// const int ALERT0_PIN = 16;
// const int ALERT1_PIN = 15;
// const int ALERT2_PIN = 7;
// const int ALERT3_PIN = 5;
// const int CALBUTTON0_PIN = 2;
// const int CALLBUTTON1_PIN = 29;
// const int CALLBUTTON2_PIN = 30;
// const int CALLBUTTON3_PIN = 27;
