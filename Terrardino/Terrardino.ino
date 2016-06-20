//------------------------------------------------------------------------------
//
//   Main code based on Stilo
//   http://code.google.com/p/stilo/
//
//   And Ferduino
//   https://github.com/FernandoGarcia/
//
//   Free icons from http://icons.mysitemyway.com/
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//------------------------------------------------------------------------------

//LIBRARIES
#include <ITDB02_Graph16.h>
#include <avr/pgmspace.h>
#include <ITDB02_Touch.h>
#include <EEPROM.h>
#include "EEPROM_writeAnything.h"
#include <DS3231.h>
#include "DHT.h"
#include <ButtonITDB02.h>

// Touch Screen and ITDB02 MEGA Shield
//(Mega Shield utiliza pins 5V, 3V3, GND, 2-6, 20-41, & (50-53 para la tarjeta SD))
ITDB02       myGLCD(38, 39, 40, 41, ITDB32S);
ITDB02_Touch myTouch(6, 5, 4, 3, 2);

// Humidity and temperature Sensor
#define DHTPIN 43 // Pin 43
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// Init Clock DS3231
DS3231  rtc(SDA, SCL);

// Init humidity sensor
DHT dht(DHTPIN, DHTTYPE);

/***************** PIN ***************************/
// Fan pin
const int onOffFanPin = 47;   // Pin to enabled/disabled fan relay
// Heater pin
const int onOffCalPin = 48;   // Pin to enabled/disabled heater relay
// Rain pion
const int onOffRain = 49; // Pin to enabled/disabled rain relay

//Declare which fonts to be utilized
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

#define LARGE true
#define SMALL false

int x, y; // Screen coordinate

// Global var that contain the screen visible in this moment.
int dispScreen = 0;

Time timeTemp, timeCurrent;

// Icons
extern unsigned int Fan_on_icon[1764];
extern unsigned int Fan_off_icon[1764];
extern unsigned int Rain_on_icon[1764];
extern unsigned int Rain_off_icon[1764];
extern unsigned int Heat_on_icon[1764];
extern unsigned int Heat_off_icon[1764];
extern unsigned int Humidity_icon[1764];
extern unsigned int Temperature_icon[1764];
extern unsigned int On_icon[900];
extern unsigned int Off_icon[900];
extern unsigned int Plus_icon[625];
extern unsigned int Minus_icon[625];
extern unsigned int Back_icon[900];
extern unsigned int Save_icon[900];
extern unsigned int Cancel_icon[900];
extern unsigned int About_icon[625];

// Rain booleans
boolean isRainOn[15];

boolean isRainTimerModify[15];

// Rain flag --> The logic result of sum all rain flags
// 1 if some raintimer is on. 0 if all rainTimers are off.
boolean rainOn = false;

float humidityTerrarium = 0; // Humidity
float tempTerrarium = 0;    // Temperature

float setHumTerrarium = 0.0;
float offHumTerrarium = 0.0;
float setTempTerrarium = 0.0;
float offTempTerrarium = 0.0;

boolean isCoolerOn = false;

boolean isHeaterOn = false;

// Refresh counter
long previousMillis = 0;

// Colors
RGB greenColor{51, 102, 51};
RGB greenLightColor{0, 255, 0};
RGB whiteColor{255, 255, 255};
RGB blackColor{0, 0, 0};
RGB redColor{255, 0, 0};
RGB blueColor{9, 184, 255};
RGB blueCleanColor{0, 0, 255};
RGB greyColor {176, 176, 176};
RGB orangeColor {255, 128, 0};

/***************************** DECLARED BUTTONS *********************************/

ButtonImage backButton(&myGLCD);
ButtonImage saveButton(&myGLCD);
ButtonImage cancelButton(&myGLCD);

// Main screen buttons
Button timeDateButton(&myGLCD);
ButtonImage humidityButton(&myGLCD);
ButtonImage temperatureButton(&myGLCD);
ButtonText lightButton(&myGLCD);
ButtonText ledSettingsButton(&myGLCD);
ButtonImage rainSettingsButton(&myGLCD);
ButtonImage aboutButton(&myGLCD);

