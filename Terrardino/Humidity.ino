/**************************** HUMIDITY SCREEN ****************************************/
void humidityScreen(boolean refreshAll = false) {

  if (refreshAll) {

    if ((setHumTerrarium == 0)) {
      setHumTerrarium = 80.0;
    }

    printHead("Conf. Humedad");

    printLineSeparator();

    printButtonCommon();

    setFont(SMALL, blackColor, whiteColor);
    myGLCD.print("Humedad a mantener en terrario (%):", 20, 34);

    myGLCD.print("Variacion Humedad (%):", CENTER, 84);

    temperatureMinus.print();
    temperaturePlus.print();
    offsetPlus.print();
    offsetMinus.print();
  }

  setFont(LARGE, blackColor, whiteColor);
  myGLCD.printNumF(setHumTerrarium, 1, CENTER, 54);
  myGLCD.printNumF(offHumTerrarium, 1, CENTER, 104);

}
/**************************** HUMIDITY SCREEN END *******************************/

void checkHumidity() {

  humidityTerrarium = dht.readHumidity();

  float humidityLimit = setHumTerrarium - offHumTerrarium;

  if (humidityLimit > 100.0) {
    humidityLimit = 100.0;
  } else if (humidityLimit < 0 ) {
    humidityLimit = 0.0;
  }


  if (humidityTerrarium > (setHumTerrarium)) {
    isCoolerOn = true;
    digitalWrite(onOffFanPin, LOW);
  } else if (humidityTerrarium < humidityLimit ) {
    isCoolerOn = false;
    digitalWrite(onOffFanPin, HIGH);
  }
}

void processHumidityScreen() {

  if (saveButton.pushed(x, y)) {
    saveHumiditytoEEPROM();
    goToMainScreen();
  } else {
    if (temperatureMinus.pushed(x, y)) {
      setHumTerrarium -= 0.1;
      humidityScreen();
    }
    if (offsetMinus.pushed(x, y)) {
      offHumTerrarium -= 0.1;
      humidityScreen();
    }
    if (temperaturePlus.pushed(x, y)) {
      setHumTerrarium += 0.1;
      humidityScreen();
    }
    if (offsetPlus.pushed(x, y)) {
      offHumTerrarium += 0.1;
      humidityScreen();
    }
  }
}
