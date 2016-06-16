
byte validateDateForMonth(byte day, byte month, word yearC)
{
  byte monthArray[12] = {
    31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };

  if (month == 2) {
    if ((yearC % 4) == 0) {
      if (day > 29) {
        day = 29;
      }
    } else {
      if (day > 28) {
        day = 28;
      }
    }
  } else {
    if (day > monthArray[month - 1]) {
      day = monthArray[month - 1];
    }
  }

  return day;
}

byte calcDOW(byte d, byte month, int y)
{
  int dow;
  byte monthArray[12] = {
    6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4
  };

  dow = (y % 100);
  dow = dow * 1.25;
  dow += d;
  dow += monthArray[month - 1];
  if (((y % 4) == 0) && (month < 3))
    dow -= 1;
  while (dow > 7)
    dow -= 7;

  return dow;
}

byte validateDate(byte day, byte month, word yearC)
{
  byte monthArray[12] = {
    31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };
  byte oldDay;

  if (month == 2) {
    if ((yearC % 4) == 0) {
      if (day == 30)
        oldDay = 1;
      else if (day == 0)
        oldDay = 29;
      else
        oldDay = day;
    } else {
      if (day == 29)
        oldDay = 1;
      else if (day == 0)
        oldDay = 28;
      else
        oldDay = day;
    }
  } else {
    if (day == 0)
      oldDay = monthArray[month - 1];
    else if (day == (monthArray[month - 1] + 1))
      oldDay = 1;
    else
      oldDay = day;
  }

  return oldDay;
}

/********************************* CLOCK SCREEN ***************************************/
void printClockButtons(Button *hourButton, Button *minButton, Button *dateButton, Button *monthButton, Button *yearButton)
{

  switch (clockIndex) {
    case 0:
      hourButton -> _borderColor = redColor;
      minButton -> _borderColor = whiteColor;
      dateButton -> _borderColor = whiteColor;
      monthButton -> _borderColor = whiteColor;
      yearButton -> _borderColor = whiteColor;
      break;
    case 1:
      hourButton -> _borderColor = whiteColor;
      minButton -> _borderColor = redColor;
      dateButton -> _borderColor = whiteColor;
      monthButton -> _borderColor = whiteColor;
      yearButton -> _borderColor = whiteColor;
      break;
    case 2:
      hourButton -> _borderColor = whiteColor;
      minButton -> _borderColor = whiteColor;
      dateButton -> _borderColor = redColor;
      monthButton -> _borderColor = whiteColor;
      yearButton -> _borderColor = whiteColor;
      break;
    case 3:
      hourButton -> _borderColor = whiteColor;
      minButton -> _borderColor = whiteColor;
      dateButton -> _borderColor = whiteColor;
      monthButton -> _borderColor = redColor;
      yearButton -> _borderColor = whiteColor;
      break;
    case 4:
      hourButton -> _borderColor = whiteColor;
      minButton -> _borderColor = whiteColor;
      dateButton -> _borderColor = whiteColor;
      monthButton -> _borderColor = whiteColor;
      yearButton -> _borderColor = redColor;
      break;
  }

  hourButton -> print();
  minButton -> print();
  dateButton -> print();
  monthButton -> print();
  yearButton -> print();

}

int clockSelectProcess(Button *hourButton, Button *minButton, Button *dateButton, Button *monthButton, Button *yearButton) {

  if (hourButton -> pushed(x, y)) {
    return 0;
  }
  else if (minButton -> pushed(x, y)) {
    return 1;
  }
  else if (dateButton -> pushed(x, y)) {
    return 2;
  }
  else if (monthButton -> pushed(x, y)) {
    return 3;
  }
  else if (yearButton -> pushed(x, y)) {
    return 4;
  }
  else {
    return 0;
  }
}

