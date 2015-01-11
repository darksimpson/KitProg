#include "kitprog.h"

//uint8 SWDFlag = TRUE;

uint8 bufOut[68]; 
uint8 bufIn[384]; 
int16 bufOutOffset;
int16 bufInOffset;
int16 bufOutLen;
uint8 fIsShortPkt = FALSE;

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
        fIsShortPkt = TRUE;
	}
    else
	{
        fIsShortPkt = FALSE;
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
void SWDComm(void)
{
	// Global variable pendingOp is set inside the function VendorCmd() defined in USBFS_commandInterface.c
	if (pendingOp)
    {
        switch(pendingOp)
        {
			// Command = 0x01, get status (connected, powered) of the KitProg
			/*case CMD_STATUS:
					// Power is detected by the programmer
					pifBuf[3] = ACK_POWER_DETECTED;
					// Power is supplied by the programmer. Works on inverse logic. 0x00 for power supplied and 0x01 for not supplied.
					pifBuf[4] = POWER_SUPPLIED_INVERSE;
					break;*/
			
			// Command = 0x04, reset the PSoC
            case CMD_RESET:
					SWDResetPSoC();
                    break;	
			
			// Command = 0x40, set the protocol		
			/*case CMD_SET_PROTOCOL:
					// pifData[0] is for Protocol
                    if (pifData[0] == 0x01) // If Protocol is SWD (0x01) set the flag. Otherwise ignore the command.
                    {
                        SWDFlag = TRUE;
					} 
					else 
					{
                        SWDFlag = FALSE;
					}
					break;*/
					
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
						if (SWDAcquirePSoC4(pifData[1]) == TRUE) 
							pifBuf[0] = 0x01; // ACQUIRE_PASS
						else
							pifBuf[0] = 0x00; // ACQUIRE_FAIL
					}
					else // Unknown acquire method
					{
						pifBuf[0] = 0x00; // ACQUIRE_FAIL
					}
                    break;
					
            // Command = 0x43, special command
            /*case CMD_SWD_SPECIAL:
					// pifData[0]: Action (?)
                    if (pifData[0] == 0x00) // Action: Reset the SWD bus
					{
                        SWDResetBus();
                    } 
                    break;*/

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
		// If protocol is SWD, then use FSM algorithm for 64-bytes packets on SWD-commands
        //if (SWDFlag == TRUE) 
        //{
            // Handles the swd commands. Defined in swd.c
			HandleSWDRequest();
        //}
    }
}