// Leds buttons
ButtonText testLedButton(&myGLCD);
ButtonText whiteButton(&myGLCD);
ButtonText redButton(&myGLCD);
ButtonText blueButton(&myGLCD);
ButtonText lunarButton(&myGLCD);

// Time and date buttons
int clockIndex;
ButtonImage clockScreenUp(&myGLCD);
ButtonImage clockScreenDown(&myGLCD);
Button hourSelect(&myGLCD);
Button minutesSelect(&myGLCD);
Button dateSelect(&myGLCD);
Button monthSelect(&myGLCD);
Button yearSelect(&myGLCD);

// Temperature and humidity buttons
ButtonImage temperatureMinus(&myGLCD);
ButtonImage temperaturePlus(&myGLCD);
ButtonImage offsetMinus(&myGLCD);
ButtonImage offsetPlus(&myGLCD);

// Test leds buttons
ButtonText startStoptTest(&myGLCD);
ButtonText tenMinus(&myGLCD);
ButtonText tenPlus(&myGLCD);

// Index of Hour/Minute/seconds selected by user
int rainOnIndex;
int rainOffIndex;
// Rain settings buttons
Button hourOnRainSelect(&myGLCD);
Button minutesOnRainSelect(&myGLCD);
Button secondsOnRainSelect(&myGLCD);
Button hourOffRainSelect(&myGLCD);
Button minutesOffRainSelect(&myGLCD);
Button secondsOffRainSelect(&myGLCD);
ButtonImage rainOnUP(&myGLCD);
ButtonImage rainOnDown(&myGLCD);
ButtonImage rainOffUP(&myGLCD);
ButtonImage rainOffDown(&myGLCD);
ButtonImage onRain(&myGLCD);
// Rain array buttons
ButtonText hoursButtonRain[15];

char* hoursCounterRain[15] = {
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"
};

// Rain settings index selected. 1-15 counters
int rainIndexButton;

/********************************* EEPROM FUNCTIONS *****************************/
struct configTemperature {
  int tempset;
  int tempoff;
}
temperatureSettings;

struct configHumidity {
  float humidityTerrarium;
  float humedadOffset;
}
humiditySettings;

typedef struct {
  int rainEnabled;
  int onHour;
  int onMinute;
  int onSecond;
  int offHour;
  int offMinute;
  int offSecond;
}
rainDate;

struct configRain {
  rainDate arrayRain[15];
}
rainSettings;

void saveHumiditytoEEPROM() {
  humiditySettings.humidityTerrarium = int(setHumTerrarium * 10);
  humiditySettings.humedadOffset = int(offHumTerrarium * 10);
  EEPROM_writeAnything(620, humiditySettings);
}

void saveTempToEEPROM() {
  temperatureSettings.tempset = int(setTempTerrarium * 10);
  temperatureSettings.tempoff = int(offTempTerrarium * 10);
  EEPROM_writeAnything(629, temperatureSettings);
}

void saveRainToEEPROM() {
  EEPROM_writeAnything(634, rainSettings);
}

void readFromEEPROM() {
  int offsetEEprom;
  int k = EEPROM.read(0);

  offsetEEprom = EEPROM_readAnything(620, humiditySettings);
  setHumTerrarium = humiditySettings.humidityTerrarium;
  setHumTerrarium /= 10;
  offHumTerrarium = humiditySettings.humedadOffset;
  offHumTerrarium /= 10;

  EEPROM_readAnything(629, temperatureSettings);
  setTempTerrarium = temperatureSettings.tempset;
  setTempTerrarium /= 10;
  offTempTerrarium = temperatureSettings.tempoff;
  offTempTerrarium /= 10;

  EEPROM_readAnything(634, rainSettings);
  //  EEPROM_readAnything(845, nextSetting);

}
/***************************** END OF EEPROM FUNCTIONS ********************************/
void cleanScreen()
{
  myGLCD.setColor(blackColor.r, blackColor.g, blackColor.b);
  myGLCD.fillRect(1, 0, 318, 239);
}

