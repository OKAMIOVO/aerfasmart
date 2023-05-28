#ifndef __Project__
#define __Project__	1

#include "StdTypes.h"
#include "Font.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"


//#define ProjectIs_AutoBarLock_S30Z03			//伊洛特，全自动S30-Z03

//#define ProjectIs_AutoBarLock_S32Z04			//马其诺,全自动S32-Z04

//#define ProjectIs_AutoBarLock_S51Z02			//宝合电子

//#define ProjectIs_AutoBarLock_S51Z03			//宝合电子,三排软板按键

//#define ProjectIs_AutoBarLock_S51Z05			//宝合电子,米家,单板,双排按键，LCD, 硬件支持人脸，软件不带人脸
//#define ProjectIs_AutoBarLock_S51Z06			//宝合电子,米家,单板,双排按键，支持人脸，雷达人体感应


//#define ProjectIs_AutoBarLock_S58Z02			//爱狮顿，

//#define ProjectIs_AutoBarLock_S08Z05			//名佳

//#define ProjectIs_AutoBarLock_S49Z05			//因硕,双排软板按键
//#define ProjectIs_AutoBarLock_S49Z06			//因硕, 嵌入式
//#define ProjectIs_AutoBarLock_S49Z15			//因硕,双排软板按键，支持人脸, LCD，雷达人体感应

//#define ProjectIs_AutoBarLock_S61Z03			//米家，双排软板按键

//#define ProjectIs_AutoBarLock_S11Z04			//烁鑫,双排软板按键
//#define ProjectIs_AutoBarLock_S11Z05			//烁鑫,嵌入式, 三排按键,支持人脸, OLED，雷达人体感应，


//#define ProjectIs_AutoBarLock_S67Z01			//翔鹏, 嵌入式

//#define ProjectIs_AutoBarLock_S67Z02			//翔鹏, 嵌入式,不带屏

//#define ProjectIs_AutoBarLock_S64Z04			//峻凯，因硕,双排软板按键，支持人脸, OLED，红外人体感应
//#define ProjectIs_AutoBarLock_S64Z07			//峻凯，因硕,单板，支持人脸,LCD,红外人体感应
//#define ProjectIs_AutoBarLock_S64Z08			//峻凯，双排软板按键，支持人脸, LCD，红外人体感应
//#define ProjectIs_AutoBarLock_S64Z09			//峻凯，双排软板按键，不支持人脸，LCD，
//#define ProjectIs_AutoBarLock_S64Z10			//峻凯，双排软板按键,OLED,不支持人脸
//#define ProjectIs_AutoBarLock_S64Z19			//峻凯，因硕,单板，支持人脸,LCD,雷达人体感应



//#define ProjectIs_AutoBarLock_S70Z01			//志威,双排软板按键，支持人脸, OLED，雷达人体感应，FPM383C
//#define ProjectIs_AutoBarLock_S70Z02			//志威,航鹰(亿昊)，单板双排按键，LCD, FM5114(刷卡触摸二合一),雷达人体感应, 
//#define ProjectIs_AutoBarLock_S70Z09			//志威,航鹰(亿昊)，单板双排按键，LCD, FM5114(刷卡触摸二合一),雷达人体感应, 前板门铃不响

//#define ProjectIs_AutoBarLock_S06Z04			//超常，单板，双排按键，支持人脸, OLED，雷达人体感应

//#define ProjectIs_AutoBarLock_S31Z01			//鸿阳, 嵌入式

//#define ProjectIs_AutoBarLock_S20Z04			//顾万家,嵌入式双板，OLED, FM5114(刷卡触摸二合一),硬件支持人脸

//#define ProjectIs_AutoBarLock_S61Z11			//米家,亿昊，单板,双排按键，支持人脸，雷达人体感应

//#define ProjectIs_AutoBarLock_S58Z07			//爱狮顿,嵌入式双板，LCD, FM5114(刷卡触摸二合一),硬件支持人脸

