#include <avr/io.h>

int main(){
   // valorile memorate ale intrarilor
   unsigned char inputs;
   // daca nu va place unsigned char puteti folosi uint8_t definit in stdint.h
   // uint8_t este definit ca typedef unsigned char uint8_t
   // Variabila xi, i(=0,1,2) memoreaza in bitul i valoare intrarii xi.
   // ceilalti biti sunt 0.
   unsigned char x2,x1,x0;

   //in variabila outs se asambleaza iesirile astfel:
   // bit  7 6 5 4  3  2  1  0
   //outs- - - - -  -  f2 f1 f0
   unsigned char outs;
   unsigned char temp;

   DDRA = 0xff;
   DDRB = 0x00;

   while(1){
      //memoreaza valorile variabilelor de intrare
      //bit    7654 3 2  1  0
      //inputs=       x2 x1 x0
      inputs = PINB;
      inputs &= 0x07;

      x0 = inputs & 1<<0;
      x1 = inputs>>1 & 1;
      x2 = inputs>>2 & 1;

      outs = 0;
      temp = x0 ^ x1 ^ x2;
      // testam bitu 0 din temp
      if(temp & 1<<0){
         outs |= 1<<0; //setam bitu 0 din outs
      }
      
      if(inputs < 3){
         outs |= 1<<1; // setam bitu 1 din outs
      }

      if(inputs==2 || inputs == 3 || inputs == 5 || inputs == 7){
         outs |= 1<<2; // setam bitu 2 din outs
      }

      PORTA = outs;
   }
}
