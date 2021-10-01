#include <SensorOp.h>

const int sensorPin = A0;
const int ledControl = 2;
const int samples = 4;
const int filtro = 7;

SensorOp sensor(ledControl,sensorPin,samples);

int sensorRead1;
int sensorRead2;
int sensorRead3;
int sensorRead4;

void setup() 
{
  Serial.begin(9600); 
  pinMode(ledControl, OUTPUT);
}

void loop() 
{
  digitalWrite(ledControl,HIGH); 
  delay(2);
  sensorRead1 = analogRead(sensorPin); 
  sensorRead2 = sensor.readSensor(); 
  sensorRead3 = sensor.readSensor(sensorRead3); 
  sensorRead4 = sensor.readSensor(sensorRead4,filtro); 
  Serial.print(sensorRead1);      
  Serial.print("\t");
  Serial.print(sensorRead2);      
  Serial.print("\t");
  Serial.print(sensorRead3);      
  Serial.print("\t");
  Serial.println(sensorRead4);
  delay(7);                     
}
