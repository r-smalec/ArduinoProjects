float odczyt;

void setup()
{
  Serial.begin(9600);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  delay(1000);
  pinMode(A3, INPUT);
}

void loop()
{
  odczyt = analogRead(A3);
  Serial.println(odczyt);
  delay(100);
}
