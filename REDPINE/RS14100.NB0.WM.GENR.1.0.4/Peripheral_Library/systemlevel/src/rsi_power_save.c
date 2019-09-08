/**
 * @file       rsi_power_save.c
 * @version    0.1
 * @date       1 AUG 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes related to power save
 *
 * @section Description
 * This file contains the list of function prototypes for the Power save and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */
#include "rsi_chip.h"
#define FPGA_MODE 0

uint32_t rsi_select_ta_boot_option(uint32_t option);
void rsi_goto_sleep_with_retention(uint32_t stack_address,
		uint32_t jump_cb_address,
		uint32_t vector_offset,
		uint32_t mode);
void rsi_wakeup_wireless_processor(void);
/* This API is used to used to change the power transition state from Power save state 4/3 to power save state 2 */

error_t ps_power_state_change_ps4tops2
(
		ULP_MODE_T enCtxSel        ,
		uint8_t PwrMuxSelUlpssRam    ,
		uint8_t pwrMuxSelM4UlpRam    ,
		uint8_t pwrMuxSelM4UlpRam16K ,
		uint8_t pwrMuxSelM4Ulp       ,
		uint8_t pwrMuxSelUlpss       ,
		uint8_t bgSampleEnable       ,
		uint8_t dcDcEnable           ,
		uint8_t socLdoEnable         ,
		uint8_t standByDc
)
{
	volatile int x=0;

	/*return error if this function is called from PS2*/
	if(M4_ULP_SLP_STATUS_REG & ULP_MODE_SWITCHED_NPSS){
		return ERROR_PS_INVALID_STATE;
	}

	/*Static clock enable m4 core in ULP mode*/
	M4LP_CTRL_REG |= ULP_M4_CORE_CLK_ENABLE;

	/*flushes the transactions in the Bridge before transition to PS2 state*/
	x= ULPCLK->ULP_TA_CLK_GEN_REG;
	/*Select the MCU mode*/
	MCU_FSM->MCU_FSM_PERI_CONFIG_REG_b.M4SS_CONTEXT_SWITCH_TOP_ULP_MODE = enCtxSel;

	/*Avoid the junk system bus access*/
	for (x = 0; x < 10; x++) {
		__ASM("NOP");
	}
	/*
	Indicates the status of functional switching to ULP Mode operation
	0 - PS4 state
	1 - PS2 state
	 */
	do{
		/*wait for status of functional switching to ULP Mode operation*/
		x++;
	}while((M4_ULP_SLP_STATUS_REG & ULP_MODE_AFTR_CLK_SW) != ULP_MODE_AFTR_CLK_SW);

	/*Select value for ULPSS RAM Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_ULPSS_RAM =PwrMuxSelUlpssRam;
	/*Select value for M4 ULP RAM Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_M4_ULP_RAM =pwrMuxSelM4UlpRam;
	/*Select value for M4 ULP RAM 16K Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_M4_ULP_RAM_16K =pwrMuxSelM4UlpRam16K;
	/*Select value for M4 ULP (Peripherals + Cortex Core )Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_M4_ULP =pwrMuxSelM4Ulp;
	/*Select value for ULPSS(Peripherals) Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_ULPSS =pwrMuxSelUlpss;

	/*update the SOC LDO*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.FSM_PERI_SOC_LDO_EN = socLdoEnable;

	if(socLdoEnable == 1){
		MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.FSM_PERI_DCDC_EN = 1;
		MCU_FSM->MCU_FSM_PERI_CONFIG_REG_b.BGPMU_SAMPLING_EN_R = 0;
	}

	else{
		MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.FSM_PERI_DCDC_EN = dcDcEnable;
	}

	if(dcDcEnable == 1){
		MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.FSM_PERI_DCDC_EN = dcDcEnable;
		MCU_FSM->MCU_FSM_PERI_CONFIG_REG_b.BGPMU_SAMPLING_EN_R = 0;
	}
	else {
		MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.FSM_PERI_DCDC_EN = dcDcEnable;
		MCU_FSM->MCU_FSM_PERI_CONFIG_REG_b.BGPMU_SAMPLING_EN_R = bgSampleEnable;
	}

	if((socLdoEnable == 0) && (dcDcEnable == 1)){
		MCU_FSM->MCU_FSM_PMU_STATUS_REG_b.STANDBY_DC1P3_R = standByDc;
	}
	/*Set the LDO turn off delay*/

	/*PMU off delay */
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.PS2_PMU_LDO_OFF_DELAY = PMU_LDO_OFF_DELAY;

	/*Bridge clock disable*/
	ULPCLK->ULP_TA_CLK_GEN_REG_b.ULP2M4_A2A_BRDG_CLK_EN_b = 0;
	/*Set ulp mode isolation */
	BATT_FF->M4_ULP_MODE_CONFIG = (BATT_FF->M4_ULP_MODE_CONFIG | 0x3F);
	
	/* Setting RETN_LDO voltage to 0.75V */
	RSI_IPMU_RetnLdo0p75();
	
	/* enabling the RETN_LDO HP MODE */
	RSI_IPMU_RetnLdoHpmode();
		
	/*Enable the ULP mode */
	MCU_FSM->MCU_FSM_PERI_CONFIG_REG_b.ULP_MCU_MODE_EN = 1;

	/*	Wait for Status bit indicating Physical transition from PS2 to PS4*/
	do{
		/*wait for  status of Physical switching to ULP Mode operation*/
		x++;
	}while((M4_ULP_SLP_STATUS_REG & ULP_MODE_SWITCHED_NPSS) != ULP_MODE_SWITCHED_NPSS);

	return RSI_OK;
}


