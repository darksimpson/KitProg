#include "ProgComm.h"

#include "SWD.h"

// Vendor control endpoint operations
#define CLEAR_OP			0x00
#define CLEAR_BUSY			0x00
#define SET_BUSY			0x01
#define USB_OK_ACK			0x01
#define READ_CMD			0x01
#define WRITE_CMD			0x02
#define PROGRAM_CMD			0x07

// SWD commands
#define CMD_BUFFER			0x00
#define CMD_STATUS 			0x01
#define CMD_RESET			0x04
#define CMD_SWD_SYNC    	0x41
#define CMD_SWD_ACQUIRE 	0x42

// Power status
#define POWER_SUPPLIED_INVERSE	0x00
#define ACK_POWER_DETECTED		0x40

uint8 pendingOp = CLEAR_OP;
uint8 fTcBusy = CLEAR_BUSY;

uint8 pifData[2];
uint8 pifBuf[64];
uint8 progMode;

uint8 bufOut[68]; 
uint8 bufIn[384]; 
int16 bufOutOffset;
int16 bufInOffset;
int16 bufOutLen;
uint8 fIsShortPkt = false;

extern volatile T_USBFS_TD USBFS_currentTD;

// Reads the SWD request issued by the PSoC programmer and retrieves 
// the data packets from USB lines for the programming operation
// Parameters:
//  bufOut: OUT EP buffer
//  bufOutOffset: offset from the starting of bufOut to the location of data
//  bufOutLen: Number of data bytes stored in the buffer
void ReceiveSWDRequest(uint8 *bufOut, int16 *bufOutOffset, int16 *bufOutLen)
{
	int16 size = USBFS_GetEPCount(SWD_OUT_EP);
	*bufOutLen = *bufOutOffset + size;
	USBFS_ReadOutEP(SWD_OUT_EP, bufOut + *bufOutOffset, size);
	USBFS_EnableOutEP(SWD_OUT_EP);

    if (size < 64)
	{
        fIsShortPkt = true;
	}
    else
	{
        fIsShortPkt = false;
	}
	return;
}

// Determines the type of request issued by the Programmer and invokes read or write
// functions until the operations with the received data packet (upto 64 bytes) is complete
// Parameters:
//  bufOut: OUT EP buffer
//  bufIn: IN EP buffer
//  bufOutOffset: offset from the starting of bufOut to the location of data
//  bufInOffset: offset from the starting of bufIn to the location of data
//  bufOutLen: Number of data bytes stored in the OUT EP buffer
void ProcessSWDRequest(uint8 *bufOut, uint8 *bufIn, int16 *bufOutOffset, int16 *bufInOffset, int16 *bufOutLen)
{
	int16 i = 0;
	int16 k = 0;
	
	for(i = 0; i < *bufOutLen; )
	{
        if ((bufOut[i] & 0x04) == 0x04) // Process Read SWD command
        {
            // Execute Read transaction
            ReadSWD(bufOut + i, bufIn + *bufInOffset);
            
            *bufInOffset = *bufInOffset + 5;
            i = i + 1;
            if (*bufOutLen == i) *bufOutOffset = 0;
        }
        else // Process Write SWD command
        {
            int16 delTr = *bufOutLen - (i + 5);
			
            if (delTr < 0)
            {
                for(k = 0; k < (5 + delTr); k++)
                {
                    bufOut[k] = bufOut[i + k];
                }
            }
            else
            {
                // Execute Write transaction
                WriteSWD(bufOut + i, bufIn + *bufInOffset);
				
                *bufInOffset = *bufInOffset + 1;
            }
			
            if (delTr <= 0)
            {
                if (delTr == 0)
				{
                    *bufOutOffset = 0;
				}
                else
				{
                    *bufOutOffset = 5 + delTr;
				}
                break;
            }
            i = i + 5;
        }
	}
}

// Writes data to the USB IN endpoint
// Parameters:
//  bufIn: IN EP buffer
//  size: Number of bytes to be sent to the IN endpoint
void SWDLoadUsbIn(uint8 *bufIn, int16 size)
{
    while(USBFS_bGetEPState(SWD_IN_EP) != USBFS_IN_BUFFER_EMPTY)
	{
		// Wait till the previous IN endpoint transaction has completed
	}
    USBFS_LoadInEP(SWD_IN_EP, bufIn, size);
}

// Returns the result of SWD programming command request to the PSoC programmer via the USB lines
// Parameters:
//  bufIn: IN EP buffer
//  bufInSize: Number of bytes to be sent through the USB lines
void SendSWDResult(uint8 *bufIn, int16 *bufInSize)
{
    int16 i = 0;
    int16 offset = 0;
    int16 EPs = ((*bufInSize) / 64);

    *bufInSize = ((*bufInSize) % 64);
	
    for(i = 0; i < EPs; i++, offset+=64)
    {
		SWDLoadUsbIn(bufIn + offset, 64);
	}

    if (fIsShortPkt)
    {
        SWDLoadUsbIn(bufIn + offset, *bufInSize);
        *bufInSize = 0;
    }
    else
    {
        for(i = 0; i < *bufInSize; i++)
		{
            bufIn[i] = bufIn[offset + i];
		}
    }
	return;
}

