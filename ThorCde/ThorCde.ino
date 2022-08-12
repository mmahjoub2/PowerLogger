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
bool buttonFlag[] = {false, false, false, false};
//Alert Flags 
bool alertFlag[] {false, false, false, false};
bool startFlag = false;
int restartCount = 0;
int previnaNum = -1000;
int calNum = 0;
int prevCalNum = 100;
int prevCount = 1000;
bool first = true;
bool CalibrationChoiceState;
bool cal;

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
		if (buttonFlag[0]) {
			buttonFlag[0] = false;
			if (count[0] == 1) {
				count[0] = 0;
			}
			else {
				count[0]++;
			}
		}
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
	delay(1000);
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

void DisplayData(float arr[]) {
	if (buttonFlag[0] == true) {
		count[0]++;
		if (count[0] == 4) {
			count[0] = 0;
    	}
    	buttonFlag[0] = false;
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
	display.println("     ");
	display.setCursor(50,10);
	display.setTextSize(.0001);
	display.setTextColor(WHITE, BLACK);
	display.println("     ");
	display.setTextSize(.0001);
	display.setCursor(50,20);
	display.println("     ");
	display.display();

	//Display new vaLues
	display.setCursor(50,0);
	float data = arr[count[0]] * pow(10,3);
	display.println(String(data, 1));
	data = arr[count[0]+4] * pow(10,3);
	display.setCursor(50,10);
	display.println(String(data, 1));
	display.setCursor(50,20);
	data = arr[count[0]+8] * pow(10,3);
	display.println(String(data, 1));
	// display.setCursor(110,8*i);
	// display.println(String(int(arr[12])));
	display.display();
	delay(100);
}

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
				inaArray[inaNum].setCalibration(100);
			}
			else if(inaNum == 1) {
				inaArray[inaNum].setCalibration(100);
			}
			else if (inaNum == 2) {
				inaArray[inaNum].setCalibration(100);
			}
			else if(inaNum == 3) {
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
				inaArray[inaNum].setCalibration(1);
			}
			else if(inaNum == 1) {
				inaArray[inaNum].setCalibration(1);
			}
			else if (inaNum == 2) {
				inaArray[inaNum].setCalibration(1);
			}
			else if(inaNum == 3) {
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
			inaArray[inaNum].setCalibration(0.01);
		}
		else if(inaNum == 1) {
			inaArray[inaNum].setCalibration(.01);
		}
		else if (inaNum == 2) {
			inaArray[inaNum].setCalibration(.01);
		}
		else if(inaNum == 3) {
			inaArray[inaNum].setCalibration(.01);
		}

			count[1] = 0;
			buttonFlag[1] = false;
			inaNum++;
		} 
	}
}

float calibrateINA(int iterations, int inaIndex, float load, double shuntRes) {
	float total = 0;
	float inputVoltage;
	if (shuntRes == 1 || shuntRes == 0.01) {
		inputVoltage = 5;
	}

	if (shuntRes == 100) {
			inputVoltage = 0.04;
	}

	for (int i = 0; i< iterations; i++) {
		float voltage = inaArray[inaIndex].readVoltage();
		total = total + inaArray[inaIndex].calculateShuntResitance(load, inputVoltage ,voltage,shuntRes);
	}
	total = total/iterations;
	inaArray[inaIndex].setShuntRes(total);
	return total;
}

void blinky() {
	digitalToggle(LED_BUILTIN); // turn the LED on (HIGH is the voltage level)
	delay(100); 
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
	attachInterrupt(digitalPinToInterrupt(CALBUTTON0_PIN), button0PressInterupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(CALLBUTTON1_PIN), button1PressInterupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(CALLBUTTON2_PIN), button2PressInterupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(CALLBUTTON3_PIN), button3PressInterupt, FALLING);
	pinMode(LED_BUILTIN, OUTPUT);

	for (int i=0; i<4; i++) {
		inaArray[i].reset();
	} 
}

