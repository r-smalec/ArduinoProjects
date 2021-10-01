/*
Program do ustawiania skokowo prędkości na silnikach łódki
Autor: R.A.S.
07.07.2015
*/
#define SILNIKA 11
#define SILNIKB 12

#define PRZYCISKA 3
#define PRZYCISKB 4
#define PRZYCISKC 5
#define PRZYCISKD 6

#include <Bounce2.h>
Bounce PA = Bounce();
Bounce PB = Bounce();
Bounce PC = Bounce();
Bounce PD = Bounce();
#include <Timers.h>
Timers <1> timer;

boolean flaga = 1;
boolean s11 = 0, s12 = 0, s21 = 0, s22 = 0;
char silnik1 = 0, silnik2 = 0;

void sprawdzenieSygnalow(void);

void setup()
{
 timer.attach(0, 500, sprawdzenieSygnalow);
 pinMode(SILNIKA, OUTPUT); //PWM do L293D z silnikami
 pinMode(SILNIKB, OUTPUT);
 pinMode(PRZYCISKA, INPUT);
 pinMode(PRZYCISKB, INPUT);
 pinMode(PRZYCISKC, INPUT);
 pinMode(PRZYCISKD, INPUT);
 PA.attach(PRZYCISKA);  //sygnały z pilota RC
 PA.interval(15);
 PB.attach(PRZYCISKB);
 PB.interval(15);
 PC.attach(PRZYCISKC);
 PC.interval(15);
 PD.attach(PRZYCISKD);
 PD.interval(15);
}

void loop()
{
  timer.process();
  
  if(flaga == 1)
  {
        if(PA.update() == LOW && PA.read() == LOW)
        {
          s11 = 1;
          s12 = 0;
          s21 = 0;
          s22 = 0;
          flaga = 0;
        }
        
        if(PB.update() == LOW && PB.read() == LOW)
        {
          s11 = 0;
          s12 = 1;
          s21 = 0;
          s22 = 0;
          flaga = 0;
        }
        
        if(PC.update() == LOW && PC.read() == LOW)
        {
          s11 = 0;
          s12 = 0;
          s21 = 1;
          s22 = 0;
          flaga = 0;
        }
        
        if(PD.update() == LOW && PD.read() == LOW)
        {
          s11 = 0;
          s12 = 0;
          s21 = 0;
          s22 = 1;
          flaga = 0;
        }
   }
}

void sprawdzenieSygnalow(void)
{
  if(s11 == 1 && (silnik1 >= 0 && silnik1 < 3))
    silnik1++;
  
  else if(s12 == 1 && (silnik1 > 0 && silnik1 < 4))
    silnik1--;
    
  else if(s21 == 1 && (silnik2 >= 0 && silnik2 < 3))
    silnik2++;
  
  else if(s22 == 1 && (silnik2 > 0 && silnik2 < 4))
    silnik2--;
  
  analogWrite(SILNIKA, ((silnik1) * 50));
  analogWrite(SILNIKB, ((silnik2) * 50));
  
  s11 = 0;
  s12 = 0;
  s21 = 0;
  s22 = 0;
  flaga = 1;
}

