/*
 * Program do obsługi stopera do biegania, umożliwiający nastawienie czasów na biegi interwałowe oraz minutnika
 * autor: R.A.S.
 * 2.11.2016
 */

#define anodeChangePin 0
#define clockPin 1
#define dataPin 2
#define sw 3
#define clearPin 4

bool sw1 = 1, sw2 = 1; //symulacja przycisków cyfrowych przyciskami analogowymi

char num[10] = {B00010001, B11011101, B00100101, B10000101, B11001001, B10000011, B00000011, B11010101, B00000001, B10000001};
char minus = B11101111;
char m[2] = {B01110001, B11010001}; //m od minut
char min_ = 0, min_of_breaks = 0, min_tens = 0, min_of_breaks_tens = 0; //minuty i dziesiatki minut
char s[2] = {B10100011, B10000111}; //s od sekund
char sec_ = 0, sec_of_breaks = 0, sec_tens = 0, sec_of_breaks_tens = 0; //sekundy i dziesiątki sekund
char r[2] = {B01000011, B11111011}; //r od ilości powtórzeń
char repeats_ = 1, repeats_tens = 0; //ilość powtórzeń i dzięsiątki ilości powtórzeń
char repeats_total;
char b[2] = {B00100011, B01111011}; //b od oznaczneia przerwy
char nothing = B11111111;
char squeak = B11111110; //pod ostatni pin expandera podłączony jest buzzer

int i = 0, j = 0, k = 0;

typedef enum {SEC, MIN} options;

void checkInputs();
void showSymbol(int time_ms, char * symbol, char * symbol2);
void countClicks(char * a, char * b);
void countingDown(char tens, char unity, options opt);

