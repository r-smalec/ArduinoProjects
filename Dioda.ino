#define PIN 13
#define czas 100

boolean fl = 1;

void setup()
{
  pinMode(PIN, OUTPUT);
}

void loop()
{
  fl = fl ^ 1;
 digitalWrite(PIN, fl);
 delay(czas);
}
