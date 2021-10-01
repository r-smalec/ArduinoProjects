#define UST1 2
#define UST2 3
#define UST3 4

#define SILA1 5
#define SILA2 6
#define SILB1 9
#define SILB2 10

#define SWITCH 12
#define LED 13

#define CZUJ1 14
#define CZUJ2 15
#define CZUJ3 16
#define CZUJ4 17
#define CZUJ5 18

void skret(void);

boolean przycisk = 1;
char i = 0;
char moc;
char srednia = 0, pomiar = 0;
const char prog = 10;
char roznica = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(SWITCH, INPUT);
  pinMode(SILA1, OUTPUT);
  pinMode(SILA2, OUTPUT);
  pinMode(SILB1, OUTPUT);
  pinMode(SILB2, OUTPUT);
  
  pinMode(CZUJ1, INPUT);
  pinMode(CZUJ2, INPUT);
  pinMode(CZUJ3, INPUT);
  pinMode(CZUJ4, INPUT);
  pinMode(CZUJ5, INPUT);
  
  digitalWrite(SILA2, LOW);
  digitalWrite(SILB2, LOW);
  
  if(digitalRead(UST1))
  {
    moc = 60;
  }
  else
  {
    moc = 70;
  }
  
    if(digitalRead(UST2))
  {
    
  }
  else
  {
    
  }
  
    if(digitalRead(UST3))
  {
    
  }
  else
  {
    
  }
  
  while(przycisk == 1) //czekanie na naciśnięcie przycisku
  {
    if(!digitalRead(SWITCH))
      przycisk = 0;
  }
  for(i = 0; i < 5; i++)
  {
    srednia += analogRead(CZUJ3);
    delay(200);
  }
  srednia = srednia/5; //konfiguracja dla danych warunków
                       //środowiskowych
    digitalWrite(LED, HIGH);
  }

void loop()
{
  delay(120);
  analogWrite(SILA1, moc);
  analogWrite(SILB1, moc);
  
  pomiar = analogRead(CZUJ3);
  if(pomiar < (srednia-prog) || pomiar > (srednia + prog))
   {
    skret();
   }
}

//############################ SKRĘT ############################
void skret()
{
  roznica = 2*analogRead(CZUJ1);
  roznica += analogRead(CZUJ2);
  roznica -= analogRead(CZUJ4);
  roznica -= 2*analogRead(CZUJ5);
  
  roznica = roznica/3;
  
  analogWrite(SILA1, moc + roznica);
  analogWrite(SILB1, moc + roznica);
}
