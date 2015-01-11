#include "kitprog.h"

uint8 pendingOp;
uint8 fTcBusy;
uint8 pifData[2];
uint8 pifBuf[64];
uint8 progMode;

extern volatile T_USBFS_TD USBFS_currentTD;

// Initializes the variables for the programming operation.
void ProgramInit(void)
{
    pendingOp = CLEAR_OP;
    fTcBusy = CLEAR_BUSY;
}

// Entry point from the USBFS user module. Called inside the function 
// USBFS_HandleVendorRqst() defined inside USBFS_vnd.c
//
// Global Variables: 
// pifBuf	: Bufer to send data to through the USB control endpoint
// pifData	: Buffer to store commands received through the USB control endpoint
// progMode	: Next programming operation to be performed
void VendorCmd(void)
{
	/* Varaible to store ACK to be send to the USB packet */
    uint8 progRes;
	/* Varaible to store the data which indicates whether the received command is a programming command */
	uint8 progCmd;
	progRes = USB_OK_ACK;
    
	/* If executing a previous instruction, initiate a zero length USB transfer */
	if (fTcBusy == SET_BUSY)
	{
		USBFS_currentTD.count = 0;
	} 
    else 
    {
		/* Extract the count of bytes received. */
		USBFS_currentTD.count = ((uint16)CY_GET_REG8(USBFS_EP0_DR6_PTR)) + ((uint16)CY_GET_REG8(USBFS_EP0_DR7_PTR)<<8);

		/* If a read operation, set the data pointer to the databuffer address. */
        if (CY_GET_REG8(USBFS_EP0_DR1_PTR) == READ_CMD)
        {
            USBFS_currentTD.pData = &pifBuf[0];
        } 
		/* If a write operation, extract the commands from the USB control endpoint registers */
        else if (CY_GET_REG8(USBFS_EP0_DR1_PTR) == WRITE_CMD)
        {
            /* Acknowledge the command */
			USBFS_currentTD.pData = &progRes;
			
			/* progCmd checks if the command is a programing command. */
            progCmd  = CY_GET_REG8(USBFS_EP0_DR2_PTR);
			
			/* progMode stores the programming operation type */
            progMode = CY_GET_REG8(USBFS_EP0_DR3_PTR);
			
			/* Array pifData stores the values such as the aquire mode (Reset/Powercycle), device to be acquired (PSoC 3/4/5LP), 
			* number of retries before declaring the programming operation to have failed, and the protocol type (SWD). The values
			* depend on the current value of the variable progMode */
            pifData[0] = CY_GET_REG8(USBFS_EP0_DR4_PTR);
            pifData[1] = CY_GET_REG8(USBFS_EP0_DR5_PTR);
            
			/* If the command is a programming command */
            if (progCmd == PROGRAM_CMD)
            {	
                if (progMode != CMD_BUFFER)
                {
                    /* Pass the command to pending operation */
					pendingOp = progMode;
					
                    /* Set the busy flag */
                    fTcBusy = SET_BUSY;				
                }
            }
        } 
        else 
        {
			/* do nothing */
        }
	
    }
	return;
}
