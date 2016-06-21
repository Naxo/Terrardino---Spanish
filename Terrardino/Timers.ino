int getIntMinutes(uint8_t scheduleHour, uint8_t scheduleMinutes)
{
  return getIntSeconds(scheduleHour, scheduleMinutes, 0);
}

int getIntSeconds(uint8_t scheduleHour, uint8_t scheduleMinutes , uint8_t scheduleSeconds)
{
  return (scheduleHour * 60 * 60) + (scheduleMinutes * 60) + (scheduleSeconds)    ;
}
/*************************************CHECK RAIN TIMERS*********************************/

void checkRainTimers() {

  // Restart global rain flag
  rainOn = false;

  for (int i = 0; i < 15; i++) {
    timeCurrent = rtc.getTime();

    int isEnabled = rainSettings.arrayRain[i].rainEnabled;

    if ((isEnabled == 1) && isRainTimerModify[i] == false)
    {
      if (getIntSeconds(rainSettings.arrayRain[i].offHour, rainSettings.arrayRain[i].offMinute, rainSettings.arrayRain[i].offSecond) > getIntSeconds(rainSettings.arrayRain[i].onHour, rainSettings.arrayRain[i].onMinute, rainSettings.arrayRain[i].onSecond))
      {
        if ((getIntSeconds(timeCurrent.hour, timeCurrent.min, timeCurrent.sec) >= getIntSeconds(rainSettings.arrayRain[i].onHour, rainSettings.arrayRain[i].onMinute, rainSettings.arrayRain[i].onSecond))
            && (getIntSeconds(timeCurrent.hour, timeCurrent.min, timeCurrent.sec) <= getIntSeconds(rainSettings.arrayRain[i].offHour, rainSettings.arrayRain[i].offMinute, rainSettings.arrayRain[i].offSecond)))
        {
          isRainOn[i] = true;
        }
        if (getIntSeconds(timeCurrent.hour, timeCurrent.min, timeCurrent.sec) > getIntSeconds(rainSettings.arrayRain[i].offHour, rainSettings.arrayRain[i].offMinute, rainSettings.arrayRain[i].offSecond))
        {
          isRainOn[i] = false;
        }
      }
    }
    else
    {
      isRainOn[i] = false;
    }
  }

  for (int i = 0; i < 15; i++) {
    rainOn = rainOn || isRainOn[i];
  }

  if (rainOn) {
    digitalWrite(onOffRain, LOW);
    rainSettingsButton.image = Rain_on_icon;
  } else {
    digitalWrite(onOffRain, HIGH);
    rainSettingsButton.image = Rain_off_icon;
  }

}
