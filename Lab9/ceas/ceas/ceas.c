#include <avr/io.h>


void sysinit();
void putchLCD(char ch);
void putsLCD (char* ch);
void gotoLC(unsigned char line, unsigned char col);
void clrLCD();
void initLCD();
char kbscan();
void wait();
unsigned char temp;

#define NOKEY 0x7f
#define DELAY 200
int main(){
   char code_ante = NOKEY;
	char code_now = NOKEY;
	unsigned char kbhit = 0;
	char kbcode;
	unsigned char loop_cnt=0;


   PORTB = 0;
   DDRB = 0b11100000;
   DDRD = 0;
   initLCD();

   TCCR2 = 0b00001110;
   OCR2 = 250-1;
   int p = 256;

   unsigned long Tcycles = 0, k =125;
   unsigned char sec = 30, min = 23, ora = 10;
   char c = 0;
	 //nu este in curs, dar este necesar
	sysinit();
	while(1){ //bucla principala
	//
   if(loop_cnt==DELAY){ //citirile se fac din 15 ms in 15 ms
		loop_cnt=0;
		code_ante = code_now;
		code_now = kbscan();
		if( code_ante == NOKEY && code_now != NOKEY){
			kbhit=1;
			kbcode=code_now;
		}
	}
	//
	//consuma codul
	if(kbhit){
		kbhit=0;
      gotoLC(1,1);
		putchLCD(kbcode);
	//prelucreaza kbcode; de exemplu afiseaza-l
	}
   loop_cnt++;
	if(TIFR & 1 << OCF2){ //citirile se fac din 15 ms in 15 ms
	   TIFR |= 1 << OCF2;
      Tcycles++;
      if(Tcycles == k){
         Tcycles = 0;
         sec++;
         if( sec == 60){
          sec = 0;
          min++;
          if(min==60){
            min = 0;
            ora++;
            if(ora == 24){
               ora = 0;
            }
          }

        }
		  gotoLC(1,1);
        if ( ora > 9) {
         c = ora / 10 + '0';
         putchLCD(c);
         c = ora %10 + '0';
         putchLCD(c);
        }
        else{
         putchLCD(',');
         c = ora + '0';
         putchLCD(c);
        }
       putchLCD(':');
       c=min / 10 +'0';
       putchLCD(c);
       c=min%10 + '0';
       putchLCD(c);
       
       putchLCD(':');
       c=sec / 10 + '0';
       putchLCD(c);
       c = sec % 10 + '0';
       putchLCD(c);
       
       if(TIFR &1 <<0){
            putsLCD("Gresit");
            while(1);
       }
      }
     }
    }         

} //end main