/*  This API is used to change the power state from PS2 to PS4 */
error_t ps_power_state_change_ps2_to_Ps4(uint32_t PmuBuckTurnOnWaitTime , uint32_t SocLdoTurnOnWaitTime)
{
	/*Return if this is issues in PS4 state */
	if(!(M4_ULP_SLP_STATUS_REG & ULP_MODE_SWITCHED_NPSS)){
		return ERROR_PS_INVALID_STATE;
	}
	/*Disables clock to ULP-M4SS AHB-AHB Bridge since the MCU is in PS2 state logically*/
	ULPCLK->ULP_TA_CLK_GEN_REG_b.ULP2M4_A2A_BRDG_CLK_EN_b = 0;
	/*Select value for ULPSS RAM Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_ULPSS_RAM =_SOC_LDO;
	/*Select value for M4 ULP RAM Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_M4_ULP_RAM =_SOC_LDO;
	/*Select value for M4 ULP RAM 16K Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_M4_ULP_RAM_16K =_SOC_LDO;
	/*Select value for M4 ULP (Peripherals + Cortex Core )Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_M4_ULP =_SOC_LDO;
	/*Select value for ULPSS(Peripherals) Power Mux*/
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.POWER_MUX_SEL_ULPSS =0x01;

	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.PG4_BUCK_ON_DELAY = PmuBuckTurnOnWaitTime;
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.PS4_SOCLDO_ON_DELAY = SocLdoTurnOnWaitTime;

	MCU_FSM->MCU_FSM_PMU_STATUS_REG_b.STANDBY_DC1P3_R = 0;

	/*Disable the ULP mode */
	MCU_FSM->MCU_FSM_PERI_CONFIG_REG_b.ULP_MCU_MODE_EN = 0;
	// NOTE :  nopes issue issue is fixed by adding this bit
	/*	Wait for Status bit indicating Physical transition from PS2 to PS4*/
	while((M4_ULP_SLP_STATUS_REG & ULP_MODE_SWITCHED_NPSS));

	/*Disable ulp mode isolation */
	BATT_FF->M4_ULP_MODE_CONFIG= ((BATT_FF->M4_ULP_MODE_CONFIG) &  ~(0x3F));
	/*Disables clock to ULP-M4SS AHB-AHB Bridge since the MCU is in PS2 state logically*/
	ULPCLK->ULP_TA_CLK_GEN_REG_b.ULP2M4_A2A_BRDG_CLK_EN_b = 1;

	/*Select the MCU mode*/
	MCU_FSM->MCU_FSM_PERI_CONFIG_REG_b.M4SS_CONTEXT_SWITCH_TOP_ULP_MODE = 0;
	// NOTE :  nopes issue issue is fixed by adding this bit
	/*	Wait for Status bit indicating Functional transition from PS4 to PS2*/
	while((M4_ULP_SLP_STATUS_REG & ULP_MODE_AFTR_CLK_SW));

  /* Setting RETN_LDO voltage to 0.7V */
	RSI_IPMU_RetnLdoVoltsel();
	
	/* enabling the RETN_LDO LP MODE  */
	RSI_IPMU_RetnLdoLpmode();
	
	return RSI_OK;
}

