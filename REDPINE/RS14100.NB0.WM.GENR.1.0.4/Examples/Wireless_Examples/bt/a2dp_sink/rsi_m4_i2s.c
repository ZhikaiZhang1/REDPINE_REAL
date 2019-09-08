/**
 * @file    rsi_a2dp_i2s.c
 * @version 0.1
 * @date    xx jan 2018
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for I2S configuration for audio using A2DP
 *
 *  @section Description  This application serves as a I2S configuration.
 *
 */

/**
 * Include files
 * */
#ifdef RSI_M4_INTERFACE 

#include <rsi_board.h>
#include <rsi_bt_config.h>
#include <rsi_a2dp.h>
#include "UDMA.h"
#include "SAI.h"


extern ARM_DRIVER_SAI Driver_SAI0;
ARM_DRIVER_SAI *SAIdrv = &Driver_SAI0;
void ARM_SAI_SignalEvent(uint32_t event);

ARM_DRIVER_SAI *drv_info;

//#define  I2S0_IRQHandler   IRQ064_Handler
//#define  UDMA0_IRQHandler	 IRQ033_Handler	


/*Pre-fetch and regestring */
#define ICACHE2_ADDR_TRANSLATE_1_REG  *(volatile uint32_t *)(0x20280000 + 0x24)
#define MISC_CFG_SRAM_REDUNDANCY_CTRL *(volatile uint32_t *)(0x46008000 + 0x18)
#define MISC_CONFIG_MISC_CTRL1        *(volatile uint32_t *)(0x46008000 + 0x44)
#define MISC_QUASI_SYNC_MODE          *(volatile uint32_t *)(0x46008000 + 0x84)

#define SOC_PLL_REF_FREQUENCY         40000000  /*<! PLL input REFERENCE clock 40MHZ */ 

//Note: Change this macro to required PLL frequency in hertz
#define PS4_SOC_FREQ                32000000 /*<! PLL out clock 100MHz            */
/* Buffer sizes for this example and total transfer size */
//#define  BUFFER_SIZE       1024


//uint8_t ret =1;
volatile uint32_t tx_done = 1, rx_done = 0;
extern volatile uint8_t pcm_mutex;
extern data_pkt_t *pcm_queue_head;
extern data_pkt_t  pcm_data_pkts[MAX_DATA_PKTS];


/*==============================================*/
/**
 * @fn         RSI_SOC_PllChangeToCortex
 * @brief      Configures the PLL and swicthes the M4 SOC clock
 * @param[in]  none.
 * @return     none.
 * @section description
 * Configures the PLL and swicthes the M4 SOC clock
 */
int RSI_SOC_PllChangeToCortex(void)
{
	/*Switch M4 SOC clock to Reference clock*/
	/*Default keep M4 in reference clock*/
  RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);

  /*Enable fre-fetch and register if SOC-PLL frequency is more than or equal to 120M*/
#if (PS4_SOC_FREQ >= 120000000)
	/*Configure the prefetch and registering when SOC clock is more than 120Mhz*/
	/*Configure the SOC PLL to 220MHz*/
	ICACHE2_ADDR_TRANSLATE_1_REG  = BIT(21);  //Icache registering when clk freq more than 120

	/*When set, enables registering in M4-TA AHB2AHB. This will have performance penalty. This has to be set above 100MHz*/
	MISC_CFG_SRAM_REDUNDANCY_CTRL = BIT(4);
	MISC_CONFIG_MISC_CTRL1 |= BIT(4); //Enable Register ROM as clock frequency is 200 Mhz

	/*Enable Intersubstsem memory Registering as m4_soc_clk clock is going to tass. above 120Mhz we have to enable this.Also enabling prefetch as when registering by default prefetch is expected to be enabled to save the cycles which are lost in registering*/
#if !(defined WISE_AOC_4)
	MISC_QUASI_SYNC_MODE |= BIT(6);
	/*Enable Intersubstsem memory Registering as m4_soc_clk clock is going to tass. above 120Mhz we have to enable this.Also enabling prefetch as when registering by default prefetch is expected to be enabled to save the cycles which are lost in registering*/
	MISC_QUASI_SYNC_MODE |= (BIT(6) | BIT(7));
#endif //(defined WISE_AOC_4)
#endif //(PS4_SOC_FREQ > 120000000)

  /*Configure the PLL frequency*/
	RSI_CLK_SetSocPllFreq(M4CLK , PS4_SOC_FREQ , SOC_PLL_REF_FREQUENCY);
	/*Switch M4 clock to PLL clock for speed operations*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);
	
	SysTick_Config(PS4_SOC_FREQ / 1000);
	
	return 0;
}

/*==============================================*/
/**
 * @fn         get_i2s_version
 * @brief      This function is used to get the I2S driver version.
 * @param[in]  pSAIdrv, serial audio interface driver structure.
 * @return     none.
 * @section description
 * This function is used to get the I2S driver version.
 */
