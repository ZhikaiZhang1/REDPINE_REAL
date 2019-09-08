/**
 * @file    main.c
 * @version 0.1
 * @date    8 May  2019
 *
 *  Copyright(C) Redpine Signals 2019
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains driver, module initialization and application execution
 *
 *  @section Description  This file contains driver, module initializations
 *
 *
 */

#include "rsi_common_app.h"
#include "rsi_driver.h"
#include "rsi_wlan.h"
#ifdef RSI_WITH_OS
//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Wlan task priority
#if BT_A2DP_SOURCE_WIFI_HTTP_S_RX
#define RSI_WLAN_TASK_PRIORITY   			3
#else
#define RSI_WLAN_TASK_PRIORITY   			1
#endif

//! Wireless driver task priority
#if WIFI_SSL_RX_THROUGHPUT
#define RSI_DRIVER_TASK_PRIORITY   			1
#else
#define RSI_DRIVER_TASK_PRIORITY   			2
#endif

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  			512

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  		1024

#define RSI_BT_TASK_STACK_SIZE     			1024

#define RSI_ANT_TASK_STACK_SIZE     		1024
//! BT task priority
#define RSI_BT_TASK_PRIORITY           		1

#define RSI_ANT_TASK_PRIORITY           	1

#define RSI_BLE_TASK_STACK_SIZE         	1024

//! BT task priority
#if BLE_ADV_BT_A2DP_SOURCE
#define RSI_BLE_TASK_PRIORITY           	3
#else
#define RSI_BLE_TASK_PRIORITY           	1
#endif

rsi_semaphore_handle_t coex_sem;

rsi_semaphore_handle_t event_sem;

rsi_semaphore_handle_t suspend_sem;
#endif

#define GLOBAL_BUFF_LEN 					10000

//! Flag for infinite loop
#define RSI_FOREVER             			1

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN] = {0};
uint8_t powersave_d = 0, ble_powersave_d = 0, wifi_powersave_d=0;
uint8_t tx_rx_Completed = 0;
uint8_t loopback_done = 0;
#if(BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX || BLE_DUAL_MODE_BT_SPP_SLAVE)
extern uint16_t num_of_conn_slaves;
extern uint8_t num_of_conn_masters;
#endif

