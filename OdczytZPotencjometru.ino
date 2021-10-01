int moc;
void setup()
{
  pinMode(PB0, OUTPUT);
}

void loop()
{
 moc = analogRead(PB3) / 4;
 analogWrite(PB0, moc);
}
