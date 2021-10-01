char szmoc = 128;
char krok = 1;

void setup()
{
 pinMode(10, OUTPUT);
 pinMode(11, OUTPUT); 
}

void loop()
{
 szmoc = szmoc + krok;
 
 if(szmoc == 254 || szmoc == 1)
  krok = -1 * krok;
 
 analogWrite(10, szmoc);
 analogWrite(11, (255 - szmoc));
 delay(3);
}
