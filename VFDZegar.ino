/*
Program do wyświetlania czasu z RTC8563 na wyświetlacu 4 x lampa VFD
Autor: R.A.S.
08.10.2015
*/
#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <Timers.h>

Rtc_Pcf8563 rtc;
Timers <1> multiplekser;
Timers <2> odczytCzasu;
Timers <3> czasWyswietlania;

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define G 5
#define H 6

#define LAMPA1 13
#define LAMPA2 12
#define LAMPA3 11
#define LAMPA4 10

#define SW1 15
#define SW2 7
#define SW3 14

#define PRZEKAZNIK 8

char minuta, godzina;
char czas[4];

byte pin0 = 0;
byte pin1 = 0;
byte pin2 = 0;
byte pin3 = 0;
byte *pinPointer;
char i = 0;
int cykl = 0;
char podCykl = 0;

boolean nastaw = 0;
char nowaGodzina = 0, nowaMinuta = 0;

boolean odczytSW1;
boolean flagaPrzekaznika;

void startStop(char lampa, boolean w);
void konwersja(void);
void multi(void);
void wyswietlanie(void);

void setup()
{
  //to initialise clock
  rtc.initClock();
  //day, weekday, month, century, year
  rtc.setDate(21, 6, 11, 0, 15);
  //hour, minute, second
  rtc.setTime(15, 00, 0);
  
  multiplekser.attach(0, 1, multi);
  odczytCzasu.attach(0, 400, konwersja);
  czasWyswietlania.attach(0, 5000, wyswietlanie);
  
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(H, OUTPUT);
  pinMode(LAMPA1, OUTPUT);
  pinMode(LAMPA2, OUTPUT);
  pinMode(LAMPA3, OUTPUT);
  pinMode(LAMPA4, OUTPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(PRZEKAZNIK, OUTPUT);
  
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(H, HIGH);
  startStop(1, 1);
  startStop(2, 1);
  startStop(3, 1);
  startStop(4, 1);
  digitalWrite(PRZEKAZNIK, LOW);
  flagaPrzekaznika = 0;
}
//#################### PĘTLA ####################
void loop()
{
  czasWyswietlania.process();
  
 if(digitalRead(SW1) == LOW) //włącza przetwornicę i działanie
 {                          //wyswietlacza
   delay(50);
   if(digitalRead(SW1) == LOW)
   {
      digitalWrite(PRZEKAZNIK, HIGH);
      flagaPrzekaznika = 1;
      delay(50);
   }
 }
 if(flagaPrzekaznika)
 {
   
    multiplekser.process();
    odczytCzasu.process();
        
       if(digitalRead(SW2) == LOW)
       {
         delay(100);
         if(digitalRead(SW2) == LOW)
         {
           nastaw = 1;
           delay(400);
         }
       }
       
       if(nastaw)
       {
         while(digitalRead(SW2) == HIGH) //ustawianie godziny
         {
           multiplekser.process();
           odczytCzasu.process();
           rtc.setTime(nowaGodzina, nowaMinuta, 0);
           
             if(digitalRead(SW3) == LOW)
             {
               nowaGodzina++;
               if(nowaGodzina == 24)
               {
                 nowaGodzina = 0;
               }
               delay(200);
             }
         }
         delay(400);
         while(digitalRead(SW2) == HIGH) //ustawianie minuty
         {
           multiplekser.process();
           odczytCzasu.process();
           rtc.setTime(nowaGodzina, nowaMinuta, 0);
           
             if(digitalRead(SW3) == LOW)
             {
               nowaMinuta++;
               if(nowaMinuta == 60)
               {
                 nowaMinuta = 0;
               }
               delay(200);
             }
         }
         delay(400);
         
         rtc.setTime(nowaGodzina, nowaMinuta, 0);
         nowaGodzina = nowaMinuta = 0;
         nastaw = 0;
       }
 }
 
}
//#################### START/STOP ####################
void startStop(char lampa, boolean w)
{                      //stan: 0 - start, 1 - stop
  switch(lampa)
  {
   case 1:
     digitalWrite(LAMPA1, w);
     break;
   case 2:
     digitalWrite(LAMPA2, w);
     break;
   case 3:
     digitalWrite(LAMPA3, w);
     break;
   case 4:
     digitalWrite(LAMPA4, w);
     break;
  }
}
//#################### KONWERSJA ####################
void konwersja(void)
{
  minuta = (char)rtc.getSecond();
  godzina = (char)rtc.getHour();
  czas[0] = czas[1] = czas[2] = czas[3] = 0;
  
  while(minuta > 9)    //wyszukiwanie jedności i dziesiątek minuty
  {
    czas[1]++;
    minuta -= 10;
  }
  czas[0] = minuta;
  while(godzina > 9)    //wyszukiwanie jedności i dziesiątek godziny
  {
    czas[3]++;
    godzina -= 10;
  }
  czas[2] = godzina;
  
  for(i = 0; i < 4; i++)
  {
    switch(i)
    {
      case 0:
      pinPointer = &pin0;
        break;
      case 1:
      pinPointer = &pin1;
        break;
      case 2:
      pinPointer = &pin2;
        break;
      case 3:
      pinPointer = &pin3;
        break;
    }
      switch(czas[i])
          {
            case 0:
              *pinPointer = 0b01000000;
              break;
            case 1:
              *pinPointer = 0b01011110;
              break;
            case 2:
              *pinPointer = 0b00100100;
              break;
            case 3:
              *pinPointer = 0b00001100;
              break;
            case 4:
              *pinPointer = 0b00011010;
              break;
            case 5:
              *pinPointer = 0b00001001;
              break;
            case 6:
              *pinPointer = 0b00000001;
              break;
            case 7:
              *pinPointer = 0b01011100;
              break;
            case 8:
              *pinPointer = 0b00000000;
              break;
            case 9:
              *pinPointer = 0b00001000;
              break;
            default:
              *pinPointer = 0b00001111;
          }
  }
}
//#################### MULTIPLEXER ####################
void multi(void) //Multiplekserowanie z częstotliwością 1000Hz.
{                //Czyli 1000Hz:(2*4) = 125Hz na lampę
  podCykl = cykl % 8;
  switch(podCykl)
    {
      case 0:

      startStop(4, 1);
      PORTD = pin0;
        break;
        
      case 1:
      startStop(1, 0);
        break;
        
      case 2:
      startStop(1, 1);
      PORTD = pin1;
        break;
        
      case 3:
      startStop(2, 0);
        break;
        
      case 4:
      startStop(2, 1);
      PORTD = pin2;
        break;
        
      case 5:
      startStop(3, 0);
        break;
        
      case 6:
      startStop(3, 1);
      PORTD = pin3;
        break;
        
      case 7:
      startStop(4, 0);
        break;
    }
  
  cykl++;
}
//#################### WYŚWIETLANIE ####################
void wyswietlanie(void)
{
   flagaPrzekaznika = 0;
   startStop(1, 1); //procedura wyłączania VFD, najpierw
   startStop(2, 1); //napięcie siatki, potem żarzenia
   startStop(3, 1);
   startStop(4, 1);
   cykl = 0;     // po tym czasie przełącznik wylącza przetwornicę
   delay(300);   // i wyświetlacz
   digitalWrite(PRZEKAZNIK, LOW);
}