//#define ProjectIs_AutoBarLock_S68Z04			//亿昊,嵌入式双板，LCD, FM5114(刷卡触摸二合一),硬件支持人脸，后板蜂鸣器

//#define ProjectIs_AutoBarLock_S68Z10			//亿昊,嵌入式双板，LCD, FM5114(刷卡触摸二合一),硬件支持人脸,后板喇叭

//#define ProjectIs_AutoBarLock_S72Z03			//鸿盾,单板双排按键，OLED, FM5114(刷卡触摸二合一),硬件支持人脸，雷达人体感应

//#define ProjectIs_AutoBarLock_S78Z01			//神鼎，东洋,双排软板按键，支持人脸, LCD，雷达人体感应

//#define ProjectIs_AutoBarLock_S83Z01			//玖瑞达,三排软板按键，,支持人脸, LCD，雷达人体感应
//#define ProjectIs_AutoBarLock_S83Z02			//玖瑞达，单板，LCD, FM5114(刷卡触摸二合一),雷达人体感应, 
 
#define ProjectIs_AutoBarLock_S90Z01			//好太太，单板三排按键，支持人脸, OLED，红外人体感应

//#define ProjectIs_AutoBarLock_S90Z11			//好太太，单板三排按键，支持人脸, OLED，雷达人体感应

//#define ProjectIs_AutoBarLock_S90Z20			//好太太，三排软板，支持人脸, LCD，雷达人体感应

//#define ProjectIs_AutoBarLock_S95Z01			//罗马达，双排软板按键，不支持人脸，LCD，

//#define ProjectIs_AutoBarLock_S86Z02			//亿昊,嵌入式双板，LCD, FM5114(刷卡触摸二合一),硬件支持人脸

#define DEBUG_MODE 

#ifdef ProjectIs_AutoBarLock_S30Z03
#define DEF_CustomerNumber	30		//should be less than 256
#define DEF_ModelNumber		03		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed

#elif defined ProjectIs_AutoBarLock_S32Z04
#define DEF_CustomerNumber	32		//should be less than 256
#define DEF_ModelNumber		04		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed

#elif defined ProjectIs_AutoBarLock_S49Z05
#define DEF_CustomerNumber	49		//should be less than 256
#define DEF_ModelNumber		05		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
//#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine


#elif defined ProjectIs_AutoBarLock_S49Z06
#define DEF_CustomerNumber	49		//should be less than 256
#define DEF_ModelNumber		06		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_TMS12S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed

#elif defined ProjectIs_AutoBarLock_S49Z15
#define DEF_CustomerNumber	49		//should be less than 256
#define DEF_ModelNumber		15		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	131		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify



#elif defined ProjectIs_AutoBarLock_S51Z02
#define DEF_CustomerNumber	51		//should be less than 256
#define DEF_ModelNumber		02		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed

#elif defined ProjectIs_AutoBarLock_S51Z03
#define DEF_CustomerNumber	51		//should be less than 256
#define DEF_ModelNumber		03		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
//#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify


#elif defined ProjectIs_AutoBarLock_S51Z05
#define DEF_CustomerNumber	51		//should be less than 256
#define DEF_ModelNumber		05		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
//#define Function_FaceRecoginition
//#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
//#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
//#define Function_VoiceReportFaceStatusWhileIdendify


#elif defined ProjectIs_AutoBarLock_S51Z06
#define DEF_CustomerNumber	51		//should be less than 256
#define DEF_ModelNumber		06		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	131		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S61Z11
#define DEF_CustomerNumber	61		//should be less than 256
#define DEF_ModelNumber		11		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	133		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify



#elif defined ProjectIs_AutoBarLock_S58Z02
#define DEF_CustomerNumber	58		//should be less than 256
#define DEF_ModelNumber		02		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_TMS12S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_VoiceIcPowerTurnOffInSleepMode

#elif defined ProjectIs_AutoBarLock_S08Z05
#define DEF_CustomerNumber	8		//should be less than 256
#define DEF_ModelNumber		5		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_YouzhiyunjiaWifi


