/*******************RAIN SCREEN ***********************/
void rainScreen() {

  printHead("Ajustes de horario para lluvias");

  printLineSeparator();

  for (int i = 0; i < 15; i++) {
    hoursButtonRain[i].print();
  }

  cancelButton.print();

}
/*******************END RAIN SCREEN *******************/

int getRainButtonIndex() {

  for (int i = 0; i < 15; i++) {
    if (hoursButtonRain[i].pushed(x, y)) {
      return i;
      break;
    }
  }
  return 0;
}

void printHourOnOff(boolean refreshAll = true) {

  int    yHourRainOn = 52;
  int    yHourRainOff = 130;
  int    xInit = 30;
  int    hourScreenOffset = 18;
  int    xHourRain = 50;
  int    colonOffset = 36;

  setFont(LARGE, blackColor, whiteColor);

  if (refreshAll)
  {
    myGLCD.print(":", xInit + xHourRain + colonOffset + 1 , yHourRainOn);

    myGLCD.print(":", xInit + xHourRain + colonOffset + hourScreenOffset + colonOffset + 1 , yHourRainOn);

    myGLCD.print(":", xInit + xHourRain + colonOffset + 1 , yHourRainOff);

    myGLCD.print(":", xInit + xHourRain + colonOffset + hourScreenOffset + colonOffset + 1 , yHourRainOff);
  }

  // Print On Hour for rain
  if ((rainSettings.arrayRain[rainIndexButton].onHour >= 0) && (rainSettings.arrayRain[rainIndexButton].onHour <= 9)) {
    myGLCD.print("0", xInit + xHourRain , yHourRainOn);
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].onHour, xInit + xHourRain + hourScreenOffset, yHourRainOn);
  } else {
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].onHour, xInit + xHourRain + 1, yHourRainOn);
  }

  int xMinRain = xInit + xHourRain + colonOffset + hourScreenOffset;

  if ((rainSettings.arrayRain[rainIndexButton].onMinute >= 0) && (rainSettings.arrayRain[rainIndexButton].onMinute <= 9)) {
    myGLCD.print("0", xMinRain , yHourRainOn);
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].onMinute, xMinRain + hourScreenOffset, yHourRainOn);
  } else {
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].onMinute, xMinRain + 2, yHourRainOn);
  }

  int xSecRain = xMinRain + hourScreenOffset + colonOffset;

  if ((rainSettings.arrayRain[rainIndexButton].onSecond >= 0) && (rainSettings.arrayRain[rainIndexButton].onSecond <= 9)) {
    myGLCD.print("0", xSecRain , yHourRainOn);
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].onSecond, xSecRain + hourScreenOffset, yHourRainOn);
  } else {
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].onSecond, xSecRain + 2, yHourRainOn);
  }

  // Print On Hour for rain
  if ((rainSettings.arrayRain[rainIndexButton].offHour >= 0) && (rainSettings.arrayRain[rainIndexButton].offHour <= 9)) {
    //
    myGLCD.print("0", xInit + xHourRain , yHourRainOff);
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].offHour, xInit + xHourRain + hourScreenOffset, yHourRainOff);
  } else {
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].offHour, xInit + xHourRain + 1 , yHourRainOff);
  }

  if ((rainSettings.arrayRain[rainIndexButton].offMinute >= 0) && (rainSettings.arrayRain[rainIndexButton].offMinute <= 9)) {
    myGLCD.print("0", xMinRain , yHourRainOff);
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].offMinute, xMinRain + hourScreenOffset, yHourRainOff);
  } else {
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].offMinute, xMinRain + 2, yHourRainOff);
  }

  if ((rainSettings.arrayRain[rainIndexButton].offSecond >= 0) && (rainSettings.arrayRain[rainIndexButton].offSecond <= 9)) {
    myGLCD.print("0", xSecRain , yHourRainOff);
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].offSecond, xSecRain + hourScreenOffset, yHourRainOff);
  } else {
    myGLCD.printNumI(rainSettings.arrayRain[rainIndexButton].offSecond, xSecRain + 2, yHourRainOff);
  }
}

