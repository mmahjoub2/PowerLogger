
#include "setup.h"


Adafruit_SSD1306 display(-1);

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
float shuntValues[4];
bool flag;
bool buttonFlag[] = {false, false, false, false};
bool button2pressFlag = false;
bool button3pressFlag = false;
//Alert Flags 
bool alertFlag[] {false, false, false, false};

bool startFlag = false;

/*
	TODO:	
		Calibration Choice Menu
		Integrate calculate power and current functions with current implemention
		Find the best way to create new files on SD card without having to delete 
*/

void clearDisplay() {
	display.clearDisplay();
	display.display();
}
void staticCalibrationChoice() {
	display.setCursor(20, 0);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.println("Calibration Menu");

	display.setCursor(0, 10);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.println("Yes");

	display.setCursor(0, 20);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.println("No");

}

bool calibrationChoice() { 
	while(1) {
		Serial.println(buttonFlag[0]);
		if (buttonFlag[0]) {
			buttonFlag[0] = false;
			if (count[0] == 1) {
				count[0] = 0;
			}
			else {
				count[0]++;
			}
		}
		Serial.print("calibration choice count:");
		Serial.println(count[0]);

		staticCalibrationChoice();
		
		if (count[0] == 0) {
			display.setTextColor(WHITE);
			display.setCursor(50, 10);
			display.println("<--");
			display.display();

			display.setTextColor(WHITE, BLACK);
			display.setCursor(50, 20);
			display.println("   ");
			display.display();
			if (buttonFlag[1]) {
				buttonFlag[1] = false;
				return true;
				
			}

		}
		else if (count[0] == 1) {
			display.setTextColor(WHITE, BLACK);
			display.setCursor(50, 10);
			display.println("   ");
			display.display();

			display.setTextColor(WHITE);
			display.setCursor(50, 20);
			display.println("<--");
			display.display();

			if (buttonFlag[1]) {
				buttonFlag[1] = false;
				return false;
				
			}
		}
	}
	
}
void staticCalibrationMenu() {
	display.setTextSize(.01);
	display.setTextColor(WHITE);

	display.setCursor(0, 0);
	display.println("Load 50 ohm");

	display.setTextSize(.01);
	display.setCursor(0, 10);
	display.println("Load 100 ohm");


	display.setTextSize(.01);
	display.setCursor(0, 20);
	display.println("Load 200 Ohm");

	display.setCursor(100, 20);
	display.println("INA");

	display.display();
}

void showCalRes(float shuntRes) {
	display.clearDisplay();
	display.display();
	display.setCursor(0, 0);
	display.setTextColor(WHITE);
	display.println("Calibrated Shunt Res: ");
	display.setCursor(60, 20);
	display.println(String(shuntRes));
	display.display();
	delay(2000);
	display.clearDisplay();
	display.display();		
}
void calibrationLoadMenu() {
	clearDisplay();
	display.setCursor(0, 10);
	display.setTextColor(WHITE);
	display.println("Calibrating...");
	display.display();

}
int calNum = 0;
int prevCalNum = 100;

