char spid;

void setup()
{
 Serial.begin(9600);
 spid = 0;
 pinMode(13, OUTPUT);
 digitalWrite(13, LOW);
 Serial.println("Gotowy");
 while(spid == 0)
 {
    if(Serial.read() == 'a')
    {
      spid = 3;
      Serial.println("a");
    }
    else if(Serial.read() == 'b')
    {
      spid = 1;
      Serial.println("b");
    }
 }
}

void loop()
{
 digitalWrite(13, HIGH);
 delay(spid*100);
 digitalWrite(13, LOW);
 delay(spid*100);
}
