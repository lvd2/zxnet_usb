
//#include "../my.h"
#include "../zxevo_io.h"
#include <stdio.h>
#include <Intrz80.h>
#include <string.h>
#include "sl811.h"   
#include "tpbulk.h"   
#include "hal.h"  

//extern void EZUSB_Delay(unsigned char);

//////////////////////////////////   
extern unsigned char xdata DBUF[BUFFER_LENGTH];   
//SYS_INFO_BLOCK xdata DeviceInfo; 
   
unsigned char idata out[0x20];   
///////////////////////////////////////////////////////////////////////////   
unsigned char EnumMassDev(void)   
{   
    if(!SPC_Inquiry())
        return 0x81;//FALSE;   
    if(!SPC_TestUnit())   
        return 0x82;//FALSE;   
    if(!SPC_LockMedia())   
        return 0x83;//FALSE;   
    if(!SPC_RequestSense())   
        return 0x84;//FALSE;   
    if(!SPC_TestUnit())   
        return 0x85;//FALSE;   
    if(!RBC_ReadCapacity())   
        return 0x86;//FALSE;   
   
    ////////////////////////////////////////////////////   
    //DeviceInfo.BPB_BytesPerSec=512; //SectorSize 512   
       
    if(!SPC_RequestSense())   
        return 0x87;//FALSE;   
    if(!SPC_TestUnit())   
        return 0x88;//FALSE;   
    if(!RBC_ReadCapacity())   
        return 0x89;//FALSE;   
    ////////////////////////////////////////////////////   
    if(!RBC_Read(0x0,1,DBUF))   
        return 0x8a;//FALSE;   
    //////////////////////////////////

    return TRUE;   
}   

const unsigned char data0[25]={
    	0x55,0x53,0x42,0x43,0x60,0xa6,0x24,0xde,   
    	0x24,0x00,0x00,0x00,0x80,0x00,0x06,SPC_CMD_INQUIRY,
    	0x00,0x00,0x00,0x24,}; 
    	     
unsigned char SPC_Inquiry(void)   
{   
    memcpy(out,data0,21); 
    if(!epBulkSend(out,0x1f))      
        return FALSE;   
    DelayMs(150);   
    if(!epBulkRcv(DBUF,36))   
        return FALSE;      
    if(!epBulkRcv(out,13))   
        return FALSE;   
    ////////////////////////////////   
    return TRUE;       
}   
   
unsigned char SPC_RequestSense(void)   
{   
    memcpy(out,data0,21);  
    out[8]=0x0e;   
    out[15]=SPC_CMD_REQUESTSENSE;
    out[19]=0x0e;      
    //////////////////////////////////////   
    if(!epBulkSend(out,0x1f))      
        return FALSE;   
    DelayMs(5);   
    if(!epBulkRcv(out,18))   
        return FALSE;      
    //delay_dsp   
    if(!epBulkRcv(out,13))   
        return FALSE;   
/////////////////////////////   
    return TRUE;   
}   
   
unsigned char SPC_TestUnit(void)   
{   
    memcpy(out,data0,21);  
    ////////////////////////////////      
    out[8]=0x00;  
    out[12]=0x00;
    /////////////////////////////////////      
    out[15]=SPC_CMD_TESTUNITREADY;   
    out[19]=0;  
    //////////////////////////////////////   
    if(!epBulkSend(out,0x1f))
        return FALSE;
    DelayMs(5);   
    if(!epBulkRcv(out,13))
        return FALSE;
    return TRUE;   
}   
   
unsigned char SPC_LockMedia(void)   
{   
    memcpy(out,data0,21);      
    out[8]=0x00;   
    out[12]=0x00;
    out[14]=5;   
    ///////////////////////////////////////////   
    out[15]=SPC_CMD_PRVENTALLOWMEDIUMREMOVAL;   
    out[19]=1;   
    ///////////////////////////////////////////   
    if(!epBulkSend(out,0x1f))      
        return FALSE;   
    DelayMs(5);   
   
    if(!epBulkRcv(out,13))   
        return FALSE;   
   
/////////////////////////////   
    return TRUE;   
}   
   
unsigned char RBC_ReadCapacity(void)   
{   
    memcpy(out,data0,25);    
    out[8]=0x08;     
    out[14]=10;   
    /////////////////////////////////////   
    out[15]=RBC_CMD_READCAPACITY;   
    out[19]=0;   
    /////////////////////////////////////   
    if(!epBulkSend(out,0x1f))      
        return FALSE;   
    DelayMs(10);   
    if(!epBulkRcv(DBUF,8))   
        return FALSE;   
    if(!epBulkRcv(out,13))   
        return FALSE;   
    /////////////////////////////   
    return TRUE;   
}   
   
unsigned char RBC_Read(unsigned long lba,unsigned char len,unsigned char *pBuffer)   
{   
    //unsigned long lout;    
    memcpy(out,data0,8);       
    //lout=len*DeviceInfo.BPB_BytesPerSec; 
    *((unsigned long *) (out+8))=len*512; 
    out[12]=0x80;
	out[13]=0x00;
	out[14]=10;   
    /////////////////////////////////////   
    out[15]=RBC_CMD_READ10;
	out[16]=0x00;   
    out[17]=(unsigned char)((lba>>24)&0xff);   
    out[18]=(unsigned char)((lba>>16)&0xff);   
    out[19]=(unsigned char)((lba>>8)&0xff);   
    out[20]=(unsigned char)(lba&0xff);     
    out[21]=0x00;   
    out[22]=(unsigned char)((len>>8)&0xff);   
    out[23]=(unsigned char)(len&0xff);     
    out[24]=0x00;   
    //////////////////////////////////////     
    if(!epBulkSend(out,0x1f))      
    {	puts("epBulkSend Error");
        return FALSE;
    }
    EZUSB_Delay(10);   
    if(!epBulkRcv(pBuffer,len*512))   
    {	puts("epBulkRcv Error 1");
        return FALSE;
    }
    if(!epBulkRcv(out,13))   
    {	puts("epBulkRcv Error 2");
        return FALSE;
    }
    /////////////////////////////   
    return TRUE;   
}   
   
unsigned char RBC_Write(unsigned long lba,unsigned char len,unsigned char *pBuffer)   
{   
    //unsigned long lout;   
    out[0]=0x55;out[1]=0x53;out[2]=0x42;out[3]=0x43;   
    out[4]=0xb4;out[5]=0xd9;out[6]=0x77;out[7]=0xc1;   
    //lout=len*DeviceInfo.BPB_BytesPerSec; 
    *((unsigned long *) (out+8))=len*512; 
    out[12]=0x00;out[13]=0x00;out[14]=10;   
    /////////////////////////////////////   
    out[15]=RBC_CMD_WRITE10;       
    out[16]=0x00;   
    out[17]=(unsigned char)((lba>>24)&0xff);   
    out[18]=(unsigned char)((lba>>16)&0xff);   
    out[19]=(unsigned char)((lba>>8)&0xff);   
    out[20]=(unsigned char)(lba&0xff);   
    out[21]=0x00;   
    out[22]=(unsigned char)((len>>8)&0xff);   
    out[23]=(unsigned char)(len&0xff);     
    out[24]=0x00;   
    //////////////////////////////////////   
    if(!epBulkSend(out,0x1f))      
        return FALSE; 
    DelayMs(15);   
    if(!epBulkSend(pBuffer,len*512))   
        return FALSE;   
    DelayMs(10);        
    if(!epBulkRcv(out,13))   
        return FALSE;   
    return TRUE;   
}
