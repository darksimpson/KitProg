#if !defined(SWD_H)
#define SWD_H

#include <stdbool.h>

#include <project.h>

extern void SWDHalt(void);
extern void SWDResetPSoC(void);
extern void WriteSWD(uint8 *bufOut, uint8 *buf_ACK);
extern void ReadSWD(uint8 *bufOut, uint8 *buf_ACK_Data32);
extern uint8 SWDAcquirePSoC4(uint8 attempts);

#endif
