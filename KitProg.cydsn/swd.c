#include "kitprog.h"

// 8-bit header
uint8 gbHeader;
// 32-bit data
uint8 gbData[SWD_XFER_SIZE];
// 8-bt staus
uint8 gbStatsReg;
uint8 gbParity;
// 3-bit ACK response
uint8 gbAck;
uint8 ack0 = 0;
uint8 ack1 = 0;
uint8 ack2 = 0;

// Puts all the SWD Pins in High-Z
void SWDHalt(void)
{
	SWD_SET_SDA_IN;
	SWD_SET_SCK_IN;
	SWD_SET_XRES_IN;
}

// Initializes SWD side of communication for PSoC 5LP
void SWDInit(void)
{
   	uint8 count;
	
   	gbHeader = 0; // 8-bit header       
   	gbStatsReg = SWD_DONE | SWD_ACK; // 8-bit status - default goodness     
   	gbAck = 0; // 8-bit configuration

   	for (count = 0; count < SWD_XFER_SIZE; count++)
    	gbData[count] = 0; // 32-bit data
	
	SWDHalt();
}

// Resets the PSoC
void SWDResetPSoC(void)
{
	SWD_SET_XRES_OUT;
    SWD_SET_XRES_LO;
	
    CyDelayUs(400);
	
	SWDHalt();
}

// Puts the PSoC 5LP in the turn around mode of SWD programming
void SWDClockTrn(void)
{
    SWD_SET_SDA_IN; // Float the Data line
    SWD_CLOCK_BIT;
}

// Retrieves the ack bits from the SWD lines 
// Returns Ack = 0:fail, 1:ACK, 2:WAIT, 4:FAULT
void SWDGetAckSegment(void)
{
    SWD_SET_SDA_IN; 
    
    SWD_SET_SCK_LO;
    ack0 = SWD_GET_SDA;
    SWD_SET_SCK_HI;
    
    SWD_SET_SCK_LO;
    ack1 = SWD_GET_SDA;
    SWD_SET_SCK_HI;
    
    SWD_SET_SCK_LO;
    ack2 = SWD_GET_SDA;
    SWD_SET_SCK_HI;
}

// Write a byte on to the SWD lines
// Parameters:
//  bPutData: Byte which is to be written on to the SWD lines
void SWDPutByte(uint8 bPutData)
{
	SWDByte = bPutData;
    
    SWD_SET_SCK_LO; SWD_SDA = B0; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; SWD_SDA = B1; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; SWD_SDA = B2; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; SWD_SDA = B3; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; SWD_SDA = B4; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; SWD_SDA = B5; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; SWD_SDA = B6; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; SWD_SDA = B7; SWD_SET_SCK_HI;
}

// Puts the data segment on to the SWD lines
// Parameters:
//  *pbData: Pointer to the address to where the data read from the SWD lines is to be stored
void SWDPutDataSegment(uint8 *pbData) 
{ 
    SWD_SET_SDA_OUT;
    SWDPutByte(*pbData++);
    SWDPutByte(*pbData++);
    SWDPutByte(*pbData++);
    SWDPutByte(*pbData);
}

// Read a byte from the SWD lines
// Return:
//  SWDByte: Byte read from the SWD lines
uint8 SWDGetByte(void)
{
    SWD_SET_SCK_LO; B0 = SWD_GET_SDA; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; B1 = SWD_GET_SDA; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; B2 = SWD_GET_SDA; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; B3 = SWD_GET_SDA; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; B4 = SWD_GET_SDA; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; B5 = SWD_GET_SDA; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; B6 = SWD_GET_SDA; SWD_SET_SCK_HI;
    SWD_SET_SCK_LO; B7 = SWD_GET_SDA; SWD_SET_SCK_HI;
    
    return SWDByte;
}

// Reads the data segment from the SWD lines
// Parameters:
//  *pbData: Pointer to the address where data to be wriiten to the SWD lines is stored
void SWDGetDataSegment(uint8  *pbData)
{ 
    SWD_SET_SDA_IN; 
    *pbData++ = SWDGetByte(); 
    *pbData++ = SWDGetByte(); 
    *pbData++ = SWDGetByte(); 
    *pbData = SWDGetByte();
}

