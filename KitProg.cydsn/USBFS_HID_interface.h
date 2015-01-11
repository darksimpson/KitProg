#if !defined(USB_HID_INTERFACE_H) 
#define USB_HID_INTERFACE_H 

#include "kitprog.h"

// Macros used in power supply monitoring
#define POWER_SUPPLIED				0x01
#define ACK_WITHOUT_POWER			0x01
#define ACK_WITH_POWER 				0x05

// USB-I2C protocol
#define CONTROL_START               0x02

// Extended commands
#define PACKET_VALID				0x80
#define GET_POWER_SETTING			0x80
#define GET_KITPROG_VERSION			0x81
#define RESET_KITPROG				0x82

extern uint8 bOutEndpointData[64];
extern uint8 bInEndpointData[64];
extern uint8 bBulkInEndpointData[513];
extern uint8 bOutPacketIndex;
extern uint8 bInPacketIndex;

void SendToUSB(void);

extern void HostComm(void);

#endif /* USB_HID_INTERFACE_H */
