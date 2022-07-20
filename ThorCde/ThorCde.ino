#include <Wire.h> 
#include <SPI.h> 
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 
#include "INA.h"
#include "RTClib.h"


// #include "INA.cpp"

Adafruit_SSD1306 display(-1);
RTC_PCF8523 rtc;
File logfile;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// Global Enums 

INA ina_0(0);
INA ina_1(1);
INA ina_2(2);
INA ina_3(3);

INA inaArray[] = {ina_0,ina_1,ina_2,ina_3};
float inputData[13];
int inaNum = 0 ;
//Class Objects
int lastFire[] = {0,0,0,0};
bool clear;
int count[] = {0,0,0,0};
float prevTime = -10000;

bool flag;



void staticMenu() {
  display.setTextSize(.01);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("SHUNT 100");
  
  display.setTextSize(.01);
  display.setCursor(0, 10);
  display.println("SHUNT 1");
  
  
  display.setTextSize(.01);
  display.setCursor(0, 20);
  display.println("SHUNT 0.01");
  
 
  display.display();
}

void DisplayData(float arr[]) {
  display.clearDisplay();
  display.display();
  String dummy[] = {"1","2","3","4"};
  for(int i = 0; i<4; i++) {
    display.setTextSize(.0001);
    
    display.setTextColor(WHITE);
    display.setCursor(0,8*i);
    display.println(String(i));

    display.setCursor(20,8*i);
    display.println(String(arr[i]));
   
    display.setCursor(60,8*i);
    display.println(String(arr[i+4]));

    display.setCursor(100,8*i);
    display.println(String(arr[i+8]));

    // display.setCursor(110,8*i);
    // display.println(String(int(arr[12])));

  }
  
  display.display();
  delay(100);

}

void readINA()
{

    Wire.beginTransmission(0x43);
    Wire.write(0x05);   
    Wire.endTransmission();   
    
    Wire.requestFrom(0x43,2,true);  
    byte highByte = Wire.read();    // read that byte into 'slaveByte2' variable
    byte lowByte = Wire.read();
    uint16_t read = (highByte <<8) | lowByte; 
    // Serial.println(highByte, HEX);
    // Serial.println(lowByte, HEX);
    Serial.println(read, HEX);
    
 
}

void MoveCursor() {

  if (count[0] == 0) {
    // if callbutton1 pressed here
    // call set =
    display.setCursor(70, 0);
    display.println("<--");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(70, 10);
    display.println("   ");
    display.setCursor(70, 20);
    display.println("   ");
    display.display();
    if (count[1] == 1) {
      if(inaNum == 1) {
        Serial.print("INA0 100 set");
        ina_0.setCalibration(100);
      }
      else if(inaNum == 1) {
        Serial.print("INA1 100 set");
        ina_1.setCalibration(100);
      }
      else if (inaNum == 2) {
         Serial.print("INA2 100 set");
        ina_2.setCalibration(100);
      }
      else if(inaNum == 3) {
         Serial.print("INA3 100 set");
        ina_3.setCalibration(100);
      }
      count[1] = 0;
      inaNum++;

    }

    
  }
  if (count[0] == 1) {
    display.setCursor(70, 10);
    display.println("<--");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(70, 0);
    display.println("   ");
    display.setCursor(70, 20);
    display.println("   ");
    display.display();
     if (count[1] == 1) {
      if(inaNum == 0) {
        Serial.print("INA0 1 set");
        ina_0.setCalibration(1);
      }
      else if(inaNum == 1) {
        Serial.print("INA1 1 set");
        ina_1.setCalibration(1);
      }
      else if (inaNum == 2) {
        Serial.print("INA2 1 set");
        ina_2.setCalibration(1);
      }
      else if(inaNum == 3) {
        Serial.print("INA3 1 set");
        ina_3.setCalibration(1);
      }
      count[1] = 0;
      inaNum++;
    }
  }

  if (count[0] == 2) {
    display.setCursor(70, 20);
    display.println("<--");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(70, 0);
    display.println("   ");
    display.setCursor(70, 10);
    display.println("   ");
    display.display();
     if (count[1] == 1) {
      if(inaNum == 0) {
        Serial.print("INA0 0.01 set");
        ina_0.setCalibration(0.01);
      }
      else if(inaNum == 1) {
        Serial.print("INA1 0.01 set");
        ina_1.setCalibration(.01);
      }
      else if (inaNum == 2) {
        Serial.print("INA2 0.01 set");
        ina_2.setCalibration(.01);
      }
      else if(inaNum == 3) {
        Serial.print("INA3 0.01 set");
        ina_3.setCalibration(.01);
      }
      count[1] = 0;
      inaNum++;
    } 
  }

  
}
void scanI2c() 
{
 

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



void RTCsetUp() {
  #ifndef ESP8266
    while (!Serial){
      Serial.println("stuck in serial");
    } // wait for serial port to connect. Needed for native USB
  #endif

  if (! rtc.begin()) {
    Serial.print("HERW");
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
    
  }
  else {
    Serial.println("made it");
;  }


  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
  // When time needs to be set on a new device, or after a power loss, the
  // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  rtc.start();
  Serial.print("HERW");
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  // float drift_unit = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  int offset = round(deviation_ppm / drift_unit);
  // rtc.calibrate(PCF8523_TwoHours, offset); // Un-comment to perform calibration once drift (seconds) and observation period (seconds) are correct
  // rtc.calibrate(PCF8523_TwoHours, 0); // Un-comment to cancel previous calibration

  Serial.print("Offset is "); Serial.println(offset); // Print to control offset

}


char filename[15];
void setupSD() {
   if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
  }
  
  strcpy(filename, "/DATALOG0.csv");
  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = '0' + i/10;
    filename[8] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (SD.exists(filename)) {
      SD.remove(filename);
    }
    else {
      break;
    }
  }

 
  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);
  logfile.println("Voltage INA1, Voltage INA2, VoltageINA3, VoltageINA3, Current INA1, Current INA2, Current INA3, Current INA4, Power INA1, Power INA2, Power INA3, Power INA4, Time");
  logfile.close();

  Serial.println("Ready!");
}





