
//#include "../my.h"
#include "../zxevo_io.h"

#include <stdio.h>
#include <Intrz80.h>
#include <string.h>
#include "hal.h"
#include "sl811.h"

extern unsigned char xdata DBUF[BUFFER_LENGTH];
extern WORD wordswap(WORD);
FLAGS bdata bFlags;

void SL11HardReset(void)
{
	unsigned char val=input(0x83ab)&0x10;
	halt();
	output(0x83ab,val);
	output(0x82ab,input(0x82ab)&0xbf);
	halt();
	val|=0x20;
	output(0x83ab,val);
	halt();
	halt();
	halt();
}
void SL11HostInit(void)
{       
	SL811Write(cSOFcnt, 0xae);  // Set SOF high counter, no change D+/D-SL11Write(CtrlReg, 0x48); // Setup Normal Operation
	SL811Write(IntEna, 0x63); // USBA/B, Insert/Remove,USBRest/Resume.
	SL811Write(cSOFcnt, 0xae);  // Set SOF high counter, no change D+/D-SL11Write(CtrlReg, 0x48); // Setup Normal Operation
	SL811Write(CtrlReg, 0); // Disable USB transfer operation and SOF
	SL811Write(cSOFcnt, 0xae); // Set SOF high counter, no change D+/D-SL11Write(CtrlReg, 0x48); // Clear SL811H mode and setup normal operation
	halt();
	halt();      // Delay for HW stablize
	SL811Write(CtrlReg, 0); // Disable USB transfer operation and SOF
}

void USBReset(void)   
{	
	BYTE tmp;
	tmp =  SL811Read(CtrlReg);
	SL811Write(CtrlReg,0x08);
	EZUSB_Delay(100);
	SL811Write(CtrlReg,0x18);
	EZUSB_Delay(100);
	SL811Write(CtrlReg,0x08);
	EZUSB_Delay(500);
	SL811Write(CtrlReg,tmp);
}   

int sl811_init_my(void)
{	
	unsigned char i,count=50;

	do{
		SL11HardReset();
		SL11HostInit();
	}while((i=SL811Read(IntStatus))==0x05);

	bFlags.bits.FULL_SPEED=FALSE;
	bFlags.bits.SLAVE_FOUND=FALSE;
	bFlags.bits.SLAVE_ENUMERATED=FALSE;
	bFlags.bits.BULK_OUT_DONE=FALSE;
	bFlags.bits.DATA_INPROCESS=FALSE;
	bFlags.bits.DATA_STOP=FALSE;
	while(count--){
		SL811Write(cSOFcnt,0xae);
		USBReset();
		SL811Write(IntEna,0x00);
		SL811Write(IntStatus,INT_CLEAR);
		EZUSB_Delay(100);
		i=SL811Read(IntStatus);
		if(i&USB_RESET)		//0x40
			break;
		if(!(i&USB_DPLUS))	//0x80
			continue;
		puts("'Full' Speed is detected!");                               // ** Full Speed is detected ** //   
		SL811Write(cSOFcnt,0xae);       // Set up Master & low speed direct and SOF cnt high=0x2e   
		SL811Write(cDATASet,0xe0);      // SOF Counter Low = 0xE0; 1ms interval   
		SL811Write(CtrlReg,0x05);       // Setup 48MHz and SOF enable
		bFlags.bits.FULL_SPEED=TRUE;
		bFlags.bits.SLAVE_FOUND=TRUE;
		SL811Write(EP0Status,0x50);	//90);
		SL811Write(EP0Counter,0x00);
		SL811Write(EP0Control,1);	//sDATA0_RD);	//0x23
		EZUSB_Delay(15);
		SL811Write(IntEna,0x61);
		SL811Write(IntStatus,INT_CLEAR);	//0xff
		return TRUE;
	}
	puts("NO device detected!");
	return FALSE;
}

BYTE SL811Read(BYTE adr)   
{
	output(SL811H_ADDR,adr);
	return input(SL811H_DATA);
}


void SL811Write(BYTE adr, BYTE dat)   
{	output(SL811H_ADDR,adr);
	output(SL811H_DATA,dat);
}

void SL811BufRead(BYTE addr, BYTE *s, BYTE c)   
{	
	while (c--){
		output(SL811H_ADDR,addr++);	
		*s++ = input(SL811H_DATA);
	}
}
unsigned char test(unsigned char c)
{
 return (c == 17) ? 0:(c + 1);
}

void SL811BufWrite(BYTE addr, BYTE *s, BYTE c)   
{	if(c<=0) return;
	while (c--){
		output(SL811H_ADDR,addr++);
		output(SL811H_DATA,*s++);
	}
}

WORD WordSwap(WORD input)   
{	return (((input&0x00FF)<<8)|((input&0xFF00)>>8));   
}

   
unsigned int LSwapINT16(unsigned short dData1,unsigned short dData2)   
{   
    unsigned int dData;   
    dData = ((dData2<<8)&0xff00)|(dData1&0x00ff);   
    return dData;   
}   
   
unsigned long LSwapINT32(unsigned long dData1,unsigned long dData2,unsigned long dData3,unsigned long dData4)   
{   
    unsigned long dData;   
    dData = ((dData4<<24)&0xff000000)|((dData3<<16)&0xff0000)|((dData2<<8)&0xff00)|(dData1&0xff);   
    return dData;   
}
