byte pin = 0b11100000;
byte stan;
byte maska = 00000001;

void setup()
{
 pinMode(13, OUTPUT);
 Serial.begin(9600); 
}

void loop()
{
  stan = maska & pin;
  digitalWrite(13, stan);
  Serial.println((unsigned int)pin);
  Serial.println((unsigned int)stan);
  Serial.println("");
  delay(500);
  pin = pin >> 1;
}
