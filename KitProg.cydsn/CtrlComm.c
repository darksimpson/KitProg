#include "CtrlComm.h"

// Macros used in power supply monitoring
#define POWER_SUPPLIED				0x01
#define ACK_WITHOUT_POWER			0x01
#define ACK_WITH_POWER 				0x05

// USB-I2C protocol
#define CONTROL_START               0x02

// Extended commands
#define PACKET_VALID				0x80
#define POWER_SETTING				0x80
#define KITPROG_VERSION				0x81

void SendToUSB(void);

uint8 bOutEndpointData[64];
uint8 bInEndpointData[64];
uint8 bOutPacketIndex;
uint8 bInPacketIndex;
uint8 bHostInterface;

// Send data to USB IN endpoint
void SendToUSB(void)
{
    // Wait for the previous IN endpoint trasaction to complete
	while(USBFS_bGetEPState(HOST_IN_EP) != USBFS_IN_BUFFER_EMPTY);
	
    USBFS_LoadInEP(HOST_IN_EP, bInEndpointData, 64);
    USBFS_EnableOutEP(HOST_OUT_EP);
}

// Handles the communication between the PC and the PSoC 5LP via the HID interface.
void CtrlComm(void)
{
    uint8 bControl;
    //uint8 bLength;
    uint16 wCount;

    // Check for data from the USB host
	// Request structure: (1 Control byte) + (1 Length byte) + (1 Command byte) + (61 Data bytes)
    if (USBFS_bGetEPAckState(HOST_OUT_EP))
    {
        // A new USB packet has been received. Retrieve it.
        wCount = USBFS_wGetEPCount(HOST_OUT_EP);
        USBFS_ReadOutEP(HOST_OUT_EP, bOutEndpointData, (wCount & 0x00FF));

        // Reset request position counter
        bOutPacketIndex = 0;
		// Reset response position counter and first data byte
		bInPacketIndex = 0;
		bInEndpointData[bInPacketIndex] = 0;
        
		// Retrieve control byte
        bControl = bOutEndpointData[bOutPacketIndex++];
		
		// Skip length byte
        bOutPacketIndex++;

        // Retrieve the command byte if a proper start is detected.
        if ((bControl & CONTROL_START) && (bOutEndpointData[bOutPacketIndex] & PACKET_VALID))
        {
            // Acknowledge the extended command and indicate presence of power. 
			// Bit 2 of the Ack indicates presence of power.
           	bInEndpointData[bInPacketIndex] = ACK_WITH_POWER;  
           	bInPacketIndex++;
                
            // Parse the extended command
			if (bOutEndpointData[bOutPacketIndex] == POWER_SETTING)
            {
                // Command 0x80 = Power and configuration
                if (bControl & 0x01) // Get
                {
                    // Get power configuration
                    bInEndpointData[bInPacketIndex++] = POWER_SUPPLIED;
					
					// Not used by KitProg, Used for I2C speed update by TTBridge
                    bInEndpointData[bInPacketIndex++] = 0x00; // For I2C speed 400
					
					// Return the voltage value to the Programmer GUI
					bInEndpointData[bInPacketIndex++] = 0xE4; // Lo
                    bInEndpointData[bInPacketIndex++] = 0x0C; // Hi
					
					// Not used by KitProg, used for VAUX measurement by TTBridge
                    bInEndpointData[bInPacketIndex++] = 0x00;
                    bInEndpointData[bInPacketIndex++] = 0x00;
                }
            }
            else if (bOutEndpointData[bOutPacketIndex] == KITPROG_VERSION)
            {
                // Command 0x81 = Get KitProg version info
                bInEndpointData[bInPacketIndex++] = 0x00; // Revision (can be 0x00 to 0x3F)
                bInEndpointData[bInPacketIndex++] = USBFS_DEVICE0_DESCR[13]; // Minor ver from USB descriptor (f.e. 0x0A)
                bInEndpointData[bInPacketIndex] = USBFS_DEVICE0_DESCR[12]; // Major ver rom USB descriptor (f.e. 0x02)
            }
        }
		// Return the acknowledgements and data to the USB
        SendToUSB();
    }
}