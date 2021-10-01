 float x = 0;
 float krok = 0.1;
 int z;
 int linia;

void setup()
{
 Serial.begin(9600);
}

void loop()
{
 z = (int)4*sin(x*2.0*PI);
 linia = pow(10, z);
 Serial.println(linia);
 
 if(x == 1.0)
 {
  krok = -1.0 * krok; 
 }
 else
 {
  x = x+krok;
 }
}
