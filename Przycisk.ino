/*
Program do obsługi przycisku i pinów wejściowych
Autor: R.A.S.
05.02.2015
*/

void setup()
{
  Serial.begin(9600);
  pinMode(11, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Serial.println("Jestem");
}

void loop()
{
  digitalWrite(13, LOW);
  while(digitalRead(11) == LOW)
  {
   digitalWrite(13, HIGH); 
  }
}
