#include <Adafruit_NeoPixel.h>
#define Liczba_diod 6

#include <avr/io.h>
#include <avr/interrupt.h>

Adafruit_NeoPixel linijka = Adafruit_NeoPixel(Liczba_diod, 4, NEO_GRB + NEO_KHZ800);

unsigned char diody[Liczba_diod][4]; //Liczba diod | RGB + Narastanie/opadanie
int i, j, k;
unsigned char poprzednieDiody[3];

int wybor = 0;

void inicjalizacja(void);
void zerowanieKolorem(unsigned char kolor0, unsigned char kolor1, unsigned char kolor2);
void opcja1(void);
void opcja2(void);
void opcja3(int x, int y, int z, int xAdd, int yAdd, int zAdd);

ISR(INT0_vect) {
  wybor++;
  if(wybor == 5) wybor = 0;
}

void setup() {

  DDRB &= ~(1<<DDB2);
  GIMSK |= (1<<INT0);
  sei();
  MCUCR |= (1<<ISC00);
  MCUCR |= (1<<ISC01);
  
  inicjalizacja();

}

void loop() {

        switch(wybor) {
                case 0: opcja1();
                        break;
                case 1: opcja2();
                        break;
                case 2: opcja3(50, 5, 10, 0, 1, 2);
                        break;
                case 3: opcja3(0, 15, 20, 1, 1, 0);
                        break;
                case 4: opcja3(0, 90, 0, 2, 1, 3);
                        break;
        }
}

//////////////////// INICJALIZACJA ////////////////////

void inicjalizacja(void) {

  linijka.begin();
  linijka.show();
  
  diody[0][0] = 0;
  diody[0][1] = 20;
  diody[0][2] = 80;
  diody[0][3] = 0;

  diody[1][0] = 20;
  diody[1][1] = 100;
  diody[1][2] = 60;
  diody[1][3] = 1;

  diody[2][0] = 20;
  diody[2][1] = 80;
  diody[2][2] = 0;
  diody[2][3] = 0;
  
  diody[3][0] = 100;
  diody[3][1] = 80;
  diody[3][2] = 20;
  diody[3][3] = 1;

  diody[4][0] = 0;
  diody[4][1] = 20;
  diody[4][2] = 60;
  diody[4][3] = 0;

  diody[5][0] = 20;
  diody[5][1] = 100;
  diody[5][2] = 80;
  diody[5][3] = 1;
  
}

//////////////////// ZEROWANIE KOLOREM ////////////////////

void zerowanieKolorem(unsigned char kolor0, unsigned char kolor1, unsigned char kolor2) {

  int a, b;

      for(a = 0; a < Liczba_diod; a++) {
        
        for(b = 0; b < 3; b++) {
          
              if(b == 0)
                diody[a][b] = kolor0;
              else if(b == 1)
                diody[a][b] = kolor1;
              else if(b == 2)
                diody[a][b] = kolor2;
                
        }
        
      }
  
}

//////////////////// OPCJA 1 ////////////////////

 void opcja1(void) {
  
  for(i = 0; i < Liczba_diod; i++) {
        
        j = i%3;
      
            if(diody[i][j] > 100) //pomiajszanie liczby przy dojściu do progu
              diody[i][3] = 1;
              
            if(diody[i][j] < 5) //zwiększanie liczby przy dojściu do progu
              diody[i][3] = 0;
      
            if(diody[i][3] == 0)
              diody[i][j] += 1;
            if(diody[i][3] == 1)
              diody[i][j] -= 1;
        
          linijka.setPixelColor(i, linijka.Color(diody[i][0], diody[i][1], diody[i][2]));
        
  }
  linijka.show();
  delay(10);
  
 }

//////////////////// OPCJA 2 ////////////////////

void opcja2(void) {
  
  for(i = 0; i < Liczba_diod; i++) {

  poprzednieDiody[0] = diody[i][0];
  poprzednieDiody[1] = diody[i][1];
  poprzednieDiody[2] = diody[i][2];
  
      for(j = 0; j < 20; j++) {
        
        if(diody[i][0] < 230)
          diody[i][0] += 10;

        if(diody[i][1] < 230)
          diody[i][1] += 10;

        if(diody[i][2] < 230)
          diody[i][2] += 10;

        linijka.setPixelColor(i, linijka.Color(diody[i][0], diody[i][1], diody[i][2]));
        linijka.show();
        delay(4);
      }
      
  diody[i][0] = poprzednieDiody[0];
  diody[i][1] = poprzednieDiody[1];
  diody[i][2] = poprzednieDiody[2];
  linijka.setPixelColor(i, linijka.Color(diody[i][0], diody[i][1], diody[i][2]));
  linijka.show();
  delay(400 + 8*diody[i][2]);
  }
  
}

//////////////////// OPCJA 2 ////////////////////

void opcja3(int x, int y, int z, int xAdd, int yAdd, int zAdd) {

  for(j = 0; j < 20; j++) {
    x = x + xAdd;
    y = y + yAdd;
    z = z + zAdd;
    zerowanieKolorem((unsigned char)x, (unsigned char)y, (unsigned char)z);
    
    for(i = 0; i < Liczba_diod; i++) {
    linijka.setPixelColor(i, linijka.Color(diody[i][0], diody[i][1], diody[i][2]));
    }
    linijka.show();
    delay(50);
  }

  for(j = 0; j < 20; j++) {
    x = x - xAdd;
    y = y - yAdd;
    z = z - zAdd;
    zerowanieKolorem((unsigned char)x, (unsigned char)y, (unsigned char)z);
    
    for(i = 0; i < Liczba_diod; i++) {
    linijka.setPixelColor(i, linijka.Color(diody[i][0], diody[i][1], diody[i][2]));
    }
    linijka.show();
    delay(50);
  }
  
}

