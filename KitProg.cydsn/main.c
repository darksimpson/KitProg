#include <project.h>

#include "ProgComm.h"
#include "CtrlComm.h"
#include "SWD.h"

int main()
{
  // Enable global interrupts
  CyGlobalIntEnable;

  // Start USBFS component
  USBFS_Start(0, USBFS_DWR_VDDD_OPERATION);

  // Main loop
  while (1)
  {
    SWDHalt(); // Free target SWD bus (in case we was reenumerated suddenly)

    while (!USBFS_GetConfiguration()); // Wait for Device to enumerate

    // Enable USB out end points
    USBFS_EnableOutEP(SWD_OUT_EP);
    USBFS_EnableOutEP(HOST_OUT_EP);

    do
    {
      // Handle the config and status communication via USB HID interface
      CtrlComm();
      // Handle the target programming based on variables set by control endpoint vendor requests
      ProgComm();
    }
    while (!USBFS_IsConfigurationChanged());
  }
}
