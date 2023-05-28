#ifndef BeepMgr_h
#define BeepMgr_h


#define DEF_VoiceSegmentEndFlag 0xFFFA

#define VOICE_Mute20ms 					0
#define VOICE_WaterDrop 				1
#define VOICE_Beep						2
#define VOICE_Alarm						3
#define VOICE_Doorbell					4
#define VOICE_VolumeAdjust				5
#define VOICE_POWERON					6
#define VOICE_FailVoice					7
#define VOICE_Language					8
#define VOICE_Chinese					10
#define VOICE_English					11
#define VOICE_Zero					12
#define VOICE_One					14
#define VOICE_Two					16
#define VOICE_Three					18
#define VOICE_Four					20
#define VOICE_Five					22
#define VOICE_Six					24
#define VOICE_Seven					26
#define VOICE_Eight					28
#define VOICE_Nine					30
#define VOICE_Press					32
#define VOICE_Management			34
#define VOICE_SystemSetting			36
#define VOICE_InputError			38
#define VOICE_SingleUnlock			40
#define VOICE_CombinationUnlock		42
#define VOICE_DoorOpened			44
#define VOICE_DoorClosed			46
#define VOICE_SystemLockedPleaseTryLater			48
#define VOICE_Success					50
#define VOICE_Fail						52
#define VOICE_Confirm					54
#define VOICE_PressPoundKey				56
#define VOICE_PressAsteriskKey			58
#define VOICE_ConfirmToDelete			60
#define VOICE_PleaseAddMasterFirst		62
#define VOICE_IdentifyFail				64
#define VOICE_RestoreFactoryDefault		66
#define VOICE_Operation					68
#define VOICE_Add						70
#define VOICE_Delete					72
#define VOICE_All						74
#define VOICE_Administrator				76
#define VOICE_User						78
#define VOICE_Fingerprint				80
#define VOICE_Password					82
#define VOICE_Card						84
#define VOICE_Duplicate					86
#define VOICE_UsersAreFull				88
#define VOICE_IdentifyMaster			90
#define VOICE_PleaseTryAgain			92
#define VOICE_ReturnPreviousMenu		94
#define VOICE_ExitMenu					96
#define VOICE_PleaseSelectDigitalOperation					98
#define VOICE_PleaseInputID					100
#define VOICE_PleaseEnter					102
#define VOICE_Again							104
#define VOICE_UserIDisBeUsed				106
#define VOICE_PleasePutFinger				108
#define VOICE_PleasePutFingerAgain			110
#define VOICE_PleaseSwingCard				112
#define VOICE_Voice							114
#define VOICE_PleaseReplaceTheBattery		116
#define VOICE_Time							118
#define VOICE_NetworkingConfiguration		120
#define VOICE_PleaseInputSecondIdentity		122
#define VOICE_NormallyOpen					124
#define VOICE_UnlockingMode					126
#define VOICE_AntiSkidAlarm					128
#define VOICE_Setting						130
#define VOICE_Opened						132
#define VOICE_Closed						134
#define VOICE_Open							136
#define VOICE_Close							138

#define VOICE_ID							140
#define VOICE_Type							142	
#define VOICE_Unlock						144
#define VOICE_SystemLocked					146
#define VOICE_Year							148
#define VOICE_Month							150
#define VOICE_Date							152
#define VOICE_Hour							154
#define VOICE_Minute						156
#define VOICE_Second						158
#define VOICE_Face							160
#define VOICE_Volume						162
#define VOICE_Lock							164
#define VOICE_AutoLock						166
#define VOICE_Delay							168
#define VOICE_UnlockingDirect				170
#define VOICE_Motor							172
#define VOICE_Torque						174
#define VOICE_Big							176
#define VOICE_Middle						178
#define VOICE_Small							180
#define VOICE_LockTongueReturn				182
#define VOICE_LockDistance					184
#define VOICE_SelfBounce					186
#define VOICE_SelfTest						188
#define VOICE_PleaseInputFace				190
#define VOICE_PleaseMoveFaceToRight			192
#define VOICE_PleaseMoveFaceToLeft			194
#define VOICE_PleaseMoveFaceToDown			196
#define VOICE_PleaseMoveFaceToUp			198
#define VOICE_FaceTooUp						200
#define VOICE_FaceTooLow					202
#define VOICE_FaceTooAway					204
#define VOICE_FaceTooClose					206
#define VOICE_FaceTooLeft					208
#define VOICE_FaceTooRight					210
#define VOICE_RepeatedFace					212
#define VOICE_FaceVerifyInProgress			214
#define VOICE_NoFaceDetected				216
#define VOICE_HardwareNotSupported			218
#define VOICE_CharS							220
#define VOICE_CharZ							222
#define VOICE_Dot							224
#define VOICE_MainBoard						226
#define VOICE_DriverBoard					228
#define VOICE_NetworkConfig					230
#define VOICE_InfomationInquiry				232
#define VOICE_ToRepeatListening				234
#define VOICE_DoorLock						236
#define VOICE_Stress						238
#define VOICE_Current						240
#define VOICE_Cannot						242
#define VOICE_NotExist						244
#define VOICE_Mute							246

#define VOICE_BodyInduction					248	
#define VOICE_MemoryUsage					250
#define VOICE_SystemVersion					252
#define VOICE_Singal						254
#define VOICE_LeftOpen						256
#define	VOICE_RightOpen						258
#define VOICE_Adjust						260
#define VOICE_Log							262
#define VOICE_Query							264
#define VOICE_Not							266

#define VOICE_Previous						268
#define VOICE_Next							270
#define VOICE_QueryInSequence				272
#define VOICE_QueryByDate					274
#define VOICE_Temporary						276
#define VOICE_Used							278
#define VOICE_Remained						280
#define VOICE_PleaseWait					282
/***************************************************/
#define VOICE_AdminIsLoginCannotbeDelete	284
#define VOICE_Communication					286
#define VOICE_Fault							288
#define VOICE_New							290
#define VOICE_Touch							292
#define VOICE_SelfCheck						294
#define VOICE_Validation					296
#define VOICE_DoingLock						298
#define VOICE_RequestingRemoteUnlock		300



#define DEF_ButtonPress_Voice  PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_WaterDrop)
#define DEF_Fail_Beep  PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FailVoice)


extern void VOICE_Init(void);
extern void PLAY_BUTTON_VOICE(uint16_t segment);
extern void PLAY_VOICE_ONESEGMENT(uint8_t volume,uint16_t segment);
extern void PLAY_VOICE_MULTISEGMENTS(uint8_t volume,uint16_t BUFF[]);
extern void PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(uint8_t volume,uint16_t BUFF[]);

extern void PLAY_VOICE_DOORBELL(void);
extern void STOP_VOICEPLAY(void);
extern void PLAY_VOICE_DOORBELL_5times(void);



#endif
