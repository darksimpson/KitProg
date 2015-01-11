#if !defined(COMMAND_PROCESSOR_H) 
#define COMMAND_PROCESSOR_H 

//#define SET 					0x01
//#define NOT_SET 				0x00

// SWD Commands
#define CMD_BUFFER				0x00
#define CMD_STATUS 				0x01
#define CMD_RESET				0x04
#define CMD_SET_PROTOCOL		0x40
#define CMD_SWD_SYNC    		0x41
#define CMD_SWD_ACQUIRE 		0x42
#define CMD_SWD_SPECIAL 		0x43

// Power Status Commands
#define POWER_SUPPLIED_INVERSE	0x00
#define POWER_SUPPLIED			0x01
#define POWER_DETECTED			0x02
#define ACK_POWER_NOT_DETECTED	0x00
#define ACK_POWER_DETECTED		0x40

void SWDComm(void);

#endif /* COMMAND_PROCESSOR_H */
