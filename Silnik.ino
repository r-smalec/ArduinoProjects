/*
Program do sterowania silnikiem za pomicą tranzystora
Autor: R.A.S.
30.01.2015
*/

void setup()
{
 pinMode(11, OUTPUT); 
 pinMode(13, OUTPUT);
}

void loop()
{
 //analogWrite(11, i);
 tone(13, 36000);
}