#elif defined ProjectIs_AutoBarLock_S61Z03
#define DEF_CustomerNumber	61		//should be less than 256
#define DEF_ModelNumber		3		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine

#elif defined ProjectIs_AutoBarLock_S11Z04
#define DEF_CustomerNumber	11		//should be less than 256
#define DEF_ModelNumber		04		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine

#elif defined ProjectIs_AutoBarLock_S11Z05
#define DEF_CustomerNumber	11		//should be less than 256
#define DEF_ModelNumber		05		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify


#elif defined ProjectIs_AutoBarLock_S67Z01
#define DEF_CustomerNumber	67		//should be less than 256
#define DEF_ModelNumber		01		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed

#elif defined ProjectIs_AutoBarLock_S67Z02
#define DEF_CustomerNumber	67		//should be less than 256
#define DEF_ModelNumber		02		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
//#define Function_FaceRecoginition
//#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
//#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
//#define Function_VoiceReportFaceStatusWhileIdendify



#elif defined ProjectIs_AutoBarLock_S31Z01
#define DEF_CustomerNumber	31		//should be less than 256
#define DEF_ModelNumber		01		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify


#elif defined ProjectIs_AutoBarLock_S70Z01
#define DEF_CustomerNumber	70		//should be less than 256
#define DEF_ModelNumber		01		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	131		//should be less than 256
#define Function_DisplayUsed_OLED128X64
//#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar

#elif defined ProjectIs_AutoBarLock_S70Z02
#define DEF_CustomerNumber	70		//should be less than 256
#define DEF_ModelNumber		02		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	134		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
//#define Function_TouchUsed_CSK14S
#define Function_TouchUsed_FM5114
//#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
//#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S70Z09
#define DEF_CustomerNumber	70		//should be less than 256
#define DEF_ModelNumber		9		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	135		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
//#define Function_TouchUsed_CSK14S
#define Function_TouchUsed_FM5114
//#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
//#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify


#elif defined ProjectIs_AutoBarLock_S72Z03
#define DEF_CustomerNumber	72		//should be less than 256
#define DEF_ModelNumber		03		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
//#define Function_DisplayUsed_HFG12864
//#define Function_TouchUsed_CSK14S
#define Function_TouchUsed_FM5114
//#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
//#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify


#elif defined ProjectIs_AutoBarLock_S64Z07
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber		07		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	132		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S64Z19
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber		19		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	131		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify



#elif defined ProjectIs_AutoBarLock_S64Z04
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber		04		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
//#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify



#elif defined ProjectIs_AutoBarLock_S64Z08
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber		8		//should be less than 256
#define DEF_HardwareVerion	110		//should be less than 256
#define DEF_FirmwareVerion	131		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_FaceRecoginition
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S64Z09
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber		9		//should be less than 256
#define DEF_HardwareVerion	110		//should be less than 256
#define DEF_FirmwareVerion	132		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
//#define Function_FaceRecoginition
//#define Function_FaceRegisterMultiTimes
//#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
//#define Function_VoiceReportFaceStatusWhileIdendify



#elif defined ProjectIs_AutoBarLock_S64Z10
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber		10		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	131		//should be less than 256
#define Function_DisplayUsed_OLED128X64
//#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
//#define Function_FaceRecoginition
//#define Function_FaceRegisterMultiTimes
//#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
//#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S06Z04
#define DEF_CustomerNumber	06		//should be less than 256
#define DEF_ModelNumber		04		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
#define Function_DisplayUsed_OLED128X64
#define Function_TouchUsed_CSK14S
#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S20Z04
#define DEF_CustomerNumber	20		//should be less than 256
#define DEF_ModelNumber		04		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
//#define Function_TouchUsed_CSK14S
#define Function_TouchUsed_FM5114
//#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
//#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S58Z07
#define DEF_CustomerNumber	20		//should be less than 256
#define DEF_ModelNumber		04		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	135		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
//#define Function_TouchUsed_CSK14S
#define Function_TouchUsed_FM5114
//#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
//#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S68Z04
#define DEF_CustomerNumber	68		//should be less than 256
#define DEF_ModelNumber		04		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	132		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
//#define Function_TouchUsed_CSK14S
#define Function_TouchUsed_FM5114
//#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
//#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S68Z10
#define DEF_CustomerNumber	68		//should be less than 256
#define DEF_ModelNumber		10		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	136		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
//#define Function_TouchUsed_CSK14S
#define Function_TouchUsed_FM5114
//#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
//#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify


