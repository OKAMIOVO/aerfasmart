#ifndef RTC_H
#define RTC_H

extern void RealTimeClock_Init(void);
extern void ReadSystemTime(void);
extern void WriteSystemTime(void);
extern void EnableRTCWakeup(void);
extern void DisableRTCWakeup(void);
extern void SystemTimeUpdate(void);

#endif
