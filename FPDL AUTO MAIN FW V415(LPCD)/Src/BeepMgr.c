#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "StdTypes.h"
#include "BeepMgr.h"

#ifdef Function_MainBoardWithoutVoicePlayer

void VOICE_Init(void)
{
	VoiceDataTransferMgr.PulseWidthCnt = 0;
	VoiceDataTransferMgr.Status = VoiceIdle;
	VoiceDataBufferMgr.VoiceDataReady = bFALSE;
	VoiceDataTransferMgr.VoicePlayEnd = bTRUE;
}


void PLAY_VOICE_ONESEGMENT(uint8_t volume,uint16_t segment)
{
	uint8_t SegmentCnt;
	uint8_t VoiceVolume;

	uint8_t VOICEBUFF[31];
	
	if (	( CurrentScreen == SCREEN_Main)
			||( CurrentScreen == SCREEN_IdentifySuccess)
			||( CurrentScreen == SCREEN_IdentifyFail)
			||( CurrentScreen == SCREEN_RemoteUnlockRequest)
		)
	
	{
		 if ( volume == 0x00)
		 {
				DEBUG_MARK;
				return;		//do not send data to voice player
		 }
		 else
		 {
			 DEBUG_MARK;
		 	 VoiceVolume = volume;
		 }
	}
	else		// in menu 
	{
		if (volume == 0 )
		{
			VoiceVolume = 1;		//in menu , voice can not be mute
		}
		else
		{
			VoiceVolume = volume ;	
		}
	}
	
	#if defined (ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S90Z11)
	VOICEBUFF[0]=VoiceVolume*4;
	#else
	VOICEBUFF[0]=VoiceVolume*5;
	#endif
		
	if ( segment < 8 )
	{
		VOICEBUFF[1] = segment/256; 
		VOICEBUFF[2] = segment%256; 
	}
	else
	{
		VOICEBUFF[1] = (segment+SystemLanguage)/256; 
		VOICEBUFF[2] = (segment+SystemLanguage)%256; 
	}
		
	VOICEBUFF[3] = 0xFF;
	VOICEBUFF[4] = 0xFA;
	
	ComPort_SetPost_PlayVoice(VOICEBUFF);


}

void PLAY_VOICE_DOORBELL_2times(void)
{
	uint16_t VoiceStr_DoorBell5times[]={VOICE_Doorbell,VOICE_Doorbell,DEF_VoiceSegmentEndFlag};
	PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(3,VoiceStr_DoorBell5times);
}

void PLAY_VOICE_DOORBELL_5times(void)
{
	uint16_t VoiceStr_DoorBell5times[]={VOICE_Doorbell,VOICE_Doorbell,VOICE_Doorbell,VOICE_Doorbell,VOICE_Doorbell,DEF_VoiceSegmentEndFlag};
	PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(3,VoiceStr_DoorBell5times);
}