// Resetd the SWD bus by clocking 60 times
void SWDResetBus (void)
{
    uint8 enableInterrupts;

    SWD_SET_SCK_OUT;
	
	// Disable all the interrupts
    enableInterrupts = CyEnterCriticalSection();
    
    SWD_SET_SDA_HI;
    SWD_SET_SDA_OUT;
	
    SWD_CLOCK_BIT; //  1
    SWD_CLOCK_BIT; //  2
    SWD_CLOCK_BIT; //  3
    SWD_CLOCK_BIT; //  4
    SWD_CLOCK_BIT; //  5
    SWD_CLOCK_BIT; //  6
    SWD_CLOCK_BIT; //  7
    SWD_CLOCK_BIT; //  8
    SWD_CLOCK_BIT; //  9
    SWD_CLOCK_BIT; //  10
    SWD_CLOCK_BIT; //  11
    SWD_CLOCK_BIT; //  12
    SWD_CLOCK_BIT; //  13
    SWD_CLOCK_BIT; //  14
    SWD_CLOCK_BIT; //  15
    SWD_CLOCK_BIT; //  16
    SWD_CLOCK_BIT; //  17
    SWD_CLOCK_BIT; //  18
    SWD_CLOCK_BIT; //  19
    SWD_CLOCK_BIT; //  20
    SWD_CLOCK_BIT; //  21
    SWD_CLOCK_BIT; //  22
    SWD_CLOCK_BIT; //  23
    SWD_CLOCK_BIT; //  24
    SWD_CLOCK_BIT; //  25
    SWD_CLOCK_BIT; //  26
    SWD_CLOCK_BIT; //  27
    SWD_CLOCK_BIT; //  28
    SWD_CLOCK_BIT; //  29
    SWD_CLOCK_BIT; //  30
    SWD_CLOCK_BIT; //  31
    SWD_CLOCK_BIT; //  32
    SWD_CLOCK_BIT; //  33
    SWD_CLOCK_BIT; //  34
    SWD_CLOCK_BIT; //  35
    SWD_CLOCK_BIT; //  36
    SWD_CLOCK_BIT; //  37
    SWD_CLOCK_BIT; //  38
    SWD_CLOCK_BIT; //  39
    SWD_CLOCK_BIT; //  40
    SWD_CLOCK_BIT; //  41
    SWD_CLOCK_BIT; //  42
    SWD_CLOCK_BIT; //  43
    SWD_CLOCK_BIT; //  44
    SWD_CLOCK_BIT; //  45
    SWD_CLOCK_BIT; //  46
    SWD_CLOCK_BIT; //  47
    SWD_CLOCK_BIT; //  48
    SWD_CLOCK_BIT; //  49
    SWD_CLOCK_BIT; //  50
    SWD_CLOCK_BIT; //  51
    SWD_CLOCK_BIT; //  52
    SWD_CLOCK_BIT; //  53
    SWD_CLOCK_BIT; //  54
    SWD_CLOCK_BIT; //  55
    SWD_CLOCK_BIT; //  56
    SWD_CLOCK_BIT; //  57
    SWD_CLOCK_BIT; //  58
    SWD_CLOCK_BIT; //  59
    SWD_CLOCK_BIT; //  60
	
	// Enable all the interrupts
    CyExitCriticalSection(enableInterrupts);
    
    SWD_SET_SDA_IN;
}

// Counts the number of '1's in the byte passed
// Parameters:
//  bData: Byte for which number of ones is to be calculated
// Return:
//  bCount: Number of ones in the byte
uint8 SWDCountBits(uint8 bData)
{
    register uint8 bCount = 0;
	
	// bit 0 to bit 7
    if (bData & 1) bCount++; bData >>= 1;   
    if (bData & 1) bCount++; bData >>= 1;   
    if (bData & 1) bCount++; bData >>= 1;   
    if (bData & 1) bCount++; bData >>= 1;   
    if (bData & 1) bCount++; bData >>= 1;   
    if (bData & 1) bCount++; bData >>= 1;   
    if (bData & 1) bCount++; bData >>= 1;   
    if (bData & 1) bCount++; bData >>= 1;
	
    return bCount;
}

// Calculates the parity for the data segment
// Parameters:
//  pbData: Pointer to the address location of data whose parity is to be calculated
// Return:
//  Parity of the data segment
uint8 SWDComputeDataParity(uint8 *pbData)
{
    uint8 bCount = 0;
	
    bCount += SWDCountBits(*pbData++);
    bCount += SWDCountBits(*pbData++);
    bCount += SWDCountBits(*pbData++);
    bCount += SWDCountBits(*pbData++);
	
    return (bCount & 1);
}

// Puts the Header segment on to the SWD lines
void SWDPutHeaderSegment(void)
{
    SWD_SET_SDA_LO;
    SWD_SET_SDA_OUT;
    CY_NOP;
    CY_NOP;
   
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    SWD_CLOCK_BIT;
    
    SWDPutByte(gbHeader);
    SWD_SET_SDA_IN;
}

