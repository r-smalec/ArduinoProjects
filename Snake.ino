/*
Gra w snake'a zwanego także 'fonszem'.
Autor: R.A.S.
31.03.2015
*/
#include <LedControl.h>
#include <Bounce2.h>
#include <Timers.h>

void losujKropke(char *tab);
void miganie(void);
void ruchSnake(void);
void wyswietl(void);
void zapisz(void);
void reset(void);

LedControl maks = LedControl(13, 11, 12, 1);
Bounce prawo = Bounce();
Bounce lewo = Bounce();
Timers <1> timerMig;
Timers <2> timerRuchS;

char krop[2];
bool fMiganie = 1;

char snake[2][11];  //fonsz, maksymalna długość 14, 15. komórka, by gasić ślad po wężu przy długości 14
char kier;          //0 prawo, 1 dół, 2 lewo, 3 góra
char dlug = 1;
char i;
bool przyciskP = 0;
bool przyciskL = 0;
//################### SETUP ###################
void setup()
{
  Serial.begin(9600);
  
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(A0, INPUT);
  
  prawo.attach(3);
  prawo.interval(5);
  lewo.attach(6);
  lewo.interval(5);
  
  maks.shutdown(0, false);
  maks.setIntensity(0, 0);
  maks.clearDisplay(0);
 
  timerMig.attach(0, 500, miganie);
  timerRuchS.attach(0, 900, ruchSnake);
  
  for(i = 0; i < 10; i++)
  {
   snake[0][i] = snake[1][i] = 0; 
  }
  
  randomSeed(analogRead(A0));
  
  snake[0][0] = 5;
  snake[1][0] = 4;
  kier = random(0, 4);
  
  losujKropke(krop);
}
//################### PĘTLA ###################
void loop()
{
 timerMig.process();
 timerRuchS.process();
 
 if(snake[0][0] == krop[0] && snake[1][0] == krop [1])
 {
   dlug++;
   losujKropke(krop);
 }
 
 if(snake[0][0] == 8 || snake[1][0] == 8 || snake[0][0] == -1 || snake[1][0] == -1)
 {
  reset(); 
 }
 
 if(prawo.update() == LOW && prawo.read() == LOW)
  {
    przyciskP = 1;
  }
  if(lewo.update() == LOW && lewo.read() == LOW)
  {
    przyciskL = 1;
  }
}

//################### LOSUJ KROPKE ###################
void losujKropke(char *tab)
{
 do
 {
 tab[0] = random(0, 8);
 tab[1] = random(0, 8);
 }
 while(tab[0] != 5 && tab[1] != 4);  //w innym miejscu niż początek węża
}
//################### MIGANIE ###################
void miganie(void)
{
 fMiganie = !fMiganie;
 maks.setLed(0, krop[0], krop[1], fMiganie);
}
//################### RUCH SNAKE ###################
void ruchSnake(void)
{
 zapisz();
 
  if(przyciskP)
  {
   if(kier < 3)  kier++;
   else kier = 0;
   przyciskP = 0;
  }
  if(przyciskL)
  {
   if(kier > 0)  kier--;
   else kier = 3;
   przyciskL = 0;
  }
  
  switch(kier)
  {
     case 0:
   snake[0][0]++; //x rosną - prawo
   break;
     case 1:
   snake[1][0]--; //y maleje - dół
   break;
     case 2:
   snake[0][0]--; //x maleje - lewo
   break;
     case 3:
   snake[1][0]++; //y rośnie - góra
   break; 
  }
  
 wyswietl();
}
//####################### WYSWIETL #############################
void wyswietl(void)
{
  for(i = 0; i < 10; i++) //wyświetlenie węża zależnie od dlug - jego długości
  {
    if(i < dlug)  maks.setLed(0, snake[0][i], snake[1][i], true);
    else  maks.setLed(0, snake[0][i], snake[1][i], false);
  } 
}
//####################### ZAPISZ #############################
void zapisz(void)
{
  for(i = 10; i > 0; i--)  //zapisywanie drogi przebytej przez węża
  {
   snake[0][i] = snake[0][i - 1];
   snake[1][i] = snake[1][i - 1];
  }
}
//####################### RESET #############################
void reset(void)
{
   asm volatile ("  jmp 0"); 
}

