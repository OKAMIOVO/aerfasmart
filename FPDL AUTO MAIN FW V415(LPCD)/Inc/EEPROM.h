#ifndef EEPROM_h
#define EEPROM_h

#include "StdTypes.h"

#define ManagerPasscodeMemoryStartAddr		0x0000

#define CardUserMemoryStartAddr				0x0020		//100 card user, 6 bytes for one user  = 600 bytes
#define PasscodeUserMemoryStartAddr			(0x0020+(6*DEF_MAX_CARDUSER)) /* if DEF_MAX_PASSCODEUSER = 20, 14 bytes for one passcode user,  0x0278 ~  0x38f */

#define FaceUserMemoryStartAddr				0x0400		/* 6 bytes per Face user,if DEF_MAX_FACEUSER = 100, 0x0400~0x0658*/

#define FPMserialNumberStartAddr			(0x0660)	//32 bytes, from 0x05A0~0x05BF
#define FPuserIDListStartAddr				(0x0680)	//32 bytes, from 0x05C0~0x05E0

/*************  from 0x05A0 ~ 0x1DFF is Spare  *****************/

#define LockBrandStartAddr					(0x1C00)	//from 0x1C00 to 0x1F00 ,512 bytes

#define	AutoMotorMgrUnlockTimeAddr          (0x1FE2)	// 1 byte
#define	AutoMotorMgrAutoLockTimeAddr        (0x1FE3)	// 1 byte
#define	AutoMotorMgrLockDirectionAddr       (0x1FE4)	// 1 byte
#define	AutoMotorMgrTorqueAddr              (0x1FE5)	// 1 byte
#define AutoMotorMgrBoltLockTimeAddr      	(0x1FE6)	// 1 byte
#define AutoMotorMgrLockingTravelAddr      	(0x1FE7)	// 1 byte
#define AutoMotorMgrAutoEjectAddr      		(0x1FE8)	// 1 byte
#define FrmFunctionConfigAddr      			(0x1FE9)	// 1 byte
#define BodyInductionStartAddr				(0x1FFA)
#define SystemLanguageStartAddr				(0x1FFB)
#define PickAlarmSwitchStartAddr			(0x1FFC)
#define UnlockModeStartAddr					(0x1FFD)		
#define VolumeValueStartAddr				(0x1FFE)		
#define SelfTestMemoryStartAddr				(0x1FFF)	//last byte

	

extern void EEPROM_ReadSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num);

extern void EEPROM_WriteSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num);

extern void EEPROM_TESTING(void);



#endif