void PLAY_VOICE_MULTISEGMENTS(uint8_t volume,uint16_t BUFF[])
{
	uint8_t SegmentCnt;
	uint8_t VoiceVolume;

	uint8_t VOICEBUFF[31];

	
	if (	( CurrentScreen == SCREEN_Main)
			||( CurrentScreen == SCREEN_IdentifySuccess)
			||( CurrentScreen == SCREEN_IdentifyFail)
			||( CurrentScreen == SCREEN_RemoteUnlockRequest)
		)
	
	{
		 if ( volume == 0x00)
		 {
				DEBUG_MARK;
				return;		//do not send data to voice player
		 }
		 else
		 {
			 DEBUG_MARK;
		 	 VoiceVolume = volume;
		 }
	}
	else		// in menu 
	{
		if (volume == 0 )
		{
			VoiceVolume = 1;		//in menu , voice can not be mute
		}
		else
		{
			VoiceVolume = volume ;	
		}
	}

	#if defined (ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S90Z11)
	VOICEBUFF[0]=VoiceVolume*4;
	#else
	VOICEBUFF[0]=VoiceVolume*5;
	#endif
		
	for ( SegmentCnt =0;SegmentCnt<15;SegmentCnt++)
	{
		if (BUFF[SegmentCnt] != DEF_VoiceSegmentEndFlag)
		{
			if ( BUFF[SegmentCnt] < 8 )
			{
				VOICEBUFF[2*SegmentCnt+1] = BUFF[SegmentCnt]/256; 
				VOICEBUFF[2*SegmentCnt+2] = BUFF[SegmentCnt]%256; 
			}
			else
			{
				VOICEBUFF[2*SegmentCnt+1] = (BUFF[SegmentCnt]+SystemLanguage)/256; 
				VOICEBUFF[2*SegmentCnt+2] = (BUFF[SegmentCnt]+SystemLanguage)%256; 
			}
		}
		else
		{
			VOICEBUFF[2*SegmentCnt+1] = BUFF[SegmentCnt]/256; 
			VOICEBUFF[2*SegmentCnt+2] = BUFF[SegmentCnt]%256;
			break;
		}
	}
	
	ComPort_SetPost_PlayVoice(VOICEBUFF);
}



void PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(uint8_t volume,uint16_t BUFF[])
{

	uint8_t SegmentCnt;
	uint8_t VoiceVolume;

	uint8_t VOICEBUFF[31];
	
	if (	( CurrentScreen == SCREEN_Main)
			||( CurrentScreen == SCREEN_IdentifySuccess)
			||( CurrentScreen == SCREEN_IdentifyFail)
			||( CurrentScreen == SCREEN_RemoteUnlockRequest)
		)
	
	{
		 if  ( volume == 0x00)
		 {
			return;		//do not send data to voice player
		 }
		 else
		 {
		 	VoiceVolume = volume;
		 }
	}
	else		// in menu 
	{
		VoiceVolume = 3;		//max. volume
	}

	VOICEBUFF[0]=VoiceVolume*5;

	for ( SegmentCnt =0;SegmentCnt<15;SegmentCnt++)
	{
		if (BUFF[SegmentCnt] != DEF_VoiceSegmentEndFlag)
		{
			VOICEBUFF[2*SegmentCnt+1] = BUFF[SegmentCnt]/256; 
			VOICEBUFF[2*SegmentCnt+2] = BUFF[SegmentCnt]%256; 
		}
		else
		{
			VOICEBUFF[2*SegmentCnt+1] = BUFF[SegmentCnt]/256; 
			VOICEBUFF[2*SegmentCnt+2] = BUFF[SegmentCnt]%256;
			break;
		}
	}
	
	ComPort_SetPost_PlayVoice(VOICEBUFF);

}



void PLAY_VOICE_DOORBELL(void)
{
	#ifdef ProjectIs_AutoBarLock_S58Z02
	PLAY_VOICE_DOORBELL_5times();
	#else
	PLAY_VOICE_DOORBELL_2times();
	#endif
}

void STOP_VOICEPLAY(void)
{
	uint16_t VoiceStopCmd = 0xFFFE;
	PLAY_VOICE_ONESEGMENT(0,VoiceStopCmd);
}

#else
void VOICE_Init(void)
{
	VoiceDataTransferMgr.PulseWidthCnt = 0;
	VoiceDataBufferMgr.CurrentVolume = 0x00;
	VoiceDataTransferMgr.Status = VoiceIdle;
	VoiceDataBufferMgr.VoiceDataReady = bFALSE;
}

