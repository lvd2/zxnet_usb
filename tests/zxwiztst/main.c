
#include <string.h>
#include <stdio.h>
#include <intrz80.h>
#include "w5300.h" 
#include "zxevo_io.h"
#include "mylib\mylib.h"
#include "usb\hal.h"    
#include "usb\sl811.h"    
#include "usb\tpbulk.h"   
  
extern char LSTMHZ[3][4];  
unsigned char RX_BUF[2*1024];  
unsigned char mhz=2; 
unsigned char editbuf[16]; 

const unsigned char mac[]={0x02,0x02,0x6A,0x6A,0x3B,0x3B};
const unsigned char wb[]={60,4,0,0,0,0,0,0};
unsigned int iinchip_source_port=25;
const unsigned char strip[]="%u.%u.%u.%u";
unsigned char wait_con; 
unsigned int len; 
extern unsigned char xdata DBUF[BUFFER_LENGTH];
extern FLAGS bdata bFlags; 

void main11(void);
void myint(void);

unsigned char rndini(unsigned char start){
	static unsigned char h,l;
	if(start) h=l=0;
	else {
		h=h+0x77;
		l=h+(l>>1);
	}
	return l;
}

unsigned char rnd(unsigned char start){
	static unsigned char tab[256];
	static unsigned char l;
	if(start){
		unsigned int i;
		l=0;
		rndini(1);
        for (i = 0; i < 256; i++)
            tab[i] = rndini(0);
        return 0;
	}
	else {
        unsigned char b,a;
        l++;
        b=tab[l];
        a = l + 31;
        b+=tab[a];
        a+=24;
        tab[a]=b;
        return b;
	}
}

void broadcast(void){	
	static unsigned char tmp[5];
	iinchip_source_port++;  
	printf("Open socket in UDP mode");
    do{
    	putchar('.');
    	*S_CR(0) =Sn_CR_CLOSE;
    	while(*S_CR(0));
		*S_MR(0) = Sn_MR_UDP; /* sets TCP mode */
		*S_PORTR(0) = iinchip_source_port; /* sets source port number */
		*S_CR(0) = Sn_CR_OPEN; /* sets OPEN command */
		while(*S_CR(0));
	}while(*S_SSR(0) != SOCK_UDP);
	puts(" OK");
	
	tmp[0]=GAR[0]|(~SUBR[0]);  /* set TCP SERVER IP address*/
	tmp[1]=GAR[1]|(~SUBR[1]);  /* set TCP SERVER IP address*/
	tmp[2]=GAR[2]|(~SUBR[2]);  /* set TCP SERVER IP address*/
	tmp[3]=GAR[3]|(~SUBR[3]);  /* set TCP SERVER IP address*/
	S_DIPR(0)[0]=tmp[0];  /* set TCP SERVER IP address*/
	S_DIPR(0)[1]=tmp[1];  /* set TCP SERVER IP address*/
	S_DIPR(0)[2]=tmp[2];  /* set TCP SERVER IP address*/
	S_DIPR(0)[3]=tmp[3];  /* set TCP SERVER IP address*/
	*S_DPORTR(0) = 4000;  /* set TCP SERVER listen port number*/
	
    printf("Send to ");
    printf(strip,tmp[1],tmp[0],tmp[3],tmp[2]); 
	*S_TX1(0) = 0;
	*S_TX(0) = 0;
	*S_TX_WRSR(0) = 2;
	*S_CR(0) = Sn_CR_SEND;
	while(*S_CR(0));
	puts(" OK");
    printf("Close socket ");
	*S_CR(0) = Sn_CR_CLOSE;
	while(*S_CR(0));
	puts(" OK\rPress any key");
}

void update_main_menu(void){
	unsigned char * ptr=RX_BUF;
	ptr+=sprintf(ptr,"   ZXUSBNet test    \r\r[0]z80 %sMHz\r[1]GATEWAY:%u.%u.%u.%u\r",
		LSTMHZ[mhz],GAR[1],GAR[0],GAR[3],GAR[2]);
	ptr+=sprintf(ptr,"[2]MASK:%u.%u.%u.%u\r",SUBR[1],SUBR[0],SUBR[3],SUBR[2]);
	ptr+=sprintf(ptr,"[3]IP:%u.%u.%u.%u\r",SIPR[1],SIPR[0],SIPR[3],SIPR[2]);
	sprintf(ptr,"[4]Send broadcast\r\
[5]Receive packet\r\
[6]Визнет мемтест\r\
[7]sl811 тест\r\
[8]Miniterminal(no return)\r\
[9]О программе");
}

void reconnect(void){
	printf("Open socket in TCP mode");
	iinchip_source_port++; 
    WIZ_SOCKET(0, Sn_MR_TCP, 23);
	puts(" OK");	
	printf("Set listening mode ");
	*S_CR(0) = Sn_CR_LISTEN;
	while(*S_CR(0));
	puts(" OK");	    
    puts("Please connect to port 23");
}

