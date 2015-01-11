#if !defined(KITPROG_H) 
#define KITPROG_H 

#include "device.h" // Autogen by PSoC Creator

#include "commandprocessor.h"
#include "USBFS_commandinterface.h"
#include "USBFS_HID_interface.h"
#include "swd.h"

#define FALSE                       0x00
#define TRUE                        0x01

#define SWD_IN_EP                  	0x01
#define SWD_OUT_EP                 	0x02
#define HOST_IN_EP					0x03
#define HOST_OUT_EP					0x04

extern volatile uint8 USBResetDetected;
extern uint8 checkUSBActivityFlag;

extern uint8 bOutEndpointData[64];
extern uint8 bInEndpointData[64];
extern uint8 bOutPacketIndex;
extern uint8 bInPacketIndex;

#endif /* KITPROG_H */
