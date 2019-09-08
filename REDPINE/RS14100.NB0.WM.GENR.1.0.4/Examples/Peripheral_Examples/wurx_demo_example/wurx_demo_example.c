/**
 * @file       wurx_demo_example.c
 * @version    1.0
 * @date       18 Augest 2018
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contains example describing usage of wurx APIs.
 *
 * @section Description
 * Detect the l2 frequcency pattern here l2 as 64 bit pattern length.
 */
 /* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define SOC_OPER_FREQUENCY              32000000
#define RSI_BLINK_RATE                  1000  
#define NPSS_CMP_WRF_IRQHandler         IRQ022_Handler      /*<! Comparator IRQ Handler              */
#define NVIC_CMP_WRF                    NPSS_TO_MCU_CMP_RF_WKP_INTR_IRQn /*<! CMP NVIC enable        */
#define L1_BYPASS_ENABLE                1
#define L1_RECEIVE_FREQ_FAC             4       
#define L2_RECEIVE_FREQ_FAC             3
#define IMPU_CALIB_CHANL_FREQ           0x780   /* channel1 frequency */
#define L1_PATTERN_LENGTH_SELECT        0
#define L2_PATTERN_LENGTH_SELECT        7
#define PATTERN_LENGTH                  64
#define THRESHOLD_PERCENTAGE            75
#define STACK_POINTER                   0xc00
#define IVT_OFFSET                      0x8012000
#define MODE                            1
#define NPSS_GPIO_PIN                   3

uint32_t Match_value[3]={0xC78E1,0x30319D,0x2783C7};
volatile uint32_t 		_dwTickCount=0 ;		//systick cout variable
volatile uint16_t ret,pattern_type,threshold1;
void hardware_setup(void);

/**
 * @brief  Systic handler use for generation delay for XTAL clock enable    
 * @param  None   
 * @retval None
 */
void SysTick_Handler(void)
{
	_dwTickCount++;

}


/**
 * @brief  Get tick count.    
 * @param  None   
 * @retval return the count of tick.
 */
uint32_t GetTickCount( void )
{
	return _dwTickCount ;						// gets the tick count from systic ISR
}

/**
 * @brief  This callback function for generation delay for XTAL clock.    
 * @param  Number of delay in us.   
 * @retval None.
 */
void rsi_delay(uint32_t val)
{
  uint32_t start;
	ret=val;
	val=val/1000;
	if (val == 0)
		return;
	 start = GetTickCount();
	do {
	} while (GetTickCount() - start < val);
	return ;
}

/**
 * @brief  Wurx interrupt handler for clearing wurx interrupt.    
 * @param  None. 
 * @retval None.
 */
void NPSS_CMP_WRF_IRQHandler(void)
{
	uint32_t wakeUpSrc = 0 ;
	wakeUpSrc = RSI_PS_GetWkpUpStatus();
  RSI_Board_Init();
  pattern_type=RSI_WURX_GetPatternType();	
	if(pattern_type==1)
	{
	   RSI_Board_LED_Toggle(0);
	}	
	if(wakeUpSrc & NPSS_TO_MCU_RFWAKEUP_INTR)
	{
	
		/*Clear wake up interrupt */
		RSI_PS_ClrWkpUpStatus(NPSS_TO_MCU_RFWAKEUP_INTR);

		RSI_WURX_CorrEnable(1);
		/* npss gpio 3*/
	  RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN,1U);	
    RSI_WURX_SoftwareRestart();
	  RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN,0U);
		/*Enable power supply*/
		RSI_PS_PowerSupplyEnable(ENABLE_WURX_DETECTION);
	}
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main()
 { 
	/* At this stage the MICROCONTROLLER clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs1xxxx.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs1xxxx.c file
	 */		 
		SystemCoreClockUpdate();
	 
	  /* Change the supply to SCDC to LDO mode */ 		
		RSI_IPMU_ProgramConfigData(hp_ldo_voltsel);
	 
		/*Set ulp_wakeup*/
		RSI_PS_EnableFirstBootUp(1);   	
	 
		RSI_IPMU_ClockMuxSel(1); 
	 
		SysTick_Config(SOC_OPER_FREQUENCY/ RSI_BLINK_RATE);
	 
		RSI_ULPSS_EnableRefClks(MCU_ULP_32KHZ_XTAL_CLK_EN,ULP_PERIPHERAL_CLK,rsi_delay);
	 
		RSI_PS_FsmLfClkSel(KHZ_XTAL_CLK_SEL);
	 
		RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);
			
		RSI_Board_LED_Set(0,0);
		RSI_Board_LED_Set(1,1);
		RSI_Board_LED_Set(2,1);		
		
		RSI_NPSSGPIO_InputBufferEn(NPSS_GPIO_PIN , 1U);
	  RSI_NPSSGPIO_SetPinMux(NPSS_GPIO_PIN , 0);
  	RSI_NPSSGPIO_SetDir(NPSS_GPIO_PIN , NPSS_GPIO_DIR_OUTPUT);	
	
  	RSI_WURX_Init(L1_BYPASS_ENABLE,L1_RECEIVE_FREQ_FAC,L2_RECEIVE_FREQ_FAC);

		RSI_IPMU_40MhzClkCalib(1,IMPU_CALIB_CHANL_FREQ);
		
		RSI_WURX_SetPatternLength(1,L1_PATTERN_LENGTH_SELECT,L2_PATTERN_LENGTH_SELECT);
		
		threshold1=RSI_WURX_CalThershValue(PATTERN_LENGTH,THRESHOLD_PERCENTAGE); 
		
		RSI_WURX_SetWakeUpThreshold(threshold1,threshold1);

		RSI_WURX_Pattern1MatchValue(Match_value);
		
		RSI_WURX_CorrEnable(1);
		
		RSI_IPMU_DCCalib();

    RSI_PS_NpssIntrUnMask(NPSS_TO_MCU_RFWAKEUP_INTR);
	 
		NVIC_EnableIRQ(NVIC_CMP_WRF);
		
		hardware_setup();
		
		while(1)
		{
		  RSI_PS_RetentionSleepConfig(STACK_POINTER , (uint32_t)RSI_PS_RestoreCpuContext, IVT_OFFSET , 1);		
		  RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,LF_32_KHZ_RC);				
		}

}
