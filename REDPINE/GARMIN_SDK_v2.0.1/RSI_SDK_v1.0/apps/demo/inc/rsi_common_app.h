/*
 * rsi_common_app.h
 *
 *  Created on: 03-Nov-2018
 *     
 */

#ifndef RSI_COMMON_APP_H_
#define RSI_COMMON_APP_H_

#include "stdint.h"
#include "stdio.h"
#include "fsl_debug_console.h"

#define ENABLE_1P8V										0 //! Disable this when using 3.3v

#define SPI_DMA											  //! Enable SPI with DMA

/*********************Demo Macros*******************************/
#define BT_A2DP_SOURCE    								1
#define BLE_ADV_BT_SPP_THROUGHPUT        				0
#ifdef BLE_ADV_BT_SPP_THROUGHPUT
#define BLE_ADVERTISE_ENABLE                  			0   //! Enable BLE with SPP
#endif
#define BT_EIR_SNIFF_SPP_SLAVE							0
#define BLE_ADV_BT_A2DP_SOURCE              		    0
#define BT_PAGE_INQUIRY                      			0
#define BT_SNIFF										0
#define BT_PER											0
#define BT_SPP_RX_TX									0
#define BT_INQUIRY_SCAN                                 0
#define BT_PAGE_SCAN                                    0
#define BT_SPP_MASTER_SNIFF								0
#define BT_SPP_SLAVE_SNIFF								0
#define BT_A2DP_SOURCE_SBC_CODEC						0	

#define BLE_PERIPHERAL 									0
#define BLE_CENTRAL 									0
#define BLE_DUAL_ROLE                                   0
#define BLE_POWERSAVE									0
#define BLE_ADV  										0
#define BLE_SCAN  										0
#define BLE_IDLE_POWERSAVE								0
#define BLE_PER											0
#define BLE_PRIVACY                                     0
#define BLE_DATA_TRANSFER                               0
#define BLE_LONG_READ                                   0
#define BLE_GATT_TEST_ASYNC                             0
#define BT_BLE_IDLE_POWERSAVE                           0
#define BLE_SIMPLE_GATT		                            0

#define WIFI_SSL_RX_THROUGHPUT 							0
#define WIFI_TCP_BI_DIR 								0
#define WIFI_SSL_TX_THROUGHPUT							0
#define WIFI_TCP_RX_POWERSAVE							0
#define WIFI_CONNECTED_STANDBY							0
#define WIFI_STA_BGSCAN									0
#define WIFI_STA_CONN_DISCONN_SSL_CONN_DISCONN			0
#define WIFI_TCP_TX_POWERSAVE							0
#define WIFI_DEEPSLEEP_STANDBY							0
#define WIFI_SSL_SERVER_CERT_BYPASS						0
#define BT_A2DP_SOURCE_WIFI_HTTP_S_RX   				0
#define WIFI_HTTP_S_5MB_RX_WITH_STANDBY					0
#define FIRMWARE_UPGRADE							    0
#define FIRMWARE_UPGRADE_VIA_BOOTLOADER					0

#define BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX		0
#define BLE_DUAL_MODE_BT_SPP_SLAVE						0

#define ANT_APP 								     	0
#define ANT_APP_POWERSAVE                               0
#define BLE_CENTRAL_ANT_APP                             0
#define BLE_PERIPHERAL_ANT_APP                          0
#define ANT_BLE_PERI_BT_A2DP_SRC_APP                    0
#define ANT_BLE_CENT_BT_A2DP_SRC_APP                    0

/****************************************************/

//! WLAN Mode
#define RSI_WLAN_MODE								RSI_WLAN_CLIENT_MODE

