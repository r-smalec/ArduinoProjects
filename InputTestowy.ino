void setup() {
  pinMode(4, INPUT);
 Serial.begin(9600);
}

void loop() {
  delay(1000);
  if(digitalRead(4) == HIGH)
   Serial.println("1");
  if(digitalRead(4) == LOW)
   Serial.println("0");
}
