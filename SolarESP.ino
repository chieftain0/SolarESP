/*  ____        _            _____ ____  ____   */
/* / ___|  ___ | | __ _ _ __| ____/ ___||  _ \  */
/* \___ \ / _ \| |/ _` | '__|  _| \___ \| |_) | */
/*  ___) | (_) | | (_| | |  | |___ ___) |  __/  */
/* |____/ \___/|_|\__,_|_|  |_____|____/|_|     */
/*   https://github.com/chieftain0/SolarESP     */

// Search for "ABCDEFGH" and replace them with your data
#include <Wire.h>
#include "INA219.h"
#include <HDC2080.h>

#include <WiFi.h>
#include "esp_wpa2.h"
#include "time.h"

#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <UnixTime.h>

//Debug options
#define LED 2
bool use_led_for_debug = 0;
bool use_serial_for_debug = 0;


// deep sleep settings
uint64_t DEEP_SLEEP_TIME = 15 * 60; // in S
#define uS_TO_S_FACTOR 1000000
#define EXTREME_LOW_BAT_SLEEP_FOREVER 1 // 1 - sleep for 24 hours, 0 - sleep until reset
#define LOW_BAT_SLEEP_FACTOR 3
#define NIGHT_TIME_SLEEP_FACTOR 2
#define BAD_WEATHER_SLEEP_FACTOR 2
#define EXTREME_LOW_BAT_mV 3200
#define LOW_BAT_mV 3300

int WakeUpTimeArray[2] = {-1, -1};     // hh:mm
int WakeUpDateArray[3] = {-1, -1, -1}; // dd/mm/yyyy

// WiFi settings
bool isEAP = true; // true - Enterprise, false - Non Enterprise
String EAP_IDENTITY = "ABCDEFGH";
String EAP_USERNAME = "ABCDEFGH";
String EAP_PASSWORD = "ABCDEFGH";
String SSID = "ABCDEFGH";

// NTP settings
#define TimeZone 4 // UAE timezone
const char *NTPserver = "pool.ntp.org";
const long GMToffset = TimeZone * 3600;
const int DayLightOffset = 0;

// for NTP parsed variables
int TimeArray[3] = {0, 0, 0}; // hh:mm:ss
int DateArray[3] = {0, 0, 0}; // dd/mm/yyyy

// OpenWeather settings
String OpenWeatherAPIkey = "ABCDEFGH";
String city = "Abu Dhabi";
String countryCode = "AE";
String serverURL = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + OpenWeatherAPIkey + "&units=metric";
HTTPClient SolarESP_OpenWeather_HTTPClient;
WiFiClient SolarESP_OpenWeather_WiFiClient;

// for OpenWeather read variables
String WeatherJSONasString = "{}";
JSONVar WeatherJSON;
String OpenWeather_Weather = "";
int OpenWeather_WeatherCode = -1;
float OpenWeather_humidity = -1;
float OpenWeather_temperature = -1;
float OpenWeather_pressure = -1;
uint32_t OpenWeather_sunrise_unix = -1;
uint32_t OpenWeather_sunset_unix = -1;
int SunriseTime[2] = {-1, -1};
int SunsetTime[2] = {-1, -1};
bool isDay = true;
bool isNight = true;
UnixTime UnixStamp(TimeZone);

// for built-in sensors
float tempBuiltin;

// Telegram settings
String authToken = "ABCDEFGH";
String ChatID = "ABCDEFGH";
HTTPClient SolarESP_Telegram_HTTPClient;

// I2C pins
#define SDA_PIN 7
#define SCL_PIN 6

// for INA220
#define INA220_BAT_ADDRESS 0x44
#define INA220_PV_ADDRESS 0x45
INA219 ina220_bat(INA220_BAT_ADDRESS);
INA219 ina220_pv(INA220_PV_ADDRESS);

// for HDC2080
#define HDC2080_ADDRESS 0x40
HDC2080 hdc2080(HDC2080_ADDRESS);

// for I2C read variables
float batt_voltage = -1;        // mV
float batt_current = -1;        // mA
float pv_voltage = -1;          // mV
float pv_current = -1;          // mA
float hdc2080_humidity = -1;    //%
float hdc2080_temperature = -1; // C

// Reconnect attempts settings
#define RECONNECT_ATTEMPT_WAIT_TIME 500

// General purpose variables
int startTime = 0;
int WiFiConnectionSuccess = -1;
int TimeConnectionSuccess = -1;
int OpenWeatherConnectionSuccess = -1;
int TelegramConnectionSuccess = -1;
int SleepTimeChanged = -1;
int ina220_bat_found = -1;
int ina220_pv_found = -1;
int hdc2080_found = -1;

