/**************************** TEMPERATURE SCREEN ********************************/
void temperatureScreen (boolean refreshAll=false){

  if (refreshAll){
    
    if ((setTempTerrarium==0)) {
      setTempTerrarium = 26.0;  
    }

    printHead("Conf. Temperatura");

    printLineSeparator();

    printButtonCommon();

    setFont(SMALL, blackColor, whiteColor);
    myGLCD.print("Temperatura a mantener en terrario (C):", 10, 34);

    myGLCD.print("Variacion de temperatura (C):", CENTER, 84);

    temperatureMinus.print();
    temperaturePlus.print();
    offsetPlus.print();
    offsetMinus.print();
  }

  setFont(LARGE, blackColor, whiteColor);
  myGLCD.printNumF(setTempTerrarium, 1, CENTER, 54);
  myGLCD.printNumF(offTempTerrarium, 1, CENTER, 104);
}
/**************************** TEMPERATURE SCREEN END  *****************************/

void checkTemp(){
  tempTerrarium = dht.readTemperature();

  if(tempTerrarium < setTempTerrarium){
    isHeaterOn = true;
    digitalWrite(onOffCalPin, LOW);
  }
  else if( tempTerrarium > (setTempTerrarium + offTempTerrarium)){
    isHeaterOn = false;
    digitalWrite(onOffCalPin, HIGH);
  }  
}

void processTemperatureScreen(){
  if (saveButton.pushed(x,y)){
    saveTempToEEPROM();
    goToMainScreen();
  }
  else{
    if(temperatureMinus.pushed(x,y)){
      setTempTerrarium -=0.1;
      temperatureScreen();
    }
    if(offsetMinus.pushed(x,y)){
      offTempTerrarium -=0.1;
      temperatureScreen();
    }
    if(temperaturePlus.pushed(x,y)){
      setTempTerrarium +=0.1;
      temperatureScreen();
    }
    if(offsetPlus.pushed(x,y)){
      offTempTerrarium +=0.1;
      temperatureScreen();
    }
  }
}