int32_t main(void)
{
#ifdef RSI_WITH_OS

	rsi_task_handle_t wlan_task_handle = NULL;

	rsi_task_handle_t bt_task_handle = NULL;

	rsi_task_handle_t ble_task_handle = NULL;

	rsi_task_handle_t ant_task_handle = NULL;

	rsi_task_handle_t driver_task_handle = NULL;
#endif

  int32_t     status = RSI_SUCCESS;

   //! Board Initialization
	rsi_hal_board_init();
  
  //! Driver initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if((status < 0) || (status > GLOBAL_BUFF_LEN))
  {
    return status;
  }

#ifndef RSI_WITH_OS
  //! Redpine module initialization
#if FIRMWARE_UPGRADE_VIA_BOOTLOADER
 status = rsi_device_init(BURN_NWP_FW);
#else
 status = rsi_device_init(LOAD_NWP_FW);
#endif
 	  if(status != RSI_SUCCESS)
 	  {
 	    return status;
 	  }

#if !(FIRMWARE_UPGRADE_VIA_BOOTLOADER)
  //! WiSeConnect initialization
  status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Send Feature frame
  status = rsi_send_feature_frame();
  if(status != RSI_SUCCESS)
  {
	  return status;
  }
#endif
#if (BT_A2DP_SOURCE)
  //! BT A2DP Initialization
  	status = rsi_bt_a2dp_source();
  	if(status != RSI_SUCCESS)
  	{
  		LOG_PRINT("bt_a2dp init failed\n");
  		return -1;
  	}
#endif
#if (BT_A2DP_SOURCE_SBC_CODEC)
  //! BT A2DP Initialization
        status = rsi_bt_a2dp_source_sbc_codec();
        if(status != RSI_SUCCESS)
        {
                LOG_PRINT("bt_a2dp init failed\n");
                return -1;
        }
#endif
#endif

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
#if WIFI_SSL_RX_THROUGHPUT
  rsi_task_create(rsi_app_task_wifi_ssl_rx_tpt, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_TCP_BI_DIR
  rsi_task_create(rsi_app_task_wifi_tcp_bi_dir, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_SSL_TX_THROUGHPUT
  rsi_task_create(rsi_app_task_wifi_ssl_tx_tpt, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_TCP_RX_POWERSAVE
  rsi_task_create(rsi_app_task_wifi_tcp_rx_ps, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_CONNECTED_STANDBY
  rsi_task_create(rsi_app_task_wifi_connected_standby, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_STA_BGSCAN
  rsi_task_create(rsi_app_task_wifi_sta_bgscan, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_STA_CONN_DISCONN_SSL_CONN_DISCONN
  rsi_task_create(rsi_app_task_wifi_sta_ssl_conn_disconn, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_TCP_TX_POWERSAVE
  rsi_task_create(rsi_app_task_wifi_tcp_tx_ps, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_DEEPSLEEP_STANDBY
  rsi_task_create(rsi_app_task_wifi_deepsleep_standby, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif FIRMWARE_UPGRADE
  rsi_task_create(rsi_app_task_fw_update, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif WIFI_SSL_SERVER_CERT_BYPASS
  rsi_task_create(rsi_app_task_wifi_ssl_server_cert_bypass, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif BT_A2DP_SOURCE_WIFI_HTTP_S_RX
  rsi_semaphore_create(&coex_sem, 0);
  rsi_semaphore_create(&event_sem, 0);
  rsi_semaphore_create(&suspend_sem, 0);
  rsi_task_create(rsi_wlan_app_task, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
  rsi_task_create(rsi_bt_app_task, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);
#elif WIFI_HTTP_S_5MB_RX_WITH_STANDBY
  rsi_task_create(rsi_app_task_wifi_https_rx_standby, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);
#elif BT_A2DP_SOURCE
  //! Task created for BLE task
  rsi_task_create((void *)rsi_app_task_bt_a2dp_source, (uint8_t *)"bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);
#elif BLE_ADV_BT_SPP_THROUGHPUT
  //! Task created for BLE task
  rsi_task_create(rsi_app_task_ble_adv_bt_spp_tpt, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);
#elif BLE_ADV_BT_A2DP_SOURCE
  rsi_semaphore_create(&coex_sem, 0);
  rsi_semaphore_create(&suspend_sem, 0);
  //! Task created for BLE task
  rsi_task_create(rsi_bt_app_task, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);
  rsi_task_create(rsi_ble_app_task, "ble_task", RSI_BLE_TASK_STACK_SIZE, NULL, RSI_BLE_TASK_PRIORITY, &ble_task_handle);
#elif BT_EIR_SNIFF_SPP_SLAVE
  rsi_task_create(rsi_app_task_eir_sniff_spp, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);
#elif BLE_PERIPHERAL
  rsi_task_create(rsi_app_task_ble_peripheral, "ble_task", RSI_BLE_TASK_STACK_SIZE, NULL, RSI_BLE_TASK_PRIORITY, &ble_task_handle);
#elif BLE_SIMPLE_GATT
  rsi_task_create(rsi_app_task_ble_simple_gatt, "ble_task", RSI_BLE_TASK_STACK_SIZE, NULL, RSI_BLE_TASK_PRIORITY, &ble_task_handle);
 #elif BLE_CENTRAL
  rsi_task_create(rsi_app_task_ble_central, "ble_task", RSI_BLE_TASK_STACK_SIZE, NULL, RSI_BLE_TASK_PRIORITY, &ble_task_handle);
#elif BLE_DUAL_ROLE
  rsi_task_create(rsi_app_task_ble_dual_role, "ble_task", RSI_BLE_TASK_STACK_SIZE, NULL, RSI_BLE_TASK_PRIORITY, &ble_task_handle);
#elif BLE_POWERSAVE
  rsi_task_create(rsi_app_task_ble_powersave, "ble_task", RSI_BLE_TASK_STACK_SIZE, NULL, RSI_BLE_TASK_PRIORITY, &ble_task_handle);
#elif BLE_PRIVACY
  rsi_task_create(rsi_app_task_ble_privacy, "ble_task", RSI_BLE_TASK_STACK_SIZE, NULL, RSI_BLE_TASK_PRIORITY, &ble_task_handle);
#endif
  //! Task created for Driver task
  rsi_task_create((void *)rsi_wireless_driver_task, (uint8_t *)"driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();
#else
  while(RSI_FOREVER)
  {
	  //! Execute demo
	  rsi_demo_app();

	  //! wireless driver task
	  rsi_wireless_driver_task();

  }
#endif
  return 0;
}

void rsi_demo_app(void)
{

#if BT_A2DP_SOURCE
	  rsi_app_task_bt_a2dp_source();
#elif BT_A2DP_SOURCE_SBC_CODEC
          rsi_app_task_bt_a2dp_source_sbc_codec();
#elif BT_PER
	  rsi_app_task_bt_per();
#elif BT_SNIFF
	  rsi_app_task_bt_connected_sniff();
#elif BT_SPP_RX_TX
	  rsi_app_task_bt_spp_rx_tx();
#elif BLE_ADV_BT_SPP_THROUGHPUT
	  rsi_app_task_ble_adv_bt_spp_tpt();
#elif BT_EIR_SNIFF_SPP_SLAVE
	  rsi_app_task_eir_sniff_spp();
#elif BLE_ADV_BT_A2DP_SOURCE
	  rsi_app_task_ble_adv_bt_a2dp();
#elif BLE_PERIPHERAL
	  rsi_app_task_ble_peripheral();
#elif BLE_SIMPLE_GATT
	  rsi_app_task_ble_simple_gatt();
#elif BLE_CENTRAL
	  rsi_app_task_ble_central();
#elif BLE_DUAL_ROLE
	  rsi_app_task_ble_dual_role();
#elif BLE_POWERSAVE
	  rsi_app_task_ble_powersave();
#elif BT_PAGE_INQUIRY
	  rsi_app_task_bt_page_inquiry();
#elif BT_INQUIRY_SCAN
	  rsi_app_task_bt_inquiry_scan();
#elif BT_PAGE_SCAN
	  rsi_app_task_bt_page_scan();
#elif BT_SPP_MASTER_SNIFF
	  rsi_app_task_bt_spp_master_sniff();
#elif BT_SPP_SLAVE_SNIFF
	  rsi_app_task_bt_spp_slave_sniff();
#elif BLE_ADV
	  rsi_app_task_ble_peripheral_adv();
#elif BLE_SCAN
	  rsi_app_task_ble_central_scan();
#elif BLE_IDLE_POWERSAVE
	  if(!ble_powersave_d)
		  rsi_app_task_ble_ilde_powersave();
#elif BT_BLE_IDLE_POWERSAVE
	  if(!powersave_d)
		  rsi_app_task_bt_ble_ilde_powersave();
#elif WIFI_SSL_RX_THROUGHPUT
	  if(!tx_rx_Completed)
		  rsi_app_task_wifi_ssl_rx_tpt();
#elif WIFI_TCP_BI_DIR
	  if(!loopback_done)
		  rsi_app_task_wifi_tcp_bi_dir();
#elif WIFI_SSL_TX_THROUGHPUT
	  if(!tx_rx_Completed)
		  rsi_app_task_wifi_ssl_tx_tpt();
#elif WIFI_TCP_RX_POWERSAVE
	  if(!tx_rx_Completed)
		  rsi_app_task_wifi_tcp_rx_ps();
#elif WIFI_CONNECTED_STANDBY
	  if(!powersave_d)
		  rsi_app_task_wifi_connected_standby();
#elif WIFI_STA_BGSCAN
		  rsi_app_task_wifi_sta_bgscan();
#elif WIFI_STA_CONN_DISCONN_SSL_CONN_DISCONN
		  rsi_app_task_wifi_sta_ssl_conn_disconn();
#elif WIFI_TCP_TX_POWERSAVE
		  rsi_app_task_wifi_tcp_tx_ps();
#elif WIFI_DEEPSLEEP_STANDBY
	  if(!powersave_d)
		  rsi_app_task_wifi_deepsleep_standby();
#elif WIFI_SSL_SERVER_CERT_BYPASS
	  rsi_app_task_wifi_ssl_server_cert_bypass();
#elif BT_A2DP_SOURCE_WIFI_HTTP_S_RX
	  rsi_app_task_bt_a2dp_wifi_http_s();
#elif WIFI_HTTP_S_5MB_RX_WITH_STANDBY
	  rsi_app_task_wifi_https_rx_standby();
#elif BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX
	  rsi_app_task_bt_ble_dual_mode_wifi_https();
#elif BLE_DUAL_MODE_BT_SPP_SLAVE
	  rsi_app_task_bt_ble_dualmode();
#elif FIRMWARE_UPGRADE
      rsi_app_task_fw_update();
#elif FIRMWARE_UPGRADE_VIA_BOOTLOADER
      rsi_app_task_fw_update_via_bootloader();
#elif BLE_PER
	  rsi_ble_per();
#elif BLE_DATA_TRANSFER
	  rsi_app_task_ble_data_transfer();	  
#elif BLE_PRIVACY
	  rsi_app_task_ble_privacy();
#elif BLE_LONG_READ
	rsi_app_task_ble_long_read();	  
#elif BLE_L2CAP_CBFC
      rsi_app_task_le_l2cap_cbfc();
#elif BLE_GATT_TEST_ASYNC
	  rsi_app_task_ble_simple_gatt_test();
#elif ANT_APP
	  rsi_app_task_ant_app();
#elif ANT_APP_POWERSAVE
	  rsi_app_task_ant_app_power_save();
#elif BLE_CENTRAL_ANT_APP
	  rsi_ant_ble_central_coex_task();
#elif BLE_PERIPHERAL_ANT_APP
	  rsi_ant_ble_peripheral_coex_task();
#elif ANT_BLE_PERI_BT_A2DP_SRC_APP
	  rsi_app_task_ant_ble_peri_bt_a2dp_coex_task();
#elif ANT_BLE_CENT_BT_A2DP_SRC_APP
	  rsi_app_task_ant_ble_cent_bt_a2dp_coex_task();
#endif
}
#if ANT_APP
void rsi_app_task_ant_app()
{
	int32_t	status = RSI_SUCCESS;
    status =rsi_ant();
}
#endif

#if ANT_APP_POWERSAVE
void rsi_app_task_ant_app_power_save()
{
	int32_t	status = RSI_SUCCESS;
    status = rsi_ant_power_save();
}
#endif

#if BLE_CENTRAL_ANT_APP
void rsi_ant_ble_central_coex_task()
{
	int32_t	status = RSI_SUCCESS;
    status = rsi_ble_central();
}
#endif

#if BLE_PERIPHERAL_ANT_APP
void rsi_ant_ble_peripheral_coex_task()
{
	int32_t	status = RSI_SUCCESS;
    status = rsi_ble_peripheral();
}
#endif
#if ANT_BLE_PERI_BT_A2DP_SRC_APP
void rsi_app_task_ant_ble_peri_bt_a2dp_coex_task(void)
{
	int32_t	status = RSI_SUCCESS;

	//! BT A2DP Initialization
	status = rsi_bt_a2dp_source();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("bt_a2dp init failed\n");
		return;
	}

	// Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! BLE dual role Initialization
	status = rsi_ble_peripheral_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("ble peripheral role init failed\n");
		return;
	}

	//! ANT  role Initialization
	status = rsi_ble_ant_app();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("ANT role init got failed\n");
		return;
	}

	while(RSI_FOREVER)
	{
		//! BT Task
		rsi_bt_app_task();

		//! BLE Task
		rsi_ble_app_task();

		//! ANT Task
		rsi_ble_ant_app_task();

		//! wireless driver task
		rsi_wireless_driver_task();
	}
}
#endif

#if ANT_BLE_CENT_BT_A2DP_SRC_APP
void rsi_app_task_ant_ble_cent_bt_a2dp_coex_task(void)
{
	int32_t	status = RSI_SUCCESS;

	//! BT A2DP Initialization
	status = rsi_bt_a2dp_source();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("bt_a2dp init failed\n");
		return;
	}

	// Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! BLE dual role Initialization
	status = rsi_ble_central_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("ble central role init failed\n");
		return;
	}

	//! ANT  role Initialization
	status = rsi_ble_ant_app();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("ANT role init got failed\n");
		return;
	}

	while(RSI_FOREVER)
	{
		//! BT Task
		rsi_bt_app_task();

		//! BLE Task
		rsi_ble_app_task();

		//! ANT Task
		rsi_ble_ant_app_task();

		//! wireless driver task
		rsi_wireless_driver_task();
	}
}
#endif

#if BT_A2DP_SOURCE_WIFI_HTTP_S_RX
void rsi_app_task_bt_a2dp_wifi_http_s(void)
{
	int32_t	status = RSI_SUCCESS;

	//! BT A2DP Initialization
	status = rsi_bt_a2dp_source();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("bt_a2dp init failed\n");
		return;
	}

	while(RSI_FOREVER)
	{
		//! WLAN Task
		rsi_wlan_app_task();

		//! BT Task
		rsi_bt_app_task();

		//! wireless driver task
		rsi_wireless_driver_task();
	}

}
#endif

#if BLE_PERIPHERAL
void rsi_app_task_ble_peripheral(void)
{
	int32_t     status = RSI_SUCCESS;

#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_dual_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE dual role init failed\n");
		return;
	}

	while (RSI_FOREVER) {

		status = rsi_ble_app_task();

#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	}
}
#endif

#if BLE_SIMPLE_GATT
void rsi_app_task_ble_simple_gatt(void)
{
	int32_t     status = RSI_SUCCESS;

#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_dual_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE dual role init failed\n");
		return;
	}

	while (RSI_FOREVER) {

		status = rsi_ble_app_task();

#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	}
}
#endif

#if BLE_PRIVACY
void rsi_app_task_ble_privacy(void)
{
	int32_t     status = RSI_SUCCESS;

#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_dual_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE dual role init failed\n");
		return;
	}

	while (RSI_FOREVER) {

		status = rsi_ble_app_task();
#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	}
}
#endif

#if BLE_GATT_TEST_ASYNC
void rsi_app_task_ble_simple_gatt_test()
{
	int32_t     status = RSI_SUCCESS;

#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! BLE dual role Initialization
	status = rsi_ble_simple_gatt_test_app_init();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE dual role init failed\n");
		return;
	}

	while (RSI_FOREVER) {

			status = rsi_ble_simple_gatt_test_app_task();
#ifndef RSI_WITH_OS
			//! wireless driver task
			rsi_wireless_driver_task();
#endif
		}
}
#endif

#if BLE_CENTRAL
void rsi_app_task_ble_central(void)
{
	int32_t     status = RSI_SUCCESS;
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_dual_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE dual role init failed\n");
		return;
	}

	while (RSI_FOREVER) {

		status = rsi_ble_app_task();
#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif

	}
}
#endif

#if BLE_DUAL_ROLE
void rsi_app_task_ble_dual_role(void)
{
	int32_t  status = RSI_SUCCESS;
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_dual_role();

	while (RSI_FOREVER) {

		rsi_ble_app_task();
#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	}
}
#endif

#if BLE_POWERSAVE
void rsi_app_task_ble_powersave(void)
{
	int32_t  status = RSI_SUCCESS;
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}  
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_dual_role();

	while (RSI_FOREVER) {

		rsi_ble_app_task();

#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	
	}
}
#endif

#if BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX
void rsi_app_task_bt_ble_dual_mode_wifi_https(void)
{
	int32_t	status = RSI_SUCCESS;
	uint8_t initialize_bt_wlan_task = 0;
	uint8_t initialize_bt_a2dp = 0;

	// Start BT Stack
		intialize_bt_stack(STACK_BTLE_MODE);

		//! BLE dual role Initialization
		status = rsi_ble_dual_role();
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("ble dual role init failed\n");
			return;
		}

	while(RSI_FOREVER)
	{
		//! BLE Task
		rsi_ble_app_task();

		if(initialize_bt_a2dp == 1)
		{
			//! BT A2DP Initialization
			status = rsi_bt_a2dp_source();
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("bt_a2dp init failed\n");
				return;
			}
			initialize_bt_a2dp = 2;
			initialize_bt_wlan_task = 1;
		}

		if( ((num_of_conn_slaves == RSI_BLE_MAX_NBR_SLAVES) && (num_of_conn_masters == RSI_BLE_MAX_NBR_MASTERS)) || (initialize_bt_wlan_task == 1))
		{
			if(initialize_bt_wlan_task == 1)
			{
			//! WLAN Task
			rsi_wlan_app_task();
			//! BT Task
			rsi_bt_app_task();
			}
			else
			{
				initialize_bt_a2dp = 1;
			}
		}
		//! wireless driver task
		rsi_wireless_driver_task();
	}

}
#endif

#if BLE_DUAL_MODE_BT_SPP_SLAVE
void rsi_app_task_bt_ble_dualmode(void)
{
	int32_t	status = RSI_SUCCESS;
	uint8_t initialize_bt_wlan_task = 0;

	/// Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! BLE dual role Initialization
	status = rsi_ble_dual_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("ble dual role init failed\n");
		return;
	}
	status = rsi_bt_spp();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("bt init failed\n");
		return;
	}

	while(RSI_FOREVER)
	{
		//! BLE Task
		rsi_ble_app_task();

		if( ((num_of_conn_slaves == RSI_BLE_MAX_NBR_SLAVES) && (num_of_conn_masters == RSI_BLE_MAX_NBR_MASTERS)) || (initialize_bt_wlan_task != 0))
		{
			initialize_bt_wlan_task = 1;
			//! BT Task
			rsi_bt_app_task();
		}
		//! wireless driver task
		rsi_wireless_driver_task();
	}

}
#endif

#if BLE_ADV_BT_A2DP_SOURCE
void rsi_app_task_ble_adv_bt_a2dp(void)
{
	int32_t	status = RSI_SUCCESS;

	//! BT A2DP Initialization
	status = rsi_bt_a2dp_source();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("bt_a2dp init failed\n");
		return;
	}

	// Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! BLE dual role Initialization
	status = rsi_ble_adv_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("ble dual role init failed\n");
		return;
	}

	while(RSI_FOREVER)
	{
		//! BT Task
		rsi_bt_app_task();

		//! BLE Task
		rsi_ble_app_task();

		//! wireless driver task
		rsi_wireless_driver_task();
	}
}
#endif

