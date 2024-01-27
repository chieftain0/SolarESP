void FlashLED()
{
  if (use_led_for_debug == 1)
  {
    digitalWrite(LED, HIGH);
    Delay(500);
    digitalWrite(LED, LOW);
    Delay(500);
  }
}