/**
 * \file    main.c
 * W5300 example applications for ATMEGA128 of AVR
 * This code is optimized at AVR-GCC 4.3.3(20090313)
 * 
 * This file shows how to use  SOCKET_API_FUNC "WIZnet SOCKET APIs" and 
 * how to implement your internet application with  W5300.\n
 * 
 * \author MidnightCow
 * \date 15/05/2008
 * \version 1.1.0
 *
 *
 * Revision History :
 * ----------  -------  -----------  ----------------------------
 * Date        Version  Author       Description
 * ----------  -------  -----------  ----------------------------
 * 24/03/2008  1.0.0    MidnightCow  Release with W5300 launching
 * ----------  -------  -----------  ----------------------------
 * 01/05/2008  1.0.1    MidnightCow  Modify 'w5300.c'. Refer M_01052008.
 * ----------  -------  -----------  ----------------------------
 * 15/05/2008  1.1.0    MidnightCow  Refer M_15052008.
 *                                   Modify 'w5300.c', 'w5300.h' and 'socket.c'. 
 * ----------  -------  -----------  ----------------------------
 * 15/03/2012  1.2.1    Dongeun      Solution of ARP problem
 *                                   Modify 'w5300.c', 'w5300.h' and 'socket.c'.
 * ----------  -------  -----------  ----------------------------
 * 12/07/2012  1.2.2    Dongeun      Solution of ARP problem
 *                                   Modify 'socket.c'.
 * ----------  -------  -----------  ----------------------------
 *
 *
 * The Main() includes the example program of  W5300 such as 
 * - loopback_tcps() \n
 * - loopback_tcpc() \n
 * - loopback_udp()
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "socket.h"
#include "types.h"
#include "uart.h"

void initMCU(void);				        // Initialize MCU

/* Definition of example code */
void     loopback_tcps(SOCKET s, uint16 port, uint8* buf,uint16 mode);
void     loopback_tcpc(SOCKET s, uint8* addr, uint16 port, uint8* buf,uint16 mode);
void     loopback_udp(SOCKET s, uint16 port, uint8* buf, uint16 mode);


uint8 IP[4]          = {192,168,1,86};			// IP Address
uint8 Gateway[4]     = {192,168,1,1};			// Gateway Address
uint8 Subnet [4]     = {255,255,255,0};			// Subnet Address
uint8 MAC[6]         = {0x00,0x08,0xDC,0x11,0x22,0x86};	// MAC Address
uint8 ServerIP[4]    = {192,168,1,202};                	// "TCP SERVER" IP address for loopback_tcpc()

/*
 * It executes example program such as loopback_tcps(), loopback_tcpc(), and loopback_udp().
 */ 
int main(void)
{
	unsigned long dd = 100000;
	uint8 SN[4];						// for check subnet mask

	uint8 tx_mem_conf[8] = {8,8,8,8,8,8,8,8};		// for setting TMSR regsiter
	uint8 rx_mem_conf[8] = {8,8,8,8,8,8,8,8};		// for setting RMSR regsiter
	uint8 *data_buf = (uint8 *)0x2000;			// buffer for loopack data

	initUART();						// initiate serial port	

	initMCU();						// initiate MCU

	/* initiate W5300 */
	iinchip_init(); 
	setIMR(1 << INT5);
	printf("\r\nIMR = 0x%04x\r\n",getIMR());
	printf("EIMSK = 0x%04x\r\n",EIMSK);

	while(dd--);

	/* allocate internal TX/RX Memory of W5300 */
	if(!sysinit(tx_mem_conf,rx_mem_conf))           
	{
		printf("MEMORY CONFIG ERR.\r\n");
		while(1);
	}

	setMR(getMR()| MR_FS);				// If Little-endian, set MR_FS.
	setSHAR(MAC);					// set source hardware address

     #ifdef __DEF_IINCHIP_PPP__
	if(pppinit((uint8*)"test01", 6, (uint8*)"pppoe1000", 9)!=1)
	{
		printf("PPPoE fail.\r\n");
		while(1);
	}
	close(0);
     #else
	/* configure network information */
	setGAR(Gateway);				// set gateway IP address
	setSUBR(Subnet);
	setSIPR(IP);					// set source IP address
     #endif

	/* verify network information */
	getSHAR(MAC);					// get source hardware address 
	getGAR(Gateway);				// get gateway IP address      
	getSUBR(SN);					// get subnet mask address     
	getSIPR(IP);					// get source IP address       

   
	printf("SHAR : %02x:%02x:%02x:%02x:%02x:%02x\r\n",MAC[0],MAC[1],MAC[2],MAC[3],MAC[4],MAC[5]);
	printf("GWR  : %d.%d.%d.%d\r\n",Gateway[0],Gateway[1],Gateway[2],Gateway[3]);
	printf("SUBR : %d.%d.%d.%d\r\n",SN[0],SN[1],SN[2],SN[3]);
	printf("SIPR : %d.%d.%d.%d\r\n",IP[0],IP[1],IP[2],IP[3]);

	while(1)
	{
		loopback_tcps(0,5000,data_buf,0);
	//      loopback_tcps(1,5001,data_buf,0);
	//      loopback_tcps(2,5002,data_buf,0);
	//      loopback_tcps(3,5003,data_buf,0);
	//      loopback_tcps(4,5004,data_buf,0);
	//      loopback_tcps(5,5005,data_buf,0);
		loopback_tcpc(6,ServerIP, 5006,data_buf,0);
		loopback_udp(7,3000,data_buf,0);
	}

	#ifdef __DEF_IINCHIP_PPP__
	{
		uint8 ppp_mac[6];
		getPDHAR(ppp_mac);
		pppterm(ppp_mac, getPSIDR());
	}
	#endif

	while(1);
}

