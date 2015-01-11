#if !defined(USBFS_COMMANDINTERFACE_H) 
#define USBFS_COMMANDINTERFACE_H

#include "kitprog.h"

#define CLEAR_OP			0x00
#define CLEAR_BUSY			0x00
#define SET_BUSY			0x01
#define USB_OK_ACK			0x01
#define READ_CMD			0x01
#define WRITE_CMD			0x02
#define PROGRAM_CMD			0x07

extern uint8 pendingOp; 
extern uint8 fTcBusy; 
extern uint8 pifData[2];
extern uint8 pifBuf[64];

extern void ProgramInit(void);
extern void VendorCmd(void);

#endif  /* USBFS_COMMANDINTERFACE_H */