unsigned int receive(void){
	unsigned int len;
	unsigned char * ptr=RX_BUF;
	if(!(*S_RX_RSR(0))) return 0;
	while(*S_RX_RSR(0)!=*S_RX_RSR(0));
	PACK_SIZE(0,len);
    disable_interrupt();
	output(0x82ab,input(0x82ab)&(0xff^0x08));	
	{
		unsigned char i=(len+1)>>9;
		while(i){
			memcpy(ptr,S_RX_FAKE(0),512);
			i--;
			ptr+=512;
		}
	}
	{
		unsigned int j=(len+1)&0x01fe;
		memcpy(ptr,S_RX_FAKE(0),j);
	}
	output(0x82ab,input(0x82ab)|0x08);
	*S_CR(0) = Sn_CR_RECV;	
	while(*S_CR(0));	
    enable_interrupt();
	return len;
}

void recv_p(void){
	unsigned int ps = 0;
	wait_con=1;
	reconnect();
	rnd(1);
	while(1){
	
		unsigned char stat;
		stat=*S_SSR(0);
		if((stat==SOCK_CLOSE_WAIT)||(stat==SOCK_CLOSED)) {
		if(len=receive()){
			unsigned char * ptr=RX_BUF;
			//unsigned char i=*RX_BUF;
			//printf("Length %u. check:\r",len);
			while(len){
				unsigned char i=rnd(0);
				if(*ptr!=i)
					printf("bad 0x%04x: 0x%04x 0x%04x\r",ptr-RX_BUF,i,*ptr);
				ptr++;
				len--;
			}
			continue;
		}
			break;
		}
		else if(stat!=SOCK_ESTABLISHED) {
			continue;
		}
		if(wait_con){
			wait_con=0;
			printf("Connected :");
    		printf(strip,S_DIPR(0)[1],S_DIPR(0)[0],S_DIPR(0)[3],S_DIPR(0)[2]); 
    		puts("\r Please send packets");
    		continue;
		}
		if(len=receive()){
			//unsigned char * ptr=RX_BUF;
			//unsigned char b=0;
			ps++;
			//output8(0xfe,((unsigned char)ps)&0x07);
			/*
			while(len){
				unsigned char i=rnd(0);
				if(*ptr!=i){
					printf("bad pack %lu 0x%04x: 0x%02x 0x%02x\r",ps,ptr-RX_BUF,i,*ptr);
					b++;
					if(b==10)getchar();
				}
				ptr++;
				len--;
			}*/
			printf("\r%x",ps);
			continue;
		}
	}
	puts("press any key");	
}

void miniterm(void){	
	wait_con=1;  
	reconnect();
	while(1){
		unsigned char stat;
		stat=*S_SSR(0);
		if((stat==SOCK_CLOSE_WAIT)||(stat==SOCK_CLOSED)) {
			puts("Socked closed");
			wait_con=1;
			reconnect();
			continue;
		}
		else if(stat!=SOCK_ESTABLISHED) {
			continue;
		}
		else if(wait_con){
			wait_con=0;
			printf("Connected :");
    		printf(strip,S_DIPR(0)[1],S_DIPR(0)[0],S_DIPR(0)[3],S_DIPR(0)[2]); 
    		puts("\r Please type the text");
    		continue;
		}
		else if(len=receive()){
			*(RX_BUF+len)=0;
			printf(RX_BUF);
			continue;
		}
/*		if(fls&FLS_STR) {
			//kbd_pars();
			msg_send(kbd_buf.buf);
			fls=fls&(FLS_STR^0xff)|FLS_KBD;
			continue;
		}
		if(fls&FLS_CURS) flash_cursor();
		if(fls&FLS_KBD) kbd_read();
*/
	}  
}

unsigned int blok;