//! CO-EX Mode configuration
#if (WIFI_SSL_RX_THROUGHPUT || WIFI_TCP_BI_DIR || WIFI_SSL_TX_THROUGHPUT || WIFI_TCP_RX_POWERSAVE || WIFI_CONNECTED_STANDBY || WIFI_STA_BGSCAN || WIFI_STA_CONN_DISCONN_SSL_CONN_DISCONN || WIFI_TCP_TX_POWERSAVE || WIFI_DEEPSLEEP_STANDBY || WIFI_HTTP_S_5MB_RX_WITH_STANDBY || FIRMWARE_UPGRADE || FIRMWARE_UPGRADE_VIA_BOOTLOADER || WIFI_IDLE_POWERSAVE || WIFI_SSL_SERVER_CERT_BYPASS)
#define RSI_COEX_MODE           					0
#elif (BT_A2DP_SOURCE_WIFI_HTTP_S_RX || BT_A2DP_SOURCE || BT_A2DP_SOURCE_SBC_CODEC || (BLE_ADV_BT_SPP_THROUGHPUT && (!BLE_ADVERTISE_ENABLE)) || BT_EIR_SNIFF_SPP_SLAVE || BT_PAGE_INQUIRY || BT_SNIFF || BT_PER || BT_SPP_RX_TX || BT_INQUIRY_SCAN || BT_PAGE_SCAN || BT_SPP_MASTER_SNIFF || BT_SPP_SLAVE_SNIFF)
#define RSI_COEX_MODE           					5
#elif ((BLE_ADV_BT_SPP_THROUGHPUT && BLE_ADVERTISE_ENABLE) || BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX || BLE_DUAL_MODE_BT_SPP_SLAVE || BLE_ADV_BT_A2DP_SOURCE || BT_BLE_IDLE_POWERSAVE)
#define RSI_COEX_MODE           					9
#elif (BLE_PERIPHERAL || BLE_CENTRAL || BLE_DUAL_ROLE || BLE_POWERSAVE || BLE_ADV || BLE_SCAN || BLE_IDLE_POWERSAVE || BLE_PER || BLE_PRIVACY || BLE_DATA_TRANSFER || BLE_LONG_READ || BLE_L2CAP_CBFC || BLE_GATT_TEST_ASYNC || BLE_SIMPLE_GATT)
#define RSI_COEX_MODE           					13
#elif ( ANT_APP || ANT_APP_POWERSAVE || BLE_CENTRAL_ANT_APP || BLE_PERIPHERAL_ANT_APP)
#define RSI_COEX_MODE           					(0x10D)
#elif (ANT_BLE_PERI_BT_A2DP_SRC_APP || ANT_BLE_CENT_BT_A2DP_SRC_APP )
#define RSI_COEX_MODE           					(0x109)
#endif

#define TICK_1_MSEC										1 	//!	0 - Disable, 1 - Enable
#ifndef RSI_WITH_OS
//! systick timer configurations
#define TICK_100_USEC									0   //!	0 - Disable, 1 - Enable
#define TICK_1_MSEC										1 	//!	0 - Disable, 1 - Enable
#define T_NOP											0 	//! Use NOP for delay
#endif

#define RSI_DEBUG_EN									1

#if RSI_DEBUG_EN
#define LOG_PRINT                       				PRINTF
#else
#define LOG_PRINT
#endif


//! Function declarations
void rsi_demo_app(void);
#if BT_A2DP_SOURCE
	  int32_t rsi_app_task_bt_a2dp_source(void);
	  int32_t rsi_bt_a2dp_source(void);

#elif BT_A2DP_SOURCE_SBC_CODEC
	  int32_t rsi_app_task_bt_a2dp_source_sbc_codec(void);
          int32_t rsi_bt_a2dp_source_sbc_codec(void);

#elif BT_PER
	  int32_t rsi_app_task_bt_per(void);

#elif BT_SNIFF
	  int32_t rsi_app_task_bt_connected_sniff(void);

#elif BT_SPP_RX_TX
	  int32_t rsi_app_task_bt_spp_rx_tx(void);

#elif BLE_ADV_BT_SPP_THROUGHPUT
	  int32_t rsi_app_task_ble_adv_bt_spp_tpt (void);

#elif BT_EIR_SNIFF_SPP_SLAVE
	  int32_t rsi_app_task_eir_sniff_spp (void);

