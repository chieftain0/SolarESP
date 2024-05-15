int SendTelegramMessage(String message)
{
  String url = "https://api.telegram.org/bot" + authToken + "/sendMessage"; // Construct the URL for the Telegram API endpoint

  String payload = "chat_id=" + ChatID + "&parse_mode=MarkdownV2" + "&text=" + message; // Construct the payload

  SolarESP_Telegram_HTTPClient.begin(url);
  SolarESP_Telegram_HTTPClient.addHeader("Content-Type", "application/x-www-form-urlencoded");

  if (SolarESP_Telegram_HTTPClient.POST(payload) > 0) // Send the POST request and receive response code
  {
    SolarESP_Telegram_HTTPClient.end();
    return 0;
  }
  else
  {
    SolarESP_Telegram_HTTPClient.end();
    return -1;
  }
}

String ConstructMessage()
{

  String message = "```SolarESP\n";
  message += "---------------------------------------\n";
  message += "SolarESP on " + String(ESP.getChipModel()) + " is alive!\n";

  // WiFi connection
  message += "---------------------------------------\n";
  if (WiFiConnectionSuccess == 0)
  {
    message += "Connected to SSID: " + String(SSID) + "\n";
    message += "IP: " + WiFi.localIP().toString() + "\n";
  }
  else
  {
    message += "WiFi connection failed\n";
  }

  // Time connection
  message += "---------------------------------------\n";
  if (TimeConnectionSuccess == 0)
  {
    message += "Time Stamp: " + String(TimeArray[0]) + ":" + String(TimeArray[1]) + ":" + String(TimeArray[2]) +
               " " + String(DateArray[0]) + "/" + String(DateArray[1]) + "/" + String(DateArray[2]) + "\n";
  }
  else
  {
    message += "NTP connection failed\n";
  }

  // OpenWeather connection
  message += "---------------------------------------\n";
  if (OpenWeatherConnectionSuccess == 0)
  {
    message +="OpenWeather\n";
    message += "Weather: " + OpenWeather_Weather + "\n";
    message += "Weather ID: " + String(OpenWeather_WeatherCode) + "\n";
    message += "Temperature: " + String(OpenWeather_temperature) + " C\n";
    message += "Humidity: " + String(OpenWeather_humidity) + " %\n";
    message += "Pressure: " + String(OpenWeather_pressure) + " hPA\n";
    message += "Sunrise Time: " + String(SunriseTime[0]) + ":" + String(SunriseTime[1]) + "\n";
    message += "Sunset Time: " + String(SunsetTime[0]) + ":" + String(SunsetTime[1]) + "\n";
    if (isDay == true && isNight == false)
    {
      message += "It is daytime in " + city + "\n";
    }
    else if (isDay == false && isNight == true)
    {
      message += "It is night time in " + city + "\n";
    }
  }
  else
  {
    message += "OpenWeather connection failed\n";
  }

  // INA220 and HDC2080 status
  message += "---------------------------------------\n";
  if (ina220_bat_found == 0)
  {
    message += "Battery Voltage: " + String(batt_voltage) + " mV\n";
    message += "Battery Current: " + String(batt_current) + " mA\n";
  }
  else
  {
    message += "INA220_BATTERY not found\n";
  }

  if (ina220_pv_found == 0)
  {
    message += "PV Voltage: " + String(pv_voltage) + " mV\n";
    message += "PV Current: " + String(pv_current) + " mA\n";
  }
  else
  {
    message += "INA220_PV not found\n";
  }

  if (hdc2080_found == 0)
  {
    message += "HDC2080 Temperature: " + String(hdc2080_temperature) + " C\n";
    message += "HDC2080 Humidity: " + String(hdc2080_humidity) + " %\n";
  }
  else
  {
    message += "HDC2080 not found\n";
  }

  // Battery status
  message += "---------------------------------------\n";
  if (batt_voltage <= EXTREME_LOW_BAT_mV && batt_voltage > 0)
  {

    message += "Battery is extremely low: " + String(batt_voltage) + " mV\n";
  }
  else if (batt_voltage <= LOW_BAT_mV && batt_voltage > EXTREME_LOW_BAT_mV && batt_voltage > 0)
  {
    message += "Battery is low: " + String(batt_voltage) + " mV\n";
  }
  else
  {
    message += "Battery is OK: " + String(batt_voltage) + " mV\n";
  }
  
  // Get PSoC temperature
  message += "---------------------------------------\n";
  message += "PSoC temperature: " + String(tempBuiltin) + " C\n";

  // Sleep time status
  message += "---------------------------------------\n";
  if (SleepTimeChanged == 0)
  {
    message += "Sleep time changed to " + String(DEEP_SLEEP_TIME) + " s\n";
  }
  else
  {
    message += "Sleep time not changed\n";
  }

  message += "---------------------------------------\n";
  message += "Going to sleep for " + String(DEEP_SLEEP_TIME) + " s\n";
  message += "Next Wake-Up: " + String(WakeUpTimeArray[0]) + ":" + String(WakeUpTimeArray[1]) + " " + String(WakeUpDateArray[0]) + "/" + String(WakeUpDateArray[1]) + "/" + String(WakeUpDateArray[2]) + "\n";
  message += "---------------------------------------\n";
  message += "\n```";
  return message;
}