/////////////////////////////// SETUP ///////////////////////////////
void setup()
{
  pinMode(anodeChangePin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
  pinMode(sw, INPUT);

  digitalWrite(anodeChangePin, LOW);
  
  while(sw1 == 1 && sw2 == 1) {
    checkInputs();
    showSymbol(20, &minus, &minus);
  }

// minuty
  showSymbol(750, m, (m + 1));
  sw1 = 1, sw2 = 1;
  countClicks(&min_, &min_tens);

//sekundy
  showSymbol(750, s, (s + 1));
  sw1 = 1, sw2 = 1;
  countClicks(&sec_, &sec_tens);

  while(sec_tens >= 6) { //by sekundy ponad 60 zamienić na pełne minuty
    sec_tens -= 6;
    min_++;
  }

    showSymbol(250, &nothing, &nothing);
    showSymbol(750, b, (b + 1)); //oznaczenie początku wprowadzania parametrów dla przerw
    showSymbol(400, &nothing, &nothing);

// minuty przerw
  showSymbol(750, m, (m + 1));
  sw1 = 1, sw2 = 1;
  countClicks(&min_of_breaks, &min_of_breaks_tens);

//sekundy przerw
  showSymbol(750, s, (s + 1));
  sw1 = 1, sw2 = 1;
  countClicks(&sec_of_breaks, &sec_of_breaks_tens);

  while(sec_of_breaks_tens >= 6) {
    sec_of_breaks_tens -= 6;
    min_of_breaks++;
  }


//ilość powtórzeń, przy założeniu że na jeden okres pracy przypada jedna przerwa
  showSymbol(750, r, (r + 1));
  sw1 = 1, sw2 = 1;
  countClicks(&repeats_, &repeats_tens);
  repeats_total = 10*repeats_tens + repeats_;

  showSymbol(500, &nothing, &nothing);
  sw1 = 1, sw2 = 1;
  
//odliczanie
  for(j = 0; j < repeats_total; j++)
  {
      countingDown(min_tens, min_, MIN);
      countingDown(sec_tens, sec_, SEC);
      
      while(sw2 == 1) { //czekanie na naciśnięcie przycisku żeby stoper odliczał dalej, forma alrmu (obecny w układzie buzzer) i wskazania na pracę urządzenia
        checkInputs();
        showSymbol(20, &minus, &minus);
        showSymbol(20, &squeak, &squeak); //buzzer wchodzi do akcji
      }
      sw1 = 1, sw2 = 1;

      if((repeats_total - j) > 0) {
        countingDown(min_of_breaks_tens, min_of_breaks, MIN);
        countingDown(sec_of_breaks_tens, sec_of_breaks, SEC);

            while(sw2 == 1) {
              checkInputs();
              showSymbol(20, &minus, &minus);
              showSymbol(20, &squeak, &squeak);
            }
            sw1 = 1, sw2 = 1;
      }
  }
 showSymbol(20, &nothing, &nothing);
}
/////////////////////////////// LOOP ///////////////////////////////
void loop()
{
  
}
/////////////////////////////// CHECK INPUTS ///////////////////////////////
void checkInputs()
{
  int voltage = (int)analogRead(sw); //liczba od 0 do 1023
  if(voltage > 600)
    { sw1 = 1; sw2 = 1; }
  else if(voltage > 300)
    { sw1 = 0; sw2 = 1; }
  else
    { sw1 = 1; sw2 = 0; }
}
/////////////////////////////// SHOW SYMBOL ///////////////////////////////
void showSymbol(int time_ms, char * symbol1, char * symbol2)
{
  time_ms /= 20;
  
  for(k = 0; k < time_ms; k++) //każda funkcja z iteracja powinna mieć swoją lokalną zmienną do obsługi iteracji, żeby jedna zmienna nie onsługiwała dwóch pętli jednocześnie
  {
    digitalWrite(clearPin, LOW);
    digitalWrite(clearPin, HIGH);
    digitalWrite(anodeChangePin, HIGH);
    if(*symbol1 == B00010001)
        shiftOut(dataPin, clockPin, LSBFIRST, nothing);
    else
        shiftOut(dataPin, clockPin, LSBFIRST, *symbol1);    
    delay(10);
    
    digitalWrite(clearPin, LOW);
    digitalWrite(clearPin, HIGH);
    digitalWrite(anodeChangePin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, *symbol2);
    delay(10);
  }
}
/////////////////////////////// COUNT CLICKS ///////////////////////////////
void countClicks(char * a, char * b) //dziesątki to b, jedności to a
{
    while(sw2 == 1)
      {
        checkInputs();
    
        showSymbol(200, (num + *b), (num + *a));
        
          if(sw1 == 0) //zliczanie naciśnięć
          {
            (*a)++; //ważne że jest to w nawiasie, inaczej nie działa
            
                  if(*a == 10) { //obsługa dziesiątek minut
                    *a = 0;
                    (*b)++;
                  }
          }
      }
}
/////////////////////////////// COUNTING DOWN ///////////////////////////////
void countingDown(char tens, char unity, options opt)
{
  int total = 10*(int)tens + (int)unity;
  char count, writingVisible = 0;
  
  for(i = 0; i < total; i++)
  {
    if(opt == MIN) {
      for(count = 0; count < 60; count++) {
        
        checkInputs();
        if(sw1 == 0 || sw2 == 0){
          writingVisible = 3; //włączenie widoczności na 3 sekundy
        }
        
        if(writingVisible != 0) {
          showSymbol(1000, (num + tens), (num + unity));
          writingVisible--;
        }
        else{
          showSymbol(1000, &nothing, &nothing); //ze względu na oszczędzanie energii
        }

        if(i == (total - 1)) {
          countingDown(6, 0, SEC);
          break;
        }
          
      }

    }
    else if(opt == SEC) {

      checkInputs();
        if(i == (total - 3) || sw1 == 0 || sw2 == 0){ //gdy zostały 3 sekundy do końca etapu odliczania lub został naciśnięty przycisk dla właczenia widoczności liczby na ekranie
          writingVisible = 3;
          sw1 = 1; sw2 = 1;
        }
        
      if(writingVisible != 0) {
          showSymbol(1000, (num + tens), (num + unity));
          writingVisible--;
        }
      else{
          showSymbol(1000, &nothing, &nothing);
        }
    }

    if(unity == 0) {
      unity = 10;
      tens--;
    }
    unity--;
  }
  
}
