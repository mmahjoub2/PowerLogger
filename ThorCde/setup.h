
#include <Arduino.h>
#include <Wire.h> 
#include <SPI.h> 
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 
#include "INA.h"
#include "RTClib.h"

// TODO: make this a .c file
RTC_PCF8523 rtc;


void RTCsetUp() {
//   #ifndef ESP8266
//     while (!Serial){
//       Serial.println("stuck in serial");
//     } // wait for serial port to connect. Needed for native USB
//   #endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10); 
  }

	//TODO: ADD some sort of serial interface from computer to set data time to now 
	/*
		set comX 
	*/

  if ((! rtc.initialized() || rtc.lostPower()) && Serial) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // to 1,1,1970
    // RTC.adjust(DateTime(1970,1,1,0,0,0));
  }
 
  rtc.start();
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  float drift_unit_1 = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  int offset = round(deviation_ppm / drift_unit);
  rtc.calibrate(PCF8523_TwoHours, offset); // Un-comment to perform calibration once drift (seconds) and observation period (seconds) are correct
  // rtc.calibrate(PCF8523_TwoHours, 0); // Un-comment to cancel previous calibration
//   rtc.calibrate(PCF8523_OneMinute, drift_unit_1);
  Serial.print("Offset is "); Serial.println(offset); // Print to control offset

}


char filename[15];
void setupSD() {
   if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
  }

  File logfile;
  strcpy(filename, "/DATALOG0.csv");
  // FIX this 
  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = '0' + i/10;
    filename[8] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (SD.exists(filename)) {
      SD.remove(filename);
    }
   
  }
  

  strcpy(filename, "/DATALOG0.csv");
  // logfile = SD.open(filename, FILE_WRITE);
  // if( ! logfile ) {
  //   Serial.print("Couldnt create "); 
  //   Serial.println(filename);
  // }
  // else {
  //   Serial.print("Writing to "); 
  //   Serial.println(filename);
  //   logfile.close();

  // }
  
  Serial.println("Ready!");
}

void scanI2c() {
  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  //Wire.begin();
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1); 
      } 
  } 
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
} 