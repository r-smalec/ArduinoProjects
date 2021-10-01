/*
Gra w tenisa z przeciwnikiem o inteligencji szamponu dla szachistów, MAX7219 jako kontroler matrycy led 8x8, dwa przyciski.
Autor: R.A.S.
21.02.2015
*/
#include <LedControl.h>
#include <Bounce2.h>
#include <Timers.h>

void ruchPaletka();
void ruchPilka();
void wyczyscRzad(char i);
void reset();

LedControl maks = LedControl(13, 11, 12, 1);
Bounce gora = Bounce();
Bounce dol = Bounce();
Timers <1> timerA;
Timers <2> timerB;
Timers <3> timerC;
volatile int poz = 2;           //pozycja paletki
volatile int pilka[6];
volatile bool przyciskG = 0;
volatile bool przyciskD = 0;
//####################### SETUP #############################
void setup()
{
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(A0, INPUT);
  gora.attach(3);
  gora.interval(5);
  dol.attach(6);
  dol.interval(5);
  
  maks.shutdown(0, false);
  maks.setIntensity(0, 0); //jasność od 0 do 15
  maks.clearDisplay(0);
  
  timerA.attach(0, 149, ruchPaletkaG);
  timerB.attach(0, 149, ruchPaletkaD);
  timerC.attach(0, 119, ruchPilka);
  
  pilka[0] = 1; //pozycja początkowa (3;3), kolumna
  pilka[1] = 7;                           //rząd
  pilka[2] = 1; //poprzednia pozycja, kolumna
  pilka[3] = 1;                     //rząd
  pilka[4] = 1; //zmierza do prawej ściany, 0 do lewej
  pilka[5] = 4; //do punktu (7;4), 4 współrządna rzędna w kolumnie 7.
  
  randomSeed(analogRead(A0));
}
//####################### PĘTLA #############################
void loop()
{
  timerA.process();
  timerB.process();
  timerC.process();
  
  maks.setLed(0, pilka[0], pilka[1], true); //wyświetlenie piłki
  maks.setLed(0, pilka[2], pilka[3], false);
  
  //maks.setLed(0, 7, poz + 2, true); //wyświetlenie paletki do gry
  maks.setLed(0, 7, poz + 1, true);
  maks.setLed(0, 7, poz, true);
  
  pilka[2] = pilka[0]; //podanie poprzedniej pozycji piłki, żeby zgasić tą diodę
  pilka[3] = pilka[1];
  
  
  if(gora.update() == LOW && gora.read() == LOW)
  {
      przyciskG = 1;
  }
  if(dol.update() == LOW && dol.read() == LOW)
  {
      przyciskD = 1;
  }
}
//####################### RUCH PALETKA #############################
void ruchPaletkaG()
{
   if(przyciskG)
 {
    if(poz >= 0 && poz <= 5)
    {
       poz++;
       wyczyscRzad(7);
       przyciskG = 0;
    }
 }
}

void ruchPaletkaD()
{
   if(przyciskD)
 {
    if(poz >= 1 && poz <= 6)
    {
       poz--;
       wyczyscRzad(7);
       przyciskD = 0;
    }
 }
}
//####################### RUCH PIŁKA #############################
void ruchPilka()
{
  if(pilka[4])                  //porusza piłką w poziomie do odpowiedniej ściany
  {
      pilka[0]++;
  }
  if(pilka[4] == 0)
  {
      pilka[0]--;
  }
  
  if(pilka[0] == 7)              //sprawdza, czy piłka bedzie odbita, jeżeli jest odpowiednio blisko
  {
     if((pilka[4] == 1) && ((pilka[1]) == poz) || ((pilka[1]) == (poz + 1))) //jeżeli za piłką jest paletka 
     {                                                                                                    //lub przy lewej ścianie, to 
        pilka[4] = 0;                                                                                     //losuję punkt do którego będzie
        pilka[5] = random(0, 8);                                                                          //zmierzać
     }
     else
        reset();
  }
  if((pilka[4] == 0) && (pilka[0] == 0))
  {
        pilka[4] = 1;
        pilka[5] = random(0, 8);
  }
  
  if((pilka[5] - pilka[1] != 0) && (pilka[0] != 6))    //losuje moment kiedy piłka poruszy sie w pionie
  {
      if((pilka[5] > pilka[1]) && pilka[1] != 8)
      {
          if(random(0, 2))
          {
              pilka[1]++;
          }
      }
      else if(pilka[5] < pilka[1] && pilka[1] != 0)
      {
           if(random(0, 2))
           {
               pilka[1]--;
           }
      }
  }
}
//####################### WYCZYŚĆ RZĄD #############################
void wyczyscRzad(char i)
{
  int j;
 for(j = 0; j < 8; j++)
 {
    maks.setLed(0, i, j, false); 
 }
}
//####################### RESET #############################
void reset()
{
   asm volatile ("  jmp 0"); 
}

