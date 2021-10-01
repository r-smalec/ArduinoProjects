float odczyt;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop()
{
 odczyt = ((analogRead(A0) * 5) / 1023);
 Serial.println(odczyt);
 Serial.println( (int)(odczyt * 10.0) / 10.0 );
 Serial.println(".");
 Serial.println( ((int)(odczyt * 10.0) - (int)(odczyt * 10.0 / 10.0) * 10.0) );
 delay(2000);
}
