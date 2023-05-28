#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "StdTypes.h"
#include "I2cBus.h"
#include "CSK14.h"

#ifdef Function_TouchUsed_CSK14S

#define Def_KeyHoldTimeLimited 128			//2s
#define Def_KeyHoldLongTimeLimited 	320		//6s

#define DEF_CSK14_TimingDelay	Hardware_DelayX1us(20);

#if defined (ProjectIs_AutoBarLock_S30Z03) || defined (ProjectIs_AutoBarLock_S51Z02) 
#define CSK14_SENS_CH0 		0x70		//KEY 0
#define CSK14_SENS_CH1 		0x70		//KEY *
#define CSK14_SENS_CH2 		0x70		//KEY 8
#define CSK14_SENS_CH3 		0x68		//KEY 7
#define CSK14_SENS_CH4 		0x70		//KEY 5
#define CSK14_SENS_CH5 		0x68		//KEY 4
#define CSK14_SENS_CH6 		0x68		//KEY 1
#define CSK14_SENS_CH7 		0x68		//KEY 2
#define CSK14_SENS_CH8 		0x78		//KEY #
#define CSK14_SENS_CH9 		0x70		//KEY 9
#define CSK14_SENS_CH10 	0x68		//KEY 6
#define CSK14_SENS_CH11 	0x68		//KEY 3
#define CSK14_SENS_CH12 	0x68		//KEY DOOR CLOSE
#define CSK14_SENS_CH13 	0x70		//

#define CSK14_SENS_SLEEP 0x30

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_EIGHT_Pressed
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_FIVE_Pressed
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_ONE_Pressed
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_TWO_Pressed
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_NINE_Pressed
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_THREE_Pressed
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed

#elif defined (ProjectIs_AutoBarLock_S51Z03) ||  defined (ProjectIs_AutoBarLock_S83Z01)

#define CSK14_SENS_CH0 		0x48		//KEY #
#define CSK14_SENS_CH1 		0x48		//KEY 9
#define CSK14_SENS_CH2 		0x40		//KEY 6
#define CSK14_SENS_CH3 		0x48		//KEY 3
#define CSK14_SENS_CH4 		0x48		//KEY DOOR CLOSE
#define CSK14_SENS_CH5 		0x48		//KEY 0
#define CSK14_SENS_CH6 		0x40		//KEY 8
#define CSK14_SENS_CH7 		0x40		//KEY 5
#define CSK14_SENS_CH8 		0x48		//KEY 2
#define CSK14_SENS_CH9 		0x48		//KEY *
#define CSK14_SENS_CH10 	0x40		//KEY 7
#define CSK14_SENS_CH11 	0x40		//KEY 4
#define CSK14_SENS_CH12 	0x40		//KEY 1
#define CSK14_SENS_CH13 	0x70		//

#define CSK14_SENS_SLEEP 0x30

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_TWO_Pressed
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define CSK14_CS13_Pressed  TouchKeyStatus.KEY_DOORBELL_Pressed


#elif defined (ProjectIs_AutoBarLock_S51Z05) || defined (ProjectIs_AutoBarLock_S51Z06)

#define CSK14_SENS_CH0 		0x2A		//KEY bell
#define CSK14_SENS_CH1 		0x28		//KEY *
#define CSK14_SENS_CH2 		0x32		//KEY 9
#define CSK14_SENS_CH3 		0x3A		//KEY 7
#define CSK14_SENS_CH4 		0x42		//KEY 5
#define CSK14_SENS_CH5 		0x40		//KEY 3
#define CSK14_SENS_CH6 		0x50		//KEY 1
#define CSK14_SENS_CH7 		0x58		//KEY 2
#define CSK14_SENS_CH8 		0x40		//KEY 4
#define CSK14_SENS_CH9 		0x3A		//KEY 6
#define CSK14_SENS_CH10 	0x35		//KEY 8
#define CSK14_SENS_CH11 	0x32		//KEY 0
#define CSK14_SENS_CH12 	0x2A		//KEY #
#define CSK14_SENS_CH13 	0x30		//KEY close
	
#define CSK14_SENS_SLEEP 0x50

#define CSK14_CS0_Pressed  TouchKeyStatus.KEY_DOORBELL_Pressed  
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed  
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_NINE_Pressed  
#define CSK14_CS3_Pressed  TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed  TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS5_Pressed   TouchKeyStatus.KEY_THREE_Pressed
#define CSK14_CS6_Pressed  TouchKeyStatus.KEY_ONE_Pressed  
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_TWO_Pressed   
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define CSK14_CS10_Pressed  TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS11_Pressed  TouchKeyStatus.KEY_ZERO_Pressed  
#define CSK14_CS12_Pressed  TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define CSK14_CS13_Pressed  TouchKeyStatus.KEY_DOORCLOSE_Pressed

#elif defined (ProjectIs_AutoBarLock_S61Z11)
#define CSK14_SENS_CH0 		0x48		//KEY bell
#define CSK14_SENS_CH1 		0x48		//KEY *
#define CSK14_SENS_CH2 		0x48		//KEY 9
#define CSK14_SENS_CH3 		0x40		//KEY 7
#define CSK14_SENS_CH4 		0x40		//KEY 5
#define CSK14_SENS_CH5 		0x48		//KEY 3
#define CSK14_SENS_CH6 		0x50		//KEY 1
#define CSK14_SENS_CH7 		0x50		//KEY 2
#define CSK14_SENS_CH8 		0x48		//KEY 4
#define CSK14_SENS_CH9 		0x40		//KEY 6
#define CSK14_SENS_CH10 	0x40		//KEY 8
#define CSK14_SENS_CH11 	0x48		//KEY 0
#define CSK14_SENS_CH12 	0x48		//KEY #
#define CSK14_SENS_CH13 	0x48		//KEY close
	
#define CSK14_SENS_SLEEP 0x20

#define CSK14_CS0_Pressed  TouchKeyStatus.KEY_DOORBELL_Pressed  
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed  
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_NINE_Pressed  
#define CSK14_CS3_Pressed  TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed  TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS5_Pressed   TouchKeyStatus.KEY_THREE_Pressed
#define CSK14_CS6_Pressed  TouchKeyStatus.KEY_ONE_Pressed  
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_TWO_Pressed   
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define CSK14_CS10_Pressed  TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS11_Pressed  TouchKeyStatus.KEY_ZERO_Pressed  
#define CSK14_CS12_Pressed  TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define CSK14_CS13_Pressed  TouchKeyStatus.KEY_DOORCLOSE_Pressed

#elif defined ProjectIs_AutoBarLock_S32Z04
#define CSK14_SENS_CH0 		0x70		//KEY 0
#define CSK14_SENS_CH1 		0x70		//KEY *
#define CSK14_SENS_CH2 		0x70		//KEY 8
#define CSK14_SENS_CH3 		0x68		//KEY 7
#define CSK14_SENS_CH4 		0x70		//KEY 5
#define CSK14_SENS_CH5 		0x68		//KEY 4
#define CSK14_SENS_CH6 		0x68		//KEY 1
#define CSK14_SENS_CH7 		0x68		//KEY 2
#define CSK14_SENS_CH8 		0x78		//KEY #
#define CSK14_SENS_CH9 		0x70		//KEY 9
#define CSK14_SENS_CH10 	0x68		//KEY 6
#define CSK14_SENS_CH11 	0x68		//KEY 3
#define CSK14_SENS_CH12 	0x68		//KEY DOOR CLOSE
#define CSK14_SENS_CH13 	0x70		//
	