void calibrationMenu() {
	if (buttonFlag[0]) {
		count[0]++;
		if (count[0] == 3) {
			count[0] = 0;
		}
		buttonFlag[0] = false;
	}

	staticCalibrationMenu();
	//Title
	
	if (calNum != prevCalNum) {
		display.setCursor(118, 20);
		display.setTextColor(WHITE, BLACK);
		display.println(" ");
		display.display();
		display.setCursor(118, 20);
		display.setTextColor(WHITE);
		display.println(String(calNum));
		prevCalNum = calNum;
	}

	if (count[0] == 0) {
		display.setCursor(75, 0);
		display.println("<--");
		display.display();
		display.setTextColor(WHITE, BLACK);
		display.setCursor(75, 10);
		display.println("   ");
		display.setCursor(75, 20);
		display.println("   ");
		display.display();
		if (buttonFlag[1]) {
			calibrationLoadMenu();
			float res = calibrateINA(30,calNum,50,shuntValues[calNum]);
			prevCalNum = calNum;
			calNum++;
			showCalRes(res);
			buttonFlag[1] = false;
			clearDisplay();
		}
	}

	if (count[0] == 1) {
		display.setCursor(75, 10);
		display.println("<--");
		display.display();
		display.setTextColor(WHITE, BLACK);
		display.setCursor(75, 0);
		display.println("   ");
		display.setCursor(75, 20);
		display.println("   ");
		display.display();

		if (buttonFlag[1]) {
			calibrationLoadMenu() ;
			float res = calibrateINA(30,calNum,100,shuntValues[calNum]);
			
			prevCalNum = calNum;
			calNum++;
			showCalRes(res);
			buttonFlag[1] = false;
			clearDisplay();
		}

	}
	if (count[0] == 2) {
		display.setCursor(75, 20);
		display.println("<--");
		display.display();
		display.setTextColor(WHITE, BLACK);
		display.setCursor(75, 0);
		display.println("   ");
		display.setCursor(75, 10);
		display.println("   ");
		display.display();

		if (buttonFlag[1]) {
			calibrationLoadMenu() ;
			float res = calibrateINA(30,calNum,200,shuntValues[calNum]);
			prevCalNum = calNum;
			calNum++;
			showCalRes(res);
			buttonFlag[1] = false;
			clearDisplay();
		}
	}
	
	
}
//Display Functions
void staticStartMenu() {
	display.setCursor(30, 0);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.println("POWER LOGGER");
	display.setCursor(25, 15);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.println("Press Button 4");
	display.setCursor(45, 25);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.println("to Start");
	display.display();
}
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

  display.setCursor(100, 20);
  display.println("INA");
 
  display.display();
}

/*
		INA #
	Voltage:		mV
	Current 		mA
	Power: 			mW
*/

void staticDisplayDataMenu() {
	Serial.println("here");
	display.setTextSize(.0001);
	display.setTextColor(WHITE);
	display.setCursor(100,0);
	display.setTextSize(.0001);
	display.println("INA");
	display.setCursor(0, 0);
	display.setTextSize(.0001);
	display.println("Voltage: ");
	display.setTextSize(.0001);
	display.setCursor(0, 10);
	display.println("Current: ");
	display.setTextSize(.0001);
	display.setCursor(0, 20);
	display.println("Power:");
	display.setCursor(83, 0);
	display.println("mV");
	display.setTextSize(.0001);
	display.setCursor(83, 10);
	display.println("mA");
	display.setTextSize(.0001);
	display.setCursor(83, 20);
	display.println("mW");
	display.display();
	delay(100);
}

int prevCount = 1000;
void DisplayData(float arr[]) {
	if (buttonFlag[0] == true) {
		count[0]++;
		if (count[0] == 4) {
			count[0] = 0;
    	}
    	
    	buttonFlag[0] = false;
		Serial.print("count:");
		Serial.println(count[0]);
		
	}
	
	staticDisplayDataMenu();

	if (prevCount != count[0]) {
		display.setCursor(120, 0);
		display.setTextColor(WHITE, BLACK);
		display.println(" ");
		display.display();
		display.setCursor(120, 0);
		String titleString = String(count[0]);
		display.println(titleString);
		prevCount = count[0];
	}

	display.setTextSize(.0001);
	display.setTextColor(WHITE);
	//clear current values from display
	display.setCursor(50,0);
	display.setTextSize(.0001);
	display.setTextColor(WHITE, BLACK);
	display.println("   ");
	display.setCursor(50,10);
	display.setTextSize(.0001);
	display.setTextColor(WHITE, BLACK);
	display.println("   ");
	display.setTextSize(.0001);
	display.setCursor(50,20);
	display.println("   ");
	display.display();

	//Display new vaLues
	display.setCursor(50,0);
	display.println(String(arr[count[0]]));
	display.setCursor(50,10);
	display.println(String(arr[count[0]+4]));
	display.setCursor(50,20);
	display.println(String(arr[count[0]+8]));
	
	// display.setCursor(110,8*i);
	// display.println(String(int(arr[12])));
	display.display();
	delay(100);

}

