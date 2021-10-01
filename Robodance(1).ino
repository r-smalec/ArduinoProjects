#define SILA 3
#define SILB 6

char i = 0;

void setup()
{
  pinMode(SILA, OUTPUT);
  pinMode(SILB, OUTPUT);
}

void loop()
{
  for(i = 15; i < 105; i++)
  {
    analogWrite(SILA, i);
    analogWrite(SILB, 165 - i);
    delay(50);
  }
}