/* 	   This API is used clear the NPSS/wake up interrupts. */
void ps_clr_wkp_up_status(uint32_t wakeUpIntrClear)
{
	if(wakeUpIntrClear & NPSS_TO_MCU_WDT_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_WDT_INTR;
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR = WWD_INTERRUPT_STATUS_CLEAR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_GPIO_INTR_0){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_GPIO_INTR_0;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_GPIO_INTR_1){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_GPIO_INTR_1;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_GPIO_INTR_2){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_GPIO_INTR_2;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_GPIO_INTR_3){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_GPIO_INTR_3;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_GPIO_INTR_4){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_GPIO_INTR_4;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_CMP_INTR_1){
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR = COMP1_BASED_WAKEUP_STATUS_CLEAR;
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_CMP_INTR_1;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_CMP_INTR_2){
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR =  COMP2_BASED_WAKEUP_STATUS_CLEAR;
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_CMP_INTR_2;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_CMP_INTR_3){
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR =  COMP3_BASED_WAKEUP_STATUS_CLEAR;
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_CMP_INTR_3;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_CMP_INTR_4){
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR =  COMP4_BASED_WAKEUP_STATUS_CLEAR;
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_CMP_INTR_4;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_RFWAKEUP_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_RFWAKEUP_INTR;
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR = RF_WAKEUP_CLEAR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_BOD_INTR){
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR =  COMP5_BASED_WAKEUP_STATUS_CLEAR;
		NPSS_INTR_CLEAR_REG =NPSS_TO_MCU_BOD_INTR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_BUTTON_INTR){
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR =  COMP6_BASED_WAKEUP_STATUS_CLEAR;
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_BUTTON_INTR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_SDC_INTR){
		*(volatile uint32_t *)(0x24042404) |=  BIT(0);
		*(volatile uint32_t *)(0x24042404) &= ~BIT(0);
		/*Clear SCDC interrupt */
		*(volatile uint32_t *)(0x24042400) |= BIT(0);
		/*Clear SDC interrupt */
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_SDC_INTR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_WIRELESS_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_WIRELESS_INTR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_ALARM_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_ALARM_INTR;
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR =RTC_ALARM_BASED_WAKEUP_STATUS_CLEAR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_SEC_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_SEC_INTR;
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR =RTC_SEC_BASED_STATUS_CLEAR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_MSEC_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_MSEC_INTR;
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_CLEAR = MILLI_SEC_BASED_STATUS_CLEAR;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_PROCESSOR_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_WAKEUP_INTR;
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_REG =1;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_HOST_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_WAKEUP_INTR;
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_REG =1;
	}

	if(wakeUpIntrClear & NPSS_TO_MCU_DST_INTR){
		NPSS_INTR_CLEAR_REG = NPSS_TO_MCU_WAKEUP_INTR;
		MCU_FSM->MCU_FSM_WAKEUP_STATUS_REG =1;
	}
	return ;
}

void ps_bg_ldo_config(uint8_t ldo_0p6_ctrl, uint8_t ldo_0p6_lp_mode)
{
	volatile uint32_t read_ldo_data=0;
	read_ldo_data = ULP_SPI_MEM_MAP(0x129);

	if(ldo_0p6_lp_mode){
		read_ldo_data |= (1 << 16);
	}else{
		read_ldo_data &= ~(1 << 16);
	}
	read_ldo_data  &= ~(0x7 << 18);
	read_ldo_data |= (ldo_0p6_ctrl << 18);
	ULP_SPI_MEM_MAP(0x129) = read_ldo_data;
	return;
}

