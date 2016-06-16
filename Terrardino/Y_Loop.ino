/********************************** BEGIN MAIN LOOP **********************************/
void loop(){

  timeCurrent = rtc.getTime();

  if(myTouch.dataAvailable()){
    processMyTouch();
  } 

  unsigned long currentMillis = millis();

  // valida si tiene que activa/desacticar el timer de lluvia cada segundo
  if (currentMillis - previousMillis > 1000){
    
    previousMillis = currentMillis;
    
    if((dispScreen == 0)){
      mainScreen();
      timeStampInitial();
    }
    checkHumidity();
    checkTemp();
    checkRainTimers();
  }
}
/********************************** END OF MAIN LOOP *********************************/
