/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "Project.h"
#include "OLEDdriver.h"
#include "LCD_HFG12864.h"
#include "IO.h"
#include "Hardware.h"
#include "GUI.h"
#include "OLEDdriver.h"
#include "LCD_HFG12864.h"
#include "CSK14.h"
#include "TSM12S.h"
#include "FM5114_Touch.h"
#include "EEPROM.h"
#include "MFC.h"
#include "fingerprint.h"
#include "BeepMgr.h"
#include "RealTimeClock.h"
#include "LEDsMgr.h"
#include "Flash.h"
#include "AppUnlock.h"
#include "HostUart.h"
#include "FaceRecoginitionMgr.h"
#include "YouzhiyunjiaWifi.h"
#include "PIR.h"
#include "Radar.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*//* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
void Main_System_Init( void);
/* USER CODE END PFP */
void SystemClock_Config(void);
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  
	uint8_t i;
  bool_t Tflag=bFALSE;

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  HAL_Init();
	
	Hardware_DelayMs(500);		//wait for power stable
	
  FLASH_OB_Config();	

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

	Main_System_Init();

  /* USER CODE END 2 */

	for (i=0;i<100;i++)						//获取比较准的电压值
	{
     Hardware_Task_Analog_Battery();
     HardwareBatteryMgr_Task();
	}
	HardwareBatteryTypeAutoSwitch();		//自动识别是锂电池还是干电池
	for (i=0;i<100;i++)						//重新计算电池电量等级
	{
     Hardware_Task_Analog_Battery();
     HardwareBatteryMgr_Task();
	}

	GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);

	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		CLRWDT();

		if ( G_tflagbits.T1024Hz == 1 )
		{
			G_tflagbits.T1024Hz =0;	
			
			//UART2_TX_Task();

			#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
			if (WifiMgr.Power.Status == PowerOn)
			{
				UART3_TX_Task();
			}
			#endif	

		}

		if ( G_tflagbits.T256Hz == 1 )
		{
			G_tflagbits.T256Hz =0;	
			#ifdef Function_DisplayUsed_HFG12864
			HFG12864_Display_Task();
			#endif
		}
		
		if ( G_tflagbits.T128Hz == 1 )
		{
			G_tflagbits.T128Hz = 0;
			ComPort_Mgr_Task();
		}	

		/*********************/
		if ( G_tflagbits.T64Hz == 1 )
		{
			G_tflagbits.T64Hz = 0;
				
			//TOGGLE_DEBUG_LED;
			
			#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
			Wifi_Mgr_Task();
			#endif
			
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				FaceRecognitionMgr_Task();
			}

			//ComPort_Mgr_Task();
			
			FPM_Mgr_Task();

			#ifdef Function_DisplayUsed_OLED128X64
			OLED_Display_Task();
			#endif

			LEDsMgr_Task();

			GUI_Task();

			if ( SystemPowerMgr.SleepDelayTimerCnt > 0 )
			{
      		SystemPowerMgr.SleepDelayTimerCnt--;
			}
			
			if ( 	
				(SystemPowerMgr.SleepDelayTimerCnt == 0x0000) 
				//&&( PINMACRO_KB_IRQ_STATUS!=0x00 )
				&&( CurrentScreen != SCREEN_Initialization )
				&&( CurrentScreen != SCREEN_PickLockAlarm )
				&&( CurrentScreen != SCREEN_SystemLocked )
				&&( CurrentScreen != SCREEN_AgingTest )
				&&(CurrentScreen != SCREEN_SelfTest)
				&&( CurrentScreen != SCREEN_IdentifySuccess )
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				&&(WifiMgr.Power.Status == PowerOff)	
				#endif
				&&(((ComportMgr.Status == AckIdle)&&( ComportMgr.DoorStatus == Standby))
					||(ComportMgr.TimeOutTimes > 5)
					)
				)
			{
				System_PowerDown();
				
				System_Awake();
				GUI_RefreshSleepTime();
				SystemPowerMgr.AwakeTimerCnt=0x0000;	
			}

		}
		
		if ( G_tflagbits.T8Hz == 1 )
		{
			G_tflagbits.T8Hz =0;
			if ( CurrentScreen == SCREEN_AgingTest )
			{
				Hardware_Task_Analog_Battery();
				HardwareBatteryMgr_Task();
			}
		}

		if ( G_tflagbits.T2Hz == 1 )
		{
			G_tflagbits.T2Hz =0;
		}
		
		if ( G_tflagbits.T1Hz == 1 )
		{
			G_tflagbits.T1Hz =0;
			SystemTimeUpdate();		
			#ifdef DEBUG_MODE 
			TOGGLE_DEBUG_LED;
			#endif
			//EEPROM_TESTING();

			//HAL_PWR_EnterSTOPMode(0,0);
			//MFC_Test();
			//Flash_Test();
			//AppUnlockTest();
			//Radar_Test();
			
			DEBUG_MARK;		
		}
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  //RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  /** Configure LSE Drive Capability 
  */
  //__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	__HAL_RCC_LSEDRIVE_CONFIG(LL_RCC_LSEDRIVE_HIGH);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
