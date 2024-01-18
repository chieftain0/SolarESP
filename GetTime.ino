int GetTime() // Get time from NTP server
{
  configTime(GMToffset, DayLightOffset, NTPserver);
  struct tm timeData;
  if (!getLocalTime(&timeData))
  {
    return -1;
  }
  
  TimeArray[0] = int(timeData.tm_hour); // hours
  TimeArray[1] = int(timeData.tm_min); // min
  TimeArray[2] = int(timeData.tm_sec); // sec
  DateArray[0] = int(timeData.tm_mday); //day
  DateArray[1] = int(timeData.tm_mon + 1); //month
  DateArray[2] = int(timeData.tm_year + 1900); //year
  return 0;
}