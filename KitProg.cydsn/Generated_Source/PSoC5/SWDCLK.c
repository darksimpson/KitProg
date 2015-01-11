/*******************************************************************************
* File Name: SWDCLK.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "SWDCLK.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 SWDCLK__PORT == 15 && ((SWDCLK__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: SWDCLK_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void SWDCLK_Write(uint8 value) 
{
    uint8 staticBits = (SWDCLK_DR & (uint8)(~SWDCLK_MASK));
    SWDCLK_DR = staticBits | ((uint8)(value << SWDCLK_SHIFT) & SWDCLK_MASK);
}


/*******************************************************************************
* Function Name: SWDCLK_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  SWDCLK_DM_STRONG     Strong Drive 
*  SWDCLK_DM_OD_HI      Open Drain, Drives High 
*  SWDCLK_DM_OD_LO      Open Drain, Drives Low 
*  SWDCLK_DM_RES_UP     Resistive Pull Up 
*  SWDCLK_DM_RES_DWN    Resistive Pull Down 
*  SWDCLK_DM_RES_UPDWN  Resistive Pull Up/Down 
*  SWDCLK_DM_DIG_HIZ    High Impedance Digital 
*  SWDCLK_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void SWDCLK_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(SWDCLK_0, mode);
}


/*******************************************************************************
* Function Name: SWDCLK_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro SWDCLK_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 SWDCLK_Read(void) 
{
    return (SWDCLK_PS & SWDCLK_MASK) >> SWDCLK_SHIFT;
}


/*******************************************************************************
* Function Name: SWDCLK_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 SWDCLK_ReadDataReg(void) 
{
    return (SWDCLK_DR & SWDCLK_MASK) >> SWDCLK_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(SWDCLK_INTSTAT) 

    /*******************************************************************************
    * Function Name: SWDCLK_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 SWDCLK_ClearInterrupt(void) 
    {
        return (SWDCLK_INTSTAT & SWDCLK_MASK) >> SWDCLK_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
