/*******************************************************************************
* File Name: SWDCLK.h  
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

#if !defined(CY_PINS_SWDCLK_H) /* Pins SWDCLK_H */
#define CY_PINS_SWDCLK_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SWDCLK_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SWDCLK__PORT == 15 && ((SWDCLK__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    SWDCLK_Write(uint8 value) ;
void    SWDCLK_SetDriveMode(uint8 mode) ;
uint8   SWDCLK_ReadDataReg(void) ;
uint8   SWDCLK_Read(void) ;
uint8   SWDCLK_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define SWDCLK_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define SWDCLK_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define SWDCLK_DM_RES_UP          PIN_DM_RES_UP
#define SWDCLK_DM_RES_DWN         PIN_DM_RES_DWN
#define SWDCLK_DM_OD_LO           PIN_DM_OD_LO
#define SWDCLK_DM_OD_HI           PIN_DM_OD_HI
#define SWDCLK_DM_STRONG          PIN_DM_STRONG
#define SWDCLK_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define SWDCLK_MASK               SWDCLK__MASK
#define SWDCLK_SHIFT              SWDCLK__SHIFT
#define SWDCLK_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SWDCLK_PS                     (* (reg8 *) SWDCLK__PS)
/* Data Register */
#define SWDCLK_DR                     (* (reg8 *) SWDCLK__DR)
/* Port Number */
#define SWDCLK_PRT_NUM                (* (reg8 *) SWDCLK__PRT) 
/* Connect to Analog Globals */                                                  
#define SWDCLK_AG                     (* (reg8 *) SWDCLK__AG)                       
/* Analog MUX bux enable */
#define SWDCLK_AMUX                   (* (reg8 *) SWDCLK__AMUX) 
/* Bidirectional Enable */                                                        
#define SWDCLK_BIE                    (* (reg8 *) SWDCLK__BIE)
/* Bit-mask for Aliased Register Access */
#define SWDCLK_BIT_MASK               (* (reg8 *) SWDCLK__BIT_MASK)
/* Bypass Enable */
#define SWDCLK_BYP                    (* (reg8 *) SWDCLK__BYP)
/* Port wide control signals */                                                   
#define SWDCLK_CTL                    (* (reg8 *) SWDCLK__CTL)
/* Drive Modes */
#define SWDCLK_DM0                    (* (reg8 *) SWDCLK__DM0) 
#define SWDCLK_DM1                    (* (reg8 *) SWDCLK__DM1)
#define SWDCLK_DM2                    (* (reg8 *) SWDCLK__DM2) 
/* Input Buffer Disable Override */
#define SWDCLK_INP_DIS                (* (reg8 *) SWDCLK__INP_DIS)
/* LCD Common or Segment Drive */
#define SWDCLK_LCD_COM_SEG            (* (reg8 *) SWDCLK__LCD_COM_SEG)
/* Enable Segment LCD */
#define SWDCLK_LCD_EN                 (* (reg8 *) SWDCLK__LCD_EN)
/* Slew Rate Control */
#define SWDCLK_SLW                    (* (reg8 *) SWDCLK__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SWDCLK_PRTDSI__CAPS_SEL       (* (reg8 *) SWDCLK__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SWDCLK_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SWDCLK__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SWDCLK_PRTDSI__OE_SEL0        (* (reg8 *) SWDCLK__PRTDSI__OE_SEL0) 
#define SWDCLK_PRTDSI__OE_SEL1        (* (reg8 *) SWDCLK__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SWDCLK_PRTDSI__OUT_SEL0       (* (reg8 *) SWDCLK__PRTDSI__OUT_SEL0) 
#define SWDCLK_PRTDSI__OUT_SEL1       (* (reg8 *) SWDCLK__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SWDCLK_PRTDSI__SYNC_OUT       (* (reg8 *) SWDCLK__PRTDSI__SYNC_OUT) 


#if defined(SWDCLK__INTSTAT)  /* Interrupt Registers */

    #define SWDCLK_INTSTAT                (* (reg8 *) SWDCLK__INTSTAT)
    #define SWDCLK_SNAP                   (* (reg8 *) SWDCLK__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SWDCLK_H */


/* [] END OF FILE */
