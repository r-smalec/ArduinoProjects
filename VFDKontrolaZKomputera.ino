/*
Do wyświetlania liczb na lampie VFD
Autor: R.A.S.
5.10.2015
*/

#define ANODOWE 10
#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define G 7
#define H 8

char odczyt;
byte pin;
byte maska = 00000001;
byte stan;
char i;

void startStop(boolean s);

void setup()
{
  Serial.begin(9600);
  pinMode(ANODOWE, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(H, OUTPUT);
  
  digitalWrite(ANODOWE, HIGH);
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(H, HIGH);
}

void loop()
{
  if(Serial.available())
  {
      digitalWrite(pin, HIGH);
      odczyt = (char)Serial.read();
      switch(odczyt)
      {
        case '0':
          pin = 0b00001000;
          break;
        case '1':
          pin = 0b01001111;
          break;
        case '2':
          pin = 0b00010001;
          break;
        case '3':
          pin = 0b00000011;
          break;
        case '4':
          pin = 0b01000110;
          break;
        case '5':
          pin = 0b00100010;
          break;
        case '6':
          pin = 0b00100000;
          break;
        case '7':
          pin = 0b00001111;
          break;
        case '8':
          pin = 0b00000000;
          break;
        case '9':
          pin = 0b00000010;
          break;
      }
      if(odczyt >= '0' && odczyt <= '9')
      {
          for(i = 2; i <= 8; i++)
         {
          stan = maska & pin;
          digitalWrite(i, stan);
          pin = pin >> 1;
         }
      }
      
      if(odczyt == 'p')
      {
       startStop(0); 
      }
      
      if(odczyt == 'z')
      {
       startStop(1); 
      }
  }
}

void startStop(boolean s)
{
  delay(500);
  digitalWrite(ANODOWE, s);
}