// Calls the functions to write data segment on to the SWD lines and checks the 
// acknowledgement to decide whether the write was successful or not
// Parameters:
//  *pbData: Pointer to the address location where data to be written to the SWD lines is stored
void SWDPutData(uint8  *pbData)
{
    uint8 bParity;
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
	
    gbStatsReg = 0;
    bParity = SWDComputeDataParity(pbData);
	
    SWDPutHeaderSegment();
    
    SWDClockTrn();
	
    SWDGetAckSegment();
    gbAck = ack0 | (ack1 << 1) | (ack2 << 2);
	
    SWDClockTrn();
	
    switch (gbAck)
    {
        case SWD_ACK:
        {
            SWDPutDataSegment(pbData);
			
            if (bParity) { SWD_SET_SDA_HI; } else { SWD_SET_SDA_LO; }
            // 46-th clock ends SWD transaction
			SWD_CLOCK_BIT;
			
			// Keep Data low to not start next SWD transaction on 47th clock
            SWD_SET_SDA_LO;
			// 47-th clock is required to move Addr/Data from DAP to AHB bus
            SWD_CLOCK_BIT; 
            
            SWD_SET_SDA_IN;
            SWD_SET_SDA_HI;
            SWD_SET_SCK_HI;
			
            gbStatsReg |= gbAck;
			
            break;
        }
        case SWD_WAIT:
        case SWD_FAULT:
        {
            SWD_SET_SDA_IN;
            SWD_SET_SDA_HI;
            SWD_SET_SCK_HI;
			
            gbStatsReg |= gbAck;
			
            break;
        }
        default:
        {
            SWD_SET_SDA_IN;
            SWD_SET_SDA_HI;
            SWD_SET_SCK_HI;
			
			// Protocol error - no response from target
            gbStatsReg |= SWD_ERROR;
			
            break;
        }
    }
	
    CyExitCriticalSection(enableInterrupts);
}

// Executes SWD-write command on the bus
// Command located at (bufOut + *bufOutOffset), result (ACK) is returned to buf_ACK buffer
// Parameters:
//  pointers to the buffer addresses
void WriteSWD(uint8 *bufOut, uint8 *buf_ACK)
{
	// bRetry - should only take 2 at most
    register uint8 bRetry = 10; 

    gbHeader = bufOut[0];
	
    do
	{
        SWDPutData(&bufOut[1]);
    }
	while (--bRetry && ((gbStatsReg & SWD_ACK_BITS) == SWD_ACK_BITS));

	buf_ACK[0] = gbStatsReg | SWD_DONE;
}

// Invokes the functions to read a data segment from the SWD lines and checks the 
// acknowledgement to decide whether the read operation was successful or not
// Parameters:
//  *pbData: Pointer to the address location where data read from the SWD lines is to be stored
void SWDGetData(uint8 *pbData)
{
    register uint8 i;
    register uint8 bParity;
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
	
    gbStatsReg = 0;
	
    SWDPutHeaderSegment();
	
    SWDClockTrn();
	
    SWDGetAckSegment();
    gbAck = ack0 | (ack1 << 1) | (ack2 << 2);
	
    switch (gbAck)
    {
		// This handles error in pre-ES10 silicon for reads
        case SWD_ACK+SWD_WAIT:
		// This handles normal expected response for ACK=OK
        case SWD_ACK:           
        {
            SWDGetDataSegment(pbData);
			
            SWD_SET_SCK_LO;
            bParity = SWD_GET_SDA;
            SWD_SET_SCK_HI;
			
            SWDClockTrn();
            
            if (SWDComputeDataParity(pbData) != bParity)
            {
                gbStatsReg |= SWD_PERR;
            }
			
            SWD_SET_SDA_IN;
            SWD_SET_SDA_HI;
            SWD_SET_SCK_HI;
			
            gbStatsReg |= gbAck;
			
            break;
        }
        case SWD_WAIT:
        case SWD_FAULT:
        {
            SWDClockTrn();
			
            for (i = 0; i < SWD_XFER_SIZE; i++)
                *pbData++ = 0;
            
            SWD_SET_SDA_IN;
            SWD_SET_SDA_HI;
            SWD_SET_SCK_HI;
			
            gbStatsReg |= gbAck;
			
            break;
        }
        default:
        {
            SWDClockTrn();
			
            SWD_SET_SDA_IN;
            SWD_SET_SDA_HI;
            SWD_SET_SCK_HI;
			
			// Protocol error - no response from target
            gbStatsReg |= SWD_ERROR;  
            
			break;
        }
    }
	
    CyExitCriticalSection(enableInterrupts);
}