#if BLE_ADV
void rsi_app_task_ble_peripheral_adv(void)
{
	int32_t     status = RSI_SUCCESS;
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_peripheral_adv();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE init failed\n");
		return;
	}

	while (RSI_FOREVER) {

		status = rsi_ble_adv();

#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	
	}
}
#endif

#if BLE_SCAN
void rsi_app_task_ble_central_scan(void)
{
	int32_t     status = RSI_SUCCESS;
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_central_scan();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE init failed\n");
		return;
	}

	while (RSI_FOREVER) {

		status = rsi_ble_scan();

#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	}
}
#endif

#if BLE_LONG_READ
void rsi_app_task_ble_long_read() {
	int32_t status = RSI_SUCCESS;
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_app_init_ble_simple_gatt_test();
	if (status != RSI_SUCCESS) {
		LOG_PRINT("BLE init failed\n");
		return;
	}

	while (RSI_FOREVER) {
		status = rsi_ble_app_task();
#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	}
}
#endif

#if BLE_DATA_TRANSFER
void rsi_app_task_ble_data_transfer(void)
{
	int32_t     status = RSI_SUCCESS;
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! //! BLE dual role Initialization
	status = rsi_ble_peripheral_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE dual role init failed\n");
		return;
	}

	while (RSI_FOREVER) {

		status = rsi_ble_app_task();
#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	}
}
#endif

#if BLE_L2CAP_CBFC
void rsi_app_task_le_l2cap_cbfc(void)
{
	int32_t     status = RSI_SUCCESS;
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! BLE initialization
	status = rsi_ble_central_l2cap_cbfc();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("BLE init failed\n");
		return;
	}

	while (RSI_FOREVER) {

		status = rsi_ble_app_task();
#ifndef RSI_WITH_OS
		//! wireless driver task
		rsi_wireless_driver_task();
#endif
	}
}
#endif
