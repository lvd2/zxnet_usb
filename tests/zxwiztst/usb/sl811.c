
//#include "../my.h"
#include "../zxevo_io.h"
#include <stdio.h>
#include <Intrz80.h>
#include <string.h> 
#include "SL811.H"   
#include "HAL.H"   

extern FLAGS bdata bFlags;   
PKG usbstack;   
unsigned char xdata DBUF[BUFFER_LENGTH];   
pUSBDEV xdata uDev; // Multiple USB devices attributes, Max 5 devices   

//*****************************************************************************************   
// usbXfer:   
// successful transfer = return TRUE   
// fail transfer = return FALSE   
//*****************************************************************************************   
unsigned char usbXfer(void)   
{     
    unsigned char xferLen, data0, data1,cmd;   
    unsigned char intr,result,remainder,dataX,bufLen,addr,timeout;   
    unsigned char out[8];   

    //------------------------------------------------   
    // Default setting for usb trasnfer   
    //------------------------------------------------   
    dataX=timeout=0;   
       
    data0 = EP0_Buf;                    // DATA0 buffer address   
    data1 = data0 + (unsigned char)usbstack.wPayload;   // DATA1 buffer address   
    bFlags.bits.DATA_STOP=FALSE;   
    bFlags.bits.TIMEOUT_ERR=FALSE;   

    //------------------------------------------------   
    // Define data transfer payload   
    //------------------------------------------------   
    if (usbstack.wLen >= usbstack.wPayload)          // select proper data payload   
        xferLen = usbstack.wPayload;            // limit to wPayload size    
    else                            // else take < payload len   
        xferLen = usbstack.wLen;            //     
       
    // For IN token   
    if (usbstack.pid==PID_IN)               // for current IN tokens   
	{
//		if(bFlags.bits.FULL_SPEED)
//			cmd=sDATA0_RD;
//		else
//			cmd=DATA0_RD;
	cmd = sDATA0_RD;            // FS/FS on Hub, sync to sof   
	}   
    // For OUT token   
    else if(usbstack.pid==PID_OUT)              // for OUT tokens   
    {      
        if(xferLen)                                 // only when there are     
		{
            SL811BufWrite(data0,usbstack.buffer,xferLen);   // data to transfer on USB   
//		if(bFlags.bits.FULL_SPEED)
//			cmd=sDATA0_WR;
//		else
//			cmd=DATA0_WR;
		}
		cmd = sDATA0_WR;                        // FS/FS on Hub, sync to sof   
    // implement data toggle   
        bFlags.bits.bData1 = uDev.bData1[usbstack.endpoint];   
            uDev.bData1[usbstack.endpoint] = (uDev.bData1[usbstack.endpoint] ? 0 : 1); // DataToggle   
           
        if(bFlags.bits.bData1)   
                cmd |= 0x40;                              // Set Data1 bit in command   
    }   
    //------------------------------------------------   
    // For SETUP/OUT token   
    //------------------------------------------------   
    else                                            // for current SETUP/OUT tokens   
    {      
        if(xferLen)                                 // only when there are     
        {                                       // data to transfer on USB   
            intr=usbstack.setup.wLength;               
            out[0]=usbstack.setup.bmRequest;   
            out[1]=usbstack.setup.bRequest;   
            out[2]=(unsigned char)usbstack.setup.wValue;   
            out[3]=(unsigned char)(usbstack.setup.wValue>>8);   
            out[4]=(unsigned char)usbstack.setup.wIndex;   
            out[5]=(unsigned char)(usbstack.setup.wIndex>>8);   
            out[6]=(unsigned char)usbstack.setup.wLength;   
            out[7]=(unsigned char)(usbstack.setup.wLength>>8);   
            SL811BufWrite(data0,out,xferLen);   
            usbstack.setup.wLength=intr;   
        }   
//		if(bFlags.bits.FULL_SPEED)
//			cmd=sDATA0_WR;
//		else
//			cmd=DATA0_WR;
		cmd = sDATA0_WR;                            // FS/FS on Hub, sync to sof   
    }   
    //------------------------------------------------   
    // For EP0's IN/OUT token data, start with DATA1   
    // Control Endpoint0's status stage.   
    // For data endpoint, IN/OUT data, start ????   
    //------------------------------------------------   
    if (usbstack.endpoint == 0 && usbstack.pid != PID_SETUP)    // for Ep0's IN/OUT token   
        cmd |= 0x40;                    // always set DATA1   
    //------------------------------------------------   
    // Arming of USB data transfer for the first pkt   
    //------------------------------------------------   
    SL811Write(EP0Status,((usbstack.endpoint&0x0F)|usbstack.pid));  // PID + EP address   
    SL811Write(EP0Counter,usbstack.usbaddr);                    // USB address   
    SL811Write(EP0Address,data0);                   // buffer address, start with "data0"   
    SL811Write(EP0XferLen,xferLen);                 // data transfer length   
    SL811Write(IntStatus,INT_CLEAR);                // clear interrupt status   
    SL811Write(EP0Control,cmd);                     // Enable ARM and USB transfer start here   
       
    //------------------------------------------------   
    // Main loop for completing a wLen data trasnfer   
    //------------------------------------------------   
    while(TRUE)   
    {      
        //---------------Wait for done interrupt------------------   
        while(TRUE)                                             // always ensure requested device is   
        {                                                       // inserted at all time, then you will   
            intr = SL811Read(IntStatus);       
                                    // wait for interrupt to be done, and    
            if((intr & USB_RESET) || (intr & INSERT_REMOVE))    // proceed to parse result from slave    
            {                                                   // device.   
                bFlags.bits.DATA_STOP = TRUE;                               // if device is removed, set DATA_STOP  
                
                return FALSE;                                   // flag true, so that main loop will    
            }                                                   // know this condition and exit gracefully   
            if(intr & USB_A_DONE)                                  
                break;                          // interrupt done !!!   
        }   
   
        SL811Write(IntStatus,INT_CLEAR); // clear interrupt status   
        result    = SL811Read(EP0Status);                       // read EP0status register   
        remainder = SL811Read(EP0Counter);                      // remainder value in last pkt xfer   
   
        //-------------------------ACK----------------------------   
        if (result & EP0_ACK)                                   // Transmission ACK   
        {      
   
            // SETUP TOKEN   
            if(usbstack.pid == PID_SETUP)                               // do nothing for SETUP/OUT token    
                break;                                          // exit while(1) immediately   
   
            // OUT TOKEN                   
            else if(usbstack.pid == PID_OUT)   
                break;   
   
            // IN TOKEN   
            else if(usbstack.pid == PID_IN)   
            {                                                   // for IN token only   
                usbstack.wLen  -= (WORD)xferLen;    // update remainding wLen value   
                cmd   ^= 0x40;              // toggle DATA0/DATA1   
                dataX++;                // point to next dataX   
   
                //------------------------------------------------     
                // If host requested for more data than the slave    
                // have, and if the slave's data len is a multiple   
                // of its endpoint payload size/last xferLen. Do    
                // not overwrite data in previous buffer.   
                //------------------------------------------------     
                if(remainder==xferLen)          // empty data detected   
                    bufLen = 0;         // do not overwriten previous data   
                else                    // reset bufLen to zero   
                    bufLen = xferLen;       // update previous buffer length   
                   
                //------------------------------------------------     
                // Arm for next data transfer when requested data    
                // length have not reach zero, i.e. wLen!=0, and   
                // last xferlen of data was completed, i.e.   
                // remainder is equal to zero, not a short pkt   
                //------------------------------------------------     
                if(!remainder && usbstack.wLen)                         // remainder==0 when last xferLen   
                {                                               // was all completed or wLen!=0   
                    addr    = (dataX & 1) ? data1:data0;        // select next address for data   
                    xferLen = (BYTE)(usbstack.wLen>=usbstack.wPayload) ? usbstack.wPayload:usbstack.wLen;    // get data length required   
                                   
                    cmd |= 0x20;                            // always sync SOF when FS, regardless    
                    SL811Write(EP0XferLen, xferLen);            // select next xfer length   
                    SL811Write(EP0Address, addr);               // data buffer addr    
                    SL811Write(IntStatus,INT_CLEAR);            // is a LS is on Hub.   
                    SL811Write(EP0Control,cmd);                 // Enable USB transfer and re-arm   
                }                  
   
                //------------------------------------------------   
                // Copy last IN token data pkt from prev transfer   
                // Check if there was data available during the   
                // last data transfer   
                //------------------------------------------------   
                if(bufLen)                                         
                {      
                    SL811BufRead(((dataX&1)?data0:data1), usbstack.buffer, bufLen);   
                    usbstack.buffer += bufLen;                                 
                }   
   
                //------------------------------------------------   
                // Terminate on short packets, i.e. remainder!=0   
                // a short packet or empty data packet OR when    
                // requested data len have completed, i.e.wLen=0   
                // For a LOWSPEED device, the 1st device descp,   
                // wPayload is default to 64-byte, LS device will   
                // only send back a max of 8-byte device descp,   
                // and host detect this as a short packet, and    
                // terminate with OUT status stage   
                //------------------------------------------------   
                if(remainder || !usbstack.wLen)   
                    break;   
            }// PID IN                             
        }   
               
        //-------------------------NAK----------------------------   
        if (result & EP0_NAK)                                   // NAK Detected   
        {                                                          
            //if(usbstack.endpoint==0)                            // on ep0 during enumeration of LS device   
            //{                                                   // happen when slave is not fast enough,   
                SL811Write(IntStatus,INT_CLEAR);                // clear interrupt status, need to   
                SL811Write(EP0Control,cmd);                     // re-arm and request for last cmd, IN token   
                        result = 0;                                     // respond to NAK status only   
            //}   
            //else                                                // normal data endpoint, exit now !!! , non-zero ep   
            //    break;                                          // main loop control the interval polling   
        }   
       
        //-----------------------TIMEOUT--------------------------   
        if (result & EP0_TIMEOUT)                               // TIMEOUT Detected   
        {                                                          
            if(usbstack.endpoint==0)                                        // happens when hub enumeration   
            {   
                if(++timeout >= TIMEOUT_RETRY)   
                {      
                    timeout--;   
                    break;                                      // exit on the timeout detected    
                }   
                SL811Write(IntStatus,INT_CLEAR);                // clear interrupt status, need to   
                SL811Write(EP0Control,cmd);                     // re-arm and request for last cmd again   
            }   
            else                                                   
            {                                                   // all other data endpoint, data transfer    
                bFlags.bits.TIMEOUT_ERR = TRUE;                             // failed, set flag to terminate transfer   
                break;                                          // happens when data transfer on a device   
            }                                                   // through the hub   
        }   
   
        //-----------------------STALL----------------------------   
        if (result & EP0_STALL)                                 // STALL detected   
            return TRUE;                                        // for unsupported request.   
                                                                           
        //----------------------OVEFLOW---------------------------   
        if (result & EP0_OVERFLOW)                              // OVERFLOW detected   
            break;   
        //-----------------------ERROR----------------------------   
        //if (result & EP0_ERROR)                                 // ERROR detected   
        //    break;   
    }   // end of While(1)   
      
    if (result & EP0_ACK)   // on ACK transmission   
        return TRUE;        // return OK   
   	printf("\rresult 0x%02x",result);
    return FALSE;           // fail transmission   
   
}   
//*****************************************************************************************   
// Control Endpoint 0's USB Data Xfer   
// ep0Xfer, endpoint 0 data transfer   
//*****************************************************************************************   
unsigned char ep0Xfer(void)   
{   
    usbstack.endpoint=0;   
    //----------------------------------------------------   
    // SETUP token with 8-byte request on endpoint 0   
    //----------------------------------------------------   
    usbstack.pid=PID_SETUP;   
    usbstack.wLen=8;   
    if (!usbXfer())
	{	printf("SETUP token error 0x%x \r",PID_SETUP);
		return FALSE;
	}
    usbstack.pid  = PID_IN;   
    //----------------------------------------------------   
    // IN or OUT data stage on endpoint 0      
    //----------------------------------------------------   
    usbstack.wLen=usbstack.setup.wLength;   
    if (usbstack.wLen)                                          // if there are data for transfer   
    {   
        if (usbstack.setup.bmRequest & 0x80)        // host-to-device : IN token   
        {   
            usbstack.pid  = PID_IN;    
               
            if(!usbXfer())
		{	printf("IN token endpoint 0 error 0x%x \r",usbstack.pid);
			return FALSE;
		}
            usbstack.pid  = PID_OUT;   
        }   
        else                                            // device-to-host : OUT token   
        {
            usbstack.pid  = PID_OUT;   
                   
            if(!usbXfer())
		{	printf("OUT token endpoint 0 error 0x%x \r",usbstack.pid);
			return FALSE;
		}
            usbstack.pid  = PID_IN;   
        }   
    }   
    DelayMs(10);   
    //----------------------------------------------------   
    // Status stage IN or OUT zero-length data packet   
    //----------------------------------------------------   
    usbstack.wLen=0;   
    if(!usbXfer())
	{	printf("IN/OUT token zero-lenght error 0x%x \r",usbstack.pid);
		return FALSE;
	}
    return TRUE;                                               
}   