void PLAY_VOICE_ONESEGMENT(uint8_t volume,uint16_t segment)
{
	uint8_t VoiceVolume,OffsetBytes;

	if ( VoiceDataBufferMgr.VoiceDataReady == bTRUE )
	{
		//return;
	}
	
	if (	( CurrentScreen == SCREEN_Main)
			||( CurrentScreen == SCREEN_IdentifySuccess)
			||( CurrentScreen == SCREEN_IdentifyFail)
		)
	
	{
		 if ( volume == 0x00)
		 {
			return; 	//do not send data to voice player
		 }
		 else
		 {
			VoiceVolume = volume*4;
		 }
	}
	else		// in menu 
	{
		VoiceVolume = 12;		//max. volume
	}

	VoiceDataBufferMgr.DataBuff[0]=0xFF;
	VoiceDataBufferMgr.DataBuff[1]=0xFE;
	
	OffsetBytes=2;
	if ( VoiceDataBufferMgr.CurrentVolume !=VoiceVolume )
	{
		VoiceDataBufferMgr.DataBuff[OffsetBytes]=0xFF;	
		VoiceDataBufferMgr.DataBuff[OffsetBytes+1]=0xE0+VoiceVolume;
		VoiceDataBufferMgr.CurrentVolume =VoiceVolume;
		OffsetBytes += 2;
	}
	
	
	if (segment < 8)
	{
		VoiceDataBufferMgr.DataBuff[OffsetBytes] = 0;
		VoiceDataBufferMgr.DataBuff[OffsetBytes+1] = segment%256;
	}
	else
	{
		VoiceDataBufferMgr.DataBuff[OffsetBytes] = (segment+SystemLanguage)/256;
		VoiceDataBufferMgr.DataBuff[OffsetBytes+1] = (segment+SystemLanguage)%256;
	}
	OffsetBytes+=2;
	
	VoiceDataBufferMgr.DataNum = OffsetBytes;
	VoiceDataBufferMgr.VoiceDataReady = bTRUE;
}

void PLAY_VOICE_DOORBELL_2times(void)
{
	uint16_t VoiceStr_DoorBell5times[]={VOICE_Doorbell,VOICE_Doorbell,DEF_VoiceSegmentEndFlag};
	PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(3,VoiceStr_DoorBell5times);
}

void PLAY_VOICE_DOORBELL_5times(void)
{
	uint16_t VoiceStr_DoorBell5times[]={VOICE_Doorbell,VOICE_Doorbell,VOICE_Doorbell,VOICE_Doorbell,VOICE_Doorbell,DEF_VoiceSegmentEndFlag};
	PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(3,VoiceStr_DoorBell5times);
}
void PLAY_VOICE_MULTISEGMENTS(uint8_t volume,uint16_t BUFF[])
{
	uint8_t SegmentCnt;
	uint8_t VoiceVolume,OffsetBytes;

	if ( VoiceDataBufferMgr.VoiceDataReady == bTRUE )
	{
		//return;
	}
	
	if (	( CurrentScreen == SCREEN_Main)
			||( CurrentScreen == SCREEN_IdentifySuccess)
			||( CurrentScreen == SCREEN_IdentifyFail)
		)
	
	{
		 if ( volume == 0x00)
		 {
			return;		//do not send data to voice player
		 }
		 else
		 {
		 	VoiceVolume = volume*4;
		 }
	}
	else		// in menu 
	{
		VoiceVolume = 12;		//max. volume
	}


	VoiceDataBufferMgr.DataBuff[0]=0xFF;
	VoiceDataBufferMgr.DataBuff[1]=0xFE;

	OffsetBytes = 2;
	if ( VoiceDataBufferMgr.CurrentVolume !=VoiceVolume )
	{
		VoiceDataBufferMgr.DataBuff[OffsetBytes]=0xFF;	
		VoiceDataBufferMgr.DataBuff[OffsetBytes+1]=0xE0+VoiceVolume;
		VoiceDataBufferMgr.CurrentVolume =VoiceVolume;
		OffsetBytes += 2;
	}
	
	for ( SegmentCnt =0;SegmentCnt<15;SegmentCnt++)
	{
		if ( BUFF[SegmentCnt] == DEF_VoiceSegmentEndFlag ){break;}

		if (BUFF[SegmentCnt] < 8)
		{
			VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes] = BUFF[SegmentCnt]/256; 
			VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes+1] = BUFF[SegmentCnt]%256;

		}
		else
		{
			VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes] = (BUFF[SegmentCnt]+SystemLanguage)/256; 
			VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes+1] = (BUFF[SegmentCnt]+SystemLanguage)%256;

		}

		VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes+2] = 0xFF; //Continue play
		VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes+3] = 0xF3; //Continue play	

	}
	if ( SegmentCnt > 0 )
	{
		VoiceDataBufferMgr.DataNum = 4*SegmentCnt+OffsetBytes-2;
		VoiceDataBufferMgr.VoiceDataReady = bTRUE;
	}
	else
	{
		VoiceDataBufferMgr.DataNum = 0;
	}

}



void PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(uint8_t volume,uint16_t BUFF[])
{

	uint8_t SegmentCnt;
	uint8_t VoiceVolume,OffsetBytes;

	if ( VoiceDataBufferMgr.VoiceDataReady == bTRUE )
	{
		//return;
	}
	
	if (	( CurrentScreen == SCREEN_Main)
			||( CurrentScreen == SCREEN_IdentifySuccess)
			||( CurrentScreen == SCREEN_IdentifyFail)
		)
	
	{
		 if ( volume == 0x00)
		 {
			return;		//do not send data to voice player
		 }
		 else
		 {
		 	VoiceVolume = volume*4;
		 }
	}
	else		// in menu 
	{
		VoiceVolume = 12;		//max. volume
	}


	VoiceDataBufferMgr.DataBuff[0]=0xFF;
	VoiceDataBufferMgr.DataBuff[1]=0xFE;

	OffsetBytes = 2;
	if ( VoiceDataBufferMgr.CurrentVolume !=VoiceVolume )
	{
		VoiceDataBufferMgr.DataBuff[OffsetBytes]=0xFF;	
		VoiceDataBufferMgr.DataBuff[OffsetBytes+1]=0xE0+VoiceVolume;
		VoiceDataBufferMgr.CurrentVolume =VoiceVolume;
		OffsetBytes += 2;
	}
	
	for ( SegmentCnt =0;SegmentCnt<15;SegmentCnt++)
	{
		if ( BUFF[SegmentCnt] == DEF_VoiceSegmentEndFlag ){break;}

		VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes] = BUFF[SegmentCnt]/256; 
		VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes+1] = BUFF[SegmentCnt]%256;
		VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes+2] = 0xFF; //Continue play
		VoiceDataBufferMgr.DataBuff[4*SegmentCnt+OffsetBytes+3] = 0xF3; //Continue play	

	}
	if ( SegmentCnt > 0 )
	{
		VoiceDataBufferMgr.DataNum = 4*SegmentCnt+OffsetBytes-2;
		VoiceDataBufferMgr.VoiceDataReady = bTRUE;
	}
	else
	{
		VoiceDataBufferMgr.DataNum = 0;
	}

}



void PLAY_VOICE_DOORBELL(void)
{
	#if defined (ProjectIs_AutoBarLock_S58Z02) || defined (ProjectIs_AutoBarLock_S58Z07)
	PLAY_VOICE_DOORBELL_5times();
	//#elif defined (ProjectIs_AutoBarLock_S68Z10) || defined (ProjectIs_AutoBarLock_S70Z09)
	//PLAY_VOICE_DOORBELL_2times();
	#else
	PLAY_VOICE_DOORBELL_2times();
	#endif
}

void STOP_VOICEPLAY(void)
{
	VoiceDataBufferMgr.DataBuff[0] = 0xFF;
	VoiceDataBufferMgr.DataBuff[1] = 0xFE;
	VoiceDataBufferMgr.DataNum = 2;
	VoiceDataBufferMgr.VoiceDataReady = bTRUE;
}
#endif