void setup() {
  
  Wire.begin();
  // put your setup code here, to run once:
  Serial.begin(115200);

  RTCsetUp();
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();
  
  staticMenu();

  Serial.println("INA");
   
  flag = true;

  Serial.println("INA");



  delay(500);




  attachInterrupt(digitalPinToInterrupt(CALBUTTON0_PIN), button0PressInterupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(CALLBUTTON1_PIN), button1PressInterupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(CALLBUTTON2_PIN), button2PressInterupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(CALLBUTTON3_PIN), button3PressInterupt, FALLING);
  // setupSD();

  inaArray[0].reset();
  

}



void loop() {

  Serial.println(inaArray[0].readVoltage());
  Serial.println(inaArray[0].readCurrent());
  // if(inaNum < 4) {
  //    MoveCursor();

  //     clear = true;
  // }
  // else {
  //   if (clear) {
  //     display.clearDisplay();
  //     display.display();
  //     clear = false;
  //   }
  //   //readINA();
  //   // delay(1000);
  //   DateTime now = rtc.now();
  //   // ina_3.readVoltage();
  //   // ina_3.readCurrent();
  //   // ina_3.readPower();
  //   for (int i = 0; i < 4; i++) {
  //     inputData[i] = inaArray[i].readVoltage();
  //     inputData[i+4] = inaArray[i].readCurrent();
  //     inputData[i+8] = inaArray[i].readPower();
  //   }
   
  //   inputData[12] = now.second();
  //   //ina_2.readVoltage();

  //   // Serial.print(now.hour(), DEC);
  //   // Serial.print(':');
  //   // Serial.print(now.minute(), DEC);
  //   // Serial.print(':');
  //   // Serial.print(now.second(), DEC);
  //   // Serial.println();
  //   String message;
  //   String testString;
  //   File dataFile = SD.open(filename, FILE_WRITE);
  //   String time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  //   for (int i =0; i < 13; i++) {

  //     message = String(inputData[i],8) + ",";
  //     if (i < 12) {

  //       testString.concat(inputData[i]*1000);
  //        testString.concat(",");
  //     }
  //     else {
  //       testString.concat(time);
  //       // testString.concat(" | ");
  //     }
      
      
  //   }

  //   if (dataFile) {
  //     Serial.println(message);
  //     Serial.println(testString);
  //     dataFile.println(testString);
  //     dataFile.close();
  //     delay(10);
  //   }

    

  //   if (inputData[12] != prevTime) {
  //     DisplayData(inputData);
  //     prevTime = inputData[12];

  //   }
    
    

  //   count[0] = 0;
  //   count[1] = 0;
  // }

   
 

  
 
  

}



void button0PressInterupt() {
   if (millis() - lastFire[0] < 300) { // Debounce
    return;
  }

  lastFire[0] = millis();

  if (count[0] > 2) {
    count[0] = 0;
  }
  else {
    count[0]++;
  }

}
void button1PressInterupt() {
   if (millis() - lastFire[1] < 300) { // Debounce
    return;
  }
  //Serial.println("button pressed");
  lastFire[1] = millis();

   

  if (count[1] == 1) {
    count[1] = 0;
  }
  else {
    count[1]++;
  }
}

void button2PressInterupt() {
   if (millis() - lastFire[2] < 200) { // Debounce
    return;
  }
   
  lastFire[2] = millis();
  count[2]++;
}

void button3PressInterupt() {
   if (millis() - lastFire[3] < 200) { // Debounce
    return;
  }
  if (flag)
    flag = false;
  else 
    flag = true;
  lastFire[3] = millis();
  count[3]++;
}


 //   Serial.println(inaArray[3].ReadReg(ConfigAddr),HEX);
  //   delay(1000);
  //   // inaArray[3].AVGSample(4);
  //   // delay(1000);
  //   // Serial.println(inaArray[3].ReadReg(ConfigAddr),HEX);
  //   //inaArray[3].AVGSample(1024);
  // inaArray[3].ADCRange(false);
  //   delay(1000);
  //   Serial.println(inaArray[3].ReadReg(ConfigAddr),HEX);
  //   delay(1000);
  // // inaArray[3].ADCRange(true);
  // // delay(1000);

  // inaArray[3].reset();


    //     // if (flag)
  //     //   Serial.println("LOOP");
  //     // else 
  //     //   Serial.println("Flase");
  //     // delay(500);