#define CSK14_SENS_SLEEP 0x30

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_SEVEN_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_ONE_Pressed
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_TWO_Pressed
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_THREE_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_FIVE_Pressed
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed

#elif defined (ProjectIs_AutoBarLock_S49Z05) || defined (ProjectIs_AutoBarLock_S70Z01)\
	|| defined (ProjectIs_AutoBarLock_S64Z04)|| defined (ProjectIs_AutoBarLock_S64Z08)\
	|| defined (ProjectIs_AutoBarLock_S64Z10)|| defined (ProjectIs_AutoBarLock_S64Z09)\
	|| defined (ProjectIs_AutoBarLock_S49Z15)|| defined (ProjectIs_AutoBarLock_S78Z01)
#define CSK14_SENS_CH0 		0x48		//KEY 1
#define CSK14_SENS_CH1 		0x40		//KEY 3
#define CSK14_SENS_CH2 		0x40		//KEY 5
#define CSK14_SENS_CH3 		0x30		//KEY 7
#define CSK14_SENS_CH4 		0x30		//KEY 9
#define CSK14_SENS_CH5 		0x38		//KEY *
#define CSK14_SENS_CH6 		0x48		//KEY DOOR CLOSE
#define CSK14_SENS_CH7 		0x48		//KEY #
#define CSK14_SENS_CH8 		0x30		//KEY 0
#define CSK14_SENS_CH9 		0x28		//KEY 8
#define CSK14_SENS_CH10 	0x38		//KEY 6
#define CSK14_SENS_CH11 	0x38		//KEY 4
#define CSK14_SENS_CH12 	0x48		//KEY 2
#define CSK14_SENS_CH13 	0x30		//DOORBELL

#ifdef ProjectIs_AutoBarLock_S70Z01
#define CSK14_SENS_SLEEP 0x50
#else
#define CSK14_SENS_SLEEP 0x40
#endif

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed

#elif defined (ProjectIs_AutoBarLock_S95Z01)

#define CSK14_SENS_CH0 		0x50		//KEY 1
#define CSK14_SENS_CH1 		0x50		//KEY 3
#define CSK14_SENS_CH2 		0x50		//KEY 5
#define CSK14_SENS_CH3 		0x50		//KEY 7
#define CSK14_SENS_CH4 		0x50		//KEY 9
#define CSK14_SENS_CH5 		0x50		//KEY *
#define CSK14_SENS_CH6 		0x50		//KEY DOOR CLOSE
#define CSK14_SENS_CH7 		0x50		//KEY #
#define CSK14_SENS_CH8 		0x50		//KEY 0
#define CSK14_SENS_CH9 		0x50		//KEY 8
#define CSK14_SENS_CH10 	0x50		//KEY 6
#define CSK14_SENS_CH11 	0x50		//KEY 4
#define CSK14_SENS_CH12 	0x50		//KEY 2
#define CSK14_SENS_CH13 	0x50		//DOORBELL
	
#define CSK14_SENS_SLEEP 0x50

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed


#elif defined ProjectIs_AutoBarLock_S11Z04
#define CSK14_SENS_CH0 		0x44		//KEY 1
#define CSK14_SENS_CH1 		0x30		//KEY 3
#define CSK14_SENS_CH2 		0x30		//KEY 5
#define CSK14_SENS_CH3 		0x30		//KEY 7
#define CSK14_SENS_CH4 		0x30		//KEY 9
#define CSK14_SENS_CH5 		0x38		//KEY *
#define CSK14_SENS_CH6 		0x58		//KEY DOOR CLOSE
#define CSK14_SENS_CH7 		0x48		//KEY #
#define CSK14_SENS_CH8 		0x30		//KEY 0
#define CSK14_SENS_CH9 		0x28		//KEY 8
#define CSK14_SENS_CH10 	0x30		//KEY 6
#define CSK14_SENS_CH11 	0x30		//KEY 4
#define CSK14_SENS_CH12 	0x40		//KEY 2
#define CSK14_SENS_CH13 	0x30		//

#define CSK14_SENS_SLEEP 0x40

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed


#elif defined ProjectIs_AutoBarLock_S61Z03
#define CSK14_SENS_CH0 		0x44		//KEY 1
#define CSK14_SENS_CH1 		0x30		//KEY 3
#define CSK14_SENS_CH2 		0x30		//KEY 5
#define CSK14_SENS_CH3 		0x30		//KEY 7
#define CSK14_SENS_CH4 		0x30		//KEY 9
#define CSK14_SENS_CH5 		0x38		//KEY *
#define CSK14_SENS_CH6 		0x58		//KEY DOOR CLOSE
#define CSK14_SENS_CH7 		0x48		//KEY #
#define CSK14_SENS_CH8 		0x30		//KEY 0
#define CSK14_SENS_CH9 		0x28		//KEY 8
#define CSK14_SENS_CH10 	0x30		//KEY 6
#define CSK14_SENS_CH11 	0x30		//KEY 4
#define CSK14_SENS_CH12 	0x40		//KEY 2
#define CSK14_SENS_CH13 	0x30		//
	
#define CSK14_SENS_SLEEP 0x40

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed


#elif defined ProjectIs_AutoBarLock_S08Z05
#define CSK14_SENS_CH0 		0x70		//KEY 0
#define CSK14_SENS_CH1 		0x70		//KEY *
#define CSK14_SENS_CH2 		0x70		//KEY 8
#define CSK14_SENS_CH3 		0x70		//KEY 7
#define CSK14_SENS_CH4 		0x70		//KEY 5
#define CSK14_SENS_CH5 		0x70		//KEY 4
#define CSK14_SENS_CH6 		0x70		//KEY 1
#define CSK14_SENS_CH7 		0x70		//KEY 2
#define CSK14_SENS_CH8 		0x70		//KEY #
#define CSK14_SENS_CH9 		0x70		//KEY 9
#define CSK14_SENS_CH10 	0x68		//KEY 6
#define CSK14_SENS_CH11 	0x70		//KEY 3
#define CSK14_SENS_CH12 	0x70		//KEY DOOR CLOSE
#define CSK14_SENS_CH13 	0x70		//
	
#define CSK14_SENS_SLEEP 0x30

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_SEVEN_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed  
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed 
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_ONE_Pressed 

#elif defined (ProjectIs_AutoBarLock_S64Z07) || defined (ProjectIs_AutoBarLock_S64Z19)
#define CSK14_SENS_CH0 		0x20		//DOOR BELL
#define CSK14_SENS_CH1 		0x20		//KEY *
#define CSK14_SENS_CH2 		0x28		//KEY 9
#define CSK14_SENS_CH3 		0x30		//KEY 7
#define CSK14_SENS_CH4 		0x30		//KEY 5
#define CSK14_SENS_CH5 		0x40		//KEY 3
#define CSK14_SENS_CH6 		0x40		//KEY 1
#define CSK14_SENS_CH7 		0x40		//KEY 2
#define CSK14_SENS_CH8 		0x40		//KEY 4
#define CSK14_SENS_CH9 		0x40		//KEY 6
#define CSK14_SENS_CH10 	0x30		//KEY 8
#define CSK14_SENS_CH11 	0x28		//KEY 0
#define CSK14_SENS_CH12 	0x28		//KEY #
#define CSK14_SENS_CH13 	0x20		//DOOR CLOSE
	
