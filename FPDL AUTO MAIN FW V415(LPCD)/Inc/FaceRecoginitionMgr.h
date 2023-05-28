#ifndef _FaceRecognition_H
#define _FaceRecognition_H

extern void UART3_TX_Task(void);
extern void FaceRecognition_EnableDemoMode(void);
extern status_t  FRM_SendDisableDemoCmd(void);
extern void FaceRecognition_RegisterTemplateMultiTimesStart(FaceDirrect_t facedirrect);
extern void FaceRecognition_RegisterTemplateSingleTimesStart(void);
extern void FaceRecognition_VerifyStart(void);
extern void FaceRecognition_DeleteTemplateStart(uint16_t UserID);
extern void FaceRecognition_DeleteAllTemplateStart(void);
extern void FaceRecognition_UpdateUserIdList(void);
extern void FaceRecognition_FaceReset(void);
extern void FaceRecognition_Reset(void);
extern void FaceRecognition_PowerDown(void);
extern void FaceRecognition_HardwarePowerOff(void);
extern void FaceRecognition_HardwarePowerOn(void);
extern void FaceRecognition_GetStatus(void);
extern void FaceRecognition_GetVerionNumber(void);
extern void FaceRecognitionMgr_Init(void);
extern void FaceRecognitionMgr_Task(void);


#endif

