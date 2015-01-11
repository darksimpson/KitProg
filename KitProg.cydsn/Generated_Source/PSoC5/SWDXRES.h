/*******************************************************************************
* File Name: SWDXRES.h  
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

#if !defined(CY_PINS_SWDXRES_H) /* Pins SWDXRES_H */
#define CY_PINS_SWDXRES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SWDXRES_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SWDXRES__PORT == 15 && ((SWDXRES__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    SWDXRES_Write(uint8 value) ;
void    SWDXRES_SetDriveMode(uint8 mode) ;
uint8   SWDXRES_ReadDataReg(void) ;
uint8   SWDXRES_Read(void) ;
uint8   SWDXRES_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define SWDXRES_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define SWDXRES_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define SWDXRES_DM_RES_UP          PIN_DM_RES_UP
#define SWDXRES_DM_RES_DWN         PIN_DM_RES_DWN
#define SWDXRES_DM_OD_LO           PIN_DM_OD_LO
#define SWDXRES_DM_OD_HI           PIN_DM_OD_HI
#define SWDXRES_DM_STRONG          PIN_DM_STRONG
#define SWDXRES_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define SWDXRES_MASK               SWDXRES__MASK
#define SWDXRES_SHIFT              SWDXRES__SHIFT
#define SWDXRES_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SWDXRES_PS                     (* (reg8 *) SWDXRES__PS)
/* Data Register */
#define SWDXRES_DR                     (* (reg8 *) SWDXRES__DR)
/* Port Number */
#define SWDXRES_PRT_NUM                (* (reg8 *) SWDXRES__PRT) 
/* Connect to Analog Globals */                                                  
#define SWDXRES_AG                     (* (reg8 *) SWDXRES__AG)                       
/* Analog MUX bux enable */
#define SWDXRES_AMUX                   (* (reg8 *) SWDXRES__AMUX) 
/* Bidirectional Enable */                                                        
#define SWDXRES_BIE                    (* (reg8 *) SWDXRES__BIE)
/* Bit-mask for Aliased Register Access */
#define SWDXRES_BIT_MASK               (* (reg8 *) SWDXRES__BIT_MASK)
/* Bypass Enable */
#define SWDXRES_BYP                    (* (reg8 *) SWDXRES__BYP)
/* Port wide control signals */                                                   
#define SWDXRES_CTL                    (* (reg8 *) SWDXRES__CTL)
/* Drive Modes */
#define SWDXRES_DM0                    (* (reg8 *) SWDXRES__DM0) 
#define SWDXRES_DM1                    (* (reg8 *) SWDXRES__DM1)
#define SWDXRES_DM2                    (* (reg8 *) SWDXRES__DM2) 
/* Input Buffer Disable Override */
#define SWDXRES_INP_DIS                (* (reg8 *) SWDXRES__INP_DIS)
/* LCD Common or Segment Drive */
#define SWDXRES_LCD_COM_SEG            (* (reg8 *) SWDXRES__LCD_COM_SEG)
/* Enable Segment LCD */
#define SWDXRES_LCD_EN                 (* (reg8 *) SWDXRES__LCD_EN)
/* Slew Rate Control */
#define SWDXRES_SLW                    (* (reg8 *) SWDXRES__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SWDXRES_PRTDSI__CAPS_SEL       (* (reg8 *) SWDXRES__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SWDXRES_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SWDXRES__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SWDXRES_PRTDSI__OE_SEL0        (* (reg8 *) SWDXRES__PRTDSI__OE_SEL0) 
#define SWDXRES_PRTDSI__OE_SEL1        (* (reg8 *) SWDXRES__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SWDXRES_PRTDSI__OUT_SEL0       (* (reg8 *) SWDXRES__PRTDSI__OUT_SEL0) 
#define SWDXRES_PRTDSI__OUT_SEL1       (* (reg8 *) SWDXRES__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SWDXRES_PRTDSI__SYNC_OUT       (* (reg8 *) SWDXRES__PRTDSI__SYNC_OUT) 


#if defined(SWDXRES__INTSTAT)  /* Interrupt Registers */

    #define SWDXRES_INTSTAT                (* (reg8 *) SWDXRES__INTSTAT)
    #define SWDXRES_SNAP                   (* (reg8 *) SWDXRES__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SWDXRES_H */


/* [] END OF FILE */