#elif defined ProjectIs_AutoBarLock_S78Z01
#define DEF_CustomerNumber	78		//should be less than 256
#define DEF_ModelNumber		01		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	131		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine

#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S83Z01
#define DEF_CustomerNumber	83		//should be less than 256
#define DEF_ModelNumber		01		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	133		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_TouchUsed_FM5114
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedByNFC
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S83Z02
#define DEF_CustomerNumber	83		//should be less than 256
#define DEF_ModelNumber		02		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	130		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
//#define Function_TouchUsed_CSK14S
#define Function_TouchUsed_FM5114
//#define Function_FPM_IS_ML383C
//#define Function_FPMserialNumberCheck
//#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
//#define Function_FaceRecoginitionSwitchedByNFC
#define Function_FaceRecoginitionSwitchedAutoDetection
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S90Z01
#define DEF_CustomerNumber	90		//should be less than 256
#define DEF_ModelNumber		01		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	164		//should be less than 256
#define Function_DisplayUsed_OLED128X64
//#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
#define Function_FPM_IS_ML383C
#define Function_FPMserialNumberCheck
#define Function_DisableFpmDuplicateCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_FM17622_LPCD
#define Function_MainBoardWithoutVoicePlayer
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedAutoDetection
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify
#define Function_LiBatteryOrZnMnBatteryAutoSwitch


#elif defined ProjectIs_AutoBarLock_S90Z11
#define DEF_CustomerNumber	90		//should be less than 256
#define DEF_ModelNumber		11		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	131		//should be less than 256
#define Function_DisplayUsed_OLED128X64
//#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
#define Function_FPMserialNumberCheck
#define Function_DisableFpmDuplicateCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_FM17622_LPCD
#define Function_MainBoardWithoutVoicePlayer
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_IndependentDoorBellKey
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedAutoDetection
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify
#define Function_LiBatteryOrZnMnBatteryAutoSwitch

#elif defined ProjectIs_AutoBarLock_S90Z20
#define DEF_CustomerNumber	90		//should be less than 256
#define DEF_ModelNumber		20		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	137		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_TouchUsed_FM5114
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_FM17622_LPCD
#define Function_MainBoardWithoutVoicePlayer
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedAutoDetection
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
#define Function_VoiceReportFaceStatusWhileIdendify
#define Function_LiBatteryOrZnMnBatteryAutoSwitch

#elif defined ProjectIs_AutoBarLock_S95Z01
#define DEF_CustomerNumber	95		//should be less than 256
#define DEF_ModelNumber		1		//should be less than 256
#define DEF_HardwareVerion	110		//should be less than 256
#define DEF_FirmwareVerion	101		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_TouchKeyIsTwoLine
//#define Function_FaceRecoginition
//#define Function_FaceRegisterMultiTimes
//#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
//#define Function_BodyInductionByRadar
//#define Function_VoiceReportFaceStatusWhileIdendify

#elif defined ProjectIs_AutoBarLock_S86Z02
#define DEF_CustomerNumber	86		//should be less than 256
#define DEF_ModelNumber		02		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	133		//should be less than 256
//#define Function_DisplayUsed_OLED128X64
#define Function_DisplayUsed_HFG12864
#define Function_TouchUsed_CSK14S
//#define Function_TouchUsed_FM5114
//#define Function_FPMserialNumberCheck
#define HardwareDiff_CardReaderUseSPI
#define Function_AntiPryingDefaultDisabled
#define Function_FPMBreathingLed
#define Function_FaceRecoginition
#define Function_FaceRecoginitionSwitchedAutoDetection
//#define Function_FaceRegisterMultiTimes
#define Function_FaceRegisterSingleTimes
//#define Function_BodyInductionByIR
#define Function_BodyInductionByRadar
#define Function_VoiceReportFaceStatusWhileIdendify


