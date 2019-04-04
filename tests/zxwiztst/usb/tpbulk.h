
/////////////////////////////////// 
//#include "RBCCMD.H" 
/////////////////////////////////// 
 
unsigned char EnumMassDev(void); 
unsigned char SPC_Inquiry(void); 
unsigned char SPC_RequestSense(void); 
unsigned char SPC_TestUnit(void); 
unsigned char SPC_LockMedia(void); 
unsigned char RBC_ReadCapacity(void); 
unsigned char RBC_Read(unsigned long lba,unsigned char len,unsigned char *pBuffer); 
unsigned char RBC_Write(unsigned long lba,unsigned char len,unsigned char *pBuffer);



#define RBC_CMD_READ10					0x28 
#define RBC_CMD_READCAPACITY				0x25 
#define RBC_CMD_WRITE10					0x2A 
// RBC commands 
#define SPC_CMD_INQUIRY					0x12 
#define SPC_CMD_PRVENTALLOWMEDIUMREMOVAL		0x1E 
#define SPC_CMD_REQUESTSENSE				0x03 
#define SPC_CMD_TESTUNITREADY				0x00 
//////////////////////////////////////////////////////////////////////////////////// 
// Command Descriptor Block 
//      _RBC : Reduced Block Command 
//      _SPC : SPC-2 SCSI primary Command - 2 
//////////////////////////////////////////////////////////////////////////////////// 
 