/**
 * @file    rsi_pw_num_rx.c
 * @version 0.1
 * @date    7 Oct 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for transmit test mode.
 *  @section Description  This file contains example application for transmit test.
 *
 *
 */
/**
 * Include files
 * */

//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_board.h"
#include "rsi_chip.h"

#define MCU_FSM_SLEP_WKP_CTRL  *(volatile uint32_t *)0x24048100
//! Transmit test power 
#define RSI_TX_TEST_POWER             10                      

//! Transmit test rate
#define RSI_TX_TEST_RATE              RSI_RATE_6              

//! Transmit test length
#define RSI_TX_TEST_LENGTH            1000                      

//! Transmit test mode
#define RSI_TX_TEST_MODE              RSI_BURST_MODE      

//! Transmit test channel
#define RSI_TX_TEST_CHANNEL           1                       

//! Select Intenal antenna or uFL connector
#define RSI_ANTENNA           1

//! Antenna gain in 2.4GHz band
#define RSI_ANTENNA_GAIN_2G           0
                       
//! Antenna gain in 5GHz band
#define RSI_ANTENNA_GAIN_5G           0

//! Memory length for driver
#define GLOBAL_BUFF_LEN 10000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500
//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Variable used to stop receiving stats
volatile uint8_t stop_receiving_stats = 0; 

//!variable used to initialise stats received
uint32_t stats_received = 0;

void M4_shutdown()
{
	// Power-Down Analog Peripherals 
	RSI_IPMU_PowerGateClr(
				AUXDAC_PG_ENB
			|	AUXADC_PG_ENB
			|	WURX_CORR_PG_ENB
			|	WURX_PG_ENB
			//|	ULP_ANG_PWRSUPPLY_PG_ENB
			| ULP_ANG_CLKS_PG_ENB
			|	CMP_NPSS_PG_ENB
	); 
	//Power down NPSS peripherals
	RSI_PS_NpssPeriPowerDown(
			SLPSS_PWRGATE_ULP_NWPAPB_MCU_CTRL |
			SLPSS_PWRGATE_ULP_MCUSTORE2 |
			SLPSS_PWRGATE_ULP_MCUSTORE3 |
			SLPSS_PWRGATE_ULP_MCUSTORE2 |
			SLPSS_PWRGATE_ULP_MCUTS     |
			SLPSS_PWRGATE_ULP_MCUPS     |
			SLPSS_PWRGATE_ULP_MCUWDT);

	//Disable deep sleep timer in sleep and time stamp
	RSI_PS_PowerSupplyDisable(POWER_ENABLE_TIMESTAPING | POWER_ENABLE_DEEPSLEEP_TIMER);

	RSI_PS_PowerSupplyDisable(
			  POWER_ENABLE_TIMESTAPING
			| POWER_ENABLE_DEEPSLEEP_TIMER
	); 
	//Power Down ULPSS RAM periphery domain
	RSI_PS_UlpssRamBanksPeriPowerDown(
			ULPSS_2K_BANK_0 |
			ULPSS_2K_BANK_1 |
			ULPSS_2K_BANK_2 |
			ULPSS_2K_BANK_3 |
			ULPSS_2K_BANK_4 |
			ULPSS_2K_BANK_5 |
			ULPSS_2K_BANK_6 |
			ULPSS_2K_BANK_7 );
			
	//ULPSS RAM clear
	RSI_PS_UlpssRamBanksPowerDown(
			ULPSS_2K_BANK_0 |
			ULPSS_2K_BANK_1 |
			ULPSS_2K_BANK_2 |
			ULPSS_2K_BANK_3 |
			ULPSS_2K_BANK_4 |
			ULPSS_2K_BANK_5 |
			ULPSS_2K_BANK_6 |
			ULPSS_2K_BANK_7
	); 
//M4SS peripheral power gates 
	RSI_PS_M4ssPeriPowerDown(
			M4SS_PWRGATE_ULP_M4_DEBUG |
			M4SS_PWRGATE_ULP_M4_FPU   |
			M4SS_PWRGATE_ULP_ETHERNET |
			M4SS_PWRGATE_ULP_SDIO_SPI |
			M4SS_PWRGATE_ULP_USB      |
			M4SS_PWRGATE_ULP_RPDMA    |
			M4SS_PWRGATE_ULP_PERI1    |
			M4SS_PWRGATE_ULP_PERI2    |
			M4SS_PWRGATE_ULP_PERI3    |
			M4SS_PWRGATE_ULP_CCI      |
			M4SS_PWRGATE_ULP_SD_MEM);

	//ULPSS Peripheral power gate 
	RSI_PS_UlpssPeriPowerDown(
			ULPSS_PWRGATE_ULP_MISC 	   |
			ULPSS_PWRGATE_ULP_AUX  	   |
			ULPSS_PWRGATE_ULP_VAD  	   |
			ULPSS_PWRGATE_ULP_UART 	   |
			ULPSS_PWRGATE_ULP_SSI  	   |
			ULPSS_PWRGATE_ULP_I2S  	   |
			ULPSS_PWRGATE_ULP_I2C  	   |
			ULPSS_PWRGATE_ULP_UDMA 	   |
			ULPSS_PWRGATE_ULP_IR 	     |
			ULPSS_PWRGATE_ULP_FIM      |
			ULPSS_PWRGATE_ULP_CAP
	);
	RSI_PS_M4ss_Tass_Ctrl_Clear(BIT(0)|BIT(16)|BIT(17)|BIT(18)|BIT(19)|BIT(20)|BIT(21)|BIT(22)|BIT(23));

	MCU_FSM_SLEP_WKP_CTRL = BIT(0);
	MCU_FSM_SLEP_WKP_CTRL;
}


