#include <avr/io.h>
#include <avr/pgmspace.h>

int main(){
   unsigned char inputs;
   DDRA = 0xff;
   DDRB = 0;

   while(1){
      inputs = PINB & 0b00000111;
      switch(inputs){
         case(0):{
            PORTA = 0b0000001;
            break;
         }
         case(1):{
            PORTA = 0b1001111;
            break;
         }
         case(2):{
            PORTA = 0b0010010;
            break;
         }
         case(3):{
            PORTA = 0b0000110;
            break;
         }
         case(4):{
            PORTA = 0b1001100;
            break;
         }
         case(5):{
            PORTA = 0b0100100;
            break;
         }
         case(6):{
            PORTA = 0b0100000;
            break;
         }
         case(7):{
            PORTA = 0b0001111;
            break;
         }
         default: PORTA = 0b1111110;
      }
   }
}
