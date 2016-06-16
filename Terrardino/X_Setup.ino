/**************************************** SETUP **************************************/
void setup()
{
  Serial.begin(9600);

  dht.begin();

  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();

  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_MEDIUM);

  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 226, 319, 239);

  pinMode(onOffFanPin, OUTPUT);
  pinMode(onOffCalPin, OUTPUT);
  pinMode(onOffRain, OUTPUT);

  // Init RTC
  rtc.begin();

  readFromEEPROM();
  initButtons();
  mainScreen(true);
}
/*********************************** END of SETUP ************************************/