void ps_configure_trim_values(uint16_t lf_ro_trim ,uint16_t lf_rc_trim , uint16_t hf_ro_trim ,uint16_t hf_rc_trim ,uint16_t bg_ptat_trim , uint16_t bg_trim)
{
	uint32_t reg_read=0;

	/*Programmable trim bits for RO 32KHz clock*/
	reg_read = ULP_SPI_MEM_MAP(0x102);
	reg_read &= ~(0x1F << 16);
	ULP_SPI_MEM_MAP(0x102) = (reg_read | (lf_ro_trim << 16));

	/*Programmable fine trim for RC 32KHz clock*/
	reg_read = ULP_SPI_MEM_MAP(0x103);
	reg_read &= ~(0x7F << 14);
	ULP_SPI_MEM_MAP(0x103) = (reg_read | (lf_rc_trim << 14));

	/*Trim bits for RC 32MHz clock //Do not go beyond 100*/
	reg_read = ULP_SPI_MEM_MAP(0x104);
	reg_read &= ~(0x7F << 14);
	ULP_SPI_MEM_MAP(0x104) = (reg_read | (hf_rc_trim << 14));

	/*Trim bits for High frequency ring oscillator*/
	reg_read = ULP_SPI_MEM_MAP(0x105);
	reg_read &= ~(0x7F << 14);
	ULP_SPI_MEM_MAP(0x105) = (reg_read | BIT(12) | (hf_ro_trim << 14));

	/*update the 'bg_r_ptat' and 'bg_r' */
	reg_read = ULP_SPI_MEM_MAP(0x127);
	reg_read &= ~(0x3F << 16);
	reg_read |= ((bg_ptat_trim << 19) | (bg_trim << 16));
	ULP_SPI_MEM_MAP(0x127) = reg_read;
}

void ps_wireless_shutdown(void)
{
	/* Enable NWP NPSS FSM CLK*/
	*(volatile uint32_t *)0x41300114 |= 0x9;
	/* Step-1b/1c: Disable Oscillator Clocks */
	*(volatile uint32_t *)0x41300120 &= ~(0x790000);
	*(volatile uint32_t *)0x4130000C = 0x78;
	*(volatile uint32_t *)0x41300140 = (BIT(19) | BIT(1) | BIT(0));
	*(volatile uint32_t *)0x41300110 |= BIT(22);
	*(volatile uint32_t *)0x41300110 &= ~BIT(23);
  *(volatile uint32_t *)0x41300424 = (BIT(17) | BIT(18));
	*(volatile uint32_t *)0x41300480 |= BIT(5);
	*(volatile uint32_t *)0x41300480 &= ~(BIT(7) | BIT(1));
	*(volatile uint32_t *)0x41300470 = BIT(20);
	*(volatile uint32_t *)0x41300408 = (BIT(28) | BIT(24) | BIT(20) | BIT(16));
	*(volatile uint32_t *)0x41300418 = (BIT(4));
	/* Shutdown TA */
	*(volatile uint32_t *)0x41300100 = 0x1;
	return;
}
/**
 *@fn         void RSI_PS_AnalogPeriPtatEnable(void)
 *@brief      This API is used to enable the ptat currents to analog peripherals
 * @return 	  none 
 */
error_t RSI_PS_AnalogPeriPtatEnable(void)
{
	return RSI_IPMU_ProgramConfigData(ana_perif_ptat_common_config1);
}

/**
 *@fn         void RSI_PS_AnalogPeriPtatDisable(void)
 *@brief      This API is used to disable the ptat currents to analog peripherals
 *@return 	  none 
 */
error_t RSI_PS_AnalogPeriPtatDisable(void)
{
 return RSI_IPMU_ProgramConfigData(ana_perif_ptat_common_config2);
}

/**
 *@fn         void RSI_PS_BodClksPtatEnable(void)
 *@brief      This API is used to enable the ptat currents to clocks and bod(cmp_npss)
 *@return 	  none 
 */
error_t RSI_PS_BodClksPtatEnable(void)
{
	return RSI_IPMU_ProgramConfigData(ipmu_bod_clks_common_config1);
}

/**
 *@fn         void RSI_PS_BodClksPtatDisable(void)
 *@brief      This API is used to disable the ptat currents to clocks and bod(cmp_npss)
 *@return 	  none 
 */
 error_t RSI_PS_BodClksPtatDisable(void)
{
	return RSI_IPMU_ProgramConfigData(ipmu_bod_clks_common_config2);
}


/*End of file not truncated*/