int previnaNum = -1000;
void MoveCursor() {
  if (buttonFlag[0]) {
    if (count[0] > 2) {
      count[0] = 0;
    }
    else {
      count[0]++;
    }
    buttonFlag[0] = false;
  }

  if (previnaNum != inaNum) {
    display.setCursor(118, 20);
    display.setTextColor(WHITE, BLACK);
    display.println(" ");
    display.display();
    display.setCursor(118, 20);
    String temp = String(inaNum);
    display.println(temp);
    previnaNum = inaNum;
  }
  
  if (count[0] == 0) {
    display.setCursor(70, 0);
    display.println("<--");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(70, 10);
    display.println("   ");
    display.setCursor(70, 20);
    display.println("   ");
    display.display();
	
    if (buttonFlag[1]) {
		shuntValues[inaNum] = 100;
      if(inaNum == 0) {
        Serial.print("INA0 100 set");
        inaArray[inaNum].setCalibration(100);
      }
      else if(inaNum == 1) {
        Serial.print("INA1 100 set");
        inaArray[inaNum].setCalibration(100);
      }
      else if (inaNum == 2) {
         Serial.print("INA2 100 set");
       inaArray[inaNum].setCalibration(100);
      }
      else if(inaNum == 3) {
         Serial.print("INA3 100 set");
       inaArray[inaNum].setCalibration(100);
      }
      count[1] = 0;
      inaNum++;
      buttonFlag[1] = false;

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

		if (buttonFlag[1]) {
			shuntValues[inaNum] = 1;
			if(inaNum == 0) {
				Serial.print("INA0 1 set");
				inaArray[inaNum].setCalibration(1);
			}
			else if(inaNum == 1) {
				Serial.print("INA1 1 set");
				inaArray[inaNum].setCalibration(1);
			}
			else if (inaNum == 2) {
				Serial.print("INA2 1 set");
				inaArray[inaNum].setCalibration(1);
			}
			else if(inaNum == 3) {
				Serial.print("INA3 1 set");
				inaArray[inaNum].setCalibration(1);
			}
			buttonFlag[1] = false;
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

		if (buttonFlag[1]) {
			shuntValues[inaNum] = 0.01;
		if(inaNum == 0) {
			Serial.print("INA0 0.01 set");
			inaArray[inaNum].setCalibration(0.01);
		}
		else if(inaNum == 1) {
			Serial.print("INA1 0.01 set");
			inaArray[inaNum].setCalibration(.01);
		}
		else if (inaNum == 2) {
			Serial.print("INA2 0.01 set");
			inaArray[inaNum].setCalibration(.01);
		}
		else if(inaNum == 3) {
			Serial.print("INA3 0.01 set");
			inaArray[inaNum].setCalibration(.01);
		}

		count[1] = 0;
		buttonFlag[1] = false;
		inaNum++;
	} 
}

  
}

float calibrateINA(int iterations, int inaIndex, float load, float shuntRes) {
	float total = 0;
	float inputVoltage;
	if (shuntRes == 1 || shuntRes == 0.01) {
		inputVoltage = 5;
	}
	else if (shuntRes == 100) {
		inputVoltage = 0.04;
	}

	for (int i = 0; i< iterations; i++) {
		float voltage = inaArray[inaIndex].readVoltage();
		total = total + inaArray[inaIndex].calculateShuntResitance(100, inputVoltage ,voltage,shuntRes);
	}

	total = total/iterations;
	inaArray[inaIndex].setShuntRes(total);
	Serial.print("Shunt Res: ");
	Serial.println(total);
	return total;
}




void setup() {
  
  Wire.begin();
  // put your setup code here, to run once:
  Serial.begin(115200);

  RTCsetUp();  
  setupSD();

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();
  

  // pinMode(ALERT3_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CALBUTTON0_PIN), button0PressInterupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(CALLBUTTON1_PIN), button1PressInterupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(CALLBUTTON2_PIN), button2PressInterupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(CALLBUTTON3_PIN), button3PressInterupt, FALLING);
  // attachInterrupt(digitalPinToInterrupt(ALERT0_PIN), alertZeroInterupt, FALLING);
  // attachInterrupt(digitalPinToInterrupt(ALERT1_PIN), alertOneInterupt, FALLING);
  // attachInterrupt(digitalPinToInterrupt(ALERT2_PIN), alertTwoInterupt, FALLING);
  // attachInterrupt(digitalPinToInterrupt(ALERT3_PIN), alertThreeInterupt, FALLING);
  
  inaArray[0].reset();
  for (int i=0; i<4; i++) {
    inaArray[i].reset();
  }
 
  
}

bool first = true;
bool tesy = true;
bool cal;
void loop() {
	if (startFlag) {
		if(first) {
			display.clearDisplay();
			display.display();
			first = false;
		}
		if(inaNum < 4) {
			staticMenu();
			MoveCursor();
			clear = true;
		}
		else {
			if (clear) {
				display.clearDisplay();
				display.display();
				clear = false;
				//reset count values 
				Serial.println("clear INA NUm");
				for (int i = 0; i < 4; i++) {
					count[i] = 0;
				}
			}
			if (tesy) {
				cal = calibrationChoice();
				tesy = false;
				display.clearDisplay();
				display.display();
				count[0] = 0;
			}
			
			if (cal && calNum < 4) {
				calibrationMenu();
				Serial.println(count[0]);
				delay(1000);
				if(calNum == 3) {
					count[0] = 0;
 				}
				
			}
			else {
				// check for any interupts 
				DateTime now = rtc.now();

				for (int i = 0; i < 4; i++) {
					inputData[i] = inaArray[i].readVoltage();
					inputData[i+4] = inaArray[i].calculateCurrent(inputData[i]);
					inputData[i+8] = inaArray[i].calculatePower(inputData[i+4]);
				}

				inputData[12] = now.second();
				String testString;
				File dataFile = SD.open(filename, FILE_WRITE);
				String time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

				for (int i =0; i < 13; i++) {
					if (i < 12) {
						testString.concat(inputData[i]*pow(10,6));
						testString.concat(",");
					}
					else {
						testString.concat(time);
						// testString.concat(" | ");
					}
				}

				// if (dataFile) {
			
				//   Serial.println(testString);
				//   dataFile.println(testString);
				//   dataFile.close();
				//   delay(10);
				// }
			
				if (inputData[12] != prevTime) {
					DisplayData(inputData);
					prevTime = inputData[12];
				}
			}
		}
	}
	else {
		if (!first) {
			display.clearDisplay();
			display.display();
			for (int i=0; i<4; i++) {
    			inaArray[i].reset();
  			}
		}
		//start menu 
		staticStartMenu();
		first = true;
		inaNum = 0;
		for (int i = 0; i < 4; i++) {
			count[i] = 0;
		}
		prevCount = 1000;
		previnaNum = -1000;
		tesy = true;
	}
	

}



//Interupts 
void button0PressInterupt() {
   if (millis() - lastFire[0] < 300) { // Debounce
    return;
  }

  lastFire[0] = millis();
  buttonFlag[0] = true;
  // if (count[0] > 2) {
  //   count[0] = 0;
  // }
  // else {
  //   count[0]++;
  // }

}
void button1PressInterupt() {
   if (millis() - lastFire[1] < 300) { // Debounce
    return;
  }

  lastFire[1] = millis();
  buttonFlag[1] = true;
  // if (count[1] == 1) {
  //   count[1] = 0;
  // }
  // else {
  //   count[1]++;
  // }
}

// connect to BLE?
void button2PressInterupt() {
  if (millis() - lastFire[2] < 300) { // Debounce
    return;
  }
   lastFire[2] = millis();
   buttonFlag[2] = true;
}

// set into low power mode (on/off button)
void button3PressInterupt() {
   if (millis() - lastFire[3] < 200) { // Debounce
    return;
  }
  if (startFlag)
    startFlag = false;
  else 
    startFlag = true;
}

//alert0 interupt
void alertZeroInterupt() {
  alertFlag[0] = true;
}
void alertOneInterupt() {
  alertFlag[1] = true;
}
void alertTwoInterupt() {
  alertFlag[2] = true;
}
void alertThreeInterupt() {
  alertFlag[3] = true;
}
 