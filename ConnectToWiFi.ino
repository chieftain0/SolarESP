
int ConnectToWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  if(isEAP == true)
  {
    WiFi.begin(SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
  }
  else 
  {
    WiFi.begin(SSID, EAP_PASSWORD);
  }

  startTime = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - startTime >= 30000)
    {
      return -1;
    }
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    return 0;
  }
  return 0;
}