#endif


#define DEF_MAX_FACEMASTER		10
#define DEF_MAX_FACEUSER		90

#ifdef Function_FPM_IS_ML383C
#define DEF_MAX_FPMASTER		10
#define DEF_MAX_FPUSER			30
#define DEF_MAX_STRESSFPUSER	10
#else
#define DEF_MAX_FPMASTER		10
#define DEF_MAX_FPUSER			80
#define DEF_MAX_STRESSFPUSER	10
#endif

#define DEF_MAX_CARDUSER		100
#define DEF_MAX_PASSCODEMASTER  2
#define DEF_MAX_PASSCODEUSER	8

#ifdef DEBUG_MODE 
#define DEBUG_MARK {__ASM ("nop");__ASM ("nop");}
#define EnableWDT()	//
					//Enable watch dog  //Disable Protection  //1/256		//862ms			//Refresh WDG Timer and Enable Protection
#define CLRWDT()	//
#else
#define DEBUG_MARK //
#define EnableWDT()	{MX_IWDG_Init();}
					//Enable watch dog  //Disable Protection  //1/256		//862ms			//Refresh WDG Timer and Enable Protection
#define CLRWDT()		{HAL_IWDG_Refresh(&hiwdg);}
#endif


GLOBAL DriverBoardVersion_t DriverBoardVersion;

GLOBAL Timebase_t G_tflagbits;

GLOBAL uint8_t G_BatLevel;
GLOBAL uint16_t G_BatVoltage;

GLOBAL systemtime_t SystemTime;

GLOBAL  UartMgr_t UART1_Mgr;

GLOBAL  UartMgr_t UART2_Mgr;


GLOBAL uint8_t G_DisRAM[1024];

GLOBAL uint8_t G_TEST;

GLOBAL screen_t CurrentScreen;

GLOBAL GUI_FaceTemplateRegisterMgr_t GUI_FaceTemplateRegisterMgr;
GLOBAL FaceUserRegisterMgr_t FaceUserRegisterMgr;
GLOBAL FaceUserDeleteMgr_t FaceUserDeleteMgr;
GLOBAL AllUserFaceDeleteMgr_t AllUserFaceDeleteMgr;
GLOBAL CheckHomManyRegisteredFACEuser_t CheckHomManyRegisteredFACEuser;

GLOBAL  FpmAckMgr_t FpmAckMgr;
GLOBAL  FpUserRegisterMgr_t FpUserRegisterMgr;
GLOBAL  AllUserFpDeleteMgr_t	AllUserFpDeleteMgr;
GLOBAL  FpUserDeleteMgr_t	FpUserDeleteMgr;
GLOBAL  FpIdentifyMgr_t FpIdentifyMgr;

GLOBAL  CardMemoryMgr_t CardMemoryMgr[DEF_MAX_CARDUSER];
GLOBAL  CardRegisterMgr_t CardUserRegisterMgr;
GLOBAL  CardIdentifyMgr_t CardIdentifyMgr;
GLOBAL  CardUserDeleteMgr_t CardUserDeleteMgr;
GLOBAL  AllCardUserDeleteMgr_t AllCardUserDeleteMgr;

GLOBAL  FpRegisterMgr_t FpRegisterMgr;

GLOBAL  PasscodeMemoryMgr_t PasscodeMemoryMgr[DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER];
GLOBAL  PasscodeUserRegisterMgr_t PasscodeUserRegisterMgr;
GLOBAL  PasscodeUserDeleteMgr_t PasscodeUserDeleteMgr;
GLOBAL  AllPasscodeUserDeleteMgr_t AllPasscodeUserDeleteMgr;
GLOBAL  PasscodeUserIdentifyMgr_t PasscodeUserIdentifyMgr;
GLOBAL  BatteryMgr_t BatteryMgr;

