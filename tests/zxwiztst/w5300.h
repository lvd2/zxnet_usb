//#define TXFIFO_SIZE 2048
#define WIZ_BASE_ADDR 0X0000

//extern unsigned int MTYPER;  
//extern unsigned char WIZ_REG_MTYPER1;  

#define WMR ((unsigned char*)(WIZ_BASE_ADDR+0x0000))
#define WMRX ((unsigned int*)(WIZ_BASE_ADDR+0x0000))
#define SHAR ((unsigned char*)(WIZ_BASE_ADDR+0x0008))
//#define SHAR ((unsigned char*)(WIZ_BASE_ADDR+0x0008))
#define GAR ((unsigned char*)(WIZ_BASE_ADDR+0x0010))
#define SUBR ((unsigned char*)(WIZ_BASE_ADDR+0x0014))
#define SIPR ((unsigned char*)(WIZ_BASE_ADDR+0x0018))
#define TMSR ((unsigned char*)(WIZ_BASE_ADDR+0x0020))
#define RMSR ((unsigned char*)(WIZ_BASE_ADDR+0x0028))
#define MTYPER ((unsigned char*)(WIZ_BASE_ADDR+0x0030))
#define WID ((unsigned char*)(WIZ_BASE_ADDR+0x00fe))


#define S_MR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0200))
#define S_CR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0202))
#define S_IMR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0204))
#define S_IR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0206))
#define S_SSR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0208))
#define S_PORTR(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x020a))
#define S_DHAR(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x020c))
#define S_DHAR2(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x020E))
#define S_DHAR4(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x0210))
#define S_DPORTR(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x0212))
#define S_DIPR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0214))
#define S_DIPR1(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0215))
#define S_DIPR2(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0216))
#define S_DIPR3(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0217))
#define S_MSSR(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x0218))
#define S_KPALVTR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x021A))
#define S_PROTOR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x021B))
#define S_TOSR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x021C))
#define S_TTLR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x021E))
#define S_TX_WRSR(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x0222))
#define S_TX_FSR(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x0226))
#define S_RX_RSR(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x022A))
#define S_FRAGR(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x022C))
#define S_TX(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x022E))
#define S_TX1(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x022f))
#define S_RX(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0230))
#define S_RX1(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x40+0x0231))
#define S_RX_LEN(soc_) ((unsigned int*)(WIZ_BASE_ADDR+soc_*0x40+0x0230))
#define S_RX_FAKE(soc_) ((unsigned char*)(WIZ_BASE_ADDR+soc_*0x0200+0x3000))
/************************************/
/* The bit of MR regsiter defintion */
/************************************/
#define MR_DBW             (1 << 15)            /**< Data bus width bit of MR. */
#define MR_MPF             (1 << 14)            /**< Mac layer pause frame bit of MR. */
#define MR_WDF(X)          ((X & 0x07) << 11)   /**< Write data fetch time bit of  MR. */
#define MR_RDH             (1 << 10)            /**< Read data hold time bit of MR. */
#define MR_FS              (1 << 8)             /**< FIFO swap bit of MR. */
#define MR_RST             (1 << 7)             /**< S/W reset bit of MR. */
#define MR_MT              (1 << 5)             /**< Memory test bit of MR. */
#define MR_PB              (1 << 4)             /**< Ping block bit of MR. */
#define MR_PPPoE           (1 << 3)             /**< PPPoE bit of MR. */
#define MR_DBS             (1 << 2)             /**< Data bus swap of MR. */
#define MR_IND             (1 << 0)             /**< Indirect mode bit of MR. */


/***************************************/ 
/* The bit of Sn_MR regsiter defintion */ 
/***************************************/ 
#define Sn_MR_ALIGN        (1 << 8)             /**< Alignment bit of Sn_MR. */
#define Sn_MR_MULTI        (1 << 7)             /**< Multicasting bit of Sn_MR. */
#define Sn_MR_MF           (1 << 6)             /**< MAC filter bit of Sn_MR. */
#define Sn_MR_IGMPv        (1 << 5)             /**< IGMP version bit of Sn_MR. */
#define Sn_MR_ND           (1 << 5)             /**< No delayed ack bit of Sn_MR. */
#define Sn_MR_CLOSE        0x00                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_TCP          0x01                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_UDP          0x02                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_IPRAW        0x03                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_MACRAW       0x04                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_PPPoE        0x05                 /**< Protocol bits of Sn_MR. */