//! call back to receive RX Stats data
void rsi_wlan_stats_receive_handler(uint16_t status, const uint8_t *buffer, const uint16_t length)
{

	M4_shutdown();
  stats_received ++;

  //! For the buffer parameters information is available in SAPI's documentation
  
  if(stats_received == 10)
  {
    stop_receiving_stats = 1;
  }

}


int32_t rsi_pw_num_rx_app()
{
  int32_t     status       = RSI_SUCCESS;


  //! WC initialization
  status = rsi_wireless_init(8, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
	//! Sending feature frame
	rsi_send_feature_frame();
	
	//! Wlan radio init 
  status = rsi_wlan_radio_init();
	if(status != RSI_SUCCESS)
  {
    //! Return the status if error in sending command occurs
    return status;
  }
 
  //! Register Wlan receive stats call back handler 
  rsi_wlan_register_callbacks(RSI_WLAN_RECEIVE_STATS_RESPONSE_CB,rsi_wlan_stats_receive_handler);

  //! transmit test start
  status = rsi_transmit_test_start(RSI_TX_TEST_POWER, RSI_TX_TEST_RATE, RSI_TX_TEST_LENGTH,
                                    RSI_TX_TEST_MODE, RSI_TX_TEST_CHANNEL);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Add  delay here to see the TX packets on AIR
  rsi_delay_ms(1000);

  //! Stop TX transmit
  status = rsi_transmit_test_stop();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
	
  ////////////////////////////////////////
  //! Transmit data/TX from the peer//////
  ////////////////////////////////////////


  //! Start/Recieve publishing RX stats  
  status =  rsi_wlan_receive_stats_start(RSI_TX_TEST_CHANNEL);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  while(1)
  {
	  //To keep alive rx
	  rsi_wireless_driver_task();
  }

  return status;
}

void main_loop(void)
{
  while(1)
  {
    ////////////////////////
    //! Application code ///
    ////////////////////////

    //! event loop 
    rsi_wireless_driver_task();

  }
}

int main()
{
  int32_t status;
#ifdef RSI_WITH_OS

  rsi_task_handle_t wlan_task_handle = NULL;

  rsi_task_handle_t driver_task_handle = NULL;
#endif

#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

  //! Driver initialization
   status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
   if((status < 0) || (status > GLOBAL_BUFF_LEN))
   {
     return status;
   }

   //! Redpine module intialisation
   status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
   if(status != RSI_SUCCESS)
   {
     return status;
   }

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_transmit_test_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call Transmit test application
  status = rsi_pw_num_rx_app();

  //! Application main loop
  main_loop();
#endif
  return status;

}
