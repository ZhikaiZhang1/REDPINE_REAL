#include "rsi_temp_sensor.h"
#include "base_types.h"
#include "rsi_chip.h"
#include "rsi_board.h"
#include<math.h>

#define SIZE         15

#define NPSS_RTC_IRQ_Handler    IRQ029_Handler
#define NVIC_RTC MCU_CAL_RTC_IRQn

#define NOMI_TEMPERATURE     25
#define MSB_BIT              31

RTC_TIME_CONFIG_T rtcConfig;
uint32_t Temp,temp_read,f1,f2,f2_count;
int32_t Temp_negative;

char binary[SIZE + 1], onesComp[SIZE + 1], twosComp[SIZE + 1];
int16_t decimal_val,base=1;

/*two's complement conversion function*/
void temp_conversion(uint32_t temp)
{
	uint8_t i=0,rem,carry=1;
	uint8_t num=0;
	decimal_val=0,base=1;
	while(temp>0)
	{
		binary[i] = temp % 2;
		temp = temp / 2;
		i++;
	}

	for(i=0;i<MSB_BIT;i++)
	{
		if(binary[i]==0)
			onesComp[i]=1;
		else
			onesComp[i]=0;
	}

	for(i=0; i<MSB_BIT; i++)
	{
		if(onesComp[i] == 1&& (carry == 1))
		{
			twosComp[i] = 0;
		}
		else if((onesComp[i] == 0) && (carry == 1))
		{
			twosComp[i] = 1;
			carry = 0;
		}
		else
		{
			twosComp[i] = onesComp[i];
		}
	}

	for(i=0; i<MSB_BIT; i++)
	{ 
		num=twosComp[i];
		rem = num % 10;
		decimal_val = decimal_val + rem * base;
		base = base * 2;
	}
}

void NPSS_RTC_IRQ_Handler(void)
{
	volatile uint32_t statusRead = 0;

	statusRead = RSI_RTC_GetIntrStatus();

	if(statusRead & NPSS_TO_MCU_SEC_INTR){
		RSI_RTC_IntrClear (NPSS_TO_MCU_SEC_INTR);
	}
	/*read the temperature*/
	Temp=RSI_TS_ReadTemp(MCU_TEMP);

	rtcConfig.DayOfWeek        = Saturday;
	rtcConfig.MilliSeconds     = 999;
	rtcConfig.Century          = 0;
	rtcConfig.Day              = 18;
	rtcConfig.Hour             = 2;
	rtcConfig.Minute           = 20;
	rtcConfig.Month            = January;
	rtcConfig.Second           = 0 ;
	rtcConfig.Year             = 16;

	RSI_RTC_SetDateTime(RTC ,&rtcConfig);

	if(Temp & BIT(10))
	{
		/*read the f2 count*/
		f2=RSI_TS_GetPtatClkCnt(MCU_TEMP);
		/*substract f2_nominal count from f2_count NOTE:configure the f2_nominal
		  temp_read=(f2-f2_nominal)*/
		temp_read=(f2-RSI_IPMU_RO_TsEfuse());
		if(temp_read & BIT(31))
		{
			temp_conversion(temp_read);
			/*calculate the negative temperature*/
			Temp_negative=(25-(decimal_val * 1.3437));
#ifdef DEBUG_UART
			DEBUGOUT("temp_read=%d   ",Temp_negative);
#endif	
		}
	}
	else{
#ifdef DEBUG_UART
		DEBUGOUT("temp_read=%d  ",Temp);
#endif
	}
	return ;
}

int main()
{
	uint32_t cntr;
	SystemCoreClockUpdate();
	#ifdef DEBUG_UART
	DEBUGINIT();
#endif

	/*Enable clock sources*/
	RSI_IPMU_ClockMuxSel(1);
	RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);
	/*configure the slope,nominal temperature and f2_nominal*/	
	RSI_TS_Config(MCU_TEMP,NOMI_TEMPERATURE);
	/*disable the bjt based temp sensor*/
	RSI_TS_RoBjtEnable(MCU_TEMP,0);	
	/*Enable the RO based temp sensor*/
	RSI_TS_Enable(MCU_TEMP,1);
	/*update the temperature periodically*/
	RSI_Periodic_TempUpdate(TIME_PERIOD,1,0);
	/*read the reference clk count*/
	f1=RSI_TS_GetRefClkCnt(MCU_TEMP);
	/*read the ptat clk count*/
	f2=RSI_TS_GetPtatClkCnt(MCU_TEMP);

	/*Init RTC*/
	RSI_RTC_Init(RTC);


	/*RTC configuration with some default time */
	rtcConfig.DayOfWeek        = Saturday;
	rtcConfig.MilliSeconds     = 999;
	rtcConfig.Century          = 0;
	rtcConfig.Day              = 18;
	rtcConfig.Hour             = 2;
	rtcConfig.Minute           = 20;
	rtcConfig.Month            = January;
	rtcConfig.Second           = 0 ;
	rtcConfig.Year             = 16;

	/*start RTC */
	RSI_RTC_Start(RTC);
	/*Set the RTC configuration*/
	RSI_RTC_SetDateTime(RTC ,&rtcConfig);
	/*Enable RTC second based interrupt*/
	RSI_RTC_IntrUnMask( RTC_SEC_INTR );
	/*Start RTC */
	RSI_RTC_Start(RTC);
	/*Enable NVIC for second based interrupts */  
	NVIC_EnableIRQ(NVIC_RTC);

	while(1){
		/*Wait for interrupt */
		__WFI();
	}

	return 0;
}