#define CSK14_SENS_SLEEP 0x30

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed  		//
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed  
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_NINE_Pressed  
#define CSK14_CS3_Pressed  TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed  TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS5_Pressed   TouchKeyStatus.KEY_THREE_Pressed
#define CSK14_CS6_Pressed  TouchKeyStatus.KEY_ONE_Pressed  
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_TWO_Pressed   
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define CSK14_CS10_Pressed  TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS11_Pressed  TouchKeyStatus.KEY_ZERO_Pressed  
#define CSK14_CS12_Pressed  TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define CSK14_CS13_Pressed  TouchKeyStatus.KEY_DOORCLOSE_Pressed	//

#elif defined (ProjectIs_AutoBarLock_S06Z04)
#define CSK14_SENS_CH0 		0x30		//KEY 8
#define CSK14_SENS_CH1 		0x30		//KEY 6
#define CSK14_SENS_CH2 		0x30		//KEY 4
#define CSK14_SENS_CH3 		0x30		//KEY 2
#define CSK14_SENS_CH4 		0x30		//KEY DOOR CLOSE
#define CSK14_SENS_CH5 		0x30		//KEY #
#define CSK14_SENS_CH6 		0x30		//KEY 0
#define CSK14_SENS_CH7 		0x30		//KEY *
#define CSK14_SENS_CH8 		0x30		//KEY 1
#define CSK14_SENS_CH9 		0x30		//KEY 3
#define CSK14_SENS_CH10 	0x30		//KEY 5
#define CSK14_SENS_CH11 	0x30		//KEY 7
#define CSK14_SENS_CH12 	0x30		//KEY 9
#define CSK14_SENS_CH13 	0x30		//
	
#define CSK14_SENS_SLEEP 0x30

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_EIGHT_Pressed  
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_SIX_Pressed  
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed  
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_ZERO_Pressed  
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed  
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_ONE_Pressed  
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_THREE_Pressed  
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_FIVE_Pressed  
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_SEVEN_Pressed  
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_NINE_Pressed   
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed

#elif defined (ProjectIs_AutoBarLock_S31Z01) || defined (ProjectIs_AutoBarLock_S67Z01)
#define CSK14_SENS_CH0 		0x60		//KEY 8
#define CSK14_SENS_CH1 		0x60		//KEY 6
#define CSK14_SENS_CH2 		0x60		//KEY 4
#define CSK14_SENS_CH3 		0x60		//KEY 2
#define CSK14_SENS_CH4 		0x60		//KEY DOOR CLOSE
#define CSK14_SENS_CH5 		0x60		//KEY #
#define CSK14_SENS_CH6 		0x60		//KEY 0
#define CSK14_SENS_CH7 		0x60		//KEY *
#define CSK14_SENS_CH8 		0x60		//KEY 1
#define CSK14_SENS_CH9 		0x60		//KEY 3
#define CSK14_SENS_CH10 	0x60		//KEY 5
#define CSK14_SENS_CH11 	0x60		//KEY 7
#define CSK14_SENS_CH12 	0x60		//KEY 9
#define CSK14_SENS_CH13 	0x60		//
	
#define CSK14_SENS_SLEEP 0x20

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_EIGHT_Pressed  
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_SIX_Pressed  
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_FIVE_Pressed  
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_THREE_Pressed  
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_SEVEN_Pressed   
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed   
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_NINE_Pressed  
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_ZERO_Pressed  
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed    
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_FOUR_Pressed   
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_ONE_Pressed    
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed     
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed

#elif defined ProjectIs_AutoBarLock_S11Z05
#define CSK14_SENS_CH0 		0x60		//
#define CSK14_SENS_CH1 		0x60		//
#define CSK14_SENS_CH2 		0x60		//
#define CSK14_SENS_CH3 		0x60		// 
#define CSK14_SENS_CH4 		0x60		//
#define CSK14_SENS_CH5 		0x60		//
#define CSK14_SENS_CH6 		0x60		//
#define CSK14_SENS_CH7 		0x60		//
#define CSK14_SENS_CH8 		0x60		//
#define CSK14_SENS_CH9 		0x60		//
#define CSK14_SENS_CH10 	0x60		//
#define CSK14_SENS_CH11 	0x60		//
#define CSK14_SENS_CH12 	0x60		//
#define CSK14_SENS_CH13 	0x60		//
	
#define CSK14_SENS_SLEEP 0x40

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_FIVE_Pressed   
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_EIGHT_Pressed    
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_ZERO_Pressed     
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed   
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_SIX_Pressed      
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_THREE_Pressed    
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_ONE_Pressed    
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_FOUR_Pressed  
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_SEVEN_Pressed    
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed    
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_TWO_Pressed    
#define CSK14_CS12_Pressed  TouchKeyStatus.KEY_DOORCLOSE_Pressed     
#define CSK14_CS13_Pressed  TouchKeyStatus.KEY_DOORBELL_Pressed



#elif defined (ProjectIs_AutoBarLock_S67Z02)
#define CSK14_SENS_CH0 		0x60		//KEY 8
#define CSK14_SENS_CH1 		0x60		//KEY 6
#define CSK14_SENS_CH2 		0x60		//KEY 4
#define CSK14_SENS_CH3 		0x60		//KEY 2
#define CSK14_SENS_CH4 		0x60		//KEY DOOR CLOSE
#define CSK14_SENS_CH5 		0x60		//KEY #
#define CSK14_SENS_CH6 		0x60		//KEY 0
#define CSK14_SENS_CH7 		0x60		//KEY *
#define CSK14_SENS_CH8 		0x60		//KEY 1
#define CSK14_SENS_CH9 		0x60		//KEY 3
#define CSK14_SENS_CH10 	0x60		//KEY 5
#define CSK14_SENS_CH11 	0x60		//KEY 7
#define CSK14_SENS_CH12 	0x60		//KEY 9
#define CSK14_SENS_CH13 	0x60		//
	
#define CSK14_SENS_SLEEP 0x20

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_THREE_Pressed  
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_TWO_Pressed   
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_ONE_Pressed    
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_SIX_Pressed    
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_FOUR_Pressed    
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_SEVEN_Pressed    
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_NINE_Pressed  
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_EIGHT_Pressed  
#define CSK14_CS9_Pressed  TouchKeyStatus.KEY_ASTERISK_Pressed    
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_ZERO_Pressed      
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed    
#define CSK14_CS12_Pressed  TouchKeyStatus.KEY_DOORCLOSE_Pressed     
#define CSK14_CS13_Pressed  TouchKeyStatus.KEY_DOORBELL_Pressed

#elif defined (ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S90Z11)
	
#define CSK14_SENS_CH0 		0x40		//
#define CSK14_SENS_CH1 		0x40		//
#define CSK14_SENS_CH2 		0x40		//
#define CSK14_SENS_CH3 		0x40		//
#define CSK14_SENS_CH4 		0x40		//
#define CSK14_SENS_CH5 		0x40		//
#define CSK14_SENS_CH6 		0x40		//
#define CSK14_SENS_CH7 		0x40		//
#define CSK14_SENS_CH8 		0x40		//
#define CSK14_SENS_CH9 		0x48		//
#define CSK14_SENS_CH10 	0x40		//
#define CSK14_SENS_CH11 	0x40		//
#define CSK14_SENS_CH12 	0x40		//
#define CSK14_SENS_CH13 	0xAA		//
	
