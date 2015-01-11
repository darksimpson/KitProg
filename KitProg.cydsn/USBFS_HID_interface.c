#include "kitprog.h"

void SendToUSB(void);

uint8 bOutEndpointData[64];
uint8 bInEndpointData[64];
uint8 bOutPacketIndex;
uint8 bInPacketIndex;
uint8 bHostInterface;

// Send data to USB IN endpoint
void SendToUSB(void)
{
    while(USBFS_bGetEPState(HOST_IN_EP) != USBFS_IN_BUFFER_EMPTY)
	{
		// Wait for the previous IN endpoint trasaction to complete */
	}
    USBFS_LoadInEP(HOST_IN_EP, bInEndpointData, 64);
    USBFS_EnableOutEP(HOST_OUT_EP);
}

// Handles the communication between the PC and the PSoC 5LP via the HID interface.
void HostComm(void)
{
    uint8 bControl;
    //uint8 bLength;
    uint16 wCount;

    // Check for data from the USB host
    if (USBFS_bGetEPAckState(HOST_OUT_EP))
    {
        // A new USB packet has been received. Retrieve it.
        wCount = USBFS_wGetEPCount(HOST_OUT_EP);
        USBFS_ReadOutEP(HOST_OUT_EP, bOutEndpointData, (wCount & 0x00FF));

        // Initialize variables
        bOutPacketIndex = 0;
        bInPacketIndex  = 0;
        
		// Retrieve the first two bytes of data received. 
		// These bytes are the control and length bytes respectively.
        bControl = bOutEndpointData[bOutPacketIndex];
        bOutPacketIndex++;
		
		// bLength less than or equal to 61. bLength is the length of data bytes
		// that will follow.(1 Control byte) + (1 Length byte) + (1 Command byte) +
		// (61 data bytes) = 64 (Max. transfer size)
        //bLength = bOutEndpointData[bOutPacketIndex] & 0x3F;
        bOutPacketIndex++;
        
        bInEndpointData[bInPacketIndex] = 0;

        // Retrieve the command byte if a proper start is detected.
        if ((bControl & CONTROL_START) && (bOutEndpointData[bOutPacketIndex] & PACKET_VALID))
        {
            // Acknowledge the extended command and indicate presence of power. 
			// Bit 2 of the Ack indicates presence of power.
           	bInEndpointData[bInPacketIndex] = ACK_WITH_POWER;  
           	bInPacketIndex++;
                
            // Parse the extended command
			if (bOutEndpointData[bOutPacketIndex] == GET_POWER_SETTING)
            {
                // Command 0x80 = GET_POWER_SETTING
                // Determine if set or get power
                if (bControl & 0x01)
                {
                    // Get power configuration
                    bInEndpointData[bInPacketIndex++] = POWER_SUPPLIED;
					
					// Not used by KitProg, Used for I2C speed update by TTBridge
                    bInEndpointData[bInPacketIndex++] = 0x00; // For I2C speed 400
					
					// Return the voltage value to the programmer GUI
					bInEndpointData[bInPacketIndex++] = 0x00; // Hi
                    bInEndpointData[bInPacketIndex++] = 0x00; // Lo
					
					// Not used by KitProg, used for VAUX measurement by TTBridge
                    bInEndpointData[bInPacketIndex++] = 0x00;
                    bInEndpointData[bInPacketIndex++] = 0x00;
                }
                else 
                {                        
                    // Set Power command for powercycle mode. Do nothing for now.
                }
            }
            else if (bOutEndpointData[bOutPacketIndex] == GET_KITPROG_VERSION)
            {
                // Command 0x81 = Get KitProg version info
				// Return 2.10 rev 0
                bInEndpointData[bInPacketIndex++] = 0x00; // Rev (can be 0x00 to 0x3F)
                bInEndpointData[bInPacketIndex++] = 0x0A; // Minor ver
                bInEndpointData[bInPacketIndex] = 0x02; // Major ver
            }				
            else if (bOutEndpointData[bOutPacketIndex] == RESET_KITPROG)
            {
                // Command 0x82 = Reset the KitProg
                CySoftwareReset();
            }
            else 
            {
                // Do nothing (ignore other commands)
            }
        }
        else
        {
            // Skip packet and do nothing (ignore)
            //bInPacketIndex = bInPacketIndex + bLength;
        }
        
		// Return the acknowledgements and data to the USB
        SendToUSB();  

    }
}