unsigned char epBulkSend(unsigned char *pBuffer,unsigned int len)   
{
    usbstack.usbaddr=0x1;   
    usbstack.endpoint=usbstack.epbulkout;   
    usbstack.pid=PID_OUT;   
    usbstack.wPayload=64;   
    usbstack.wLen=len;   
    usbstack.buffer=pBuffer;   
    while(len>0)   
    {   
        if (len > usbstack.wPayload)   
            usbstack.wLen = usbstack.wPayload;   
        else                   
            usbstack.wLen = len; 
    	disable_interrupt();  
        while(!usbXfer()){
    		enable_interrupt();
    		puts("epBulkSend ERROR"); 
            return FALSE; 
        } 
    	enable_interrupt(); 
        len-=usbstack.wLen;   
        usbstack.buffer=usbstack.buffer+usbstack.wLen;   
    }   
    return TRUE;       
}   
   
unsigned char epBulkRcv(unsigned char *pBuffer,unsigned int len)   
{   
    usbstack.usbaddr=0x1;   
    usbstack.endpoint=usbstack.epbulkin;   
    usbstack.pid=PID_IN;   
    usbstack.wPayload=64;   
    usbstack.wLen=len;   
    usbstack.buffer=pBuffer;
    if(usbstack.wLen)   
    {    
    	disable_interrupt();  
        while(!usbXfer()){
    		enable_interrupt(); 
            return FALSE; 
        } 
    	enable_interrupt();    
    }   
    return TRUE;   
}   
   
