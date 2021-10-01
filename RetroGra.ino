/*
Retro gra na wyświetlacz LEDowy, do wyboru tenis lub snake.
Autor: R.A.S.
03.06.2015
*/
#include <LedControl.h>
#include <Bounce2.h>
#include <Timers.h>

void litera(bool wwybor);
void zdobPunkt(void);
void reset(void);

void ruchPaletka(void);
void ruchPilka(void);
void wyczyscRzad(char i);

void losujKropke(char *tab);
void miganie(void);
void ruchSnake(void);
void wyswietl(void);
void zapisz(void);

volatile bool wybor = 0; //do wyboru gry

LedControl maks = LedControl(13, 11, 12, 1);
Bounce gora = Bounce();
Bounce dol = Bounce();
Timers <1> timerA;
Timers <2> timerB;
Timers <3> timerC;
volatile int poz = 2;           //pozycja paletki
volatile int pilka[6];
volatile bool przyciskG = 0; //dla snake'a gora == prawo
volatile bool przyciskD = 0; //            dol == lewo

Timers <1> timerMig;
Timers <2> timerRuchS;
char krop[2];
bool fMiganie = 1;
char snake[2][15];  //fonsz, maksymalna długość 14, 15. komórka, by gasić ślad po wężu przy długości 14
char kier;          //0 prawo, 1 dół, 2 lewo, 3 góra
char dlug = 1;
char i;
//####################### SETUP #############################
void setup(void)
{
  pinMode(2, OUTPUT); //CLOCK do 4017
  digitalWrite(2, LOW);
  pinMode(3, INPUT);
  pinMode(6, INPUT);  //przyciski
  pinMode(A0, INPUT); //ziarno losowości
  randomSeed(analogRead(A0));
  
  gora.attach(3);
  gora.interval(5);
  dol.attach(6);
  dol.interval(5);
  
  maks.shutdown(0, false);
  maks.setIntensity(0, 0); //jasność od 0 do 15
  maks.clearDisplay(0);
  
  litera(1); //początkowe ustawienie wyboru na tennisa
//####################### WYBÓR GRY #############################
  while(digitalRead(3) == HIGH)
  {
    if(wybor == 1 && dol.update() == LOW && dol.read() == LOW)
    {
        litera(wybor);
        wybor = 0; // podświetla wybór tenisa
    }
    else if(wybor == 0 && dol.update() == LOW && dol.read() == LOW)
    {
        litera(wybor);
        wybor = 1; // podświetla wybór snake'a
    }
  }
  delay(1500);
  maks.clearDisplay(0);
  
  if(wybor == 0) //tenis
  {
    timerA.attach(0, 149, ruchPaletkaG);
    timerB.attach(0, 149, ruchPaletkaD);
    timerC.attach(0, 119, ruchPilka);
    
    pilka[0] = 1; //pozycja początkowa (3;3), kolumna
    pilka[1] = 7;                           //rząd
    pilka[2] = 1; //poprzednia pozycja, kolumna
    pilka[3] = 1;                     //rząd
    pilka[4] = 1; //zmierza do prawej ściany, 0 do lewej
    pilka[5] = 4; //do punktu (7;4), 4 współrządna rzędna w kolumnie 7.
  }
  else if(wybor == 1) //snake
  {
    timerMig.attach(0, 500, miganie);
    timerRuchS.attach(0, 900, ruchSnake);
    
    for(i = 0; i < 10; i++)
    {
     snake[0][i] = snake[1][i] = 0; 
    }
    
    snake[0][0] = 5;
    snake[1][0] = 4;
    kier = random(0, 4);
    
    losujKropke(krop);
  }
  else reset();
}
//####################### PĘTLA #############################
void loop(void)
{
  if(wybor == 0) //tenis
  {
      timerA.process();
      timerB.process();
      timerC.process();
      
      maks.setLed(0, pilka[0], pilka[1], true); //wyświetlenie piłki
      maks.setLed(0, pilka[2], pilka[3], false);
      
      maks.setLed(0, 7, poz + 1, true);  //wyświetlenie paletki do gry
      maks.setLed(0, 7, poz, true);
      
      pilka[2] = pilka[0]; //podanie poprzedniej pozycji piłki, żeby zgasić tą diodę
      pilka[3] = pilka[1];
  }
  else if(wybor == 1) //snake
  {
     timerMig.process();
     timerRuchS.process();
     
     if(snake[0][0] == krop[0] && snake[1][0] == krop [1])
     {
       dlug++;
       losujKropke(krop);
       zdobPunkt();
     }
     
     if(snake[0][0] == 8 || snake[1][0] == 8 || snake[0][0] == -1 || snake[1][0] == -1)
     {
      reset(); 
     }
  }
  else reset();
  
  if(gora.update() == LOW && gora.read() == LOW)
  {
      przyciskG = 1;
  }
  if(dol.update() == LOW && dol.read() == LOW)
  {
      przyciskD = 1;
  }
}
//####################### FUNKCJE #############################
//####################### LITERA  #############################
void litera(bool wwybor) //dla Tenisa wybor == 1, Snake'a wybor == 1.
{
  ustRzad(4, wwybor);
  ustRzad(5, wwybor);
  ustRzad(6, wwybor);
  ustRzad(7, wwybor);

  maks.setLed(0, 3, 4, !wwybor);
  maks.setLed(0, 4, 4, !wwybor);
  maks.setLed(0, 3, 5, !wwybor);
  maks.setLed(0, 4, 5, !wwybor);
  maks.setLed(0, 3, 6, !wwybor);
  maks.setLed(0, 4, 6, !wwybor);
  maks.setLed(0, 2, 7, !wwybor);
  maks.setLed(0, 3, 7, !wwybor);
  maks.setLed(0, 4, 7, !wwybor);
  maks.setLed(0, 5, 7, !wwybor);
  
  ustRzad(0, !wwybor);
  ustRzad(1, !wwybor);
  ustRzad(2, !wwybor);
  ustRzad(3, !wwybor);
  
  maks.setLed(0, 1, 0, wwybor);
  maks.setLed(0, 2, 0, wwybor);
  maks.setLed(0, 3, 0, wwybor);
  maks.setLed(0, 3, 1, wwybor);
  maks.setLed(0, 4, 1, wwybor);
  maks.setLed(0, 2, 2, wwybor);
  maks.setLed(0, 3, 2, wwybor);
  maks.setLed(0, 3, 3, wwybor);
  maks.setLed(0, 4, 3, wwybor);
  maks.setLed(0, 5, 3, wwybor);
}
//####################### ZDOBYTY PUNKT  #############################
void zdobPunkt()
{
  digitalWrite(2, HIGH);
  delay(10);
  digitalWrite(2, LOW);
}
//####################### USTAW RZĄD #############################
void ustRzad(char rzad, bool wwybor)
{
  char i;
  for(i = 0; i < 8; i++)
  {
   maks.setLed(0, i, rzad ,wwybor); 
  }
}
//####################### RUCH PALETKA #############################
void ruchPaletkaG()
{
   if(przyciskG)
 {
    if(poz >= 0 && poz <= 5)
    {
       poz++;
       wyczyscRzad(7);
       przyciskG = 0;
    }
 }
}