void printHoursRain(boolean refreshAll = true) {

  int    yHourRainOn = 52;
  int    yHourRainOff = 130;

  if (rainSettings.arrayRain[rainIndexButton].rainEnabled == 1) {
    onRain.image = Off_icon;
  } else {
    onRain.image = On_icon;
  }

  onRain.print();

  setFont(LARGE, blackColor, whiteColor);

  if (refreshAll) {

    myGLCD.print("On", 20 , yHourRainOn);
    myGLCD.print("Off", 20 , yHourRainOff);

    hourOnRainSelect.setPosition(78, yHourRainOn - 5 , 115, yHourRainOn + 21 );
    minutesOnRainSelect.setPosition(133, yHourRainOn - 5, 170, yHourRainOn + 21 );
    secondsOnRainSelect.setPosition(187, yHourRainOn - 5, 224, yHourRainOn + 21 );

    hourOffRainSelect.setPosition(78, yHourRainOff - 5 , 116, yHourRainOff + 21 );
    minutesOffRainSelect.setPosition(133, yHourRainOff - 5, 170, yHourRainOff + 21 );
    secondsOffRainSelect.setPosition(187, yHourRainOff - 5, 224, yHourRainOff + 21 );
  }

  printHourOnOff(refreshAll);

  printRainSetupButtons(&hourOnRainSelect, &minutesOnRainSelect, &secondsOnRainSelect, rainOnIndex);
  printRainSetupButtons(&hourOffRainSelect, &minutesOffRainSelect, &secondsOffRainSelect, rainOffIndex);

}

void processRainScreen() {

  cleanScreen();
  printHead("LLuvia: Seleccion de rango");
  printLineSeparator();

  rainIndexButton = getRainButtonIndex();

  rainOnUP.print();
  rainOnDown.print();
  rainOffUP.print();
  rainOffDown.print();
  backButton.setInitPosition(10,200);
  backButton.print();

  // Init Hour ON/OFF Box how selected
  rainOnIndex = 0;
  rainOffIndex = 0;

  // Rain timmer set to modify
  bitWrite(isRainTimerModify, rainIndexButton, 1);

  printHoursRain(true);

  printButtonCommon();

  dispScreen = 2;
}

void printRainSetupButtons(Button *hourButton, Button *minButton, Button *secButton, int index)
{

  switch (index) {
    case 0:
      hourButton -> _borderColor = redColor;
      minButton -> _borderColor = whiteColor;
      secButton -> _borderColor = whiteColor;
      break;
    case 1:
      hourButton -> _borderColor = whiteColor;
      minButton -> _borderColor = redColor;
      secButton -> _borderColor = whiteColor;
      break;
    case 2:
      hourButton -> _borderColor = whiteColor;
      minButton -> _borderColor = whiteColor;
      secButton -> _borderColor = redColor;
      break;
  }

  hourButton -> print();
  minButton -> print();
  secButton -> print();
}

int hoursRainSelectProcess(Button *hourButton, Button *minButton, Button *secButton) {

  if (hourButton -> pushed(x, y)) {
    return 0;
  } else if (minButton -> pushed(x, y)) {
    return 1;
  } else if (secButton -> pushed(x, y)) {
    return 2;
  } else {
    return 0;
  }
}