#define CSK14_SENS_SLEEP 0x40

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_EIGHT_Pressed
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_FIVE_Pressed
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_TWO_Pressed
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed 
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_NINE_Pressed
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_SPARE_Pressed

#elif defined (ProjectIs_AutoBarLock_S86Z02)

#define CSK14_SENS_CH0 		0x48		//KEY 1
#define CSK14_SENS_CH1 		0x48		//KEY 4
#define CSK14_SENS_CH2 		0x40		//KEY 7
#define CSK14_SENS_CH3 		0x48		//KEY *
#define CSK14_SENS_CH4 		0x48		//KEY 2//KEY DOOR CLOSE
#define CSK14_SENS_CH5 		0x48		//KEY 5
#define CSK14_SENS_CH6 		0x40		//KEY 8
#define CSK14_SENS_CH7 		0x40		//KEY 0
#define CSK14_SENS_CH8 		0x48		//KEY 3
#define CSK14_SENS_CH9 		0x48		//KEY 6
#define CSK14_SENS_CH10 	0x40		//KEY 9
#define CSK14_SENS_CH11 	0x40		//KEY #
#define CSK14_SENS_CH12 	0xFF		//
#define CSK14_SENS_CH13 	0xFF		//

#define CSK14_SENS_SLEEP 0x40

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_SEVEN_Pressed 
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_THREE_Pressed
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_NINE_Pressed
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define CSK14_CS12_Pressed  TouchKeyStatus.KEY_DOORCLOSE_Pressed
#define CSK14_CS13_Pressed  TouchKeyStatus.KEY_DOORBELL_Pressed

#elif defined (ProjectIs_AutoBarLock_S90Z20)

#define CSK14_SENS_CH0 		0x50		//KEY #
#define CSK14_SENS_CH1 		0x50		//KEY 9
#define CSK14_SENS_CH2 		0x50		//KEY 6
#define CSK14_SENS_CH3 		0x50		//KEY 3
#define CSK14_SENS_CH4 		0x50		//KEY DOOR CLOSE
#define CSK14_SENS_CH5 		0x50		//KEY 0
#define CSK14_SENS_CH6 		0x50		//KEY 8
#define CSK14_SENS_CH7 		0x50		//KEY 5
#define CSK14_SENS_CH8 		0x50		//KEY 2
#define CSK14_SENS_CH9 		0x50		//KEY *
#define CSK14_SENS_CH10 	0x50		//KEY 7
#define CSK14_SENS_CH11 	0x50		//KEY 4
#define CSK14_SENS_CH12 	0x50		//KEY 1
#define CSK14_SENS_CH13 	0xF0		//

#define CSK14_SENS_SLEEP 0x40

#define CSK14_CS0_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed 
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_ZERO_Pressed 
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_TWO_Pressed
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define CSK14_CS13_Pressed  TouchKeyStatus.KEY_DOORCLOSE_Pressed

#else

#endif

#ifdef DEBUG_MODE

//#define Function_CSK14_DEBUG_NormalPower
//#define Function_CSK14_DEBUG_LowPower


static uint16_t CSK14_AllChannelSignalValue_40H_5BH[14];
static uint8_t CSK14_EFH_FCH[14];

static uint8_t CSK14_1AH;
static uint8_t CSK14_26H;
static uint8_t CSK14_2AH;
static uint8_t CSK14_2FH;
static uint8_t CSK14_EEH;
static uint8_t CSK14_29H;
static uint16_t CSK14_LPSCANDR;//0x5C,0x5D


#endif


static keycode_t NewKeycode,LastKeycode;
static uint16_t KeyHoldTimeCnt=0;

static uint8_t KEYSCANTIMECNT=0;
static uint8_t TouchKeyStatusBuff[5];

static uint8_t ScanModeRegVlaue;
static uint8_t ChannelEnRegVlaue[2];

bool_t g_TouchAwakeNeedReleaseKey;

static struct
{
	bool_t KEY_ZERO_Pressed;
	bool_t KEY_ONE_Pressed;
	bool_t KEY_TWO_Pressed;
	bool_t KEY_THREE_Pressed;
	bool_t KEY_FOUR_Pressed;
	bool_t KEY_FIVE_Pressed;
	bool_t KEY_SIX_Pressed;
	bool_t KEY_SEVEN_Pressed;
	bool_t KEY_EIGHT_Pressed;
	bool_t KEY_NINE_Pressed;
	bool_t KEY_POUNDSIGN_Pressed;
	bool_t KEY_ASTERISK_Pressed;
	bool_t KEY_DOORCLOSE_Pressed;
	bool_t KEY_DOORBELL_Pressed;	
	bool_t KEY_INSIDEBUTTON_Pressed;
	bool_t KEY_SLIDETOUCH_Pressed;
	bool_t KEY_SPARE_Pressed;
}TouchKeyStatus;

AwakeSystemKeyMgr_t AwakeSystemKeyMgr;

//static uint8_t TestVal;
static uint8_t TestBuff[20];

void Debug_CSK14_LowPowerConfig(void);


void TOUCH_HardwareReset(void)
{
	SET_KB_I2C_EN_L;
	Hardware_DelayMs(20);
	SET_KB_I2C_EN_H;
	Hardware_DelayMs(30);
}

void TOUCH_Init(void)
{
	enum{
			CSK14S = 0,
			CSK14T = 1,		//CSK14T 定制版
			CSK14T_S1,		//CSK14T 标准版
			CSK14UnkownType
		}CSK14_CHIPVERSION;
	static uint8_t i,CSK14_RegValue_BFH,CSK14_RegValue_FFH;

	TOUCH_HardwareReset();

	if (I2C_ReadRandom(I2CADDR_CSK14S,0xBF,&CSK14_RegValue_BFH)!= HAL_OK)
	{
		DEBUG_MARK;
		TOUCH_HardwareReset();	
		return;
	}

	if ( CSK14_RegValue_BFH == 0x00 )
	{
		CSK14_CHIPVERSION = CSK14S;
		DEBUG_MARK;
	}
	else //CSK14T
	{
		if (I2C_ReadRandom(I2CADDR_CSK14S,0xFF,&CSK14_RegValue_FFH)!= HAL_OK)
		{
			DEBUG_MARK;
			TOUCH_HardwareReset();	
			return;
		}	
		if (CSK14_RegValue_FFH == 0x15)
		{
			DEBUG_MARK;
			CSK14_CHIPVERSION = CSK14T;
		}
		else if ((CSK14_RegValue_FFH == 0x12)||( CSK14_RegValue_FFH == 0x13))
		{
			DEBUG_MARK;
			CSK14_CHIPVERSION = CSK14T_S1;
		}
		else
		{
			return;
		}
	}
	
	I2C_WriteRandom(I2CADDR_CSK14S,0xAF,0x3A);		//Enable write
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x00);		//Stop scan before change sensitivity
	DEF_CSK14_TimingDelay;

	
	I2C_WriteRandom(I2CADDR_CSK14S,0x12,0x08);		//SCAN CONFIG
