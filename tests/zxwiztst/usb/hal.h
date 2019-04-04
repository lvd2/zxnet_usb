
///////////////////////////hal.h////////////////////////// 

#define SL811H_ADDR	0x80ab//	0x08000 
#define SL811H_DATA	0x7fab//	0x08001 
 

void USBReset(void);
int sl811_init_my(void);
extern unsigned char SL811Read(unsigned char a); 
void SL811Write(unsigned char a, unsigned char d); 
extern void SL811BufRead(unsigned char addr, unsigned char *s, unsigned char c); 
void SL811BufWrite(unsigned char addr, unsigned char *s, unsigned char c); 
unsigned short WordSwap(unsigned short input);
unsigned long SwapINT32(unsigned long dData); 
 
unsigned int LSwapINT16(unsigned short dData1,unsigned short dData2); 
unsigned long LSwapINT32(unsigned long dData1,unsigned long dData2,unsigned long dData3,unsigned long dData4); 
unsigned long SwapINT32(unsigned long dData);


#define TRUE   		1 
#define FALSE  		0 
 
#define xdata
#define bdata
#define idata
#define EZUSB_Delay DelayMs
//#define UARTBUF_LENGTH  16384//520// 
#define BUFFER_LENGTH	1024//520 // 
#define MAX_READ_LENGTH 16384//1024// 
#define MAX_WRITE_LENGTH 2048//1024// 
// Disable all interrupts 
#define DISABLE_INTERRUPTS		EA = 0 
// Enable all interrupts 
#define ENABLE_INTERRUPTS		EA = 1 
 
typedef unsigned char       INT8; 
typedef unsigned short      INT16; 
typedef unsigned long       INT32; 
 
typedef union __FLAGS 
{ 
	struct _FLAGS 
	{ 
		unsigned char   SLAVE_IS_ATTACHED	; 
		unsigned char   SLAVE_REMOVED		; 
		unsigned char  	SLAVE_FOUND		;	// Slave USB device found 
		unsigned char  	SLAVE_ENUMERATED	;	// slave USB device enumeration done 
		unsigned char   SLAVE_ONLINE		; 
		unsigned char 	TIMEOUT_ERR		;				// timeout error during data endpoint transfer 
		unsigned char	DATA_STOP		;					// device unplugged during data transfer 
		unsigned char 	bData1			; 
		unsigned char 	bUartInDone		;		 
		unsigned char 	bMassDevice		; 
		unsigned char	FULL_SPEED		;
		unsigned char	BULK_OUT_DONE		;
		unsigned char	DATA_INPROCESS		;
	} bits; 
	 
} FLAGS;