// Handles the SWD programming requests
void HandleSWDRequest(void)
{
    ReceiveSWDRequest(bufOut, &bufOutOffset, &bufOutLen);
    ProcessSWDRequest(bufOut, bufIn, &bufOutOffset, &bufInOffset, &bufOutLen);
    SendSWDResult(bufIn, &bufInOffset);
}

// Checks the command received through the USB control endpoint and invokes apporpriate functions
void ProgComm(void)
{
	// Global variable pendingOp is set inside the function USBFS_HandleVendorRqst()
	if (pendingOp)
    {
        switch(pendingOp)
        {
			// Command = 0x01, get status (connected, powered) of the KitProg
			case CMD_STATUS:
					// Power is detected by the programmer. 0x00 for power not detected ack.
					pifBuf[3] = ACK_POWER_DETECTED;
					// Power is supplied by the programmer. Works on inverse logic. 0x00 for power supplied and 0x01 for not supplied.
					pifBuf[4] = POWER_SUPPLIED_INVERSE;
					break;
			
			// Command = 0x04, reset the PSoC
            case CMD_RESET:
				SWDResetPSoC();
                break;
					
            // Command = 0x41, synchronise data transfer of the data endpoints with control endpoint
            case CMD_SWD_SYNC: 
                bufOutOffset = 0;
                bufInOffset = 0;
                bufOutLen = 0;
                break;
					
            // Command = 0x42, acquire PSoC
            case CMD_SWD_ACQUIRE: 
				// (pifData[0] & 0x0F): Device type
				// ((pifData[0] & 0xF0) >> 4): Acquire mode: Reset (0x00) or Power cycle (?) 
				// pifData[1]: Number of retries to acquire PSoC
				
				if ((pifData[0] & 0x0F) == 0x00) // 0x0 for PSoC4, according to developers
				{
					if (SWDAcquirePSoC4(pifData[1]) == true) 
						pifBuf[0] = 0x01; // ACQUIRE_PASS
					else
						pifBuf[0] = 0x00; // ACQUIRE_FAIL
				}
				else // Unknown acquire method
				{
					pifBuf[0] = 0x00; // ACQUIRE_FAIL
				}
                break;

            default:
				// Ignore other commands for now
				break;
        }		

        pendingOp = CLEAR_OP;
        fTcBusy = CLEAR_BUSY;
    }
	
	// If a packet is received in the OUT endpoint
    if (USBFS_bGetEPAckState(SWD_OUT_EP))
    {
		HandleSWDRequest();
    }
}

// Vendor USB control request handler (caller from USB INTERRUPT!)
// !!! YOU MUST ENABLE CUSTOM VENDOR REQUEST HANDLING IN USBFS' ADVANCED PROPERTIES TAB !!!
// Global Variables: 
// pifBuf	: Bufer to send data to through the USB control endpoint
// pifData	: Buffer to store commands received through the USB control endpoint
// progMode	: Next programming operation to be performed
uint8 USBFS_HandleVendorRqst(void)
{
	// Varaible to store ACK to be send to the USB packet
    uint8 progRes = USB_OK_ACK; // OK by default
	// Varaible to store the data which indicates whether the received command is a programming command
	uint8 progCmd;
    
	// If executing a previous instruction, initiate a zero length USB transfer
	if (fTcBusy == SET_BUSY)
	{
		USBFS_currentTD.count = 0;
	} 
    else 
    {
		// Extract the count of bytes received
		USBFS_currentTD.count = ((uint16)CY_GET_REG8(USBFS_EP0_DR6_PTR)) + ((uint16)CY_GET_REG8(USBFS_EP0_DR7_PTR)<<8);

		// If read operation, set the data pointer to the databuffer address
        if (CY_GET_REG8(USBFS_EP0_DR1_PTR) == READ_CMD)
        {
            USBFS_currentTD.pData = &pifBuf[0];
        } 
		// If write operation, extract the command from the USB control endpoint register
        else if (CY_GET_REG8(USBFS_EP0_DR1_PTR) == WRITE_CMD)
        {
            // Acknowledge the command
			USBFS_currentTD.pData = &progRes;
			
			// progCmd checks if the command is a programing command
            progCmd  = CY_GET_REG8(USBFS_EP0_DR2_PTR);
			
			// progMode stores the programming operation type
            progMode = CY_GET_REG8(USBFS_EP0_DR3_PTR);
			
			// Array pifData stores the values such as the acquire mode (Reset/Powercycle), device to be acquired (PSoC 3/4/5LP), 
			// number of retries before declaring the programming operation to have failed, and the protocol type (SWD). The values
			// depend on the current value of the variable progMode
            pifData[0] = CY_GET_REG8(USBFS_EP0_DR4_PTR);
            pifData[1] = CY_GET_REG8(USBFS_EP0_DR5_PTR);
            
			// If the command is a programming command
            if (progCmd == PROGRAM_CMD)
            {	
                if (progMode != CMD_BUFFER)
                {
                    // Pass the command to pending operation
					pendingOp = progMode;
					
                    // Set the busy flag
                    fTcBusy = SET_BUSY;				
                }
            }
        } 
        else 
        {
			// Do nothing
        }
	
    }
	return(USBFS_InitControlRead());
}
