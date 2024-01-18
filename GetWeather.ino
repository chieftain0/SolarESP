int GetWeather()
{
  SolarESP_OpenWeather_HTTPClient.begin(SolarESP_OpenWeather_WiFiClient, serverURL.c_str());
  int httpResponseCode = SolarESP_OpenWeather_HTTPClient.GET();
  if (httpResponseCode > 0)
  {
    WeatherJSONasString = SolarESP_OpenWeather_HTTPClient.getString();
  }

  if (WeatherJSONasString.length() > 3)
  {
    WeatherJSON = JSON.parse(WeatherJSONasString);

    OpenWeather_Weather = JSON.stringify(WeatherJSON["weather"][0]["main"]);
    OpenWeather_WeatherCode = (int)(WeatherJSON["weather"][0]["id"]);
    OpenWeather_temperature = (double)(WeatherJSON["main"]["temp"]);
    OpenWeather_humidity = (double)(WeatherJSON["main"]["humidity"]);
    OpenWeather_pressure = (double)(WeatherJSON["main"]["pressure"]);
    OpenWeather_sunrise_unix = (uint32_t)(WeatherJSON["sys"]["sunrise"]);
    OpenWeather_sunset_unix = (uint32_t)(WeatherJSON["sys"]["sunset"]);

    UnixStamp.getDateTime(OpenWeather_sunrise_unix);
    SunriseTime[0] = (int)(UnixStamp.hour);
    SunriseTime[1] = (int)(UnixStamp.minute);

    UnixStamp.getDateTime(OpenWeather_sunset_unix);
    SunsetTime[0] = (int)(UnixStamp.hour);
    SunsetTime[1] = (int)(UnixStamp.minute);

    if ((TimeArray[0] * 60 + TimeArray[1] >= SunriseTime[0] * 60 + SunriseTime[1]) && (TimeArray[0] * 60 + TimeArray[1] <= SunsetTime[0] * 60 + SunsetTime[1]))
    {
      isDay = true;
      isNight = false;
    }
    else
    {
      isDay = false;
      isNight = true;
    }

    SolarESP_OpenWeather_HTTPClient.end();
    SolarESP_OpenWeather_WiFiClient.stop();
    return 0;
  }
  else
  {
    SolarESP_OpenWeather_HTTPClient.end();
    SolarESP_OpenWeather_WiFiClient.stop();
    return -1;
  }
}
