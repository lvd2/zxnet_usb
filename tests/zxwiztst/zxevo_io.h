
#include <intrz80.h>
#define set_evo_rtc(rtc_ad,rtc_val)     {output((unsigned short)0xdef7,rtc_ad); output((unsigned short)0xbef7,rtc_val);}
//#define get_evo_rtc(rtc_ad)             {output((unsigned short)0xdef7,rtc_ad); input((unsigned short)0xbef7);}

void mcu_soft_reset(void);
unsigned char get_evo_rtc(unsigned char addr);
void wiz_reset(void);   
void DelayMs(unsigned char nFactor); 
unsigned char getkey(void);

#define SL811_RESET() output(0x82ab,input(0x82ab)&0xbf);output(0x83ab,0x20);halt()