void loop() {
	// ON STATE
	if (startFlag) {
		if(first) {
			display.clearDisplay();
			display.display();
			first = false;
			//reintialize SD Card here
			filename[7] = '0' + (restartCount/2)/10;
			filename[8] = '0' + (restartCount/2)%10;
			File dataFile = SD.open(filename, FILE_WRITE);
			if(dataFile) {
				Serial.print("Writing to "); 
    			Serial.println(filename);
				dataFile.println("Voltage INA1, Voltage INA2, VoltageINA3, VoltageINA3, Current INA1, Current INA2, Current INA3, Current INA4, Power INA1, Power INA2, Power INA3, Power INA4, Time");
				dataFile.close();
			}
			else {
				Serial.print("Could not create:");
				Serial.println(filename);
			}		
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
				for (int i = 0; i < 4; i++) {
					count[i] = 0;
				}
        		CalibrationChoiceState = true;
			}
			if (CalibrationChoiceState) {
				cal = calibrationChoice();
				CalibrationChoiceState = false;
				display.clearDisplay();
				display.display();
				count[0] = 0;
			}
			if (cal && calNum < 4) {
				calibrationMenu();
				if(calNum == 3) {
					count[0] = 0;
 				}	
			}
			else {
				// check for any interupts 
				DateTime now = rtc.now();
				for (int i = 0; i < 4; i++) {
					inputData[i] = inaArray[i].readVoltage();
					if (inputData[i] < 0.02 && inaArray[i].getADCFlag() && inputData [i] != 0) {
						inaArray[i].ADCRange(false);
					}
					if (inputData[i] > 0.02 && inaArray[i].getADCFlag() == false) {
						inaArray[i].ADCRange(true);
					}
					inputData[i+4] = inaArray[i].calculateCurrent(inputData[i]);
					inputData[i+8] = inaArray[i].calculatePower(inputData[i+4]);
				}
				inputData[12] = now.second();
				String testString;
				
				String time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

				for (int i =0; i < 13; i++) {
					if (i < 12) {
						testString.concat(String(inputData[i],6));
						testString.concat(",");
					}
					else {
						testString.concat(time);
						// testString.concat(" | ");
					}
				}

				File dataFile = SD.open(filename, FILE_WRITE);
				if (dataFile && buttonFlag[2]) {
					Serial.println(testString);
					Serial.print("FILE NAME: ");  
					Serial.println(filename);
					dataFile.println(testString);
					dataFile.close();
					delay(10);
					blinky();
				}
				else if (!dataFile && buttonFlag[2]) {
					Serial.println("could not openn file");
				}
				
				if (!buttonFlag[2]) {
					digitalWrite(LED_BUILTIN, LOW);
					dataFile.close();
				}
			
				if (inputData[12] != prevTime) {
					DisplayData(inputData);
					prevTime = inputData[12];
				}
			}
		}
	}
	// OFF STATE
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
		buttonFlag[2] = false;
		prevCount = 1000;
		previnaNum = -1000;
		CalibrationChoiceState = true;
	}
}

//Interupts 
void button0PressInterupt() {
	if (millis() - lastFire[0] < 300) { // Debounce
		return;
	}
	lastFire[0] = millis();
	buttonFlag[0] = true;
}

void button1PressInterupt() {
	if (millis() - lastFire[1] < 300) { // Debounce
		return;
	}
	lastFire[1] = millis();
	buttonFlag[1] = true;
}

// Start and Stop logging
void button2PressInterupt() {
	if (millis() - lastFire[2] < 300) { // Debounce
		return;
	}
	lastFire[2] = millis();

	if (buttonFlag[2]) {
		buttonFlag[2] = false;
	}
	else {
		buttonFlag[2] = true;
	}
}

// set into low power mode (on/off button)
void button3PressInterupt() {
	if (millis() - lastFire[3] < 400) { // Debounce
		return;
	}
	if (startFlag)
		startFlag = false;
	else 
		startFlag = true;
	restartCount++;
}
