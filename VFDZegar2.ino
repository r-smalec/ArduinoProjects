/*
Program do wyświetlania czasu z RTC8563 na wyświetlacu 4 x lampa VFD
Wersja druga
Autor: R.A.S.
13.11.2015
*/
#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <Timers.h>
#include <EEPROM.h>
Rtc_Pcf8563 rtc;
Timers <1> multiplekser;
Timers <1> pobranieCzasu;

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6

#define LAMPA1 10
#define LAMPA2 11
#define LAMPA3 12
#define LAMPA4 13

#define SW1 15
#define SW2 7
#define SW3 14

#define PRZEKAZNIK 8

void doLamp(char cyfra);
void wyswietlanie(void);
void konwersja(void);
void konwersjaIni(void);
void multi(void);

boolean odczyt = 1;

unsigned char godzina;
unsigned char minuta;
unsigned char czas[4];

unsigned int cykl = 0;
unsigned char podCykl = 0;

boolean flagaPrzekaznika;

void setup()
{
  pinMode(LAMPA1, OUTPUT);
  pinMode(LAMPA2, OUTPUT);
  pinMode(LAMPA3, OUTPUT);
  pinMode(LAMPA4, OUTPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(PRZEKAZNIK, OUTPUT);
  
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  
  PORTB |= (111111); //wyłączenie lamp
  digitalWrite(PRZEKAZNIK, HIGH);
  flagaPrzekaznika = 0;
  
  multiplekser.attach(0, 1, multi);
  pobranieCzasu.attach(0, 500, konwersja);

      godzina = EEPROM.read(10);
      minuta = EEPROM.read(11);

  //to initialise clock
  rtc.initClock();
  //day, weekday, month, century, year
  rtc.setDate(21, 6, 11, 0, 15);
  //hour, minute, second
  rtc.setTime(godzina, minuta, 0);
  
  konwersjaIni();
  
  while(digitalRead(SW2) == HIGH) //ustawianie godziny
                   {
                     multiplekser.process();
                     
                       if(digitalRead(SW3) == LOW)
                       {
                         godzina++;
                         if(godzina == 24)
                         {
                           godzina = 0;
                         }
                         konwersjaIni();
                         delay(200);
                       }
                   }
               delay(300);
               
  while(digitalRead(SW2) == HIGH) //ustawianie minuty
                   {
                     multiplekser.process();
                     
                       if(digitalRead(SW3) == LOW)
                       {
                         minuta++;
                         if(minuta == 60)
                         {
                           minuta = 0;
                         }
                         konwersjaIni();
                         delay(200);
                       }
                   }
               rtc.setTime(godzina, minuta, 0);
               delay(300);
               digitalWrite(PRZEKAZNIK, LOW);

               
   EEPROM.write(10, godzina);
   EEPROM.write(11, minuta);
}

void loop()
{
              multiplekser.process();
              pobranieCzasu.process();
}
//#################### KONWERSJA INICJALIZUJĄCA ####################
void konwersjaIni(void)
{
  czas[0] = minuta % 10;    //wyszukiwanie jedności i dziesiątek minuty
  czas[1] = (minuta - (czas[0]))/10;
  czas[2] = godzina % 10;   //wyszukiwanie jedności i dziesiątek godziny
  czas[3] = (godzina - (czas[2]))/10;
}
//#################### KONWERSJA ####################
void konwersja(void)
{
  minuta = rtc.getMinute();
  godzina = rtc.getHour();
  
  czas[0] = minuta % 10;    //wyszukiwanie jedności i dziesiątek minuty
  czas[1] = (minuta - (czas[0]))/10;
  czas[2] = godzina % 10;    //wyszukiwanie jedności i dziesiątek godziny
  czas[3] = (godzina - (czas[2]))/10;
}
//#################### DO LAMP ####################
void doLamp(char cyfra)
{
 switch(cyfra)
          {
            case 0:
              PORTD = 0b01000000;
              break;
            case 1:
              PORTD = 0b01011110;
              break;
            case 2:
              PORTD = 0b00100100;
              break;
            case 3:
              PORTD = 0b00001100;
              break;
            case 4:
              PORTD = 0b00011010;
              break;
            case 5:
              PORTD = 0b00001001;
              break;
            case 6:
              PORTD = 0b00000001;
              break;
            case 7:
              PORTD = 0b01011100;
              break;
            case 8:
              PORTD = 0b00000000;
              break;
            case 9:
              PORTD = 0b00001000;
              break;
            default:
              PORTD = 0b00001111;
          }
}
//#################### MULTIPLEXER ####################
void multi(void) //Multiplekserowanie z częstotliwością 1000Hz.
{                //Czyli 1000Hz:(2*4) = 125Hz na lampę
  podCykl = cykl % 8;
  switch(podCykl)
    {
      case 0:
      digitalWrite(LAMPA4, HIGH);
      //PORTB = 111111;
      doLamp(czas[3]);
        break;
        
      case 1:
      digitalWrite(LAMPA1, LOW);
      //PORTB = 111011;
        break;
        
      case 2:
      digitalWrite(LAMPA1, HIGH);
      //PORTB = 111111;
      doLamp(czas[2]);
        break;
        
      case 3:
      digitalWrite(LAMPA2, LOW);
      //PORTB = 110111;
        break;
        
      case 4:
      digitalWrite(LAMPA2, HIGH);
      //PORTB = 111111;
      doLamp(czas[1]);
        break;
        
      case 5:
      digitalWrite(LAMPA3, LOW);
      //PORTB = 101111;
        break;
        
      case 6:
      digitalWrite(LAMPA3, HIGH);
      //PORTB = C;
      doLamp(czas[0]);
        break;
        
      case 7:
      digitalWrite(LAMPA4, LOW);
      //PORTB = 011111;
        break;
    }
  
  cykl++;
}