#elif BLE_PERIPHERAL
	  void rsi_app_task_ble_peripheral(void);
	  int32_t rsi_ble_app_task(void);
	  int32_t rsi_ble_dual_role(void);

#elif BLE_SIMPLE_GATT
	  void rsi_app_task_ble_simple_gatt(void);
	  int32_t rsi_ble_app_task(void);
	  int32_t rsi_ble_dual_role(void);

#elif BLE_PRIVACY
	  void rsi_app_task_ble_privacy(void);
	  int32_t rsi_ble_app_task(void);
	  int32_t rsi_ble_dual_role(void);
	  	  
#elif BLE_PER
	  int32_t rsi_ble_per(void);

#elif BLE_DATA_TRANSFER
	  void rsi_app_task_ble_data_transfer(void);
	  int32_t rsi_ble_app_task(void);
	  int32_t rsi_ble_peripheral_role(void);
	  
#elif BLE_L2CAP_CBFC
	  void rsi_app_task_le_l2cap_cbfc(void);
	  int32_t rsi_ble_central_l2cap_cbfc(void);
	  int32_t rsi_ble_app_task(void);
	  
#elif BLE_LONG_READ
	  void rsi_app_task_ble_long_read(void);
	  int32_t rsi_app_init_ble_simple_gatt_test(void);
	  int32_t rsi_ble_app_task(void);
	  
#elif BLE_GATT_TEST_ASYNC
	  void rsi_app_task_ble_simple_gatt_test(void);
	  int32_t rsi_ble_simple_gatt_test_app_task(void);
	  int32_t rsi_ble_simple_gatt_test_app_init(void);
	  
#elif BLE_CENTRAL
	  void rsi_app_task_ble_central(void);
	  int32_t rsi_ble_app_task(void);
	  int32_t rsi_ble_dual_role(void);

#elif BLE_DUAL_ROLE
	  void rsi_app_task_ble_dual_role(void);
	  void rsi_ble_app_task(void);
	  int32_t rsi_ble_dual_role(void);
#elif BLE_POWERSAVE
	  void rsi_app_task_ble_powersave(void);
	  int32_t rsi_ble_app_task(void);
	  int32_t rsi_ble_dual_role(void);

#elif BT_PAGE_INQUIRY
	  int32_t rsi_app_task_bt_page_inquiry (void);

#elif BT_INQUIRY_SCAN
	  int32_t rsi_app_task_bt_inquiry_scan (void);

#elif BT_PAGE_SCAN
	  int32_t rsi_app_task_bt_page_scan (void);

#elif BT_SPP_MASTER_SNIFF
	  int32_t rsi_app_task_bt_spp_master_sniff(void);

#elif BT_SPP_SLAVE_SNIFF
	  int32_t rsi_app_task_bt_spp_slave_sniff(void);

#elif BLE_ADV
	  void rsi_app_task_ble_peripheral_adv(void);
	  int32_t rsi_ble_peripheral_adv(void);
	  int32_t rsi_ble_adv(void);
#elif BLE_SCAN
	  void rsi_app_task_ble_central_scan(void);
	  int32_t rsi_ble_central_scan(void);
	  int32_t rsi_ble_scan(void);

#elif BLE_IDLE_POWERSAVE
	  int32_t rsi_app_task_ble_ilde_powersave(void);

#elif WIFI_SSL_RX_THROUGHPUT
	  void rsi_app_task_wifi_ssl_rx_tpt(void);

#elif WIFI_TCP_BI_DIR
	  void rsi_app_task_wifi_tcp_bi_dir(void);

#elif WIFI_SSL_TX_THROUGHPUT
	  void rsi_app_task_wifi_ssl_tx_tpt(void);

#elif WIFI_TCP_RX_POWERSAVE
	  void  rsi_app_task_wifi_tcp_rx_ps(void);

#elif WIFI_CONNECTED_STANDBY
	  void  rsi_app_task_wifi_connected_standby(void);