//	I2C_WriteRandom(I2CADDR_CSK14S,0x12,0x10);		//SCAN CONFIG
	DEF_CSK14_TimingDelay;
	/*
			   7 6     5 4    3    2 1 0
	ScanCfg Reserved MultiOn DNPR NPRR[2:0]
	
	7:5 Reserved 必须写 0
	4 MultiOn 使能多个感应通道同时输出触摸结果。1：使能 0：禁用，缺省
	为 1
	3 DNPR 禁用噪声保护，如禁用噪声保护，则任何超出触摸信号阈值的检
	测结果都会被采用。1：禁止 0：使能 缺省为 0
	2:0 NPRR 噪声门限比例倍数，如果检测信号超出触摸信号阈值的 NPRR
	倍，则忽略此次检测结果。0：4 倍阈值 1:：6 倍阈值 … 7：18
	倍阈值缺省为 0
	客户在做灵敏度调整测试时建议先设置 DNPR 为 1，暂时关闭噪声保护，避免灵敏度设
	置过高误触发噪声保护。
	*/

	I2C_WriteRandom(I2CADDR_CSK14S,0x13,0x02);		//Debounce
	DEF_CSK14_TimingDelay;

	#if  defined (ProjectIs_AutoBarLock_S08Z05)\
		|| defined (ProjectIs_AutoBarLock_S51Z05) || defined (ProjectIs_AutoBarLock_S51Z06)\
		|| defined (ProjectIs_AutoBarLock_S64Z07) || defined (ProjectIs_AutoBarLock_S61Z11)\
		|| defined (ProjectIs_AutoBarLock_S64Z19)
	I2C_WriteRandom(I2CADDR_CSK14S,0x14,0xFF);		//Enable channel 0~7
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x15,0x3F);		//Enable channel 8~13
	DEF_CSK14_TimingDelay;	
	I2C_WriteRandom(I2CADDR_CSK14S,0xB0,0xff);		//Enable channel0~channel7 lp scan
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xB1,0x3f);		//Enable channel8~channel13 lp scan
	DEF_CSK14_TimingDelay;
	
	#elif defined (ProjectIs_AutoBarLock_S31Z01) || defined (ProjectIs_AutoBarLock_S67Z02)\
		|| defined (ProjectIs_AutoBarLock_S67Z01)|| defined ( ProjectIs_AutoBarLock_S11Z05)\
		|| defined (ProjectIs_AutoBarLock_S86Z02)
	I2C_WriteRandom(I2CADDR_CSK14S,0x14,0xFF);		//Enable channel 0~7
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x15,0x0F);		//Enable channel 8~11
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xB0,0xff);		//Enable channel0~channel7 lp scan
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xB1,0x0f);		//Enable channel8~channel12 lp scan
	DEF_CSK14_TimingDelay;
	
	#else
	I2C_WriteRandom(I2CADDR_CSK14S,0x14,0xFF);		//Enable channel 0~7
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x15,0x1F);		//Enable channel 8~12
	DEF_CSK14_TimingDelay;
	
	#if defined (ProjectIs_AutoBarLock_S90Z01)  || defined (ProjectIs_AutoBarLock_S90Z11)
	I2C_WriteRandom(I2CADDR_CSK14S,0xB0,0x43);		//Enable LP SCAN Key2,5,8,#,doorbell
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xB1,0x0A);		//
	DEF_CSK14_TimingDelay;		
	#else
	I2C_WriteRandom(I2CADDR_CSK14S,0xB0,0xff);		//Enable channel0~channel7 lp scan
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xB1,0x1f);		//Enable channel8~channel12 lp scan
	DEF_CSK14_TimingDelay;
	#endif
	
	#endif
	
	I2C_WriteRandom(I2CADDR_CSK14S,0x1E,0x70);		//Set Global sensitivity
	DEF_CSK14_TimingDelay;
	
	

	if ( CSK14_CHIPVERSION == CSK14S )
	{
		I2C_WriteRandom(I2CADDR_CSK14S,0x29,0x50);		//CSK14S,群扫描补偿电流 群扫描补偿电流器，针对休眠唤醒，不影响工作状态下的灵敏度
		DEF_CSK14_TimingDelay;
		DEBUG_MARK;
	}
	else if ( CSK14_CHIPVERSION == CSK14T )
	{
		#if defined (ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S90Z11)
		I2C_WriteRandom(I2CADDR_CSK14S,0x1A,0x1E);		//default value is 0x1E
		DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x29,0x28);		//CSK14T,群扫描补偿电流 群扫描补偿电流器，针对休眠唤醒，不影响工作状态下的灵敏度
		DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x26,0x0A);		//default is 0x0F
		DEF_CSK14_TimingDelay;
		
		#elif defined ProjectIs_AutoBarLock_S70Z01
		I2C_WriteRandom(I2CADDR_CSK14S,0x1A,0x28);		//default value is 0x1E
		DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x26,0x45);		//default is 0x0F
		DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x29,0x60);		//CSK14T,群扫描补偿电流 群扫描补偿电流器，针对休眠唤醒，不影响工作状态下的灵敏度
		DEF_CSK14_TimingDelay;
		
		#elif defined ProjectIs_AutoBarLock_S90Z20
		I2C_WriteRandom(I2CADDR_CSK14S,0x1A,0x30);		//default value is 0x1E
		DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x26,0x20);		//default is 0x0F
		DEF_CSK14_TimingDelay;		
		I2C_WriteRandom(I2CADDR_CSK14S,0x29,0x20);		//CSK14T,群扫描补偿电流 群扫描补偿电流器，针对休眠唤醒，不影响工作状态下的灵敏度
		DEF_CSK14_TimingDelay;
		
		#else
		//I2C_WriteRandom(I2CADDR_CSK14S,0x1A,0x1E);		//
		//DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x29,0x20);		//CSK14T,群扫描补偿电流 群扫描补偿电流器，针对休眠唤醒，不影响工作状态下的灵敏度
		DEF_CSK14_TimingDelay;
		#endif
		
		DEBUG_MARK;
	}
	else if ( CSK14_CHIPVERSION == CSK14T_S1 )
	{
		I2C_WriteRandom(I2CADDR_CSK14S,0x17,0x01);		//专用调试模式
		DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x1A,0x10);		//default value is 0x1E
		DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x2A,0x07);		//default value is 0x07
		DEF_CSK14_TimingDelay;
		I2C_WriteRandom(I2CADDR_CSK14S,0x2F,0x10);		//CSK14T标准升级版,群扫描补偿电流 群扫描补偿电流器，针对休眠唤醒，不影响工作状态下的灵敏度
		DEF_CSK14_TimingDelay;
		DEBUG_MARK;
	}

	I2C_WriteRandom(I2CADDR_CSK14S,0x2C,0x01);		//key
	DEF_CSK14_TimingDelay;

	I2C_WriteRandom(I2CADDR_CSK14S,0x30,0x1E);		//Low power delay,30S
	DEF_CSK14_TimingDelay;

	I2C_WriteRandom(I2CADDR_CSK14S,0x31,0x01);		//Scan Period, 16ms
	DEF_CSK14_TimingDelay;

	I2C_WriteRandom(I2CADDR_CSK14S,0x32,0xff);		//Awake throuhold, LP mode is software scan mode
	DEF_CSK14_TimingDelay;

  	I2C_WriteRandom(I2CADDR_CSK14S,0xE0,CSK14_SENS_CH0);		
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE1,CSK14_SENS_CH1);	
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE2,CSK14_SENS_CH2);		
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE3,CSK14_SENS_CH3);	
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE4,CSK14_SENS_CH4);		
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE5,CSK14_SENS_CH5);	
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE6,CSK14_SENS_CH6);		
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE7,CSK14_SENS_CH7);	
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE8,CSK14_SENS_CH8);		
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xE9,CSK14_SENS_CH9);	
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xEA,CSK14_SENS_CH10);		
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xEB,CSK14_SENS_CH11);	
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xEC,CSK14_SENS_CH12);		
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0xED,CSK14_SENS_CH13);	
	DEF_CSK14_TimingDelay;

	I2C_WriteRandom(I2CADDR_CSK14S,0xEE,CSK14_SENS_SLEEP);		//LP sensitivity
	DEF_CSK14_TimingDelay;


	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x01);		//Set scan mode to normal mode
	
	
	Hardware_DelayMs(100);  //上电复位后等待 80ms 再发扫描参数，芯片需要初始化一些模块。

	DEBUG_MARK;


	TouchPowerMgr.Status = HighSensitivity;

	
