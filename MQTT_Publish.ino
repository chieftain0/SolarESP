int MQTT_Publish()
{
  const char *adafruitio_root_ca = // io.adafruit.com root CA
      "-----BEGIN CERTIFICATE-----\n"
      "MIIEjTCCA3WgAwIBAgIQDQd4KhM/xvmlcpbhMf/ReTANBgkqhkiG9w0BAQsFADBh\n"
      "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
      "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
      "MjAeFw0xNzExMDIxMjIzMzdaFw0yNzExMDIxMjIzMzdaMGAxCzAJBgNVBAYTAlVT\n"
      "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
      "b20xHzAdBgNVBAMTFkdlb1RydXN0IFRMUyBSU0EgQ0EgRzEwggEiMA0GCSqGSIb3\n"
      "DQEBAQUAA4IBDwAwggEKAoIBAQC+F+jsvikKy/65LWEx/TMkCDIuWegh1Ngwvm4Q\n"
      "yISgP7oU5d79eoySG3vOhC3w/3jEMuipoH1fBtp7m0tTpsYbAhch4XA7rfuD6whU\n"
      "gajeErLVxoiWMPkC/DnUvbgi74BJmdBiuGHQSd7LwsuXpTEGG9fYXcbTVN5SATYq\n"
      "DfbexbYxTMwVJWoVb6lrBEgM3gBBqiiAiy800xu1Nq07JdCIQkBsNpFtZbIZhsDS\n"
      "fzlGWP4wEmBQ3O67c+ZXkFr2DcrXBEtHam80Gp2SNhou2U5U7UesDL/xgLK6/0d7\n"
      "6TnEVMSUVJkZ8VeZr+IUIlvoLrtjLbqugb0T3OYXW+CQU0kBAgMBAAGjggFAMIIB\n"
      "PDAdBgNVHQ4EFgQUlE/UXYvkpOKmgP792PkA76O+AlcwHwYDVR0jBBgwFoAUTiJU\n"
      "IBiV5uNu5g/6+rkS7QYXjzkwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsG\n"
      "AQUFBwMBBggrBgEFBQcDAjASBgNVHRMBAf8ECDAGAQH/AgEAMDQGCCsGAQUFBwEB\n"
      "BCgwJjAkBggrBgEFBQcwAYYYaHR0cDovL29jc3AuZGlnaWNlcnQuY29tMEIGA1Ud\n"
      "HwQ7MDkwN6A1oDOGMWh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEds\n"
      "b2JhbFJvb3RHMi5jcmwwPQYDVR0gBDYwNDAyBgRVHSAAMCowKAYIKwYBBQUHAgEW\n"
      "HGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwDQYJKoZIhvcNAQELBQADggEB\n"
      "AIIcBDqC6cWpyGUSXAjjAcYwsK4iiGF7KweG97i1RJz1kwZhRoo6orU1JtBYnjzB\n"
      "c4+/sXmnHJk3mlPyL1xuIAt9sMeC7+vreRIF5wFBC0MCN5sbHwhNN1JzKbifNeP5\n"
      "ozpZdQFmkCo+neBiKR6HqIA+LMTMCMMuv2khGGuPHmtDze4GmEGZtYLyF8EQpa5Y\n"
      "jPuV6k2Cr/N3XxFpT3hRpt/3usU/Zb9wfKPtWpoznZ4/44c1p9rzFcZYrWkj3A+7\n"
      "TNBJE0GmP2fhXhP1D/XVfIW/h0yCJGEiV9Glm/uGOa3DXHlmbAcxSyCRraG+ZBkA\n"
      "7h4SeM6Y8l/7MBRpPCz6l8Y=\n"
      "-----END CERTIFICATE-----\n";

  Adafruit_MQTT_Publish DateToPublish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/datetime");

  Adafruit_MQTT_Publish INA220_BATTERY_mV_mA = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ina220-battery-mv-ma");
  Adafruit_MQTT_Publish INA220_PV_mV_mA = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ina220-pv-mv-ma");
  Adafruit_MQTT_Publish HDC2080_TEMP_HUMID = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hdc2080-temp-humid");

  Adafruit_MQTT_Publish OPENWEATHER_WEATHER = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/openweather-weather");
  Adafruit_MQTT_Publish OPENWEATHER_TEMP_HUMID_PRES = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/openweather-temp-humid-pres");
  Adafruit_MQTT_Publish OPENWEATHER_SUNRISE_SUNSET = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/openweather-sunrise-sunset");

  Adafruit_MQTT_Publish TIME_TO_SLEEP_NEXT_WAKE_UP = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/time-to-sleep-next-wake-up");
  Adafruit_MQTT_Publish BATTERY_LEVEL = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/battery-level");

  // Variables to Publish:

  // Date obtained from NTP (as last reported time) (done)

  // INA220 batt mV and mA (done)
  // INA220 pv mV and mA (done)
  // HDC2080 temp and humidity (done)

  // Weather by OpenWeather (done)
  // Temperature, Humidity, Pressure (done)
  // SunriseTime, SunsetTime (done)

  // Time to sleep, Expected wake up time (done)

  // INA220 batt mV as integer

  SolarESP_WiFiClientSecure.setCACert(adafruitio_root_ca);

  if (mqtt.connected() == false)
  {
    mqtt.disconnect();
    Delay(50); // Allow some time to establish connection
    mqtt.connect();
    Delay(50);
  }

  if (mqtt.connected() == true)
  {
    DateToPublish.publish((String(TimeArray[0]) + ":" + String(TimeArray[1]) + ":" + String(TimeArray[2]) + " " + String(DateArray[0]) + "/" + String(DateArray[1]) + "/" + String(DateArray[2])).c_str());

    INA220_BATTERY_mV_mA.publish((String(batt_voltage) + " mV, " + String(batt_current) + " mA").c_str());
    INA220_PV_mV_mA.publish((String(pv_voltage) + " mV, " + String(pv_current) + " mA").c_str());
    HDC2080_TEMP_HUMID.publish((String(hdc2080_temperature) + " C, " + String(hdc2080_humidity) + " %").c_str());

    OPENWEATHER_WEATHER.publish(OpenWeather_Weather.c_str());
    OPENWEATHER_TEMP_HUMID_PRES.publish((String(OpenWeather_temperature) + " C, " + String(OpenWeather_humidity) + " %, " + String(OpenWeather_pressure) + " hPa").c_str());
    OPENWEATHER_SUNRISE_SUNSET.publish((String(SunriseTime[0]) + ":" + String(SunriseTime[1]) + " - " + String(SunsetTime[0]) + ":" + String(SunsetTime[1])).c_str());

    TIME_TO_SLEEP_NEXT_WAKE_UP.publish(("Sleeping for " + String(DEEP_SLEEP_TIME) + " s. Next Wake-Up: " + String(WakeUpTimeArray[0]) + ":" + String(WakeUpTimeArray[1]) + " " + String(WakeUpDateArray[0]) + "/" + String(WakeUpDateArray[1]) + "/" + String(WakeUpDateArray[2])).c_str());

    if (batt_voltage == -1)
    {
      BATTERY_LEVEL.publish(69); // LoL
    }
    else
    {
      BATTERY_LEVEL.publish((batt_voltage - 3000) * (100 - 0) / (4200 - 3000));
    }

    mqtt.disconnect();
    SolarESP_WiFiClientSecure.stop();
    return 0;
  }
  else
  {
    return -1;
  }
}