//                              |RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
                              |RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
	
}

/* USER CODE BEGIN 4 */
void Main_System_Init( void)
{	
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	//MX_IWDG_Init();
	
	MX_I2C1_Init(); 
		
	MX_ADC1_Init();
	MX_RTC_Init();
	MX_TIM3_Init();
	MX_TIM6_Init();

	MX_SPI1_Init();
	MX_SPI2_Init();
	
	MX_USART1_UART_Init();

	MX_USART2_UART_Init();
	ComPort_Init();

	SystemConfigLoad();

	#ifdef Function_FaceRecoginition

		if (Radar_Init() == S_SUCCESS )
		{
			BodyInductionMgr.BodyInductionDeviceType = RadarModule;
		}
		else
		{
			BodyInductionMgr.BodyInductionDeviceType = PirModule;
			PIR_PowerEnIO_Init();
		}

		MX_USART3_UART_Init();
		FaceRecognitionMgr_Init();
	#else
		BodyInductionMgr.BodyInductionDeviceType = Absent;
		HAL_UART_DeInit(&huart3);
	#endif

	#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
	Wifi_Init();
	#endif
	
	SET_MFC_CS_H;
	SET_FLASH_CS_H;
	
	SET_OLEDPOWER_ON;
	SET_FPMPOWER_ON; 
	SET_LEDPOWER_ON;
	FPMpowerMgr.Status = FPMpowerOn;
	PINMACRO_TurnOnVbatToADC;
	Hardware_DelayMs(100);		//wait for OLED Driver power on reset
	VOICE_Init();

	SET_ALLKEYLED_OFF();
	
	#ifdef Function_DisplayUsed_OLED128X64
	OLED_Init();
	#elif defined Function_DisplayUsed_HFG12864
	HFG12864_Init();
	#endif
	
	MFC_Init();
	Hardware_DelayMs(10);	
	
//	RealTimeClock_Init();

	
	TOUCH_Init();
	Hardware_DelayMs(10);
	
	GUI_Init();			//Init GUI
	
	PIR_Init();

	SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
	SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
	SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
	SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes = 0x00;
	SafetyMonitorMgr.ManagerFpIdentifyFailedTimes = 0x00;
	SafetyMonitorMgr.SystemLocked = bFALSE;

	LogMgr_Init();

	LEDsMgr_Init();

	CurrentScreen = SCREEN_Initialization;
	InitializationMgr.Status = StartInitialization;
	FrmConnectionCheckMgr.Status = FrmCheckInit;


	BatteryMgr.ProtectVoltageTriggerTimes = 0;
	BatteryMgr.LowBatteryProtectionEnabled = bFALSE;
	BatteryMgr.BatteryVoltage = 0;

 	StrongUnlockMgr.LastUTCtime = 0x00000000;
	StrongUnlockMgr.TrigStrongUnlocking = bFALSE;

//	VoiceMgr.volume = 3;			//default volume
		
	//enableInterrupts();
	
	HAL_TIM_Base_Start_IT(&htim3);
	
	HAL_TIM_Base_Start_IT(&htim6);

	EnableWDT();//IWDG
	
	GUI_RefreshSleepTime();

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