#ifdef Function_CSK14_DEBUG_LowPower
		
		Debug_CSK14_LowPowerConfig();
		
#endif

	
	DEBUG_MARK;
}

void SET_TOUCH_NO_SENSING(void)
{

	
	I2C_WriteRandom(I2CADDR_CSK14S,0xAF,0x3A);		//Enable write
	Hardware_DelayMs(1);
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x00);		//Stop scan before change sensitivity
	Hardware_DelayMs(1);
	I2C_WriteRandom(I2CADDR_CSK14S,0x14,0x00);		//
	Hardware_DelayMs(1);
	I2C_WriteRandom(I2CADDR_CSK14S,0x15,0x00);		//
	Hardware_DelayMs(1);
	I2C_WriteRandom(I2CADDR_CSK14S,0xB0,0x00);		//
	Hardware_DelayMs(1);
	I2C_WriteRandom(I2CADDR_CSK14S,0xB1,0x00);		//
	Hardware_DelayMs(1);
	I2C_WriteRandom(I2CADDR_CSK14S,0xEE,0xFF);		//
	Hardware_DelayMs(1);
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x41);		//power down
	Hardware_DelayMs(1);

	TouchPowerMgr.Status = NoSensing;

}

void SET_TOUCH_SLEEP_SENSITIVITY(void)
{
//return;
	I2C_WriteRandom(I2CADDR_CSK14S,0xAF,0x3A);		//Enable write
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x00);		//Stop scan before change sensitivity
	DEF_CSK14_TimingDelay;
	
	I2C_WriteRandom(I2CADDR_CSK14S,0xEE,CSK14_SENS_SLEEP);	
	DEF_CSK14_TimingDelay;
	
	I2C_WriteRandom(I2CADDR_CSK14S,0x30,0x05);		//Low power delay,1S
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x41);		//Set scan mode to Auto LP mode
	DEF_CSK14_TimingDelay;

	TouchPowerMgr.Status = LowSensitivity;


}
void SET_TOUCH_AWAKE_SENSITIVITY(void)
{
	uint8_t TempBuff[7];
	
	I2C_WriteRandom(I2CADDR_CSK14S,0xAF,0x3A);		//Enable write
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x00);		//Stop scan before change sensitivity
	DEF_CSK14_TimingDelay;
	
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x01);		//Set scan mode
	//I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x0D);		//Set scan mode to auto adjust mode

	TouchPowerMgr.Status = HighSensitivity;

	
}

void TOUCH_POWERDOWN(void)
{

	SET_TOUCH_SLEEP_SENSITIVITY();    
	DEF_CSK14_TimingDelay;
}

void TOUCH_AWAKE(void)
{

	static keycode_t KEY;
	
	Hardware_DelayMs(3);
		
	if ( SystemPowerMgr.AwakeSource == KeyBoardTouch )
	{

    	Key_Scan_Init();
		
		KEY = Key_Scan();
		
		AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsDoorBellKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsPoundsignKeyAwake = bFALSE;

		#if defined (ProjectIs_AutoBarLock_S31Z01) || defined (ProjectIs_AutoBarLock_S67Z02)\
		|| defined (ProjectIs_AutoBarLock_S67Z01) || defined ( ProjectIs_AutoBarLock_S11Z05)
		
		if ( KEY == KEY_POUNDSIGN )
		{
			AwakeSystemKeyMgr.IsDoorBellKeyAwake = bTRUE;
		}
		else if ( KEY == KEY_ASTERISK )
		{
			AwakeSystemKeyMgr.IsDoorColseKeyAwake = bTRUE;
			DEBUG_MARK;
		}
		
		#elif defined ( ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S90Z11)

		if ( KEY == KEY_DOORBELL )
		{
			AwakeSystemKeyMgr.IsDoorBellKeyAwake = bTRUE;
			DEBUG_MARK;
		}
		else if ( KEY == KEY_POUNDSIGN )
		{
			AwakeSystemKeyMgr.IsDoorColseKeyAwake = bTRUE;
			DEBUG_MARK;
		}

		
		#else
		
		if ( KEY == KEY_DOORBELL )
		{
			AwakeSystemKeyMgr.IsDoorBellKeyAwake = bTRUE;
			DEBUG_MARK;
		}
		else if ( KEY == KEY_DOORCLOSE )
		{
			AwakeSystemKeyMgr.IsDoorColseKeyAwake = bTRUE;
			DEBUG_MARK;
		}
		else if ( KEY == KEY_POUNDSIGN )
		{
			AwakeSystemKeyMgr.IsPoundsignKeyAwake = bTRUE;
			DEBUG_MARK;
		}
		#endif
		
		TOUCH_Init();  
		DEBUG_MARK;
	}
	else
	{
		TOUCH_Init();           //Touch Recalibration
		DEBUG_MARK;
	}

}

void TOUCH_SUSPEND(void)
{

}
void TOUCH_RESUME(void)
{

}

void Key_Scan_Init(void)
{

	TouchKeyStatus.KEY_ZERO_Pressed = bFALSE;
	TouchKeyStatus.KEY_ONE_Pressed = bFALSE;
	TouchKeyStatus.KEY_TWO_Pressed = bFALSE;
	TouchKeyStatus.KEY_THREE_Pressed = bFALSE;
	TouchKeyStatus.KEY_FOUR_Pressed = bFALSE;
	TouchKeyStatus.KEY_FIVE_Pressed = bFALSE;
	TouchKeyStatus.KEY_SIX_Pressed = bFALSE;
	TouchKeyStatus.KEY_SEVEN_Pressed = bFALSE;
	TouchKeyStatus.KEY_EIGHT_Pressed = bFALSE;
	TouchKeyStatus.KEY_NINE_Pressed = bFALSE;
	TouchKeyStatus.KEY_POUNDSIGN_Pressed = bFALSE;
	TouchKeyStatus.KEY_ASTERISK_Pressed = bFALSE;
	TouchKeyStatus.KEY_DOORCLOSE_Pressed = bFALSE;
	TouchKeyStatus.KEY_DOORBELL_Pressed = bFALSE;
	LastKeycode = NewKeycode = KEY_NONE;
}

