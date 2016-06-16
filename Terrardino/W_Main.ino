void goToMainScreen() {
  dispScreen = 0;
  cleanScreen();
  mainScreen(true);
}

void processMyTouch() {

  myTouch.read();

  x = myTouch.getX();
  y = myTouch.getY();

  if (cancelButton.pushed(x, y) && (dispScreen != 0)) {
    readFromEEPROM();
    goToMainScreen();
  }
  else {
    switch (dispScreen) {
      case 0:     //--------------- Pantalla Principal (Press Any Key) ---------------
        if (humidityButton.pushed(x, y)) {
          readFromEEPROM();
          dispScreen = 3;
          cleanScreen();
          humidityScreen(true);
        } else if (temperatureButton.pushed(x, y)) {
          readFromEEPROM();
          dispScreen = 4;
          cleanScreen();
          temperatureScreen(true);
        } else if (rainSettingsButton.pushed(x, y)) {
          readFromEEPROM();
          dispScreen = 1;
          cleanScreen();
          rainScreen();
        } else if (timeDateButton.pushed(x, y)) {
          dispScreen = 7;
          cleanScreen();
          clockScreen();
        } else if (aboutButton.pushed(x, y)) {
          dispScreen = 8;
          cleanScreen();
          aboutScreen();
        }
        break;
      case 1:
        processRainScreen();
        break;
      case 2:
        processRainSetupScreen();
        break;
      case 3:
        processHumidityScreen();
        break;
      case 4:
        processTemperatureScreen();
        break;
      case 5:
        // ledTestScreen
        break;
      case 6:
        //processLedScreenTest();
        break;
      case 7:
        processClockScreen();
        break;
    }
  }
}
