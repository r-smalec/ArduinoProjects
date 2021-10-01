/*
Program bada częstotliwość, zakładając, że ta nie zmienia się w badanym przedziale.
Zakres pomiarowy 10Hz...1000Hz
Autor: R.A.S.
*/

char tab[1000];

void badanieF();
char obliczanieF();

void setup()
{
 pinMode(A0, INPUT);
 pinMode(12, OUTPUT);
 digitalWrite(12, LOW);
 pinMode(13, OUTPUT);
 Serial.begin(9600);
 Serial.println("Gotowy do pracy");
}

void loop()
{
 badanieF();
 
 if(obliczanieF() != 0)
  Serial.println( 1000/(obliczanieF()) ); 
 else
 {
  Serial.println("Czestotliwosc nie miesci sie w przedzile");
 }
}

void badanieF()
  {
   for(char i = 0; i < 1000; i++)
   {
    char odczyt = analogRead(A0);
    Serial.println(odczyt);
    if(odczyt > 512)
     tab[i] = 1;
    else
     tab[i] = 0;
    delay(10); //pomiar z f = 1000Hz
    
    while(i<500)
     {digitalWrite(13, HIGH);}
    
   }
   digitalWrite(12, HIGH);
   delay(2000);
  }

 char obliczanieF()
  { 
   for(int i; i < 1000; i++)
   {
    char spr = tab[0];
    if(tab[i] != spr)
     return i; //zwrot elementu, dla którego nastąpiła zmiana
    if(tab[999])
     return 0;
   }
  }