void memtest(unsigned char wp){
	unsigned char hmr=0;
	rnd(1);
	blok=0;
	output(0x83ab,0);
	DelayMs(100);
	output(0x83ab,0x10);
	DelayMs(500);
	
	//output(0x82ab,0x0c);
	
	if(wp)
	{
		output(0x82ab,0x50);
		output(0x01ab,0x80);
	}
	else	
	{
		output(0x82ab,0x4c);
		*WMR=0x80;
	}
	
	DelayMs(500);
	//printf("Введите старший байт MR, в формате FF: ");
    //scanf("%.2X",hmr); 
	if(wp){
		output(0x00ab,hmr);
		output(0x01ab,0x20);
	}
	else{
		*(WMR+1)=hmr;
		*WMR=0x20;
	}
	if(wp){
		output(0x81ab,0x08);	
		output(0x01ab,Sn_MR_TCP);
		output(0x03ab,Sn_CR_OPEN);
		while(input(0x09ab) != SOCK_INIT);
	}
	else
	{
		*S_MR(0)=Sn_MR_TCP;
		*S_CR(0)=Sn_CR_OPEN;
		while(*S_SSR(0) != SOCK_INIT);
	}
	while(1){
		unsigned int i=8192;
		printf("\nБлок(8KB): %u    ",blok);
		rnd(1);
		while(i)
		{
			i--;
			i--;
			if(wp)
			{
				//output(0x82ab,0x90);	
				output(0x30ab,rnd(0));
				//output(0x82ab,0x90);
				output(0x31ab,rnd(0));
			}
			else
			{
				*S_TX1(0) = rnd(0);
				*S_TX(0)  = rnd(0);
			}
		}
		i=8192;
		rnd(1);
		while(i)
		{
			unsigned char trueb=rnd(0);
			unsigned char readb1;
			unsigned char readb0;
			if(wp)
			{
				//output(0x82ab,0x90);	
				readb1=input(0x30ab);
				//output(0x82ab,0x90);
				readb0=input(0x31ab);
			}
			else
			{
				readb1=*S_TX1(0);
				readb0=*S_TX(0);
			}
			i--;
			if(trueb != readb1){
				printf("bad 0x%04x: 0x%02x 0x%02x\r",8192-i,trueb,readb1);
				if(getkey())while(!getkey());
			}
			trueb=rnd(0);
			i--;
			if(trueb != readb0){
				printf("bad 0x%04x: 0x%02x 0x%02x\r",8192-i,trueb,readb0);
				if(getkey())while(!getkey());				
			}
		}
		blok++;
	}
}

void usb_test(void){
	static unsigned char temp;
    SL811_RESET();
    USBReset();
	temp=SL811Read(cDATASet)>>4;
	switch(temp)
	{	case 0:puts("SL811H");break;
		case 1:puts("SL811HS rev 1.2");break;
		case 2:puts("SL811HS rev 1.5");break;
		default:puts("Unknown rev");
	}
    sl811_init_my();

	printf("SL811 init Ok \n");
	//DirStartCluster=0;   
	for(temp=0;temp<64;temp++)   
		DBUF[temp]=0; 
		
		if(bFlags.bits.SLAVE_FOUND)
		{
			bFlags.bits.SLAVE_FOUND=FALSE;
			DelayMs(25);
			if(EnumUsbDev(1))				// enumerate USB device, assign USB address = #1
			{
				bFlags.bits.SLAVE_ENUMERATED = TRUE;	// Set slave USB device enumerated flag
				puts("\rSlave enumerated Ok");
			}
		}
		if(bFlags.bits.SLAVE_REMOVED)
		{
			bFlags.bits.SLAVE_REMOVED=FALSE;
			bFlags.bits.SLAVE_ENUMERATED = FALSE;
			bFlags.bits.SLAVE_IS_ATTACHED = FALSE;
			puts("\rSlave removed");
		}
		if(bFlags.bits.bMassDevice)
		{
			bFlags.bits.bMassDevice=FALSE;
			temp=EnumMassDev();
			if(temp==TRUE)
			{
				bFlags.bits.SLAVE_IS_ATTACHED = TRUE;
				puts("\rSlave is attached");
			}
			else
			{
				printf("\rMass device, ERROR = 0x%x \n",temp);
				bFlags.bits.SLAVE_IS_ATTACHED = FALSE;
				temp=SL811Read(IntStatus);
				if(!(temp & 0x40)) bFlags.bits.SLAVE_FOUND=TRUE;
				printf("\rSlave is NOT attached, ERROR = 0x%x \n",temp);
            }
		}
	if(bFlags.bits.SLAVE_IS_ATTACHED){
		unsigned char b=0;
		blok=1;
		puts("Press any key, for read USB test");
		while(!getkey());
		
		puts("Write 128KB");
		rnd(1);
		while(blok<0x100){
			unsigned char * ptr=RX_BUF;
			len=2048;
			while(len){
				*ptr=rnd(0);
				ptr++;
				len--;
			}
			RBC_Write(blok,4,RX_BUF);
			blok+=4;
			printf("\nSectors: %u",blok);
		}
		blok=1;
		
		puts("\rRead test:");
		rnd(1);
		while(1){
			unsigned char * ptr=RX_BUF;
			printf("\nSectors: %u    ",blok);
			RBC_Read(blok&0xff,4,RX_BUF);
			//RBC_Write(blok|0x4000,1,RX_BUF);
			len=2048;
			while(len){
				unsigned char i=rnd(0);
				if(*ptr!=i){
					printf("\rbad 0x%04x: 0x%02x 0x%02x",ptr-RX_BUF,i,*ptr);
					b++;
					if(getkey())while(!getkey());
				}
				ptr++;
				len--;
			}
			blok+=4;
			if((blok&0xff)==1){
				rnd(1);
			}
		}
		printf("\nBads: %u",b);
	}	
}

void main(void){ 
    wiz_reset();
    WIZ_SYS_INIT(wb,wb);
    memcpy(SHAR,mac,6); 
	my_im2_init(myint);
	enable_interrupt();
	main11();
	
	while(1);
}