GLOBAL  EMAGNETMgr_t EMAGNETMgr;

GLOBAL  SystemPowerMgr_t SystemPowerMgr;

GLOBAL  BeepMgr_t BeepMgr;

GLOBAL  FpUserMemoryMgr_t FpUserMemoryMgr[DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER];

GLOBAL  RestoreFactoryDefaultMgr_t RestoreFactoryDefaultMgr;

GLOBAL  VoiceMgr_t VoiceMgr;
GLOBAL  PickAlarmEnableMgr_t PickAlarmEnableMgr;

GLOBAL  CheckHomManyRegisteredFPuser_t CheckHomManyRegisteredFPuser;
GLOBAL  CheckIfFpUserIDisRegistered_t CheckIfFpUserIDisRegistered;
GLOBAL  CheckMemoryMgr_t CheckMemoryMgr;
GLOBAL  ManagerIdentifyMgr_t ManagerIdentifyMgr;
GLOBAL  DisplayMemoryUsageMgr_t DisplayMemoryUsageMgr;
GLOBAL  InitializationMgr_t InitializationMgr;
GLOBAL  FrmConnectionCheckMgr_t FrmConnectionCheckMgr;
GLOBAL  ManagerPasscodeModifyMgr_t	ManagerPasscodeModifyMgr;
GLOBAL  UserIdentifyResultMgr_t	UserIdentifyResultMgr;
GLOBAL  SelfTestMgr_t SelfTestMgr;

GLOBAL  SafetyMonitorMgr_t SafetyMonitorMgr;
GLOBAL  UnlockingModeMgr_t UnlockingModeMgr;

GLOBAL VoiceDataTransferMgr_t VoiceDataTransferMgr; 
GLOBAL VoiceDataBufferMgr_t VoiceDataBufferMgr;


GLOBAL  FPMpowerMgr_t FPMpowerMgr;

GLOBAL  bool_t g_ASTERISK_PressedOnMainScreen; 
GLOBAL  AgingTestMgr_t AgingTestMgr;

GLOBAL  LogMgr_t LogMgr;

GLOBAL LockBrand_t LockBrand;

GLOBAL CheckEventLogByDateMgr_t CheckEventLogByDateMgr;

GLOBAL CheckEventLogBySequenceMgr_t CheckEventLogBySequenceMgr;

GLOBAL VoiceReportLogMgr_t VoiceReportLogMgr;

GLOBAL TouchPowerMgr_t TouchPowerMgr;

GLOBAL SystemLanguage_t SystemLanguage;

GLOBAL AntiPryingMgr_t AntiPryingMgr;

GLOBAL LogDeleteMgr_t LogDeleteMgr;

GLOBAL FPMserialNumberManager_t FPMserialNumberManager;

GLOBAL LEDsMgr_t LEDsMgr;

GLOBAL WifiMgr_t WifiMgr;

GLOBAL ComportMgr_t ComportMgr;

GLOBAL AutoMotorMgr_t  AutoMotorMgr;

GLOBAL DisplayDoorStatusMgr_t DisplayDoorStatusMgr;

GLOBAL ErrorMessageMgr_t ErrorMessageMgr;
GLOBAL DoorBellMgr_t DoorBellMgr;

GLOBAL FrmAckMgr_t FrmAckMgr;
GLOBAL FaceUserMemoryMgr_t FaceUserMemoryMgr[DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER];
GLOBAL FrmMgr_t FrmMgr;
GLOBAL FaceIdentifyMgr_t FaceIdentifyMgr;
GLOBAL DeleteAllFaceTemplateMgr_t DeleteAllFaceTemplateMgr;
GLOBAL PIRmgr_t PIRmgr;

GLOBAL FpmLEDsMgr_t FpmLEDsMgr;

GLOBAL AwakeDisplayMgr_t AwakeDisplayMgr;

GLOBAL keycode_t Debug_Keycode;

GLOBAL StrongUnlockMgr_t StrongUnlockMgr;



#endif
