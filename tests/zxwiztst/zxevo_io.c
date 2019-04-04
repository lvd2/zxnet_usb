
// Standard Input/Output functions
#include <stdio.h>
#include <intrz80.h>
#include <string.h>
#include "zxevo_io.h"
#include "w5300.h"


void DelayMs(unsigned char nFactor)   
{    
   
    while(nFactor--)   
    {   
    	unsigned int j=0x012c;   
        while(j--);  
    } 
}   


void mcu_soft_reset(void)
{
	while(1);
} 

unsigned char get_evo_rtc(unsigned char addr)
{
	output(0xdef7,addr);
	return input(0xbef7);
}       
void wiz_reset(void)
{
	output(0x83ab,0);
	DelayMs(100);
	output(0x83ab,0x10);
	DelayMs(500);
	
	output(0x82ab,0x4c);
	
	*WMR=0x80;
	DelayMs(500);
	*(WMR+1)=0x38;
}
   