//*****************************************************************************************   
// Set Device Address :    
//*****************************************************************************************   
unsigned char SetAddress(unsigned char addr)   
{   
    usbstack.usbaddr=0;   
    usbstack.setup.bmRequest=0;   
    usbstack.setup.bRequest=SET_ADDRESS;   
    usbstack.setup.wValue=addr;   
    usbstack.setup.wIndex=0;   
    usbstack.setup.wLength=0;   
    return ep0Xfer();   
}   

//*****************************************************************************************   
// Set Device Configuration :    
//*****************************************************************************************   
unsigned char Set_Configuration(void)   
{   
    usbstack.setup.bmRequest=0;   
    usbstack.setup.bRequest=SET_CONFIG;   
    usbstack.setup.wIndex=0;   
    usbstack.setup.wLength=0;   
    usbstack.buffer=NULL;   
    return ep0Xfer();   
}

//*****************************************************************************************   
// Get Device Descriptor : Device, Configuration, String   
//*****************************************************************************************   
unsigned char GetDesc(void)   
{    
    usbstack.setup.bmRequest=0x80;   
    usbstack.setup.bRequest=GET_DESCRIPTOR;   
    usbstack.setup.wValue=WordSwap(usbstack.setup.wValue);     
    usbstack.wPayload=uDev.wPayLoad[0];   
    return ep0Xfer();   
}   
   
