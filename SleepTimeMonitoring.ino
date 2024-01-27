int MonitorSleepTime()
{
  uint64_t PreviousSleepTime = DEEP_SLEEP_TIME;

  AlterSleepTime();

  UnixStamp.setDateTime(DateArray[2], DateArray[1], DateArray[0], TimeArray[0], TimeArray[1], TimeArray[2]);
  UnixStamp.getDateTime(UnixStamp.getUnix() + (uint32_t)(DEEP_SLEEP_TIME));

  WakeUpTimeArray[0] = UnixStamp.hour;
  WakeUpTimeArray[1] = UnixStamp.minute;

  WakeUpDateArray[0] = UnixStamp.day;
  WakeUpDateArray[1] = UnixStamp.month;
  WakeUpDateArray[2] = UnixStamp.year;

  if (PreviousSleepTime == DEEP_SLEEP_TIME)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

void AlterSleepTime() // Alter the sleep time based on Day/Night, Weather, Battery charge level
{
  if (batt_voltage < EXTREME_LOW_BAT_mV && batt_voltage > 0) // Extremely low battery
  { 
    DEEP_SLEEP_TIME = 24 * 60 * 60 * EXTREME_LOW_BAT_SLEEP_FOREVER; // Sleep for 24 hours and hope the battery is charged or sleep until reset
    return;
  }
  if (batt_voltage < LOW_BAT_mV && batt_voltage > 0) // Low battery
  {
    DEEP_SLEEP_TIME = DEEP_SLEEP_TIME * LOW_BAT_SLEEP_FACTOR;
    return;
  }
  if (isDay == false && isNight == true) // Nighty Night
  {
    DEEP_SLEEP_TIME = DEEP_SLEEP_TIME * NIGHT_TIME_SLEEP_FACTOR;
    return;
  }
  // https://openweathermap.org/weather-conditions
  if (OpenWeather_WeatherCode < 800 || OpenWeather_WeatherCode > 802) // Bad Weather
  {
    DEEP_SLEEP_TIME = DEEP_SLEEP_TIME * BAD_WEATHER_SLEEP_FACTOR;
    return;
  }
}