#ifdef DEBUG_MODE

void CSK14_ParameterMonitor_NormalPower(void)
{
	uint8_t tempbuffer[28];
		uint8_t i;
		
	I2C_ReadSequential(I2CADDR_CSK14S,0x40,&tempbuffer[0],28);	
	for(i=0;i<28;i++)
	{
		CSK14_AllChannelSignalValue_40H_5BH[i]=tempbuffer[2*i]*256+tempbuffer[2*i+1];
	}
	
	I2C_ReadSequential(I2CADDR_CSK14S,0xEF,&CSK14_EFH_FCH[0],14);
	
	I2C_ReadRandom(I2CADDR_CSK14S,0x1A,&CSK14_1AH);
	I2C_ReadRandom(I2CADDR_CSK14S,0x2A,&CSK14_2AH);	

	
}


void Debug_CSK14_LowPowerConfig(void)
{

/********* low power scan test *****************/	
	I2C_WriteRandom(I2CADDR_CSK14S,0xAF,0x3A);		//Enable write
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x00);		//Stop scan before change sensitivity
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x30,0x05);		//Low power delay,5S
	DEF_CSK14_TimingDelay;
	
	I2C_WriteRandom(I2CADDR_CSK14S,0x29,0x20);		//
	DEF_CSK14_TimingDelay;
	I2C_WriteRandom(I2CADDR_CSK14S,0x26,0x20);		//default is 0x0F
	DEF_CSK14_TimingDelay;
	
	I2C_WriteRandom(I2CADDR_CSK14S,0xEE,0x40);		//
	DEF_CSK14_TimingDelay;
	
	I2C_WriteRandom(I2CADDR_CSK14S,0xFE,0x01);		//DEBUG MODE
	DEF_CSK14_TimingDelay;
	
	I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x41);		//Set scan mode to auto LP mode
	
	Hardware_DelayMs(2000);		//delay 5s

	DEBUG_MARK;
	DEBUG_MARK;

}

void CSK14_ParameterMonitor_LowPower(void)
{
	uint8_t tempbuffer[28];
	uint8_t i;
		
	
	I2C_ReadSequential(I2CADDR_CSK14S,0x5C,&tempbuffer[0],2);	
	CSK14_LPSCANDR = tempbuffer[0]*256+tempbuffer[1];
		
	I2C_ReadRandom(I2CADDR_CSK14S,0x1A,&CSK14_1AH);
	I2C_ReadRandom(I2CADDR_CSK14S,0x2A,&CSK14_2AH);	
	I2C_ReadRandom(I2CADDR_CSK14S,0x26,&CSK14_26H);	
	I2C_ReadRandom(I2CADDR_CSK14S,0x29,&CSK14_29H);
	I2C_ReadRandom(I2CADDR_CSK14S,0x2F,&CSK14_2FH);
	I2C_ReadRandom(I2CADDR_CSK14S,0xEE,&CSK14_EEH);	
	
}
#endif

