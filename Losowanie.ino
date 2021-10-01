int losowa;
int licznik = 0;

void setup()
{
 Serial.begin(9600);
 pinMode(A0, INPUT);
 randomSeed(analogRead(A0)); 
}

void loop()
{
  delay(100);
  losowa = random(1, 11);
  if(losowa == 10)
  {
   licznik++;
  }
  Serial.println(licznik);
}
