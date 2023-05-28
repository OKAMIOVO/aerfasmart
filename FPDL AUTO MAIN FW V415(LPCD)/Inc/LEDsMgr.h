#ifndef LEDSMGR_h
#define LEDSMGR_h

extern void SET_ALLKEYLED_ON(void);
extern void SET_ALLKEYLED_OFF(void);

extern void SET_LED_RGB_OFF(void);
extern void SET_LED_RB_OFF(void);
extern void SET_ALLKEYLEDPIN_POWERDOWN(void);

extern void SET_UDandAPled_ON(void);
extern void SET_LRandConfirmLED_ON(void);
extern void SET_UDandConfirmLED_ON(void);
extern void SET_UDandLRandConfirmLED_ON(void);
extern void SET_UDandTHREEandConfirmLED_ON(void);
extern void SET_AsteriskLED_ON(void);

extern void SET_APled_ON(void);
extern void SET_12andAPled_ON(void);
extern void SET_123andAPled_ON(void);
extern void SET_1234andAPled_ON(void);
extern void SET_123456andAPled_ON(void);
extern void SET_1234567andAPled_ON(void);
extern void SET_12345678andAPled_ON(void);

extern void SET_KEYLED_OFF(keycode_t key);
extern void SET_KEYLED_ON(keycode_t key);
extern void Enable_KEYLED_WATERLIGHT(void);
extern void KEYLED_WATERLIGHT_Task(void);

extern void KEYLED_ASTERISK_Flash(void);

extern void SET_RBLED_R_Flash(uint16_t times);
extern void SET_RBLED_B_Flash(uint16_t times);


extern void LEDsMgr_Init(void);
extern void LEDsMgr_Task(void);


#endif
