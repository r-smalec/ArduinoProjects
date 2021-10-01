/*
Program do zczytywania komend z pilota IR
Autor: R.A.S.
05.02.2015
*/
const int siz = 200;
int tab[siz], i = 0;

void setup()
{
 Serial.begin(9600);
 Serial.println("Jestem");
 pinMode(11, INPUT_PULLUP);
 for(i; i < siz; i++)
 {
  tab[i] = 1; 
 }
}

void loop()
{
  if(digitalRead(11) == LOW)
  {
    tab[0] = 0;
    Serial.println("Wszedlem");
    for(i = 1; i < siz; i++)
    {
      delay(1);
      if(digitalRead(11) == LOW)
      {
        tab[i] = 0;
      }
     Serial.print(tab[i]);
    }
    Serial.println("\n");
  }
}