//*****************************************************************************************   
// USB Device Enumeration Process   
// Support 1 confguration and interface #0 and alternate setting #0 only   
// Support up to 1 control endpoint + 4 data endpoint only   
//*****************************************************************************************   
unsigned char EnumUsbDev(BYTE usbaddr)   
{
    unsigned char i;                                    // always reset USB transfer address    
    unsigned char uAddr = 0;                            // for enumeration to Address #0   
    unsigned char epLen;

    //------------------------------------------------   
    // Reset only Slave device attached directly   
    //------------------------------------------------   
    uDev.wPayLoad[0] = 64;  // default 64-byte payload of Endpoint 0, address #0   
    if(usbaddr == 1)        // bus reset for the device attached to SL811HS only   
        USBReset();     // that will always have the USB address = 0x01 (for a hub)   
//        DelayMs(25);

    //------------------------------------------------   
    // Get USB Device Descriptors on EP0 & Addr 0   
    // with default 64-byte payload   
    //------------------------------------------------   

    usbstack.usbaddr=uAddr;   
    usbstack.setup.wValue=DEVICE;   
    usbstack.setup.wIndex=0;   
    usbstack.setup.wLength=18;   
    usbstack.buffer=DBUF;   

    if (!GetDesc())         // and determine the wPayload size   
        return FALSE;                               // get correct wPayload of Endpoint 0   
    uDev.wPayLoad[0]=DBUF[7];   // on current non-zero USB address   
    //------------------------------------------------   
    // Set Slave USB Device Address   
    //------------------------------------------------
	printf("Set Slave USB Device Address 0x%x  \r",usbaddr);
    if (!SetAddress(usbaddr))                       // set to specific USB address   
        return FALSE;                               //   
    uAddr = usbaddr;                                // transfer using this new address   

    //------------------------------------------------   
    // Get USB Device Descriptors on EP0 & Addr X   
    //------------------------------------------------   

    usbstack.usbaddr=uAddr;    
    usbstack.setup.wLength=DBUF[0];    
    usbstack.setup.wValue=DEVICE;   
    usbstack.setup.wIndex=0;       
    usbstack.buffer=DBUF;      
    if (!GetDesc())        
        return FALSE;                               // For this current device:   
    uDev.iMfg  = DBUF[14];
    uDev.iPdt  = DBUF[15];
	uDev.wVID=DBUF[8]+(DBUF[9]<<8);
	uDev.wPID=DBUF[10]+(DBUF[11]<<8);
	printf("VID 0x%x, PID 0x%x MNFG 0x%x PROD 0x%x \r",uDev.wVID,uDev.wPID,uDev.iMfg,uDev.iPdt);

    //------------------------------------------------   
    // Get Slave USB Configuration Descriptors   
    //------------------------------------------------   

    usbstack.usbaddr=uAddr;   
    usbstack.setup.wValue=CONFIGURATION;   
    usbstack.setup.wIndex=0;   
    usbstack.setup.wLength=64;   
    usbstack.buffer=DBUF;      
    if (!GetDesc())
        return FALSE;      

    uDev.bClass     = DBUF[9+5];    // update to class type   
    uDev.bNumOfEPs = (DBUF[9+4] <= MAX_EP) ? DBUF[9+4] : MAX_EP;   
    if(uDev.bClass==8) //mass storage device   
        bFlags.bits.bMassDevice=TRUE;   
    //------------------------------------------------   
    // Set configuration (except for HUB device)   
    //------------------------------------------------   
    usbstack.usbaddr=uAddr;   
    usbstack.setup.wValue=DEVICE;   
					// enumerating a FS/LS non-hub device   
        if (!Set_Configuration())	// connected directly to SL811HS   
                return FALSE;   

    //------------------------------------------------   
    // For each slave endpoints, get its attributes   
    // Excluding endpoint0, only data endpoints   
    //------------------------------------------------   

    epLen = 0;   
    for (i=1; i<=uDev.bNumOfEPs; i++)                // For each data endpoint   
    {
        uDev.bEPAddr[i]     = DBUF[9 + 9 + epLen+2];    // Ep address and direction                
        uDev.bAttr[i]       = DBUF[9 + 9 + epLen+3];    // Attribute of Endpoint               
        uDev.wPayLoad[i]    = LSwapINT16(DBUF[9 + 9 + epLen+4],DBUF[9 + 9 + epLen+5]);  // Payload of Endpoint   
        uDev.bInterval[i]   = DBUF[9 + 9 + epLen+6];    // Polling interval   
        uDev.bData1[i] = 0;                     // init data toggle   
	printf("EndPoint 0x%x, attr = 0x%x, \npkt_size = 0x%x, interval = 0x%x \r",uDev.bEPAddr[i],uDev.bAttr[i],uDev.wPayLoad[i],uDev.bInterval[i]);
        epLen += 7;   
        //////////////////////////////   
        if(uDev.bAttr[i]==0x2)     //bulk transfer   
        {
            if(uDev.bEPAddr[i]&0x80)
                usbstack.epbulkin=uDev.bEPAddr[i];
            else   
                usbstack.epbulkout=uDev.bEPAddr[i];
        }
        //////////////////////////////   
    }
	printf("epbulkin = 0x%x epbulkout = 0x%x \r",usbstack.epbulkin,usbstack.epbulkout);
    return TRUE;   
}