// Executes SWD-read command on the bus
// Command located at (bufOut + *bufOutOffset), result (ACK+data) is returned to buf_ACK_Data32 buffer
// Parameters:
//  Pointers to buffer addresses
void ReadSWD(uint8 *bufOut, uint8 *buf_ACK_Data32)
{
    uint8 j;
	
	// bRetry - should only take 2 at most
    register uint8 bRetry = 10; 
    
    for(j=0; j<5; j++)
    {
        buf_ACK_Data32[j] = 0;
    }
    
    gbHeader = bufOut[0];
    
	do
	{
        SWDGetData(&buf_ACK_Data32[0]);
    }
	while (--bRetry && ((gbStatsReg & SWD_ACK_BITS) == SWD_ACK_BITS));

    buf_ACK_Data32[4] = gbStatsReg | SWD_DONE;
}

// Acquires PSoC 4
// Parameters:
//  acquireMode: Reset or Power cycle acquire. For now, only reset mode is supported
//  attempts: Number of retries before declaring acquire failed
// Return:
//  bRet: Flag whether the PSoC 4 is acquired or not
uint8 SWDAcquirePSoC4(uint8 attempts)
{
	uint8 count;
	uint8 bRet = FALSE;
	uint8 bSwdAcquired;
	uint16 timeout;
	uint8 bData[5];
	uint8 bAck;
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

	for (count = 0; count < attempts; count++)
	{
        // Initial PSoC reset, only Acquire by Reset is supported now
        // Generate XRES cycle
        SWD_SET_XRES_OUT;
        SWD_SET_XRES_LO;
        // Drive the clock and data lines
        SWD_SET_SCK_OUT;
        SWD_SET_SDA_OUT;
        SWD_SET_SDA_HI;
        SWD_SET_SCK_HI;
		// Duration of XRES low
        CyDelayUs(100);  
        // Remove reset from PSoC4
        SWD_SET_XRES_IN; 
        timeout = 100;
		
		// Try to acquire SWD port
		timeout = 100;
		bSwdAcquired = FALSE;
		do { 
			// Reset SWD bus
			SWDResetBus();
			// Read SWD ID code
            gbHeader = 0xA5;
			gbData[0] = 0;
			// gbData contains actual ID
            SWDGetData(gbData);
            if ((gbStatsReg & (SWD_ACK_BITS | SWD_PERR)) == SWD_ACK) bSwdAcquired = TRUE; // Acquired
	    	timeout--;
		} while((timeout > 0) && !bSwdAcquired);
		if (!bSwdAcquired) continue; // Not acqired (timeout), try to acquire again
		
		// Configure Debug Port (DAP)
		bData[0] = 0xA9; 
		bData[1] = 0x00; bData[2] = 0x00; bData[3] = 0x00; bData[4] = 0x54;
		WriteSWD(bData, &bAck);
		if ((bAck & SWD_ACK_BITS) != SWD_ACK) continue; // NACK, try to acquire again 
		bData[0] = 0xB1;
		bData[1] = 0x00; bData[2] = 0x00; bData[3] = 0x00; bData[4] = 0x00;
		WriteSWD(bData, &bAck);
		if ((bAck & SWD_ACK_BITS) != SWD_ACK) continue; // NACK, try to acquire again
		bData[0] = 0xA3;
		bData[1] = 0x02; bData[2] = 0x00; bData[3] = 0x00; bData[4] = 0x00;
		WriteSWD(bData, &bAck);
		if ((bAck & SWD_ACK_BITS) != SWD_ACK) continue; // NACK, try to acquire again
		
		// Enter Test Mode
		bData[0] = 0x8B;
		bData[1] = 0x14; bData[2] = 0x00; bData[3] = 0x03; bData[4] = 0x40;
		WriteSWD(bData, &bAck);
		if ((bAck & SWD_ACK_BITS) != SWD_ACK) continue; // NACK, try to acquire again
		bData[0] = 0xBB;
		bData[1] = 0x00; bData[2] = 0x00; bData[3] = 0x00; bData[4] = 0x80;
		WriteSWD(bData, &bAck);
		if ((bAck & SWD_ACK_BITS) != SWD_ACK) continue; // NACK, try to acquire again
						
		bRet = TRUE;
		break;
	}
	
    CyExitCriticalSection(enableInterrupts);
    return (bRet);
}