void initMCU(void)
{
	EICRA = 0x00;			// External Interrupt Control Register A clear
	EICRB = 0x02;			// External Interrupt Control Register B clear // edge 
	EIMSK = (1 << INT5);		// External Interrupt Mask Register : 0x10
	EIFR = 0xFF;			// External Interrupt Flag Register all clear

	DDRE &= ~(1 << INT5);		// Set PE Direction 
	PORTE |= (1 << INT5);		// Set PE Default value
	sei();				// enable interrupts

	MCUCR |= (1 << SRE) | (1 << SRW10);
	XMCRA |= (1 << SRL2) | (1 << SRW00);
}

/**
 * "TCP SERVER" loopback program.
 */ 
void     loopback_tcps(SOCKET s, uint16 port, uint8* buf, uint16 mode)
{
	uint32 len;

	switch(getSn_SSR(s))						// check SOCKET status
	{	
		// ------------	
		case SOCK_ESTABLISHED:					// ESTABLISHED?
			if(getSn_IR(s) & Sn_IR_CON)			// check Sn_IR_CON bit
			{
				printf("%d : Connect OK\r\n",s);
				setSn_IR(s,Sn_IR_CON);			// clear Sn_IR_CON
			}
			if((len=getSn_RX_RSR(s)) > 0)			// check the size of received data
			{
				len = recv(s,buf,len);			// recv
				if(len !=send(s,buf,len))		// send
				{
					printf("%d : Send Fail.len=%ld\r\n",s,len);
				}
			}
			break;
		
		// ---------------
		case SOCK_CLOSE_WAIT:					// PASSIVE CLOSED
			disconnect(s);					// disconnect 
			break;
		
		// --------------
		case SOCK_CLOSED:					// CLOSED
			close(s);					// close the SOCKET
			socket(s,Sn_MR_TCP,port,mode);			// open the SOCKET  
			break;
		
		// ---------------
		case SOCK_INIT:						// The SOCKET opened with TCP mode
			listen(s);					// listen to any connection request from "TCP CLIENT"
			printf("%d : LOOPBACK_TCPS(%d) Started.\r\n",s,port);
			break;
		
		default:
			break;
	}
}

/**
 * "TCP CLIENT" loopback program.
 */ 
void     loopback_tcpc(SOCKET s, uint8* addr, uint16 port, uint8* buf, uint16 mode)
{
	uint32 len;
	static uint16 any_port = 6000;

	switch(getSn_SSR(s))					// check SOCKET status
	{ 
		// ------------
		case SOCK_ESTABLISHED:				// ESTABLISHED?
			if(getSn_IR(s) & Sn_IR_CON)		// check Sn_IR_CON bit
			{
				printf("%d : Connect OK\r\n",s);
				setSn_IR(s,Sn_IR_CON);		// clear Sn_IR_CON
			}
			if((len=getSn_RX_RSR(s)) > 0)		// check the size of received data
			{
				len = recv(s,buf,len);		// recv
				if(len !=send(s,buf,len))	// send
				{
					printf("%d : Send Fail.len=%ld\r\n",s,len);
				}
			}
			break;
		
		// ---------------
		case SOCK_CLOSE_WAIT:				// PASSIVE CLOSED
			disconnect(s);				// disconnect 
			break;
		
		// --------------
		case SOCK_CLOSED:				// CLOSED
			close(s);				// close the SOCKET
			socket(s,Sn_MR_TCP,any_port++,mode);	// open the SOCKET with TCP mode and any source port number
			break;
		
		// ------------------------------
		case SOCK_INIT:					// The SOCKET opened with TCP mode
		connect(s, addr, port);				// Try to connect to "TCP SERVER"
			printf("%d : LOOPBACK_TCPC(%d.%d.%d.%d:%d) Started.\r\n",s,addr[0],addr[1],addr[2],addr[3],port);
			break;
		
		default:
			break;
	}
}

/**
 * UDP loopback program.
 */ 
void     loopback_udp(SOCKET s, uint16 port, uint8* buf, uint16 mode)
{
	uint32 len;
	uint8 destip[4];
	uint16 destport;

	switch(getSn_SSR(s))
	{
									// -------------------------------
		case SOCK_UDP:						// 
		if((len=getSn_RX_RSR(s)) > 0)                   	// check the size of received data
		{
			len = recvfrom(s,buf,len,destip,&destport);	// receive data from a destination
			if(len !=sendto(s,buf,len,destip,destport))	// send the data to the destination
			{
				printf("%d : Sendto Fail.len=%ld,",s,len);
				printf("%d.%d.%d.%d(%d)\r\n",destip[0],destip[1],destip[2],destip[3],destport);
			}
		}
		break;
									// -----------------
		case SOCK_CLOSED:					// CLOSED
			close(s);                                       // close the SOCKET
			socket(s,Sn_MR_UDP,port,mode);                  // open the SOCKET with UDP mode
			break;

		default:
			break;
		}
}
