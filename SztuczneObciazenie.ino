/*
Program do sterowania sztucznym obciążeniem - wyświetlanie ustawionej mocy, ustawianie mocy na enkoderze obrotowym i ustawianie wyjść by podłączyć rezystory o rządnaej wartości
Autor: R.A.S.
08.06.2018
*/
#include <LiquidCrystal.h>
#include <Rotary.h>
#include <stdio.h>

#define ENKODER1 12
#define ENKODER2 11
#define SW 10

#define RS 16
#define E 17
#define D4 18
#define D5 19
#define D6 20
#define D7 21

#define U A15
#define I1 A11
#define I2 A12
#define PWM 8

#define SEKCJA1 23
#define SEKCJA2 25
#define SEKCJA3 27
#define SEKCJA4 29
#define SEKCJA5 31
#define SEKCJA6 33
#define SEKCJA7 35
#define SEKCJA8 37

char section[8];
char lcd_buffer[20];
int i = 20;
int j = 0;
int powerOnScreen = 0;
bool powerSet = 0;

Rotary r = Rotary(ENKODER1, ENKODER2);

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

void setPower(int power, char mode);

void setup() {
  pinMode(ENKODER1, INPUT);
  pinMode(ENKODER2, INPUT);
  pinMode(SW, INPUT);
  
  pinMode(RS, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  pinMode(U, INPUT);
  pinMode(I1, INPUT);
  pinMode(I2, INPUT);
  pinMode(PWM, OUTPUT);

  section[0] = 23;
  section[1] = 25;
  section[2] = 27;
  section[3] = 29;
  section[4] = 31;
  section[5] = 33;
  section[6] = 35;
  section[7] = 37;
  pinMode(section[0], OUTPUT);
  pinMode(section[1], OUTPUT);
  pinMode(section[2], OUTPUT);
  pinMode(section[3], OUTPUT);
  pinMode(section[4], OUTPUT);
  pinMode(section[5], OUTPUT);
  pinMode(section[6], OUTPUT);
  pinMode(section[7], OUTPUT);

  digitalWrite(section[0], HIGH);
  digitalWrite(section[1], HIGH);
  digitalWrite(section[2], HIGH);
  digitalWrite(section[3], HIGH);
  digitalWrite(section[4], HIGH);
  digitalWrite(section[5], HIGH);
  digitalWrite(section[6], HIGH);
  digitalWrite(section[7], HIGH);
  
  lcd.begin(16, 2);
  lcd.setCursor(3, 0);
  lcd.print("SmartPower");
  lcd.setCursor(3, 1);
  lcd.print("R. Smalec");
  delay(750);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Moc [W]");
  
}

void loop() {
        unsigned char result = r.process();
        if (result == DIR_NONE) {
          //nic nie rób
        }
            else if (result == DIR_CW) {
              i++;
                  if(i == 9 || i == 99) {            //by wyczyścić niepotrzebne znaki na wyświetlaczu
                  lcd.clear();
                  lcd.setCursor(4, 0);
                  lcd.print("Moc [W]");
                  }
            }
                else if (result == DIR_CCW) {
                  i--;
                      if(i == 9 || i == 99) {       //by wyczyścić niepotrzebne znaki na wyświetlaczu
                      lcd.clear();
                      lcd.setCursor(4, 0);
                      lcd.print("Moc [W]");
                      }
                }
        
    powerOnScreen = i*10;
    lcd.setCursor(6, 1);
    sprintf(lcd_buffer,"%d", powerOnScreen);
    lcd.print(lcd_buffer);

    if(digitalRead(SW) == 0 && powerSet == 0) {
        setPower(powerOnScreen, 0);
        powerSet = 1;
    }

    if(digitalRead(SW) == 0 && powerSet == 1) {
        setPower(0, 1);
        powerSet = 0;
    }

    if(powerSet){
      setPower(powerOnScreen, 2);
    }
}

/////////////////////////////USTAW MOC/////////////////////////////
void setPower(int power, char mode) {
   int voltage = analogRead(U);
   float v = (float)map(v, 0, 1023, 0, 5);
   float P = 45.4 * v * v; //wartość 45.4 ze względu na wyliczenia związane z wartością rezystora (57,6 om, 48 V, 40 W),
                           //wyliczenia mocy zależnej od pomiaru napięcia na dzielniku w dokumentacji w pliku Mathcada
   int nrOfSections = (int)((float)power/P);

   if(mode == 0) {
       int maxPower = 9*P;
       if(power > maxPower) {
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print("Maksymalna moc:");
            lcd.setCursor(6, 1);
            sprintf(lcd_buffer,"%d", maxPower);
            lcd.print(lcd_buffer);
            delay(1000);
            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print("Moc [W]");
            return 0;
       }
       
        lcd.setCursor(3, 1);
        lcd.print("Ustawianie");
        
       for(int k = 0; k < nrOfSections; k++) {
          digitalWrite(section[k], LOW);
          delay(250);
       }
       for(int k = nrOfSections; k < 8; k++) {
          digitalWrite(section[k], HIGH);
       }
    
       float restOfPower = sqrt(power - nrOfSections * P);
       int duty = map((int)restOfPower, 0, 17, 0, 255);
       analogWrite(PWM, duty);
    
       lcd.clear();
       lcd.setCursor(4, 0);
       lcd.print("Moc [W]");
   }

   if(mode == 1) {
    for(int k = nrOfSections; k < 8; k++) {
          digitalWrite(section[k], HIGH);
       }
    analogWrite(PWM, 0);
   }

   if(mode == 2) {
    for(int k = 0; k < nrOfSections; k++) {
          digitalWrite(section[k], LOW);
    }
    
    for(int k = nrOfSections; k < 8; k++) {
          digitalWrite(section[k], HIGH);
    }
    
       float restOfPower = sqrt(power - nrOfSections * P);
       int duty = map((int)restOfPower, 0, 17, 0, 255);
       analogWrite(PWM, duty);
   }
}