#elif WIFI_STA_BGSCAN
	  void	rsi_app_task_wifi_sta_bgscan(void);

#elif WIFI_STA_CONN_DISCONN_SSL_CONN_DISCONN
	  void  rsi_app_task_wifi_sta_ssl_conn_disconn(void);

#elif WIFI_TCP_TX_POWERSAVE
	  void  rsi_app_task_wifi_tcp_tx_ps(void);

#elif WIFI_DEEPSLEEP_STANDBY
	  void rsi_app_task_wifi_deepsleep_standby(void);

#elif WIFI_SSL_SERVER_CERT_BYPASS
	  int32_t rsi_app_task_wifi_ssl_server_cert_bypass(void);
	  int32_t rsi_certificate_valid(uint16_t valid, uint16_t socket_id);

#elif BT_A2DP_SOURCE_WIFI_HTTP_S_RX
	  void rsi_app_task_bt_a2dp_wifi_http_s(void);
	  void rsi_wlan_app_task(void);
	  int32_t rsi_bt_app_task(void);
	  int32_t rsi_bt_a2dp_source(void);

#elif WIFI_HTTP_S_5MB_RX_WITH_STANDBY
	  void rsi_app_task_wifi_https_rx_standby(void);

#elif BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX
	  void rsi_app_task_bt_ble_dual_mode_wifi_https(void);
	  void rsi_wlan_app_task(void);
	  int32_t rsi_bt_app_task(void);
	  void rsi_ble_app_task(void);
	  int32_t rsi_bt_a2dp_source(void);
	  int32_t rsi_ble_dual_role(void);

#elif BLE_DUAL_MODE_BT_SPP_SLAVE
	  void rsi_app_task_bt_ble_dualmode(void);
	  int32_t rsi_bt_app_task(void);
	  void rsi_ble_app_task(void);
	  int32_t rsi_bt_spp(void);
	  int32_t rsi_ble_dual_role(void);

#elif BLE_ADV_BT_A2DP_SOURCE
	  void rsi_app_task_ble_adv_bt_a2dp(void);
	  int32_t rsi_bt_app_task(void);
	  void rsi_ble_app_task(void);
	  int32_t rsi_bt_a2dp_source(void);
	  int32_t rsi_ble_adv_role(void);

#elif FIRMWARE_UPGRADE
	  void rsi_app_task_fw_update(void);

#elif FIRMWARE_UPGRADE_VIA_BOOTLOADER
    int32_t rsi_app_task_fw_update_via_bootloader(void);
	  
#elif BT_BLE_IDLE_POWERSAVE
	  int32_t rsi_app_task_bt_ble_ilde_powersave(void);

#elif ANT_APP
	  void rsi_app_task_ant_app(void);
	  int32_t rsi_ant(void);
	  int32_t rsi_ant_power_save(void);
#elif ANT_APP_POWERSAVE
	  void rsi_app_task_ant_app_power_save(void);
	  int32_t rsi_ant_power_save(void);
	  extern int32_t rsi_bt_power_save_profile(uint8_t psp_mode, uint8_t psp_type);
#elif BLE_CENTRAL_ANT_APP
	  int32_t rsi_ble_central(void);
#elif BLE_PERIPHERAL_ANT_APP
	  int32_t rsi_ble_peripheral(void);
	//  extern void rsi_ant_register_callbacks (rsi_ant_resp_handler_t   ant_async_resp_handler);
#elif ANT_BLE_PERI_BT_A2DP_SRC_APP
	  uint32_t rsi_ble_ant_app(void);
	  void rsi_ble_ant_app_task(void);
	  int32_t rsi_ant_ble_peri_bt_a2dp_source(void);
#elif ANT_BLE_CENT_BT_A2DP_SRC_APP
	  uint32_t rsi_ble_ant_app(void);
	  void rsi_ble_ant_app_task(void);
	  int32_t rsi_ant_ble_cent_bt_a2dp_source(void);
#endif
#endif
