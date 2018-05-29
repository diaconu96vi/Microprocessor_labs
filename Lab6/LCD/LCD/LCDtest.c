#include <avr/io.h>

#define S2 250000UL

void sysinit();
void putchLCD(char ch);
void putsLCD (char* ch);
void clrLCD();
void gotoLC(unsigned char line, unsigned char col);


int main(){
   unsigned char k;
   sysinit();

   while(1){
      putsLCD("0123456789abcdef");
      gotoLC(2,1);
      putsLCD("ghijklmnopqrstuv");
      
         for(k=16;k>0;k--){
            gotoLC(2,k);
            wait(S2);
         }
         for(k=16;k>0;k--){   
            gotoLC(1,k);
            wait(S2);
         }
      clrLCD();
      wait(S2);
      
   }   
}

