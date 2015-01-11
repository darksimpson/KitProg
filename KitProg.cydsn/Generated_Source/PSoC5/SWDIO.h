/*******************************************************************************
* File Name: SWDIO.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_SWDIO_H) /* Pins SWDIO_H */
#define CY_PINS_SWDIO_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SWDIO_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SWDIO__PORT == 15 && ((SWDIO__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    SWDIO_Write(uint8 value) ;
void    SWDIO_SetDriveMode(uint8 mode) ;
uint8   SWDIO_ReadDataReg(void) ;
uint8   SWDIO_Read(void) ;
uint8   SWDIO_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define SWDIO_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define SWDIO_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define SWDIO_DM_RES_UP          PIN_DM_RES_UP
#define SWDIO_DM_RES_DWN         PIN_DM_RES_DWN
#define SWDIO_DM_OD_LO           PIN_DM_OD_LO
#define SWDIO_DM_OD_HI           PIN_DM_OD_HI
#define SWDIO_DM_STRONG          PIN_DM_STRONG
#define SWDIO_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define SWDIO_MASK               SWDIO__MASK
#define SWDIO_SHIFT              SWDIO__SHIFT
#define SWDIO_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SWDIO_PS                     (* (reg8 *) SWDIO__PS)
/* Data Register */
#define SWDIO_DR                     (* (reg8 *) SWDIO__DR)
/* Port Number */
#define SWDIO_PRT_NUM                (* (reg8 *) SWDIO__PRT) 
/* Connect to Analog Globals */                                                  
#define SWDIO_AG                     (* (reg8 *) SWDIO__AG)                       
/* Analog MUX bux enable */
#define SWDIO_AMUX                   (* (reg8 *) SWDIO__AMUX) 
/* Bidirectional Enable */                                                        
#define SWDIO_BIE                    (* (reg8 *) SWDIO__BIE)
/* Bit-mask for Aliased Register Access */
#define SWDIO_BIT_MASK               (* (reg8 *) SWDIO__BIT_MASK)
/* Bypass Enable */
#define SWDIO_BYP                    (* (reg8 *) SWDIO__BYP)
/* Port wide control signals */                                                   
#define SWDIO_CTL                    (* (reg8 *) SWDIO__CTL)
/* Drive Modes */
#define SWDIO_DM0                    (* (reg8 *) SWDIO__DM0) 
#define SWDIO_DM1                    (* (reg8 *) SWDIO__DM1)
#define SWDIO_DM2                    (* (reg8 *) SWDIO__DM2) 
/* Input Buffer Disable Override */
#define SWDIO_INP_DIS                (* (reg8 *) SWDIO__INP_DIS)
/* LCD Common or Segment Drive */
#define SWDIO_LCD_COM_SEG            (* (reg8 *) SWDIO__LCD_COM_SEG)
/* Enable Segment LCD */
#define SWDIO_LCD_EN                 (* (reg8 *) SWDIO__LCD_EN)
/* Slew Rate Control */
#define SWDIO_SLW                    (* (reg8 *) SWDIO__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SWDIO_PRTDSI__CAPS_SEL       (* (reg8 *) SWDIO__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SWDIO_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SWDIO__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SWDIO_PRTDSI__OE_SEL0        (* (reg8 *) SWDIO__PRTDSI__OE_SEL0) 
#define SWDIO_PRTDSI__OE_SEL1        (* (reg8 *) SWDIO__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SWDIO_PRTDSI__OUT_SEL0       (* (reg8 *) SWDIO__PRTDSI__OUT_SEL0) 
#define SWDIO_PRTDSI__OUT_SEL1       (* (reg8 *) SWDIO__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SWDIO_PRTDSI__SYNC_OUT       (* (reg8 *) SWDIO__PRTDSI__SYNC_OUT) 


#if defined(SWDIO__INTSTAT)  /* Interrupt Registers */

    #define SWDIO_INTSTAT                (* (reg8 *) SWDIO__INTSTAT)
    #define SWDIO_SNAP                   (* (reg8 *) SWDIO__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SWDIO_H */


/* [] END OF FILE */
