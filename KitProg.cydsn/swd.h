#if !defined(SWD_H) 
#define SWD_H

#include "kitprog.h"

// Bitmasks
#define BIT0   				0x01
#define BIT1   				0x02
#define BIT2   				0x04
#define BIT3   				0x08
#define BIT4   				0x10
#define BIT5   				0x20
#define BIT6   				0x40
#define BIT7   				0x80

// Programming pin drive modes
#define SWD_SET_XRES_OUT 	CyPins_SetPinDriveMode(SWDXRES_0, SWDXRES_DM_STRONG)
#define SWD_SET_XRES_IN 	CyPins_SetPinDriveMode(SWDXRES_0, SWDXRES_DM_DIG_HIZ)
#define SWD_SET_SCK_OUT 	CyPins_SetPinDriveMode(SWDCLK_0, SWDCLK_DM_STRONG)
#define SWD_SET_SCK_IN  	CyPins_SetPinDriveMode(SWDCLK_0, SWDCLK_DM_DIG_HIZ)
#define SWD_SET_SDA_OUT 	CyPins_SetPinDriveMode(SWDIO_0, SWDIO_DM_STRONG)
#define SWD_SET_SDA_IN  	CyPins_SetPinDriveMode(SWDIO_0, SWDIO_DM_DIG_HIZ)

// Bit banding of the SRAM addresses in order to address each bit of a byte seperately
#define BIT_ADDRESS 		0x20000100u
#define SWDByte 			*((volatile uint8 *)BIT_ADDRESS)

// Convert SRAM address to SRAM bit map region
#define BITBAND_SRAM_REF 	CYREG_SRAM_DATA_MBASE  
#define BITBAND_SRAM_BASE  	CYREG_SRAM_DATA_MBASE + 0x2000000u
#define BITBAND_SRAM(a,b) 	((BITBAND_SRAM_BASE + (a-BITBAND_SRAM_REF)*32 + (b*4)))  

#define B0 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,0)))
#define B1 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,1)))
#define B2 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,2)))
#define B3 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,3)))
#define B4 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,4)))
#define B5 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,5)))
#define B6 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,6)))
#define B7 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,7)))

// Bit banding of the peripheral addresses for flexibility in addressing SWDIO and SWDCLK
// Convert Peripheral address to peripheral bit map region
#define BITBAND_PERI_REF   	0x40000000u
#define BITBAND_PERI_BASE  	0x42000000u
#define BITBAND_PERI(a,b) 	((BITBAND_PERI_BASE +(a-BITBAND_PERI_REF)*32 + (b*4)))  

#define SWD_BITS 			SWDIO__DR
#define SWD_SDA 			*((volatile uint8 *)(BITBAND_PERI(SWD_BITS,0)))
#define SWD_SCK 			*((volatile uint8 *)(BITBAND_PERI(SWD_BITS,1)))
#define SWD_XRES 			*((volatile uint8 *)(BITBAND_PERI(SWD_BITS,2)))
#define SDA_PS 				*((volatile uint8 *)(BITBAND_PERI(SWDIO__PS,SWDIO_SHIFT)))

//#define SWD_SET_XRES_HI 	CyPins_SetPin(SWDXRES_0)
//#define SWD_SET_XRES_LO 	CyPins_ClearPin(SWDXRES_0)
#define SWD_SET_XRES_HI 	SWD_XRES = 1    
#define SWD_SET_XRES_LO 	SWD_XRES = 0 
#define SWD_SET_SCK_LO   	SWD_SCK = 0
#define SWD_SET_SCK_HI   	SWD_SCK = 1
#define SWD_SET_SDA_LO   	SWD_SDA = 0
#define SWD_SET_SDA_HI   	SWD_SDA = 1
#define SWD_GET_SDA   		SDA_PS

// Issue one clock pulse on the SWDCLK line
#define SWD_CLOCK_BIT     	{ SWD_SET_SCK_LO; CY_NOP; CY_NOP; CY_NOP; SWD_SET_SCK_HI; CY_NOP; CY_NOP; CY_NOP; }

// SWD line communication macros
#define SWD_XFER_SIZE       0x04
#define SWD_OFLOW           BIT7
#define SWD_UFLOW           BIT6
#define SWD_DONE            BIT5
#define SWD_PERR            BIT3
#define SWD_FAULT           BIT2
#define SWD_WAIT            BIT1
#define SWD_ACK             BIT0
#define SWD_ACK_BITS        (SWD_FAULT | SWD_WAIT | SWD_ACK)
#define SWD_ERROR           SWD_ACK_BITS

extern int16 bufOutOffset;
extern int16 bufInOffset;
extern int16 bufOutLen;

extern void HandleSWDRequest(void);
extern void SWDInit(void);
extern void SWDHalt(void);
extern void SWDResetBus(void);
extern void SWDResetPSoC(void);
extern void WriteSWD(uint8 *bufOut, uint8 *buf_ACK);
extern void ReadSWD(uint8 *bufOut, uint8 *buf_ACK_Data32);
extern uint8 SWDAcquirePSoC4(uint8 attempts);

#endif /* SWD_H */
