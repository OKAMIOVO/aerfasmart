#ifndef PIR_H
#define PIR_H
#include "StdTypes.h"

extern BodyInductionMgr_t BodyInductionMgr;

extern void PIR_Init(void);
extern void PIR_Task(void);
extern void PIR_SensitivityDecrease(void);
extern void PIR_PowerEnIO_Init(void);


#endif

