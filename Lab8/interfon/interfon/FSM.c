#include <avr/io.h>
#include <stdio.h>

#define FMSG   0
#define WAIT   1
#define RING   2
#define OPEN   3
#define COD1   4
#define COD2   5
#define COD3   6
#define ERR    7

void sysinit();
void initLCD();
void putchLCD(char ch);
void putsLCD(char *ch);
void clrLCD();
void gotoLC(unsigned char line, unsigned char col);
char kbscan();
void wait(unsigned long int val);
char cmd1();

int main(){

   char code_now=0x7f, code_ante;
   unsigned char kbhit=0;
   unsigned char cmd='0';
   char kbcode;
   
   unsigned char loop_cnt=0;
   volatile unsigned long int delay;
   
   int cod = 123; //sau char cod[4]="123";
   char buf[4];
   char stare = FMSG;
      
   sysinit();
   
   while (1){
      //determina daca este tasta apasata
      if(loop_cnt==100){
         code_ante=code_now;
         code_now = kbscan();
         if(code_now!=0x7f && code_ante==0x7f){
            kbhit=1;
            kbcode=code_now;
         }
         loop_cnt=0;
      }
      
      //SFSM
      switch (stare){
         case FMSG:
            clrLCD();
            putsLCD("Suna la 1..8,");
            gotoLC(2,1);
            putsLCD("C pentru cod:");
            stare = WAIT;
            break;
         
         case WAIT:
            if(kbhit){
               kbhit=0;
               if ('C'==kbcode){
                  clrLCD();
                  putsLCD("Cod=");
                  stare = COD1;
               }
               if (kbcode>='1' && kbcode <= '8'){
                  clrLCD();
                              
                  stare = RING;
               }
            }               
            break;
            
         case RING:            
            for(delay=0; delay<15;delay++){
               if(cmd1() =='1'){
                  clrLCD();
                  stare = OPEN;
                  
               }
               if(delay==0 || delay== 4 || delay==8 || delay==12){
                  putsLCD("Sun la ");
                  putchLCD(kbcode);      
                  wait(250000UL);
                  clrLCD();
               }
               else if (delay == 1 || delay == 5 || delay == 9 || delay == 13){
                  putsLCD("Sun la ");
                  putchLCD(kbcode);
                  putsLCD(".");
                  wait(250000UL);
                  clrLCD();
               }
               else if ( delay == 2 || delay == 6 || delay == 10 || delay == 14){
                  putsLCD("Sun la ");
                  putchLCD(kbcode);
                  putsLCD("..");
                  wait(250000UL);
                  clrLCD();
               }
               else if ( delay == 3 || delay == 7 || delay == 11){
                  putsLCD("Sun la ");
                  putchLCD(kbcode);
                  putsLCD("...");
                  wait(250000UL);
                  clrLCD();
               }
                        
            }
            if(cmd=='0'){
               stare = FMSG;
            }
            break;
            
         case COD1:
            if(kbhit){
               kbhit=0;
               if (kbcode>='0' && kbcode <= '9'){
                  buf[0]=kbcode;
                  putchLCD('*');
                  stare = COD2;               
               }
               else{
                  stare = FMSG;
               }
            }
            break;
            
         case COD2: 
            if(kbhit){
               kbhit=0;
               if (kbcode>='0' && kbcode <= '9'){
                  buf[1]=kbcode;
                  putchLCD('*');
                  stare = COD3;               
               }
               else{
                  stare = FMSG;
               }
            }
            break;
            
         case COD3: 
            if(kbhit){
               kbhit=0;
               if (kbcode>='0' && kbcode <= '9'){
                  buf[2]=kbcode;
                  putchLCD('*');
                  clrLCD();
                  if(buf[0] == '1' && buf[1]=='2' && buf[2]=='3'){
                     putsLCD("Deschis!");
                     stare = OPEN;
                  }
                  else{
                     putsLCD("Cod invalid!");
                     stare = ERR;
                  }
                                 
               }
               else{
                  stare = FMSG;
               }
            }
            break;
            
         case OPEN: 
            putsLCD("Deschiss!");
            for(int i=0; i<5;i++){
               wait(250000UL);
            }
            stare = FMSG;
            break;
            
         case ERR: 
            //asteapta  5 secunde
            for(delay=0; delay<5;delay++){
               wait(250000UL);
            }
            stare = FMSG;
            break;
            
      }//end switch
      loop_cnt++;
 }//end while
}//end main
