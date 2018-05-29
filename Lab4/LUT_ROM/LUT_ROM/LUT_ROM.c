#include <avr/io.h>
#include <avr/pgmspace.h>

const unsigned char segLUT[] PROGMEM={
0b0000001, //0
0b1001111, //1
0b0010010, //2
0b0000110, //3
0b1001100, //4
0b0100100, //5
0b0100000, //6
0b0001111 //7
};

int main(){
   unsigned char inputs;
   DDRA=0xff;
   DDRB=0;

   while(1){

      inputs = PINB & 0b00000111;
      PORTA = pgm_read_byte (&segLUT[inputs]);
      }
}
