#ifndef	__UART__
#define	__UART__

#define CLK_CPU				8000000L	/* 8Mhz */
#define UART_BAUD_RATE		57600L		/* 57600 baud */
#define UART_BAUD_SELECT 	(CLK_CPU/(UART_BAUD_RATE*8L)-1L)

void initUART(void);

#endif