void setGreenColors(ButtonText *button) {
  button -> _backgroundColor = greenColor;
  button -> _frontColor = greenColor;
  button -> _borderColor = whiteColor;
  button -> _fontColor = whiteColor;
  button -> _fontSize = SMALL;
}

void initButtons() {

  int plusMinusSize = 25;
  int commonButtonsSize = 30;

  backButton.image = Back_icon;
  backButton.setImageSize(commonButtonsSize, commonButtonsSize);

  saveButton.image = Save_icon;
  saveButton.setInitPosition(110, 200);
  saveButton.setImageSize(commonButtonsSize, commonButtonsSize);

  cancelButton.image = Cancel_icon;
  cancelButton.setInitPosition(180 , 200);
  cancelButton.setImageSize(commonButtonsSize, commonButtonsSize);

  clockScreenUp.image = Plus_icon;
  clockScreenUp.setImageSize(plusMinusSize, plusMinusSize);
  clockScreenUp.setInitPosition(130, 140);

  clockScreenDown.image = Minus_icon;
  clockScreenDown.setImageSize(plusMinusSize, plusMinusSize);
  clockScreenDown.setInitPosition(170, 140);

  temperatureMinus.image = Minus_icon;
  temperatureMinus.setImageSize(plusMinusSize, plusMinusSize);
  temperatureMinus.setInitPosition(90, 49);

  temperaturePlus.image = Plus_icon;
  temperaturePlus.setImageSize(plusMinusSize, plusMinusSize);
  temperaturePlus.setInitPosition(205, 49);

  offsetMinus.image = Minus_icon;
  offsetMinus.setImageSize(plusMinusSize, plusMinusSize);
  offsetMinus.setInitPosition(90, 99);

  offsetPlus.image = Plus_icon;
  offsetPlus.setImageSize(plusMinusSize, plusMinusSize);
  offsetPlus.setInitPosition(205, 99);

  int columns = 6;
  int offset =  5;
  int buttonWidth = (300 - offset * 5) / columns;
  int xB = 0;
  int yB = 30;
  xB = offset * 2;
  yB = 30;

  for (int i = 0; i < 15; i++) {
    //MAX  320 x 240 pixels
    if (i == 5) {
      yB = 70;
      xB = offset * 2;
    }
    if (i == 10) {
      yB = yB + 40;
      xB = offset * 2;
    }
    xB = xB + 10;
    hoursButtonRain[i].setPosition(xB, yB, xB + buttonWidth, yB + 30);
    xB = xB + buttonWidth;
    hoursButtonRain[i].setScreen(&myGLCD);
    hoursButtonRain[i]._text = hoursCounterRain[i];
    setGreenColors(&hoursButtonRain[i]);
  }

  rainSettingsButton.image = Rain_off_icon;

  rainOnUP.image = Plus_icon;
  rainOnUP.setImageSize(plusMinusSize, plusMinusSize);
  rainOnUP.setInitPosition(265, 35);

  rainOnDown.image = Minus_icon;
  rainOnDown.setImageSize(plusMinusSize, plusMinusSize);
  rainOnDown.setInitPosition(265, 65);

  rainOffUP.image = Plus_icon;
  rainOffUP.setImageSize(plusMinusSize, plusMinusSize);
  rainOffUP.setInitPosition(265, 110);

  rainOffDown.image = Minus_icon;
  rainOffDown.setImageSize(plusMinusSize, plusMinusSize);
  rainOffDown.setInitPosition(265, 140);

  onRain.setImageSize(30, 30);
  onRain.setInitPosition(20, 85);

  aboutButton.image = About_icon;
  aboutButton.setImageSize(plusMinusSize, plusMinusSize);
  aboutButton.setInitPosition(291, 190);
}

void printButtonCommon() {
  saveButton.print();
  cancelButton.print();
}

void setFont(boolean font, RGB backColor, RGB fontColor) {

  // background color
  myGLCD.setBackColor(backColor.r, backColor.g, backColor.b);
  // font color
  myGLCD.setColor(fontColor.r, fontColor.g, fontColor.b);

  if (font == LARGE)
    myGLCD.setFont(BigFont);
  else if (font == SMALL)
    myGLCD.setFont(SmallFont);
}

