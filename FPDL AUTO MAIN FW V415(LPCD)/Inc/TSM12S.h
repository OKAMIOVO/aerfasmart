#ifndef TSM12S_h
#define TSM12S_h

#ifdef Function_TouchUsed_TMS12S

extern bool_t g_TouchAwakeNeedReleaseKey;

extern void TOUCH_Init(void);
extern void SET_TOUCH_SCAN_PAUSE(void);
extern void SET_TOUCH_SCAN_RESUME(void);
extern void SET_TOUCH_NO_SENSING(void);
extern void SET_TOUCH_AWAKE_SENSITIVITY(void);
extern void TOUCH_POWERDOWN(void);
extern void TOUCH_AWAKE(void);

extern keycode_t Key_Scan(void);
extern void Key_Scan_Init(void);

#endif

#endif