void setup()
{
}

void loop()
{
  onAwake();
  Sleep();
}

void onAwake()
{

  if(use_serial_for_debug == 1)
  {
    Serial.begin(115200);
  }
  
  Serial.println();
  Serial.println("------------------------------------------------");
  Serial.println("SolarESP on " + String(ESP.getChipModel()) + " is alive!");

  // Connect to WiFi
  Serial.println("------------------------------------------------");
  Serial.println("Attempting connection to SSID: " + String(SSID));
  if (ConnectToWiFi() == 0)
  {
    WiFiConnectionSuccess = 0;
    Serial.println("Connected to SSID: " + String(SSID));
    Serial.print("IP: ");
    Serial.println(WiFi.localIP()); // inform the user about his IP address
    FlashLED();
  }
  else
  {
    Serial.println("Connection failed");
    return;
  }

  // Get time from NTP server
  Serial.println("------------------------------------------------");
  if (GetTime() == 0)
  {
    TimeConnectionSuccess = 0;
    Serial.print("Time Stamp: ");
    Serial.println(String(TimeArray[0]) + ":" + String(TimeArray[1]) + ":" + String(TimeArray[2]) + " " + String(DateArray[0]) + "/" + String(DateArray[1]) + "/" + String(DateArray[2]));
    FlashLED();
  }
  else
  {
    Serial.println("NTP connection - attempt #1: FAIL");
    Delay(RECONNECT_ATTEMPT_WAIT_TIME);
    if (GetTime() == 0) // Attempt to connect once again
    {
      TimeConnectionSuccess = 0;
      Serial.print("Time Stamp: ");
      Serial.println(String(TimeArray[0]) + ":" + String(TimeArray[1]) + ":" + String(TimeArray[2]) + " " + String(DateArray[0]) + "/" + String(DateArray[1]) + "/" + String(DateArray[2]));
      FlashLED();
    }
    else
    {
      Serial.println("NTP connection - attempt #2: FAIL");
      Serial.println("NTP unresponsive");
    }
  }

  // Get weather
  Serial.println("------------------------------------------------");
  if (GetWeather() == 0)
  {
    OpenWeatherConnectionSuccess = 0;
    Serial.println("Weather by OpenWeather: " + OpenWeather_Weather);
    Serial.println("Weather ID by OpenWeather: " + String(OpenWeather_WeatherCode));
    Serial.println("Temperature by OpenWeather: " + String(OpenWeather_temperature) + " C");
    Serial.println("Humidity by OpenWeather: " + String(OpenWeather_humidity) + " %");
    Serial.println("Pressure by OpenWeather: " + String(OpenWeather_pressure) + " hPa");
    Serial.println("Sunrise Time by OpenWeather: " + String(SunriseTime[0]) + ":" + String(SunriseTime[1]));
    Serial.println("Sunset Time by OpenWeather: " + String(SunsetTime[0]) + ":" + String(SunsetTime[1]));
    if (isDay == true && isNight == false)
    {
      Serial.println("It is daytime in " + city);
    }
    else if (isDay == false && isNight == true)
    {
      Serial.println("It is night time in " + city);
    }
    FlashLED();
  }
  else
  {
    Serial.println("OpenWeather connection - attempt #1: FAIL");
    Delay(RECONNECT_ATTEMPT_WAIT_TIME);
    if (GetWeather() == 0) // Attempt to connect once again
    {
      OpenWeatherConnectionSuccess = 0;
      Serial.println("OpenWeather");
      Serial.println("Weather: " + OpenWeather_Weather);
      Serial.println("Weather ID: " + String(OpenWeather_WeatherCode));
      Serial.println("Temperature: " + String(OpenWeather_temperature) + " C");
      Serial.println("Humidity: " + String(OpenWeather_humidity) + " %");
      Serial.println("Pressure: " + String(OpenWeather_pressure) + " hPa");
      Serial.println("Sunrise Time: " + String(SunriseTime[0]) + ":" + String(SunriseTime[1]));
      Serial.println("Sunset Time: " + String(SunsetTime[0]) + ":" + String(SunsetTime[1]));
      if (isDay == true && isNight == false)
      {
        Serial.println("It is daytime in " + city);
      }
      else if (isDay == false && isNight == true)
      {
        Serial.println("It is night time in " + city);
      }
      FlashLED();
    }
    else
    {
      Serial.println("OpenWeather connection - attempt #2: FAIL");
      Serial.println("OpenWeather unresponsive");
    }
  }

  // Read data from I2C devices
  // Will probably need to adjust and calibrate readings
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("------------------------------------------------");
  // Read data from INA220s
  if (ina220_bat.isConnected() == true)
  {
    ina220_bat_found = 0;
    ina220_bat.setMaxCurrentShunt(1.0, 0.1); // Mandatory to set these values (maxExpectedCurrent = 1.0A, Rshunt = 0.1ohm)
    batt_voltage = ina220_bat.getBusVoltage_mV();
    batt_current = ina220_bat.getCurrent_mA();
    Serial.println("Battery Voltage: " + String(batt_voltage) + " mV");
    Serial.println("Battery Current: " + String(batt_current) + " mA");
  }
  else
  {
    Serial.println("INA220_BATTERY not found");
  }
  if (ina220_pv.isConnected() == true)
  {
    ina220_pv_found = 0;
    ina220_pv.setMaxCurrentShunt(3.0, 0.1); // Mandatory to set these values (maxExpectedCurrent = 3.0A, Rshunt = 0.1ohm)
    pv_voltage = ina220_pv.getBusVoltage_mV();
    pv_current = ina220_pv.getCurrent_mA();
    Serial.println("PV Voltage: " + String(pv_voltage) + " mV");
    Serial.println("PV Current: " + String(pv_current) + " mA");
  }
  else
  {
    Serial.println("INA220_PV not found");
  }

  // Read data from HDC2080
  if (hdc2080.isConnected() == true)
  {
    hdc2080_found = 0;
    hdc2080.setRate(0); // Manual reading
    hdc2080.triggerMeasurement();
    hdc2080_humidity = hdc2080.readHumidity();
    hdc2080_temperature = hdc2080.readTemp();
    Serial.println("HDC2080 Temperature: " + String(hdc2080_temperature) + " C");
    Serial.println("HDC2080 Humidity: " + String(hdc2080_humidity) + " %");
  }
  else
  {
    Serial.println("HDC2080 not found");
  }

  // Report Battery status
  Serial.println("------------------------------------------------");
  if (batt_voltage <= EXTREME_LOW_BAT_mV && batt_voltage > 0) // Low battery
  {
    Serial.println("Battery is extremely low: " + String(batt_voltage) + " mV");
  }
  else if (batt_voltage <= LOW_BAT_mV && batt_voltage > EXTREME_LOW_BAT_mV && batt_voltage > 0) // Extremely low battery
  {
    Serial.println("Battery is low: " + String(batt_voltage) + " mV");
  }
  else
  {
    Serial.println("Battery is OK: " + String(batt_voltage) + " mV");
  }

  // Get PSoC temperature
  if(readSoCtemp() == 0)
  {
    Serial.println("------------------------------------------------");
    Serial.println("PSoC temperature: " + String(tempBuiltin) + " C");
  }

  // Monitor sleep times
  Serial.println("------------------------------------------------");
  if (MonitorSleepTime() == 0)
  {
    Serial.println("Sleep time not changed");
    FlashLED();
  }
  else
  {
    SleepTimeChanged = 0;
    Serial.println("Sleep time changed to " + String(DEEP_SLEEP_TIME) + " s");
  }

  // Send readings via Telegram bot
  Serial.println("------------------------------------------------");
  if (SendTelegramMessage(ConstructMessage()) == 0)
  {
    TelegramConnectionSuccess = 0;
    Serial.println("Telegram message sent");
    FlashLED();
  }
  else
  {
    Serial.println("Telegram message - attempt #1: FAIL");
    Delay(RECONNECT_ATTEMPT_WAIT_TIME);
    if (SendTelegramMessage(ConstructMessage()) == 0)
    {
      TelegramConnectionSuccess = 0;
      Serial.println("Telegram message sent");
      FlashLED();
    }
    else
    {
      Serial.println("Telegram message - attempt #2: FAIL");
      Serial.println("Telegram unresponsive");
    }
  }
}

void Sleep()
{
  Serial.println("------------------------------------------------");
  Serial.println("Going to sleep for " + String(DEEP_SLEEP_TIME) + " s");
  Serial.println("Next Wake-Up: " + String(WakeUpTimeArray[0]) + ":" + String(WakeUpTimeArray[1]) + " " + String(WakeUpDateArray[0]) + "/" + String(WakeUpDateArray[1]) + "/" + String(WakeUpDateArray[2]));
  Serial.println("------------------------------------------------");
  Serial.flush();

  // stop all activities of the PSoC
  // makes me sleep better
  Serial.end();
  SolarESP_OpenWeather_WiFiClient.stop();
  SolarESP_OpenWeather_HTTPClient.end();
  SolarESP_Telegram_HTTPClient.end();
  WiFi.disconnect();
  Wire.end();

  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * (uint64_t)(uS_TO_S_FACTOR));
  esp_deep_sleep_start();
}
