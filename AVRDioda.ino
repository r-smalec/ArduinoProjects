#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRB |= (0100<<PORTB);
  
  while(1)
  {
   PORTB ^= (1<<PB4);
   _delay_ms(1000); 
  }
}
