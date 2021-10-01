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
  maks.setLed(0, 8, 8, true);
  int i, j;
  for(i = 0; i < 9; i++)
  {
    if(i%2 == 1)
    {
      for(j = 0; j< 9; j++)
      {
        maks.setLed(0, i, j, true);
        maks.setLed(0, i, j - 1, true);
        maks.setLed(0, i, j - 2, true);
        delay(250);
        maks.setLed(0, i, j, false);
        maks.setLed(0, i, j - 1, false);
        maks.setLed(0, i, j - 2, false);
      }
    }
    
    if(i%2 == 0)
    {
      for(j = 9; j > 0; j--)
      {
        maks.setLed(0, i, j, true);
        maks.setLed(0, i, j - 1, true);
        maks.setLed(0, i, j - 2, true);
        delay(250);
        maks.setLed(0, i, j, false);
        maks.setLed(0, i, j - 1, false);
        maks.setLed(0, i, j - 2, false);
      }
    }
    
  }
}