void get_i2s_version (ARM_DRIVER_SAI *pSAIdrv)  
{
  ARM_DRIVER_VERSION  version;
 
  //drv_info = &Driver_SAI0;  
  version = pSAIdrv->GetVersion ();
  if (version.api < 0x10A)  
  {      // requires at minimum API version 1.10 or higher
    // error handling
    return;
  }
}

/*==============================================*/
/**
 * @fn         read_capabilities
 * @brief      This function is used to get the I2S driver capabilities.
 * @param[in]  pSAIdrv, serial audio interface driver structure.
 * @return     none.
 * @section description
 * This function is used to get the I2S driver capabilities.
 */
void read_capabilities (ARM_DRIVER_SAI *pSAIdrv)
{
  ARM_SAI_CAPABILITIES drv_capabilities;
 
	
  drv_capabilities = pSAIdrv->GetCapabilities ();
  // interrogate capabilities
}

/*==============================================*/
/**
 * @fn         read_capabilities
 * @brief      This function is used to get the I2S driver capabilities.
 * @param[in]  pSAIdrv, serial audio interface driver structure.
 * @return     none.
 * @section description
 * This function is used to get the I2S driver capabilities.
 */
void rsi_i2s_slave_init ()
{
	uint32_t  status;
	
	
	get_i2s_version(SAIdrv);
  read_capabilities(SAIdrv);
	
	//! configure the M4 SOC colck.
	RSI_SOC_PllChangeToCortex();
	
  //! RSI_CLK_SetI2sPllFreq(M4CLK,18432000,40000000); 
	
	//! rigister the callback for i2s interface.
  SAIdrv->Initialize(ARM_SAI_SignalEvent);
	
	//! i2s interface power mode.
  SAIdrv->PowerControl (ARM_POWER_FULL);
   
  //!configure Transmitter to Asynchronous Master: I2S Protocol, 16-bit data, 44100kHz Audio sample rate frequency
  status = SAIdrv->Control(ARM_SAI_CONFIGURE_TX | 
	                             ARM_SAI_MODE_SLAVE  | 
                               ARM_SAI_ASYNCHRONOUS | //ARM_SAI_ASYNCHRONOUS | 
                               ARM_SAI_PROTOCOL_I2S | 
                               ARM_SAI_DATA_SIZE(16), 32, 44100);
 
  //! enable Transmitter
  status = SAIdrv->Control(ARM_SAI_CONTROL_TX, 1, 0);
	
	//! initilize the GPIO pin for DMA configurations.
	RSI_EGPIO_SetPinMux(EGPIO,0,9,0);
	RSI_EGPIO_SetDir(EGPIO,0,9,0);


  return;
}

/*==============================================*/
/**
 * @fn         ARM_SAI_SignalEvent
 * @brief      This function will invoke when i2s tx DMA was done 
 * @param[in]  event, event map list.
 * @return     none.
 * @section description
 * This callback function will invoke when i2s tx DMA was done.
 */
extern uint32_t  i2s_module_idel;
extern uint32_t pcm_packets_completed;
void ARM_SAI_SignalEvent(uint32_t event)
{ 
 	data_pkt_t *pcm_pkt;
	uint32_t  status;
	
	
  if(event & ARM_SAI_EVENT_SEND_COMPLETE)
  {
	  tx_done = 1;
		pcm_packets_completed++;
		RSI_EGPIO_SetPin(EGPIO,0,9,1);
		if (pcm_queue_head != NULL) {
			pcm_mutex = 1;
			pcm_pkt = (data_pkt_t *)get_packet_from_queue (&pcm_queue_head);
			pcm_mutex = 0;
			if (pcm_pkt == NULL) {
				while (1);
			}
			free_data_pkt ((data_pkt_t *)&pcm_data_pkts, pcm_pkt);
			
			if (pcm_queue_head != NULL) {
				status = SAIdrv->Send((uint8_t *)&(pcm_queue_head->data), 256);
				if (status != 0) {
					while (1);
				}
				tx_done = 0;
			} else {
				i2s_module_idel++;
			}
			RSI_EGPIO_SetPin(EGPIO,0,9,0);
			
		} else {
			while (1);
		}
	
  }
  if(event & ARM_SAI_EVENT_RECEIVE_COMPLETE)
  {
    rx_done = 1;
  }
  if(event & ARM_SAI_EVENT_TX_UNDERFLOW)
  {
    // TODO
  }
  if(event & ARM_SAI_EVENT_RX_OVERFLOW)
  {
    // TODO
  }
  if(event & ARM_SAI_EVENT_FRAME_ERROR)
  {
    // TODO
  }    
}

#endif
