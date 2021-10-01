/*
    Settings:
    Fuses:
        hFuse 0xFF
        lFuse 0x69;    4.8 MHz
    */
    #include <avr/io.h>
    #include <util/delay.h>
 
    #define SetBit(x,y)    x |= (1<<y)
    #define ClrBit(x,y) x &= ~(1<<y)
    #define NegBit(x,y) x ^= (1<<y)
 
 
    int main(void)
    { 
        DDRB=    0b11111111;
        PORTB=    0b00000000;
        //LED_driver(0b00000000); //Z Z Ż Ż C
 
        //----------ADC----------
 
        ADMUX = (1<<MUX1) | (1<<ADLAR);  //Kanał ADC2
        ADCSRA = (1<<ADEN); //ADC włączone
 
      while(1)
      {   
        ADCSRA = (1<<ADSC);
        while(ADCSRA & (1<<ADSC));
 
        if(ADC<682){LED_driver(0b11111111);}
        else{LED_driver(0b00000000);}
 
        _delay_ms(1000);
      }
    }
 
