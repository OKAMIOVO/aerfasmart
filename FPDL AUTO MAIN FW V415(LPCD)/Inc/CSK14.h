#ifndef CSK14_h
#define CSK14_h

#ifdef Function_TouchUsed_CSK14S

extern bool_t g_TouchAwakeNeedReleaseKey;

extern AwakeSystemKeyMgr_t AwakeSystemKeyMgr;



extern void TOUCH_Init(void);
extern void SET_TOUCH_NO_SENSING(void);
extern void SET_TOUCH_AWAKE_SENSITIVITY(void);
extern void TOUCH_POWERDOWN(void);
extern void TOUCH_AWAKE(void);
extern void TOUCH_SUSPEND(void);
extern void TOUCH_RESUME(void);

extern keycode_t Key_Scan(void);
extern void Key_Scan_Init(void);
#endif


#endif
