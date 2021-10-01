/*
Program do obsługi wyświetlacza led matrix 8x8 z układem MAX7219.
Autor: R.A.S.
20.02.2015
*/

#include <LedControl.h>
LedControl maks = LedControl(13, 11, 12, 1); // DIN do 13, CS do 12, CLK do 11

void setup()
{
  maks.shutdown(0, false);
  maks.setIntensity(0, 0); //jasność od 0 do 15
  maks.clearDisplay(0);
}

//maks.setLed(addr, row, col, true);

void loop()
{
 maks.setLed(0, 7, 7, true);
 delay(500);
 maks.setLed(0, 7, 7, false);
 delay(500);
}