void printHead(char* headline) {
  setFont(SMALL, orangeColor, orangeColor);
  myGLCD.fillRect (1, 1, 318, 14);
  myGLCD.setColor(blackColor.r, blackColor.g, blackColor.b);
  myGLCD.print(headline, CENTER, 1);
}

void printLineSeparator() {
  myGLCD.setColor(greyColor.r, greyColor.g, greyColor.b);
  myGLCD.drawRect(0, 196, 319, 194);
}

/********************************** TIME AND DATE BAR **********************************/
void timeStampInitial() {

  setFont(SMALL, blackColor, whiteColor);

  myGLCD.print(rtc.getTimeStr(FORMAT_LONG), 20, 223);//Display Time

  myGLCD.print(rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN, '/'), 100, 223); //Display date
}
/****************************** END OF TIME AND DATE BAR ******************************/

/************************************ MAIN SCREEN **************************************/
void mainScreen(boolean refreshAll = false) {

  int xIcon = 13;
  int yIcon = 50;
  int iconSize = 42;
  int iconOffset = 13;
  int xTempHum = 140;
  int yTempHum = 125;

  if (refreshAll) {

    cleanScreen();

    myGLCD.setColor(greenLightColor.r, greenLightColor.g, greenLightColor.b);            //Draw Borders & Dividers in Grey // Bordes y separadores en gris
    myGLCD.drawRect(0, 0, 319, 239);            //Outside Border
    myGLCD.fillRect(0, 0, 319, 14);             //Top Bar
    myGLCD.drawLine(1, 45 , 318 , 45);
    myGLCD.drawLine(1, yIcon + iconSize + 5 , (iconSize * 3 + 10 * 3 ) + 20 , yIcon + iconSize + 5);
    myGLCD.drawLine(1, 219 , 318 , 219);
    myGLCD.drawLine(176, 45 , 176 , 219);

    setFont(SMALL, greenLightColor, blackColor);
    myGLCD.print("Terrardino terrarium controler v.1.0", CENTER, 1);
    setFont(SMALL, blackColor, whiteColor);
    myGLCD.print("Control Terrario", 100, 20);

    setFont(LARGE, blackColor, whiteColor);

    myGLCD.print("%", xTempHum , yTempHum);

    myGLCD.drawCircle(xTempHum + 5 , yTempHum + 50 , 3);
    myGLCD.print("C", xTempHum + 10 , yTempHum + 50 );

    rainSettingsButton.setImageSize(iconSize, iconSize);
    rainSettingsButton.setInitPosition(xIcon + iconSize + iconOffset, yIcon);

    humidityButton.setImageSize(iconSize, iconSize);
    humidityButton.setInitPosition(xIcon, 110);
    humidityButton.image = Humidity_icon;
    humidityButton.print();

    temperatureButton.setImageSize(iconSize, iconSize);
    temperatureButton.setInitPosition(xIcon, 160);
    temperatureButton.image = Temperature_icon;
    temperatureButton.print();

    timeDateButton.setPosition( 18 , 222 , 180, 238 );
    timeDateButton._borderColor = blackColor;
    timeDateButton.print();

    aboutButton.print();
  }

  setFont(LARGE, blackColor, whiteColor);
  myGLCD.printNumF( humidityTerrarium, 1, 70, yTempHum);
  myGLCD.printNumF( tempTerrarium, 1, 70, yTempHum + 50);


  if (isCoolerOn) {
    myGLCD.drawBitmap(xIcon, yIcon, iconSize, iconSize, Fan_on_icon, 1);
  }
  else {
    myGLCD.drawBitmap(xIcon, yIcon, iconSize, iconSize, Fan_off_icon, 1);
  }

  rainSettingsButton.print();

  if (isHeaterOn) {
    myGLCD.drawBitmap(xIcon + (iconSize * 2) + (iconOffset * 2) , yIcon, iconSize, iconSize, Heat_on_icon, 1);
  }
  else {
    myGLCD.drawBitmap(xIcon + (iconSize * 2) + (iconOffset * 2), yIcon, iconSize, iconSize, Heat_off_icon, 1);
  }
}

