/*
Program do badania przebiegu zmiennego na pinie Arduino
ze zmienną częstotliwością próbkowania, wynik pomiaru na
wyświetlaczy Nokii 3310 84 x 48 pixeli.
Autor: R.A.S.
15.08.2015
*/

#define WEJSCIOWY A0
#define CZESTOTLIWOSC A1
#define ZAKRES 80
#define POL_ZAKRES 40
#include <Timers.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

void petla(void);
void zczytanieSygnalu(void);
void wyswietlNokia(void);
void rysujLinie(void);

char czasOpoznienia = 100;
char i = 200;
int sygnalA[ZAKRES];
int sygnalB[ZAKRES];
int *ptr;
boolean flaga = 0;
Timers <1> timerA;

void setup()
{
  Serial.begin(9600);
  pinMode(WEJSCIOWY, INPUT);
  pinMode(CZESTOTLIWOSC, INPUT);
  
  timerA.attach(0, 100, petla);
 
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  rysujLinie();
}

void loop()
{
  timerA.process();
}

void petla(void)
{
    czasOpoznienia = map(analogRead(CZESTOTLIWOSC), 0, 1023, 100, 500);
    //dostosowanie do możliwych opóźnień
    zczytanieSygnalu();
    wyswietlNokia();
}

void zczytanieSygnalu(void)
{
  char j = 0;
  flaga = flaga ^ 1;
  if(flaga)
    ptr = sygnalA;
  else
    ptr = sygnalB;
  
  while(j != ZAKRES)
  {
   *(ptr + j) = analogRead(WEJSCIOWY);
   delayMicroseconds(czasOpoznienia);
   j++;
  }
}

void wyswietlNokia(void)
{
  char j = 0;
  char wartosc = 0;
    
  while(j != ZAKRES)
  {
    if(flaga)                    //pierwsze poważne zastosowanie wskaźników
      ptr = sygnalB;             //w moim życiu, BITCHES!
    else
      ptr = sygnalA;
    wartosc = (char)map(*(ptr + j), 0, 1023, 1, 45);
    display.drawPixel(j, wartosc + 1, WHITE);
    display.display();
    
    if(flaga)
      ptr = sygnalA;
    else
      ptr = sygnalB;
    wartosc = (char)map(*(ptr + j), 0, 1023, 1, 45);
    display.drawPixel(j, wartosc + 1, BLACK);
    display.display();
    j++;
  }
}

void rysujLinie(void)
{
  char j;
  for(j = 0; j < 80; j++)
  {
   display.drawPixel(j, 0, BLACK);
   display.display(); 
   display.drawPixel(j, 47, BLACK);
   display.display(); 
  }
}


