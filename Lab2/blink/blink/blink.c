#include <avr/io.h>
#define P 1250L
#define TH1 (P*10/100)
#define TH2 (P*90/100)
//125 = 1ms
int main(){
	volatile long i;
   long int th = TH1;
	
	DDRA=0xff;
	i=0;
	
	while(1){
		
		if(i==0)
			PORTA=1; //aprinde LED-ul
			
			if(i==th)
				PORTA=0; //stinge LED-ul
			i++;
			
			if(i==P)
				i=0; //a trecut o secunda
	}
}