keycode_t Key_Scan(void)
{
	
	keycode_t PostKeycode;
	static uint8_t CSK14_RegValue_ScanCR,CHECKSUM;

#ifdef Function_CSK14_DEBUG_NormalPower
	
	CSK14_ParameterMonitor_NormalPower();
	
#endif
	
#ifdef Function_CSK14_DEBUG_LowPower
	
	CSK14_ParameterMonitor_LowPower();
	return KEY_NONE;
#endif	
	
	if (I2C_ReadRandom(I2CADDR_CSK14S,0x1F,&CSK14_RegValue_ScanCR)!= HAL_OK)
	{
		TOUCH_Init();
		DEBUG_MARK;
		return KEY_NONE;
	}
	if ((CSK14_RegValue_ScanCR&0x01) == 0x00 )		//scan stop
	{
		TOUCH_Init();
		DEBUG_MARK;
		return KEY_NONE;
	}

	I2C_ReadSequential(I2CADDR_CSK14S,0x22,&TouchKeyStatusBuff[0],4);	

	if ((TouchKeyStatusBuff[0]!=0x00)
		||(TouchKeyStatusBuff[1]!=0x00)
		)
	{
		I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x41);		//Clear IRQ
	}
	else
	{
		if ( SystemPowerMgr.AwakeTimerCnt < 16 ){
			SystemPowerMgr.AwakeTimerCnt++;
		}
		else{
//			g_TouchAwakeNeedReleaseKey = bFALSE;
		}
	}

	CHECKSUM = TouchKeyStatusBuff[3]+TouchKeyStatusBuff[0]+TouchKeyStatusBuff[1]+0xC5;	
	if ( CHECKSUM != 0X00 )
	{
		DEBUG_MARK;
		//return KEY_NONE;
	}


	if ((TouchKeyStatusBuff[0]&0x01) != 0x00 ){
		CSK14_CS0_Pressed = bTRUE;
		DEBUG_MARK;
	}
	else{
		CSK14_CS0_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x02) != 0x00 ){
		CSK14_CS1_Pressed = bTRUE;
	}
	else{
		CSK14_CS1_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x04) != 0x00 ){
		CSK14_CS2_Pressed = bTRUE;
	}
	else{
		CSK14_CS2_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x08) != 0x00 ){
		CSK14_CS3_Pressed = bTRUE;
	}
	else{
		CSK14_CS3_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x10) != 0x00 ){
		CSK14_CS4_Pressed = bTRUE;
	}
	else{
		CSK14_CS4_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x20) != 0x00 ){
		CSK14_CS5_Pressed = bTRUE;
	}
	else{
		CSK14_CS5_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x40) != 0x00 ){
		CSK14_CS6_Pressed = bTRUE;
	}
	else{
		CSK14_CS6_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x80) != 0x00 ){
		CSK14_CS7_Pressed = bTRUE;
	}
	else{
		CSK14_CS7_Pressed = bFALSE;
	}
	
	if ((TouchKeyStatusBuff[1]&0x01) != 0x00 ){
		CSK14_CS8_Pressed = bTRUE;
	}
	else{
		CSK14_CS8_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x02) != 0x00 ){
		CSK14_CS9_Pressed = bTRUE;
	}
	else{
		CSK14_CS9_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x04) != 0x00 ){
		CSK14_CS10_Pressed = bTRUE;
	}
	else{
		CSK14_CS10_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x08) != 0x00 ){
		CSK14_CS11_Pressed = bTRUE;
	}
	else{
		CSK14_CS11_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x10) != 0x00 ){
		CSK14_CS12_Pressed = bTRUE;
	}
	else{
		CSK14_CS12_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x20) != 0x00 ){
		CSK14_CS13_Pressed = bTRUE;
	}
	else{
		CSK14_CS13_Pressed = bFALSE;
	}

		
	if ( (TouchKeyStatus.KEY_ZERO_Pressed == bTRUE )&&(LastKeycode == KEY_ZERO)){
		NewKeycode = KEY_ZERO;
	}
	else if ( (TouchKeyStatus.KEY_ONE_Pressed == bTRUE )&&(LastKeycode == KEY_ONE)){
		NewKeycode = KEY_ONE;
	}
	else if ( (TouchKeyStatus.KEY_TWO_Pressed == bTRUE )&&(LastKeycode == KEY_TWO)){
		NewKeycode = KEY_TWO;
	}
	else if ( (TouchKeyStatus.KEY_THREE_Pressed == bTRUE )&&(LastKeycode == KEY_THREE)){
		NewKeycode = KEY_THREE;
	}
	else if ( (TouchKeyStatus.KEY_FOUR_Pressed == bTRUE )&&(LastKeycode == KEY_FOUR)){
		NewKeycode = KEY_FOUR;
	}
	else if ( (TouchKeyStatus.KEY_FIVE_Pressed == bTRUE )&&(LastKeycode == KEY_FIVE)){
		NewKeycode = KEY_FIVE;
	}
	else if ( (TouchKeyStatus.KEY_SIX_Pressed == bTRUE )&&(LastKeycode == KEY_SIX)){
		NewKeycode = KEY_SIX;
	}
	else if ( (TouchKeyStatus.KEY_SEVEN_Pressed == bTRUE )&&(LastKeycode == KEY_SEVEN)){
		NewKeycode = KEY_SEVEN;
	}
	else if ( (TouchKeyStatus.KEY_EIGHT_Pressed == bTRUE )&&(LastKeycode == KEY_EIGHT)){
		NewKeycode = KEY_EIGHT;
	}
	else if ( (TouchKeyStatus.KEY_NINE_Pressed == bTRUE )&&(LastKeycode == KEY_NINE)){
		NewKeycode = KEY_NINE;
	}
	else if ( (TouchKeyStatus.KEY_POUNDSIGN_Pressed == bTRUE )&&(LastKeycode == KEY_POUNDSIGN)){
		NewKeycode = KEY_POUNDSIGN;
	}
	else if ( (TouchKeyStatus.KEY_ASTERISK_Pressed == bTRUE )&&(LastKeycode == KEY_ASTERISK)){
		NewKeycode = KEY_ASTERISK;
	}
	else if ( (TouchKeyStatus.KEY_DOORCLOSE_Pressed == bTRUE )&&(LastKeycode == KEY_DOORCLOSE)){
		NewKeycode = KEY_DOORCLOSE;
	}
	else if ( (TouchKeyStatus.KEY_DOORBELL_Pressed == bTRUE )&&(LastKeycode == KEY_DOORBELL)){
		NewKeycode = KEY_DOORBELL;
	}
	
	else if ( TouchKeyStatus.KEY_ZERO_Pressed == bTRUE ){
		NewKeycode = KEY_ZERO;
	}
	else if ( TouchKeyStatus.KEY_ONE_Pressed == bTRUE ){
		NewKeycode = KEY_ONE;
	}
	else if ( TouchKeyStatus.KEY_TWO_Pressed == bTRUE ){
		NewKeycode = KEY_TWO;
	}
	else if ( TouchKeyStatus.KEY_THREE_Pressed == bTRUE ){
		NewKeycode = KEY_THREE;
	}
	else if ( TouchKeyStatus.KEY_FOUR_Pressed == bTRUE ){
		NewKeycode = KEY_FOUR;
	}
	else if ( TouchKeyStatus.KEY_FIVE_Pressed == bTRUE ){
		NewKeycode = KEY_FIVE;
	}
	else if ( TouchKeyStatus.KEY_SIX_Pressed == bTRUE ){
		NewKeycode = KEY_SIX;
	}
	else if ( TouchKeyStatus.KEY_SEVEN_Pressed == bTRUE ){
		NewKeycode = KEY_SEVEN;
	}
	else if ( TouchKeyStatus.KEY_EIGHT_Pressed == bTRUE ){
		NewKeycode = KEY_EIGHT;
	}
	else if ( TouchKeyStatus.KEY_NINE_Pressed == bTRUE ){
		NewKeycode = KEY_NINE;
	}
	else if ( TouchKeyStatus.KEY_POUNDSIGN_Pressed == bTRUE ){
		NewKeycode = KEY_POUNDSIGN;
	}
	else if ( TouchKeyStatus.KEY_ASTERISK_Pressed == bTRUE ){
		NewKeycode = KEY_ASTERISK;
	}
	else if ( TouchKeyStatus.KEY_DOORCLOSE_Pressed == bTRUE ){
		NewKeycode = KEY_DOORCLOSE;
	}
	else if ( TouchKeyStatus.KEY_DOORBELL_Pressed == bTRUE ){
		NewKeycode = KEY_DOORBELL;
	}
	
	else {
		NewKeycode = KEY_NONE;
	}	

	LEDsMgr.Keycode = NewKeycode;



	if ( NewKeycode != KEY_NONE )
	{
		if ( NewKeycode !=LastKeycode )
		{
			if (NewKeycode != KEY_INSIDEBUTTON)
			{
				PostKeycode = NewKeycode;
			}
			else{
				PostKeycode = KEY_NONE; 
				DEBUG_MARK;
			}
			LastKeycode = NewKeycode;
			KeyHoldTimeCnt=0;
		}
		else
		{
			if ( KeyHoldTimeCnt < Def_KeyHoldLongTimeLimited )
			{
				KeyHoldTimeCnt++;
				PostKeycode = KEY_NONE;			
			}

			if ( KeyHoldTimeCnt > Def_KeyHoldTimeLimited )
			{
				if ((KeyHoldTimeCnt == Def_KeyHoldLongTimeLimited )&&( NewKeycode == KEY_INSIDEBUTTON))
				{
					PostKeycode = KEY_INSIDEBUTTON_HOLD_LONG;				
				}
				/*
				else if ( KEYSCANTIMECNT%4==0)
				{
					if ( NewKeycode == KEY_ASTERISK)
					{
						PostKeycode = KEY_ASTERISK_HOLD;				
					}
					else if ( NewKeycode == KEY_POUNDSIGN)
					{
						PostKeycode = KEY_POUNDSIGN_HOLD;
					}
					else if ( NewKeycode == KEY_TWO)
					{
						PostKeycode = KEY_TWO_HOLD;				
					}
					else if ( NewKeycode == KEY_EIGHT)
					{
						PostKeycode = KEY_EIGHT_HOLD;
					}
					else
					{
						PostKeycode = KEY_NONE;
					}
				}
				*/
				else{

					PostKeycode = KEY_NONE;
				}
				
				if ( ++KEYSCANTIMECNT > 127 ){
					KEYSCANTIMECNT=0;
				}
			}
			else{
				PostKeycode = KEY_NONE;
			}
		}
	}
	else
	{
		if (LastKeycode == KEY_INSIDEBUTTON){
			PostKeycode = LastKeycode;
		}
		else{
			PostKeycode = KEY_NONE;	
		}
		LastKeycode = NewKeycode;
		KeyHoldTimeCnt=0;
	}

	if ( AwakeSystemKeyMgr.IsDoorBellKeyAwake == bTRUE )
	{
		PostKeycode = KEY_DOORBELL;
		AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsDoorBellKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsPoundsignKeyAwake = bFALSE;
	}
	else if ( AwakeSystemKeyMgr.IsDoorColseKeyAwake == bTRUE )
	{
		PostKeycode = KEY_DOORCLOSE;
		AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsDoorBellKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsPoundsignKeyAwake = bFALSE;
	}
	else if ( AwakeSystemKeyMgr.IsPoundsignKeyAwake == bTRUE )
	{
		PostKeycode = KEY_POUNDSIGN;
		AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsDoorBellKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsPoundsignKeyAwake = bFALSE;
	}

	
	return PostKeycode;

}

#endif