void clockScreen(boolean refreshAll = true) {

  int    yHours = 95;
  int    xInit = 20;
  int    hourScreenOffset = 19;
  int    xHourRain = 40;
  int    colonOffset = 40;

  setFont(LARGE, blackColor, whiteColor);

  if (refreshAll)
  {
    timeTemp = rtc.getTime();
    myGLCD.print(":", xInit + colonOffset, yHours);
  }

  if ((timeTemp.hour >= 0) && (timeTemp.hour <= 9)) {
    myGLCD.print("0", xInit , yHours);
    myGLCD.printNumI(timeTemp.hour, xInit + hourScreenOffset, yHours);
  }
  else {
    myGLCD.printNumI(timeTemp.hour, xInit, yHours);
  }

  int xMinutes = xInit + colonOffset + hourScreenOffset;

  if ((timeTemp.min >= 0) && (timeTemp.min <= 9)) {
    myGLCD.print("0", xMinutes , yHours);
    myGLCD.printNumI(timeTemp.min, xMinutes + hourScreenOffset, yHours);
  }
  else {
    myGLCD.printNumI(timeTemp.min, xMinutes, yHours);
  }

  int xDate = xMinutes + xHourRain + hourScreenOffset;
  // Paint date
  if (timeTemp.date < 10)
  {
    myGLCD.print("0", xDate, yHours);
    myGLCD.printNumI(timeTemp.date, xDate + hourScreenOffset, yHours);
  }
  else {
    myGLCD.printNumI(timeTemp.date, xDate, yHours);
  }

  int xMonth = xDate + xHourRain;
  // Paint month
  if (timeTemp.mon < 10)
  {
    myGLCD.print("0", xMonth + 10, yHours);
    myGLCD.printNumI(timeTemp.mon, xMonth + hourScreenOffset + 10, yHours);
  }
  else {
    myGLCD.printNumI(timeTemp.mon, xMonth + 10 , yHours);
  }

  // Paint year
  myGLCD.printNumI(timeTemp.year, xMonth + xHourRain + hourScreenOffset, yHours);

  if (refreshAll) {

    myGLCD.print("Hora", 35 , yHours - 40);

    //Formato 24 horas
    setFont(SMALL, blackColor, whiteColor);
    myGLCD.print("(24HR)", 45, yHours - 20 );

    setFont(LARGE, blackColor, whiteColor);
    myGLCD.print("Fecha", 180, yHours - 40);
    myGLCD.print(".", xMonth - 5 , yHours);
    myGLCD.print(".", xMonth + hourScreenOffset + 25 , yHours);
    clockIndex = 0;

    printHead("Conf. Fecha y Hora");

    printLineSeparator();

    printButtonCommon();

    clockScreenUp.print();
    clockScreenDown.print();

    hourSelect.setPosition( xInit - 2 , yHours - 5 , xInit + 35, yHours + 21 );
    minutesSelect.setPosition(xMinutes - 2 , yHours - 5, xMinutes + 33, yHours + 21 );
    dateSelect.setPosition(xDate - 2, yHours - 5 , xDate + 33, yHours + 21 );
    monthSelect.setPosition(xMonth + 8 , yHours - 5, xMonth + 45, yHours + 21 );
    yearSelect.setPosition(xMonth + xHourRain + hourScreenOffset - 2, yHours - 5, xMonth + xHourRain + hourScreenOffset + 65, yHours + 21 );

  }

  printClockButtons(&hourSelect, &minutesSelect, &dateSelect, &monthSelect, &yearSelect);
}
/******************************** CLOCK SCREEN END ***********************************/

void processClockScreen() {

  if (saveButton.pushed(x, y)) {
    rtc.setTime(timeTemp.hour, timeTemp.min, timeTemp.sec);
    rtc.setDate(timeTemp.date, timeTemp.mon, timeTemp.year);
    rtc.setDOW(timeTemp.dow);
    rtc.begin();
    goToMainScreen();
  } else {

    if (hourSelect.pushed(x, y) || minutesSelect.pushed(x, y) || dateSelect.pushed(x, y) || monthSelect.pushed(x, y) || yearSelect.pushed(x, y)) {
      clockIndex = clockSelectProcess(&hourOnRainSelect, &minutesSelect, &dateSelect, &monthSelect, &yearSelect);
    }

    if (clockScreenUp.pushed(x, y)) {
      switch (clockIndex) {
        case 0:
          timeTemp.hour = (timeTemp.hour + 1) % 24;
          break;
        case 1:
          timeTemp.min = (timeTemp.min + 1) % 60;
          break;
        case 2:
          timeTemp.date += 1;
          timeTemp.date = validateDate(timeTemp.date, timeTemp.mon, timeTemp.year);
          break;
        case 3:
          timeTemp.mon = (timeTemp.mon + 1) % 13;
          timeTemp.date = validateDateForMonth(timeTemp.date, timeTemp.mon, timeTemp.year);
          break;
        case 4:
          timeTemp.year += 1;
          if (timeTemp.year == 2100)
            timeTemp.year = 2000;
          timeTemp.date = validateDateForMonth(timeTemp.date, timeTemp.mon, timeTemp.year);
          break;
      }
    }
    if (clockScreenDown.pushed(x, y)) {
      switch (clockIndex) {
        case 0:
          timeTemp.hour = (timeTemp.hour + 23) % 24;
          break;
        case 1:
          timeTemp.min = (timeTemp.min + 59) % 60;
          break;
        case 2:
          timeTemp.date -= 1;
          timeTemp.date = validateDate(timeTemp.date, timeTemp.mon, timeTemp.year);
          break;
        case 3:
          timeTemp.mon = (timeTemp.mon + 12) % 13;
          if (timeTemp.mon == 0)
            timeTemp.mon = 12;
          timeTemp.date = validateDateForMonth(timeTemp.date, timeTemp.mon, timeTemp.year);
          break;
        case 4:
          timeTemp.year = (timeTemp.year + 2099) % 2100;
          timeTemp.date = validateDateForMonth(timeTemp.date, timeTemp.mon, timeTemp.year);
          break;
      }
    }
    timeTemp.dow = calcDOW(timeTemp.date, timeTemp.mon, timeTemp.year);

    clockScreen(false);
  }
}