/******************************/ 
/* The values of CR defintion */ 
/******************************/
#define Sn_CR_OPEN         0x01                 /**< OPEN command value of Sn_CR. */
#define Sn_CR_LISTEN       0x02                 /**< LISTEN command value of Sn_CR. */
#define Sn_CR_CONNECT      0x04                 /**< CONNECT command value of Sn_CR. */
#define Sn_CR_DISCON       0x08                 /**< DISCONNECT command value of Sn_CR. */
#define Sn_CR_CLOSE        0x10                 /**< CLOSE command value of Sn_CR. */
#define Sn_CR_SEND         0x20                 /**< SEND command value of Sn_CR. */
#define Sn_CR_SEND_MAC     0x21                 /**< SEND_MAC command value of Sn_CR. */ 
#define Sn_CR_SEND_KEEP    0x22                 /**< SEND_KEEP command value of Sn_CR */
#define Sn_CR_RECV         0x40                 /**< RECV command value of Sn_CR */
#define Sn_CR_PCON         0x23                 /**< PCON command value of Sn_CR */
#define Sn_CR_PDISCON      0x24                 /**< PDISCON command value of Sn_CR */ 
#define Sn_CR_PCR          0x25                 /**< PCR command value of Sn_CR */
#define Sn_CR_PCN          0x26                 /**< PCN command value of Sn_CR */
#define Sn_CR_PCJ          0x27                 /**< PCJ command value of Sn_CR */

/**********************************/ 
/* The values of Sn_SSR defintion */ 
/**********************************/
#define SOCK_CLOSED        0x00                 /**< SOCKETn is released */
#define SOCK_ARP           0x01                 /**< ARP-request is transmitted in order to acquire destination hardware address. */
#define SOCK_INIT          0x13                 /**< SOCKETn is open as TCP mode. */
#define SOCK_LISTEN        0x14                 /**< SOCKETn operates as "TCP SERVER" and waits for connection-request (SYN packet) from "TCP CLIENT". */
#define SOCK_SYNSENT       0x15                 /**< Connect-request(SYN packet) is transmitted to "TCP SERVER". */
#define SOCK_SYNRECV       0x16                 /**< Connect-request(SYN packet) is received from "TCP CLIENT". */
#define SOCK_ESTABLISHED   0x17                 /**< TCP connection is established. */
#define SOCK_FIN_WAIT      0x18                 /**< SOCKETn is closing. */
#define SOCK_CLOSING       0x1A                 /**< SOCKETn is closing. */
#define SOCK_TIME_WAIT     0x1B                 /**< SOCKETn is closing. */
#define SOCK_CLOSE_WAIT    0x1C                 /**< Disconnect-request(FIN packet) is received from the peer. */
#define SOCK_LAST_ACK      0x1D                 /**< SOCKETn is closing. */
#define SOCK_UDP           0x22                 /**< SOCKETn is open as UDP mode. */
#define SOCK_IPRAW         0x32                 /**< SOCKETn is open as IPRAW mode. */
#define SOCK_MACRAW        0x42                 /**< SOCKET0 is open as MACRAW mode. */
#define SOCK_PPPoE         0x5F                 /**< SOCKET0 is open as PPPoE mode. */

/*********************************/ 
/* The values of Sn_IR defintion */ 
/*********************************/
#define Sn_IR_PRECV        0x80                 /**< PPP receive bit of Sn_IR */
#define Sn_IR_PFAIL        0x40                 /**< PPP fail bit of Sn_IR */
#define Sn_IR_PNEXT        0x20                 /**< PPP next phase bit of Sn_IR */
#define Sn_IR_SENDOK       0x10                 /**< Send OK bit of Sn_IR */
#define Sn_IR_TIMEOUT      0x08                 /**< Timout bit of Sn_IR */
#define Sn_IR_RECV         0x04                 /**< Receive bit of Sn_IR */
#define Sn_IR_DISCON       0x02                 /**< Disconnect bit of Sn_IR */
#define Sn_IR_CON          0x01                 /**< Connect bit of Sn_IR */

