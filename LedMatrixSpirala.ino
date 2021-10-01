/*
Program do obsługi wyświetlacza led matrix 8x8 z układem MAX7219, by wąż (czyt. 'fonsz') chodził po spirali.
Autor: R.A.S.
20.02.2015
*/

#include <LedControl.h>
LedControl maks = LedControl(13, 11, 12, 1); // DIN do 13, CS do 12, CLK do 11
int kolejka = 0, bok = 0, j = 0;
boolean stan = true;

void setup()
{
  maks.shutdown(0, false);
  maks.setIntensity(0, 0); //jasność od 0 do 15
  maks.clearDisplay(0);

}

//maks.setLed(addr, row, col, true);

void loop()
{ 
  if(bok%4 == 0)
  {
   for(j = kolejka; j <= 7 - kolejka; j++)
   {
      maks.setLed(0, kolejka, j, stan);
      delay(100);
   }
  }
  
  if(bok%4 == 1)
  {
   for(j = kolejka; j <= 7 - kolejka; j++)
   {
      maks.setLed(0, j, 7 - kolejka, stan);
      delay(100);
   }
  }
  
  if(bok%4 == 2)
  {
   for(j = 7 - kolejka; j >= 0; j--)
   {
      maks.setLed(0, 7 - kolejka, j, stan);
      delay(100);
   }
  }
  
  if(bok%4 == 3)
  {
   for(j = 7 - kolejka; j >= 0; j--)
   {
      maks.setLed(0, j, kolejka, stan);
      delay(100);
   }
   
     kolejka++; //zwiększenie co jedno okrążenie
  }
  
  bok++;
  
  if(kolejka == 15)
  {
    kolejka = 0;
    bok = 0;
    if(stan == true)
    {
     stan = false;
    }
    else
    {
     stan = true; 
    }
  }
}
