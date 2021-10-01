#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

char x = 40, y = 22;
char krok = 1;
char i = 0;
char kolejka = 0;
void dodajPixel(void);

void setup(void)
{
 display.begin();
 display.setContrast(50);
 display.clearDisplay();
 
 display.drawPixel(x, y, BLACK);
 display.display();
}

void loop(void)
{
 dodajPixel();
 krok++;
 if(((krok+3)%4) == 1)
   krok += 2;
 i++;
 kolejka++;
 
 delay(200);
}

void dodajPixel(void)
{
 char kopiaKrok = (char)krok;
  
 while(kopiaKrok != 0)
 {
   if(kolejka == 0)
   {
      x++;
      y++;
   }
   else if(kolejka == 1)
   {
      x++;
      y--;        
   }
   else if(kolejka == 2)
   {
      x--;
      y--; 
   }
   else if(kolejka == 3)
   {
      x--;
      y++; 
   }
   
   if(kolejka == 4)
    kolejka = 0;
   
   kopiaKrok--;
   display.drawPixel(x, y, BLACK);
   display.display();
 }
}

