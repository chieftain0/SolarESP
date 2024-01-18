void Delay(int ms) // Efficient replacement of delay(), that does not fuck with the code execution
{
  startTime = millis();
  while (millis() <= startTime + ms)
  {
  }
}