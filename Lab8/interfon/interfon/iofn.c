#include <avr/io.h>

#define clrbit(var,bit) var &= ~(1<<bit)
#define setbit(var,bit) var |= 1<<bit

#define NOKEY 0x7f

#define E  7
#define RW 6
#define RS 5

#define IReg 0
#define SReg 0
#define DReg 1

#define nop asm("nop"::);

volatile unsigned long int delay;

void wait(unsigned long int val){
   for(delay=0;delay<val;delay++){}
}

unsigned char rd_LCDreg(unsigned char vRS){
   unsigned char data;
   DDRD = 0;
   if(vRS == 0 ) {
      clrbit(PORTB,RS); // RS = '0'
   }
   else
      setbit(PORTB,RS); // RS = '1'
   setbit(PORTB,RW); //RW = '1'
   nop;

   setbit(PORTB, E); // E='1'
   nop; nop; nop; nop; nop;

   data = PIND;
   clrbit(PORTB,E); //E = '0'
   nop;

   DDRD = 0xff;

   return data;
}

void wr_LCDreg(unsigned char vRS, unsigned char data){
   
   if(vRS == 0 ) {
      clrbit(PORTB,RS); // RS = '0'
   }
   else
      setbit(PORTB,RS); // RS = '1'
   clrbit(PORTB,RW); //RW = '0'
   PORTD = data;
   nop;

   setbit(PORTB,E); // E = '1'
   nop; nop; nop; nop; nop;

   clrbit(PORTB,E); // E = '0'
   nop;
}


void ckbf(){
   while(rd_LCDreg(SReg) & 1<<7)
   {
   }

}

void initLCD(){
   ckbf();
   // Function set
   //bit 7 6 5 4   3 2 1 0
   //    0 0 1 DL  N F — — 
   //    0 0 1 1   1 1 0 0 
   wr_LCDreg(IReg, 0b00111100);

   ckbf();
   // entry mode set
   //bit 7 6 5 4   3 2 1    0
   //    0 0 0 0   0 1 I/D  S   
   //    0 0 0 0   0 1 1    0
   wr_LCDreg(IReg, 0b00000110);

   ckbf();
   // Display on/off control
   //bit 7 6 5 4   3 2 1 0
   //    0 0 0 0   1 D C B   
   //    0 0 0 0   1 1 1 1
   wr_LCDreg(IReg, 0b00001111);

   ckbf();
   //Clear display
   //bit 7 6 5 4   3 2 1 0
   //    0 0 0 0   0 0 0 1 
   wr_LCDreg(IReg, 0b00000001);
}

void sysinit(){
   PORTA = 0;
   PORTB = 0;
   DDRB  = 0b11100000;
   DDRD  = 0xff;
   wait(120000UL);
   initLCD();
}

void putchLCD( char ch){
   ckbf();
   wr_LCDreg(DReg, ch);
}

void putsLCD( char* ch){
   ckbf();
   while(*ch != 0x00){
      putchLCD(*ch++);
   }
}

void clrLCD(){
   ckbf();
   //Clear display
   //bit 7 6 5 4   3 2 1 0
   //    0 0 0 0   0 0 0 1 
   wr_LCDreg(IReg, 0b00000001);
}

void gotoLC(unsigned char line, unsigned char col){
   ckbf();
   if((line == 1 || line == 2) && (col <= 16 && col >= 1)){
      wr_LCDreg(IReg,0b10000000 + 0b01000000 * (line-1) + (col-1));
   
   }

}

char kbscan(){
	unsigned char temp;
	unsigned char cols;
	unsigned char lia=0; //linia activa
	unsigned char cola=0; //coloana activa
	unsigned char cod_intern;
	char cod_extern;
	char tabela_trans[] = "123A456B789C*0#D";
	
	for( DDRA = 1<<4; DDRA != 0; DDRA <<=1) {
		nop; nop; nop; nop; nop; nop;
		cols=~PINA;
		cols = cols & 0b00001111;
		
		if( cols != 0){
			break;
		}
	}
	
	if ( cols == 0 ){
		return NOKEY;	
	}
	// calcul linii
	temp = DDRA;
	if (temp == 0b00010000){
		lia = 0;
	}
	else if (temp == 0b00100000){
		lia = 1;
	}
	else if (temp == 0b01000000){
		lia = 2;
	}
	else if (temp == 0b10000000){
		lia = 3;
	}
	
	// calcul coloane
	if(cols & 0b00000001){
		cola = 0;
	}
	else if(cols & 0b00000010){
		cola = 1;
	}
	else if(cols & 0b00000100){
		cola = 2;
	}
	else if(cols & 0b00001000){
		cola = 3;
	}
	
	cod_intern = lia * 4 + cola;
	
	cod_extern= tabela_trans[cod_intern];
 
	return cod_extern;
}//end kbscan

/*char cmd1(){
   DDRB = 0b00000001;
   unsigned char cmd = PINB;
   return cmd;
}
*/