void ruchPaletkaD()
{
   if(przyciskD)
 {
    if(poz >= 1 && poz <= 6)
    {
       poz--;
       wyczyscRzad(7);
       przyciskD = 0;
    }
 }
}
//####################### RUCH PIŁKA #############################
void ruchPilka()
{
  if(pilka[4])                  //porusza piłką w poziomie do odpowiedniej ściany
  {
      pilka[0]++;
  }
  if(pilka[4] == 0)
  {
      pilka[0]--;
  }
  
  if(pilka[0] == 7)              //sprawdza, czy piłka bedzie odbita, jeżeli jest odpowiednio blisko
  {
     if((pilka[4] == 1) && (((pilka[1]) == poz) || ((pilka[1]) == (poz + 1))))                            //jeżeli za piłką jest paletka 
     {                                                                                                    //lub przy lewej ścianie, to 
        pilka[4] = 0;                                                                                     //losuję punkt do którego będzie
        pilka[5] = random(0, 8);                                                                          //zmierzać
        zdobPunkt();
     }
     else
        reset();
  }
  if((pilka[4] == 0) && (pilka[0] == 0))
  {
        pilka[4] = 1;
        pilka[5] = random(0, 8);
  }
  
  if((pilka[5] - pilka[1] != 0) && (pilka[0] != 6))    //losuje moment kiedy piłka poruszy sie w pionie
  {
      if((pilka[5] > pilka[1]) && pilka[1] != 8)
      {
          if(random(0, 2))
          {
              pilka[1]++;
          }
      }
      else if(pilka[5] < pilka[1] && pilka[1] != 0)
      {
           if(random(0, 2))
           {
               pilka[1]--;
           }
      }
  }
}
//####################### WYCZYŚĆ RZĄD #############################
void wyczyscRzad(char i)
{
  int j;
 for(j = 0; j < 8; j++)
 {
    maks.setLed(0, i, j, false); 
 }
}
//################### LOSUJ KROPKE ###################
void losujKropke(char *tab)
{
 do
 {
 tab[0] = random(0, 8);
 tab[1] = random(0, 8);
 }
 while(tab[0] != 5 && tab[1] != 4);  //w innym miejscu niż początek węża
}
//################### MIGANIE ###################
void miganie(void)
{
 fMiganie = !fMiganie;
 maks.setLed(0, krop[0], krop[1], fMiganie);
}
//################### RUCH SNAKE ###################
void ruchSnake(void)
{
 zapisz();
 
  if(przyciskG)
  {
   if(kier < 3)  kier++;
   else kier = 0;
   przyciskG = 0;
  }
  if(przyciskD)
  {
   if(kier > 0)  kier--;
   else kier = 3;
   przyciskD = 0;
  }
  
  switch(kier)
  {
     case 0:
   snake[0][0]++; //x rosną - prawo
   break;
     case 1:
   snake[1][0]--; //y maleje - dół
   break;
     case 2:
   snake[0][0]--; //x maleje - lewo
   break;
     case 3:
   snake[1][0]++; //y rośnie - góra
   break; 
  }
  
 wyswietl();
}
//####################### WYSWIETL #############################
void wyswietl(void)
{
  for(i = 0; i < 10; i++) //wyświetlenie węża zależnie od dlug - jego długości
  {
    if(i < dlug)  maks.setLed(0, snake[0][i], snake[1][i], true);
    else  maks.setLed(0, snake[0][i], snake[1][i], false);
  } 
}
//####################### ZAPISZ #############################
void zapisz(void)
{
  for(i = 10; i > 0; i--)  //zapisywanie drogi przebytej przez węża
  {
   snake[0][i] = snake[0][i - 1];
   snake[1][i] = snake[1][i - 1];
  }
}
//####################### RESET #############################
void reset(void)
{
   asm volatile ("  jmp 0"); 
}

