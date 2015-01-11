#include "kitprog.h"

volatile uint8 bUSBResetDetected = FALSE;

#define USB_SUSP_MON_MS 4
uint8 checkUSBActivityFlag = FALSE;
uint8 flagUSBSuspMon = FALSE; // Disabled by default
uint8 cntUSBSuspMon = USB_SUSP_MON_MS;

void SystemSleep(void);
void SystemWakeup(void);
void HandleUSBSuspend(void);

// Called every 1ms from SysTick interrupt
void SysTickCallback0()
{
	// Set USB activity check flag
	if (flagUSBSuspMon) {
		if (cntUSBSuspMon == 0) {
		  checkUSBActivityFlag = TRUE;
		  cntUSBSuspMon = USB_SUSP_MON_MS;
		} else {
		  cntUSBSuspMon--;
		}
	}
}

void UsbSuspMon_Disable()
{
	cntUSBSuspMon = USB_SUSP_MON_MS;
	flagUSBSuspMon = FALSE;
}

void UsbSuspMon_Enable()
{
	cntUSBSuspMon = USB_SUSP_MON_MS;
	flagUSBSuspMon = TRUE;
}

int main()
{
	// Init and start SysTick, set our callback
	CySysTickStart();
	CySysTickSetCallback(0, SysTickCallback0);
	
	// Enable global Interrupts
    CyGlobalIntEnable;
	
	// Initializes the variables for the programming operation.
    ProgramInit();
	
	// Initializes SWD interface
    SWDInit();
	
	// Flag to determine if the USB is reset
	bUSBResetDetected = FALSE;

	while(1)
    {
		//while(!USBFS_VBusPresent())
		//{
			// Wait till powered via USB
		//}
		
		// If the USB is not reset.
        if (bUSBResetDetected == FALSE)
        {
            // Start USB operation at VDDD specified in the DWR
			USBFS_Start(0, USBFS_DWR_VDDD_OPERATION);
			
			// Enables handling of the USBFS Suspend Activity
			UsbSuspMon_Enable();
		}
		
		while(!USBFS_GetConfiguration())
		{
			// Handles USB Suspend event
			HandleUSBSuspend();	
			
			// Wait for Device to enumerate	
		}
		
		// Indicate successful enumeration.
        //Pin_StatusLED_Write(0);
		
		// Enable USB out end points
		USBFS_EnableOutEP(SWD_OUT_EP);
		USBFS_EnableOutEP(HOST_OUT_EP);
		
		// 'bUSBResetDetected' flag is set to false to indicate that the USB is 
		// connected and configured. This flag will be set inside USBFS_episr.c if 
		// the USB Bus Reset is detected
		bUSBResetDetected = FALSE;	
		
		// Loop is executed as long as USB is not reset or the Vbus is not removed
		while(/*USBFS_VBusPresent() &&*/ bUSBResetDetected == FALSE)
        {	
			// Handles the communication between PSoC Programmer GUI and the PSoC 5LP
			// via USB HID interface. Also handles the USB-I2C bridge functionality.
            HostComm();
			
			// Decides the commands-to-be-sent-to-the-SWD lines/configuration based on
			// variables set by VendorCmd(). Defined in commandProcessor.c
			SWDComm();
			
			// Handles the USB Suspend Event
			HandleUSBSuspend();		
			
			if (USBFS_IsConfigurationChanged())
			{
				bUSBResetDetected = TRUE;
			}
        }
		
		SWDHalt();
		
		if (bUSBResetDetected == FALSE)
        {
            // If the Vbus is removed, stop the component and try to re-enumerate. 
			// Set to true inside USBFS_episr.c [CY_ISR(USBFS_BUS_RESET_ISR)]
			USBFS_Stop();
			
			// Disables handling of the USBFS Suspend Activity
			UsbSuspMon_Disable();
        }
    }	
}

// Prepares the PSoC 5LP device for sleep and puts the device in low power mode.
void SystemSleep(void)
{
	CySysTickStop(); // Completely stop SysTick, we do not need it in sleep mode
	UsbSuspMon_Disable(); // Disable as a safety
	SWDHalt();
	CyPmSaveClocks();
	CyPmSleep(PM_SLEEP_TIME_NONE, PM_SLEEP_SRC_PICU);
}

// Restores the configurations of PSoC 5LP after resuming from sleep.
void SystemWakeup(void)
{
	CyPmRestoreClocks();
	ProgramInit();
	SWDInit();
	UsbSuspMon_Enable(); // Enable previously disabled
	CySysTickStart(); // Restart SysTick
	bUSBResetDetected = TRUE;
}

// Handles the USB suspend Event
void HandleUSBSuspend(void)
{
	if (checkUSBActivityFlag == TRUE)
	{	
		if (USBFS_CheckActivity() == 0)
		{
			USBFS_Suspend();
			SystemSleep();
			SystemWakeup();
			USBFS_Resume();
		}
		else
		{
			// Do nothing
		}
		checkUSBActivityFlag = FALSE;
	}
}
