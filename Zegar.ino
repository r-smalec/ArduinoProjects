/*
Stoper, ktory dziala w oparciu o prawdopodobienstwo. Co 1/10 sekundy losuje liczbe
i nalicza kolejna sekunde z kazdym pomiarem z prawdopodobienstwem 1/10
Autor: R.A.S.
28.01.2015
*/


int se = 0;
int mi = 0;
int go = 0;
int seR = 0;
int miR = 0;
int goR = 0; //sekundy, minuty, godziny, obliczane z prawdopodobienstwa i z rzeczywistego uplywu czasu
int losowa;
int licznik = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop()
{
  delay(100);
  licznik++;
   
  losowa = random(1, 11);
  if(losowa == 1)
  {
   se++;
   if(se == 60)
   {
     se = 0;
     mi++;
     if(mi == 60)
     {
      mi = 0;
      go++; 
     }
   }
  }
  
  if(licznik == 10)
  {
   seR++;
   if(seR == 60)
   {
     seR = 0;
     miR++;
     if(miR == 60 )
     {
      miR = 0;
      goR++; 
     }
   }
  }
 
 if(licznik == 10)
 {
  licznik = 0;
  Serial.println(" ");
  Serial.print(go);
  Serial.print(":");
  Serial.print(mi);
  Serial.print(":");
  Serial.print(se);
  Serial.println(" ");
  Serial.print(goR);
  Serial.print(":");
  Serial.print(miR);
  Serial.print(":");
  Serial.print(seR);
  
  randomSeed(analogRead(A0));
 } 
}
