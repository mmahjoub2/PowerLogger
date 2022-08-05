
#ifndef SETUP_H_
#define SETUP_H_

#include <SPI.h> 
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "INA.h"
#include "RTClib.h"

// some declarations in
// the header file.
  void RTCsetUp();
  void setupSD();
  void scanI2c();
#endif




// TODO: make this a .c file