extern unsigned int rx_rd;
	
#define WIZ_RD_BUF(soc_,buf_,len_) {\
	unsigned int i=(len_+1)>>1;\
	unsigned char* buf__=buf_;	\
	while (i){					\
		i--;					\
		*(buf__++)=*S_RX1(soc_);	\
		*(buf__++)=*S_RX(soc_);}}	

#define WIZ_READ_BUF(soc_,buf_,len_) WIZ_RD_BUF(soc_,buf_,len_);	\
	*S_CR(soc_) = Sn_CR_RECV;	\
	while(*S_CR(soc_));
			
#define WIZ_WRITE_BUF(soc_,buf_,len_) {unsigned int i=(len_);unsigned char* buf__=(buf_);\
	if(i&1)i++;\
	while (i){\
		i--;i--;\
		*S_TX1(soc_) = *buf__;\
		buf__++; \
		*S_TX(soc_) = *buf__;\
		buf__++;}}\
	*S_TX_WRSR(soc_) = (len_);\
	*S_CR(soc_) = Sn_CR_SEND;\
	while(*S_CR(soc_));

#define WIZ_SOCKET(soc_,smode_,port_) do{	\
    	*S_CR(soc_) =Sn_CR_CLOSE;\
		*S_MR(soc_) = smode_; /* sets TCP mode */\
		*S_PORTR(soc_) = port_; /* sets source port number */\
		*S_CR(soc_) = Sn_CR_OPEN; /* sets OPEN command */\
		while(*S_CR(soc_));\
	}while(*S_SSR(soc_) != SOCK_INIT)
	
#define WIZ_LISTEN(soc_) *S_CR(soc_) = Sn_CR_LISTEN

#define WIZ_DISCONNECT(soc_) {*S_CR(soc_) = Sn_CR_DISCON;while(*S_CR(soc_));}

#define WIZ_CLOSE(soc_) *S_CR(soc_) = Sn_CR_CLOSE

#define PACK_SIZE(soc_,len_) {((unsigned char*)(&len_))[1]=*S_RX1(soc_);\
	((unsigned char*)(&len_))[0]=*S_RX(soc_);}

#define WIZ_SYS_INIT(tx_,rx_) 				\
	output(0x82ab,input(0x82ab)&(0xff^0x08));	\
	memcpy(TMSR,tx_,8);				\
	memcpy(RMSR,rx_,8);				\
	output(0x82ab,input(0x82ab)|0x08);	
	
#define WIZ_IP_INIT(shar_,gar_,subr_,sipr_) 		\
	output(0x82ab,input(0x82ab)&(0xff^0x08));	\
	memcpy(SHAR,shar_,6);				\
	memcpy(GAR,gar_,4);					\
	memcpy(SUBR,subr_,4);				\
	memcpy(SIPR,sipr_,4);				\
	output(0x82ab,input(0x82ab)|0x08);	
	
#define WIZ_SOC_IPSET(soc_,ip_,port_)	\
	S_DIPR(soc_)[1]=ip_[0];  /* set TCP SERVER IP address*/\
	S_DIPR(soc_)[0]=ip_[1];  /* set TCP SERVER IP address*/\
	S_DIPR(soc_)[3]=ip_[2];  /* set TCP SERVER IP address*/\
	S_DIPR(soc_)[2]=ip_[3];  /* set TCP SERVER IP address*/\
	*S_DPORTR(soc_) = port_  /* set TCP SERVER listen port number*/
	
		
#define WIZ_CONNECT(soc_,ip_,port_)	\
	WIZ_SOC_IPSET(soc_,ip_,port_);\
	*S_CR(soc_) = Sn_CR_CONNECT;\
	while(*S_CR(soc_));
	