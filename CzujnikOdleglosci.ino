#define trig 2
#define echo 3

int distance;

void setup()
{
Serial.begin(9600);

pinMode(trig, OUTPUT);
pinMode(echo, INPUT);
}

void loop()
{
 digitalWrite(trig, LOW);
 delayMicroseconds(2);
 digitalWrite(trig, HIGH);
 delayMicroseconds(10);
 digitalWrite(trig, LOW);

 distance = (pulseIn(echo, HIGH))/58;
 
 Serial.print("Odleglosc: ");
 Serial.println(distance);
 delay(200);
}