void processRainSetupScreen() {

  if (saveButton.pushed(x, y)) {
    // If save and exit, set all timer to active (not modified more)
    for (int i = 0; i < 15; i++) {
      bitWrite(isRainTimerModify, i, 0);
    }
    cleanScreen();
    saveRainToEEPROM();
    dispScreen = 1;
    rainScreen();

  } else if (backButton.pushed(x, y)) {
    cleanScreen();
    // If exit, set all timer to active (not modified more)
    for (int i = 0; i < 15; i++) {
      bitWrite(isRainTimerModify, i, 0);
    }
    dispScreen = 1;
    rainScreen();
  } else if (cancelButton.pushed(x, y)) {
  
    // If save and exit, set all timer to active (not modified more)
    for (int i = 0; i < 15; i++) {
      bitWrite(isRainTimerModify, i, 0);
    }
    
    goToMainScreen();

  } else if (onRain.pushed(x, y)) {

    if (rainSettings.arrayRain[rainIndexButton].rainEnabled == 1) {
      rainSettings.arrayRain[rainIndexButton].rainEnabled = 0;
    } else {
      rainSettings.arrayRain[rainIndexButton].rainEnabled = 1;
    }

    printHoursRain(false);

  } else {

    if (hourOnRainSelect.pushed(x, y) || minutesOnRainSelect.pushed(x, y) || secondsOnRainSelect.pushed(x, y)) {
      rainOnIndex = hoursRainSelectProcess(&hourOnRainSelect, &minutesOnRainSelect, &secondsOnRainSelect);
    }

    if (hourOffRainSelect.pushed(x, y) || minutesOffRainSelect.pushed(x, y) || secondsOffRainSelect.pushed(x, y)) {
      rainOffIndex = hoursRainSelectProcess(&hourOffRainSelect, &minutesOffRainSelect, &secondsOffRainSelect);
    }

    if (rainOnUP.pushed(x, y)) {
      switch (rainOnIndex) {
        case 0:
          rainSettings.arrayRain[rainIndexButton].onHour = (rainSettings.arrayRain[rainIndexButton].onHour + 1) % 24;
          break;
        case 1:
          rainSettings.arrayRain[rainIndexButton].onMinute = (rainSettings.arrayRain[rainIndexButton].onMinute + 1) % 60;
          break;
        case 2:
          rainSettings.arrayRain[rainIndexButton].onSecond = (rainSettings.arrayRain[rainIndexButton].onSecond + 1) % 60;
          break;
      }
    }
    if (rainOnDown.pushed(x, y)) {
      switch (rainOnIndex) {
        case 0:
          rainSettings.arrayRain[rainIndexButton].onHour = (rainSettings.arrayRain[rainIndexButton].onHour + 23) % 24;
          break;
        case 1:
          rainSettings.arrayRain[rainIndexButton].onMinute = (rainSettings.arrayRain[rainIndexButton].onMinute + 59) % 60;
          break;
        case 2:
          rainSettings.arrayRain[rainIndexButton].onSecond = (rainSettings.arrayRain[rainIndexButton].onSecond + 59) % 60;
          break;
      }
    }
    if (rainOffUP.pushed(x, y)) {
      switch (rainOffIndex) {
        case 0:
          rainSettings.arrayRain[rainIndexButton].offHour = (rainSettings.arrayRain[rainIndexButton].offHour + 1) % 24;
          break;
        case 1:
          rainSettings.arrayRain[rainIndexButton].offMinute = (rainSettings.arrayRain[rainIndexButton].offMinute + 1) % 60;
          break;
        case 2:
          rainSettings.arrayRain[rainIndexButton].offSecond = (rainSettings.arrayRain[rainIndexButton].offSecond + 1) % 60;
          break;
      }
    }
    if (rainOffDown.pushed(x, y)) {
      switch (rainOffIndex) {
        case 0:
          rainSettings.arrayRain[rainIndexButton].offHour = (rainSettings.arrayRain[rainIndexButton].offHour + 23) % 24;
          break;
        case 1:
          rainSettings.arrayRain[rainIndexButton].offMinute = (rainSettings.arrayRain[rainIndexButton].offMinute + 59) % 60;
          break;
        case 2:
          rainSettings.arrayRain[rainIndexButton].offSecond = (rainSettings.arrayRain[rainIndexButton].offSecond + 59) % 60;
          break;
      }
    }

    printHoursRain(false);
  }
}
