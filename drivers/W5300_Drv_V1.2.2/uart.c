#include <avr/io.h>

#include <stdio.h>
#include "uart.h"

static int USART0_Transmit( char data )
{
	/* Wait for empty transmit buffer */
//	while ( !(UCSR0A & (1<<UDRE0)) );
	loop_until_bit_is_set(UCSR0A, UDRE0);

	/* Start transmittion */
	UDR0 = data;

	return 0;
}

static int USART0_Receive( void )
{
	char c;
	/* Wait for incomming data */
//	while ( !(UCSR0A & (1<<RXC0)) );
	loop_until_bit_is_set(UCSR0A, RXC0);

	c = UDR0;
	/* Echo */
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	if(c == '\r') {
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = '\n';
	}
	/* Return the data */
	return c;
}

void initUART(void){
	/* set baud rate */
	UBRR0H = (unsigned char) (UART_BAUD_SELECT>>8);
	UBRR0L = (unsigned char) UART_BAUD_SELECT;

	/* Enable UART receiver and transmitter */
	UCSR0A = ( 1 << U2X0 );
	UCSR0B = ( ( 1 << RXEN0 ) | ( 1 << TXEN0 ) ); 

	fdevopen((void*)USART0_Transmit, (void*)USART0_Receive);
}
