/**

 * @file         rsi_wlan_config_DEMO_41.h
 * @version      0.1
 * @date         06 March 2019
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains user configurable details to configure the device
 *
 *  @section Description  This file contains user configurable details to configure the device
 *
 *
 */
#ifndef SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_WLAN_CONFIG_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_H_
#define SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_WLAN_CONFIG_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_H_
#include <rsi_common_app.h>
#include "http_client.h"
#if BLE_GATT_TEST_ASYNC

/***********************************************************************************************************************************************/
//! BT_A2DP_SOURCE_WIFI_HTTP_S_RX APP CONFIG defines
/***********************************************************************************************************************************************/
#define RSI_APP_BUF_SIZE        					1600

#define THROUGHPUT_EN								0                      //!Disabled by default

#define STRING_HELPER(x)                            #x                     // turns x into a string

// Two macros are required to convert a macro value to a string
// ex: #define THE_NUMBER_NINE 9
//     STRING_HELPER(THE_NUMBER_NINE) -> "THE_NUMBER_NINE"
//     TOSTRING(THE_NUMBER_NINE) -> STRING_HELPER(9) -> "9"
#define TOSTRING(x) STRING_HELPER(x)

#define SSID               							"A2DPTEST1"             //! Access point SSID to connect

#define SECURITY_TYPE								RSI_WPA2                //! Security type -  RSI_WPA2 or RSI_OPEN

#define PSK                							"12345678"              //! Password

#define DHCP_MODE          							1                       //! DHCP mode 1- Enable 0- Disable

//! If DHCP mode is disabled give IP statically
#if !(DHCP_MODE)

#define DEVICE_IP          							0x6500A8C0               //! IP address of the module  - E.g: 0x650AA8C0 == 192.168.0.101

#define GATEWAY            							0x0100A8C0               //! IP address of Gateway  - E.g: 0x010AA8C0 == 192.168.0.1

#define NETMASK            							0x00FFFFFF               //! IP address of netmask - E.g: 0x00FFFFFF == 255.255.255.0
#endif

#define SOCKET_ASYNC_FEATURE						1

#define DEVICE_PORT        							5001                     //! Device port number

#define RSI_DNS_CLIENT								0                        //! Enable if using DNS client (when using server hostname instead of ip addr)


#if !RSI_DNS_CLIENT
#define SERVER_PORT        							5001                     //! Server port number

#define SERVER_IP_ADDRESS  							"192.168.2.2"            //! Server IP address

#define DOWNLOAD_FILENAME							"15MB.mp3"

#define HTTP_URL           							"http://" SERVER_IP_ADDRESS ":" TOSTRING(SERVER_PORT) "/" DOWNLOAD_FILENAME  //! HTTP resource name
#else
#define SSL               							1                        //! Enable SSL or not

//! Load certificate to device flash :
//! Certificate could be loaded once and need not be loaded for every boot up
#define LOAD_CERTIFICATE  							1

#define HTTP_URL           							"https://gaana.com/"     //! HTTP resource name
#endif

#define HTTP_USER_AGENT_HEADER						"User-Agent: redpine/1.0.4a"

#define HTTP_ACCEPT_HEADER							"Accept: */*"

#define MAX_DATA_PKTS  								100

#define AUDIO_FILE_NAME 							"/Song1.bin"
#define AUDIO_FILE_NAME2 							"/Song2.bin"
#define AUDIO_FILE_NAME3 							"/Song3.bin"

#define CONTINUOUS_HTTP_DOWNLOAD					0

//!Powersave configurations
#define ENABLE_POWER_SAVE  							1
#if ENABLE_POWER_SAVE
#define UAPSD_POWER_SAVE							0
#if UAPSD_POWER_SAVE
#define PSP_TYPE									RSI_UAPSD
#else
#define PSP_TYPE									RSI_MAX_PSP
#endif
#else
#define UAPSD_POWER_SAVE							0
#endif

//! Enumeration for states in application
typedef enum rsi_wlan_app_state_e
{
  RSI_WLAN_INITIAL_STATE          = 0,
  RSI_WLAN_UNCONNECTED_STATE      = 1,
  RSI_WLAN_CONNECTED_STATE        = 2,
  RSI_WLAN_IPCONFIG_DONE_STATE    = 3,
  RSI_WLAN_SOCKET_CONNECTED_STATE = 4,
  RSI_WLAN_DATA_RECEIVE_STATE  	  = 5,
  RSI_SD_WRITE_STATE			  = 6,
  RSI_WLAN_DEMO_COMPLETE_STATE	  = 7
}rsi_wlan_app_state_t;

//! wlan application control block
typedef struct rsi_wlan_app_cb_s
{
  //! wlan application state
  rsi_wlan_app_state_t state;

  //! length of buffer to copy
  uint32_t length;

  //! application buffer
  uint8_t buffer[RSI_APP_BUF_SIZE];

  //! to check application buffer availability
  uint8_t buf_in_use;

  //! application events bit map
  uint32_t event_map;

}rsi_wlan_app_cb_t;

#define SD_DEMO		                                1

#define MAX_DATA_TO_WRITE_INTO_SDCARD               (8 * 1024)

#define SD_WRITE_SZ    								(2 * 1024)

#if !SD_DEMO
//! ring buff
typedef struct {
	uint8_t buff[1024*220];
	uint32_t size;
	uint32_t head;
	uint32_t tail;
	uint32_t len;
}app_data_buff_t;

typedef struct {
	uint8_t buff[SD_WRITE_SZ];
	uint32_t len;
}sd_data_buff_t;
#endif

//! extern functions
extern void rsi_wlan_app_task(void);
extern int32_t rsi_wlan_disconnect(void);
extern void rsi_wlan_async_data_recv(uint32_t sock_no, uint8_t *buffer, uint32_t length);
extern void rsi_wlan_app_callbacks_init(void);
extern void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
extern void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
extern void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
extern void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
extern void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
extern void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
extern int32_t rsi_wlan_filter_broadcast(uint16_t beacon_drop_threshold,uint8_t filter_bcast_in_tim,uint8_t filter_bcast_tim_till_next_cmd);
extern struct http_response* parse_http_resp(char *response);
extern void http_response_free(struct http_response *hresp);
extern void parsed_url_free(struct parsed_url *purl);
extern uint64_t ip_to_reverse_hex(char *ip);
extern int32_t rsi_bt_power_save_profile(uint8_t psp_mode, uint8_t psp_type);
extern uint32_t rsi_hal_gettickcount();
extern void rsi_bt_app_clear_event(uint32_t event_num);
extern void rsi_bt_app_set_event(uint32_t event_num);
extern void rsi_bt_app_unmask_event(uint32_t event_num);
extern void rsi_bt_app_mask_event(uint32_t event_num);

//! Function declarations
int32_t rsi_app_load_ssl_cert();
int32_t rsi_app_wlan_socket_create();
int32_t rsi_app_wlan_data_recvd();
int8_t rsi_app_process_http_data(void);
void rsi_pkt_sweep_event_handler(void);

/***********************************************************************************************************************************************/
//! WLAN SAPI CONFIG DEFINES
/***********************************************************************************************************************************************/
#define RSI_ENABLE                 		1           //! Enable feature
#define RSI_DISABLE                		0           //! Disable feature
#define CONCURRENT_MODE                 RSI_DISABLE //! To enable concurrent mode

//! BT power control
#define BT_PWR_CTRL_DISABLE	            0
#define BT_PWR_CTRL 					1
#define BT_PWR_INX 						10
#define BT_EDR_3MBPS_DISABLE            0
#define BT_EDR_2MBPS_ENABLE  1
/*=======================================================================*/
//! Opermode command parameters
/*=======================================================================*/

#define RSI_CONFIG_FEATURE_BITMAP     0

#if (!ENABLE_POWER_SAVE)
#define RSI_FEATURE_BIT_MAP          	(FEAT_SECURITY_OPEN)                                                         //! To set wlan feature select bit map
#define RSI_TCP_IP_BYPASS            	RSI_DISABLE                                                                  //! TCP IP BYPASS feature check
#define RSI_TCP_IP_FEATURE_BIT_MAP   	(TCP_IP_FEAT_DHCPV4_CLIENT | TCP_IP_FEAT_DNS_CLIENT | TCP_IP_FEAT_SSL | TCP_IP_FEAT_EXTENSION_VALID)      //! TCP/IP feature select bitmap for selecting TCP/IP features
#define RSI_EXT_TCPIP_FEATURE_BITMAP   	EXT_TCP_IP_WINDOW_DIV
#define RSI_CUSTOM_FEATURE_BIT_MAP   	FEAT_CUSTOM_FEAT_EXTENTION_VALID                                             //! To set Extended custom feature select bit map

//! To set Extended custom feature select bit map
#if ENABLE_1P8V
#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP  (EXT_FEAT_320K_MODE | EXT_FEAT_1P8V_SUPPORT)
#else
#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP  (EXT_FEAT_320K_MODE)
#endif
#elif (ENABLE_POWER_SAVE)
#define RSI_FEATURE_BIT_MAP          	(FEAT_ULP_GPIO_BASED_HANDSHAKE | FEAT_DEV_TO_HOST_ULP_GPIO_1)
#define RSI_TCP_IP_BYPASS            	RSI_DISABLE                                                                  //! TCP IP BYPASS feature check
#define RSI_TCP_IP_FEATURE_BIT_MAP   	(TCP_IP_FEAT_DHCPV4_CLIENT | TCP_IP_FEAT_SSL | TCP_IP_FEAT_DNS_CLIENT | TCP_IP_FEAT_EXTENSION_VALID)       //! TCP/IP feature select bitmap for selecting TCP/IP features
#define RSI_EXT_TCPIP_FEATURE_BITMAP   	EXT_TCP_IP_WINDOW_DIV                                                                                      //! To set custom feature select bit map
#define RSI_CUSTOM_FEATURE_BIT_MAP   	FEAT_CUSTOM_FEAT_EXTENTION_VALID                                             //! To set Extended custom feature select bit map

/*=======================================================================*/
//! To set Extended custom feature select bit map
/*=======================================================================*/
#if ENABLE_1P8V
#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP  (EXT_FEAT_XTAL_CLK_ENABLE | EXT_FEAT_320K_MODE | EXT_FEAT_1P8V_SUPPORT)
#else
#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP  (EXT_FEAT_XTAL_CLK_ENABLE | EXT_FEAT_320K_MODE)
#endif
#endif

#if BT_PWR_CTRL_DISABLE
#if BT_EDR_3MBPS_DISABLE
#define RSI_BT_FEATURE_BITMAP			(A2DP_SOURCE_PROFILE_ENABLE | (BT_PWR_CTRL << 2) | (BT_PWR_INX << 3) | (BT_EDR_2MBPS_ENABLE << 8))   //! 'A2DP_ACCELERATOR_MODE_ENABLE' is disabled for SBC
#else
#define RSI_BT_FEATURE_BITMAP			(A2DP_SOURCE_PROFILE_ENABLE | (BT_PWR_CTRL << 2) | (BT_PWR_INX << 3) )   //! 'A2DP_ACCELERATOR_MODE_ENABLE' is disabled for SBC
#endif
#else
#if BT_EDR_3MBPS_DISABLE
#define RSI_BT_FEATURE_BITMAP			(A2DP_SOURCE_PROFILE_ENABLE | (BT_EDR_2MBPS_ENABLE << 8))   //! 'A2DP_ACCELERATOR_MODE_ENABLE' is disabled for SBC
#else
#define RSI_BT_FEATURE_BITMAP			(A2DP_SOURCE_PROFILE_ENABLE)   //! 'A2DP_ACCELERATOR_MODE_ENABLE' is disabled for SBC
#endif
#endif

/*=======================================================================*/
//! Band command parameters
/*=======================================================================*/
//! RSI_BAND_2P4GHZ(2.4GHz) or RSI_BAND_5GHZ(5GHz) or RSI_DUAL_BAND
#define RSI_BAND              RSI_BAND_2P4GHZ //! RSI_BAND_5GHZ or RSI_BAND_2P4GHZ


/*=======================================================================*/
//! Feature frame parameters
/*=======================================================================*/
#define PLL_MODE              0
#define RF_TYPE               1 //! 0 - External RF 1- Internal RF(use if not using external antenna) //! applicable only is feature_frame is used
#define WIRELESS_MODE         0
#define ENABLE_PPP            0
#define AFE_TYPE              1
#if(ENABLE_POWER_SAVE)
#define FEATURE_ENABLES       (RSI_FEAT_FRAME_PREAMBLE_DUTY_CYCLE | RSI_FEAT_FRAME_LP_CHAIN |RSI_FEAT_FRAME_IN_PACKET_DUTY_CYCLE)
#else
#define FEATURE_ENABLES       0
#endif
/*=======================================================================*/




/*=======================================================================*/
//! Set region command parameters
/*=======================================================================*/
#define  RSI_SET_REGION_SUPPORT            RSI_DISABLE  //@ RSI_ENABLE or RSI_DISABLE set region
#define RSI_SET_REGION_FROM_USER_OR_BEACON 1            //! If 1:region configurations taken from user ;0:region configurations taken from beacon
#define RSI_REGION_CODE                    1            //! 0-Default Region domain ,1-US, 2-EUROPE, 3-JAPAN
#define RSI_MODULE_TYPE                    1            //! 0- Without On Board Antenna , 1- With On Board Antenna


/*=======================================================================*/
//! Set region AP command parameters
/*=======================================================================*/
#define RSI_SET_REGION_AP_SUPPORT        RSI_DISABLE  //! RSI_ENABLE or RSI_DISABLE Set region AP support
#define RSI_SET_REGION_AP_FROM_USER      RSI_DISABLE  //! If 1:region configurations taken from user ;0:region configurations taken from firmware
#define RSI_COUNTRY_CODE                 "US "        //! "US" or "EU" or "JP" or other region codes


/*=======================================================================*/
//! Rejoin parameters
/*=======================================================================*/
#define RSI_REJOIN_PARAMS_SUPPORT       RSI_DISABLE    //! RSI_ENABLE or RSI_DISABLE rejoin params
#define RSI_REJOIN_MAX_RETRY           	0              //! Rejoin retry count. If 0 retries infinity times
#define RSI_REJOIN_SCAN_INTERVAL       	4              //! Periodicity of rejoin attempt
#define RSI_REJOIN_BEACON_MISSED_COUNT  40             //! Beacon missed count
#define RSI_REJOIN_FIRST_TIME_RETRY    	RSI_DISABLE    //! RSI_ENABLE or RSI_DISABLE retry for first time join failure


/*=======================================================================*/
//!BG scan command parameters
/*=======================================================================*/
#define RSI_BG_SCAN_SUPPORT     		RSI_DISABLE   //! RSI_ENABLE or RSI_DISABLE BG Scan support
#define RSI_BG_SCAN_ENABLE             	RSI_ENABLE    //! RSI_ENABLE or RSI_DISABLE BG scan
#define RSI_INSTANT_BG                 	RSI_DISABLE   //! RSI_ENABLE or RSI_DISABLE instant BG scan
#define RSI_BG_SCAN_THRESHOLD          	10            //! BG scan threshold value
#define RSI_RSSI_TOLERANCE_THRESHOLD   	4             //! RSSI tolerance Threshold
#define RSI_BG_SCAN_PERIODICITY        	10            //! BG scan Periodicity
#define RSI_ACTIVE_SCAN_DURATION       	15            //! Active scan duration
#define RSI_PASSIVE_SCAN_DURATION      	20            //! Passive scan duration
#define RSI_MULTIPROBE                 	RSI_DISABLE   //! Multi probe


/*=======================================================================*/
//!Roaming parameters
/*=======================================================================*/
#define RSI_ROAMING_SUPPORT         	RSI_DISABLE  //! RSI_ENABLE or RSI_DISABLE Roaming support
#define RSI_ROAMING_THRESHOLD          	10           //! roaming threshold value
#define RSI_ROAMING_HYSTERISIS         	4            //! roaming hysteresis value


/*=======================================================================*/
//! High Throughput Capabilities related information
/*=======================================================================*/
#define RSI_MODE_11N_ENABLE            	RSI_DISABLE  //! RSI_ENABLE or RSI_DISABLE 11n mode in AP mode
#define RSI_HT_CAPS_BIT_MAP            	2            //! HT caps bit map.


/*=======================================================================*/
//! Scan command parameters
/*=======================================================================*/
#define RSI_SCAN_CHANNEL_BIT_MAP_2_4    0                        //! scan channel bit map in 2.4GHz band,valid if given channel to scan is 0
#define RSI_SCAN_CHANNEL_BIT_MAP_5      0                        //! scan channel bit map in 5GHz band ,valid if given channel to scan is 0
#define RSI_SCAN_FEAT_BITMAP            RSI_ENABLE_QUICK_SCAN    //! scan_feature_bitmap ,valid only if specific channel to scan and SSID are given


/*=======================================================================*/
//! Enterprise configuration command parameters
/*=======================================================================*/
#define RSI_EAP_METHOD                   "TTLS"                 //! Enterprise method ,should be one of among TLS, TTLS, FAST or PEAP
#define RSI_EAP_INNER_METHOD             "\"auth=MSCHAPV2\""    //! This parameter is used to configure the module in Enterprise security mode
#define RSI_PRIVATE_KEY_PASSWORD         ""                     //! Private Key Password is required for encrypted private key, format is like "\"12345678\""


/*=======================================================================*/
//! AP configuration command parameters
/*=======================================================================*/
#define RSI_AP_KEEP_ALIVE_ENABLE        RSI_ENABLE                  //! To enable AP keep alive functionality
#define RSI_AP_KEEP_ALIVE_TYPE          RSI_NULL_BASED_KEEP_ALIVE   //! To configure keep alive type
#define RSI_AP_KEEP_ALIVE_PERIOD        100                         //! To configure keep alive period
#define RSI_MAX_STATIONS_SUPPORT         4                          //! To configure maximum stations supported


/*=======================================================================*/
//! Join command parameters
/*=======================================================================*/
#define RSI_POWER_LEVEL            RSI_POWER_LEVEL_HIGH     //! Tx power level
#define RSI_JOIN_FEAT_BIT_MAP	   0                        //! RSI_JOIN_FEAT_STA_BG_ONLY_MODE_ENABLE or RSI_JOIN_FEAT_LISTEN_INTERVAL_VALID//!
#define RSI_LISTEN_INTERVAL		   0
#define RSI_DATA_RATE              RSI_DATA_RATE_AUTO       //! Transmission data rate. Physical rate at which data has to be transmitted.


/*=======================================================================*/
//! Ipconf command parameters
/*=======================================================================*/
#define RSI_DHCP_HOST_NAME          "dhcp_client"         //! DHCP client host name


/*=======================================================================*/
//! Transmit test command parameters
/*=======================================================================*/
#define RSI_TX_TEST_RATE_FLAGS        0                 //! TX TEST rate flags
#define RSI_TX_TEST_PER_CH_BW         0                 //! TX TEST per channel bandwidth
#define RSI_TX_TEST_AGGR_ENABLE       RSI_DISABLE       //! TX TEST aggregation enable or disable
#define RSI_TX_TEST_DELAY             0                 //! TX TEST delay


/*=======================================================================*/
//! SSL parameters
/*=======================================================================*/
#define RSI_SSL_VERSION               0                       //! SSL version
#define RSI_SSL_CIPHERS               SSL_ALL_CIPHERS         //! SSL ciphers
#define PROTOCOL_DFLT_VERSION         BIT(0)                  //! Enable TCP over SSL with TLS version depends on remote side
#define PROTOCOL_TLS_1_0              ( BIT(0) | BIT(13) )    //! Enable TCP over SSL with TLS version 1.0
#define PROTOCOL_TLS_1_1              ( BIT(0) | BIT(14) )    //! Enable TCP over SSL with TLS version 1.1
#define PROTOCOL_TLS_1_2              ( BIT(0) | BIT(15) )    //! Enable TCP over SSL with TLS version 1.2


/*=======================================================================*/
//! Power save command parameters
/*=======================================================================*/
#define RSI_HAND_SHAKE_TYPE             GPIO_BASED               //! set handshake type of power mode
#define RSI_SELECT_LP_OR_ULP_MODE       RSI_ULP_WITH_RAM_RET     //! 0 - LP, 1- ULP mode with RAM retention and 2 - ULP with Non RAM retention

//! set DTIM alignment required
//! 0 - module wakes up at beacon which is just before or equal to listen_interval
//! 1 - module wakes up at DTIM beacon which is just before or equal to listen_interval
#define RSI_DTIM_ALIGNED_TYPE                     0

//! Monitor interval for the FAST PSP mode
//! default is 50 ms, and this parameter is valid for FAST PSP only
#define RSI_MONITOR_INTERVAL                      50

//! Number of DTIMs to skip during powersave
#define RSI_NUM_OF_DTIM_SKIP                      0

/*=======================================================================*/
//!WMM PS parameters
/*=======================================================================*/
#if UAPSD_POWER_SAVE

#define RSI_WMM_PS_ENABLE                         RSI_ENABLE    //! set WMM enable or disable
#define RSI_WMM_PS_TYPE                           1             //! set WMM enable or disable   //! 0- TX BASED 1 - PERIODIC
#define RSI_WMM_PS_WAKE_INTERVAL                  20            //! set WMM wake up interval
#define RSI_WMM_PS_UAPSD_BITMAP                   15            //! set WMM UAPSD bitmap

#else
#define RSI_WMM_PS_ENABLE                         RSI_DISABLE  //! set WMM enable or disable
#define RSI_WMM_PS_TYPE                           0          //! set WMM enable or disable  //! 0- TX BASED 1 - PERIODIC
#define RSI_WMM_PS_WAKE_INTERVAL                  20         //! set WMM wake up interval
#define RSI_WMM_PS_UAPSD_BITMAP                   15         //! set WMM UAPSD bitmap
#endif


/*=======================================================================*/
//! Socket configuration
/*=======================================================================*/
#define  HIGH_PERFORMANCE_ENABLE          RSI_ENABLE       //@ RSI_ENABLE or RSI_DISABLE High performance socket
#define TOTAL_SOCKETS                     1                //@ Total number of sockets. TCP TX + TCP RX + UDP TX + UDP RX
#define TOTAL_TCP_SOCKETS                 1                //@ Total TCP sockets. TCP TX + TCP RX
#define TOTAL_UDP_SOCKETS                 0                //@ Total UDP sockets. UDP TX + UDP RX
#define TCP_TX_ONLY_SOCKETS               0                //@ Total TCP TX only sockets. TCP TX
#define TCP_RX_ONLY_SOCKETS               1                //@ Total TCP RX only sockets. TCP RX
#define UDP_TX_ONLY_SOCKETS               0                //@ Total UDP TX only sockets. UDP TX
#define UDP_RX_ONLY_SOCKETS               0                //@ Total UDP RX only sockets. UDP RX
#define TCP_RX_HIGH_PERFORMANCE_SOCKETS   1                //@ Total TCP RX High Performance sockets
#define TCP_RX_WINDOW_SIZE_CAP            44               //@ TCP RX Window size - 64K (44 * 1460)
#define TCP_RX_WINDOW_DIV_FACTOR          44


/*=======================================================================*/
//! Socket Create parameters
/*=======================================================================*/
#define RSI_SOCKET_KEEPALIVE_TIMEOUT             1200      //! Initial timeout for Socket
#define RSI_VAP_ID                               0         //! VAP ID for Concurrent mode


/*=======================================================================*/
//! Timeout for join or scan
/*=======================================================================*/
#define RSI_TIMEOUT_SUPPORT         RSI_DISABLE        //!RSI_ENABLE or RSI_DISABLE Timeout support
#define RSI_TIMEOUT_BIT_MAP          1                 //! roaming threshold value
#define RSI_TIMEOUT_VALUE         	1500               //! roaming hystersis value


/*=======================================================================*/
//! Timeout for ping request
/*=======================================================================*/
#define RSI_PING_REQ_TIMEOUT	    1                  //!Timeout for PING_REQUEST

/*=======================================================================*/
//! Store Config Profile parameters
/*=======================================================================*/
#define RSI_WLAN_PROFILE_CLIENT                   0     //! Client profile
#define RSI_WLAN_PROFILE_P2P                      1     //! P2P profile
#define RSI_WLAN_PROFILE_EAP                      2     //! EAP profile
#define RSI_WLAN_PROFILE_AP                       6     //! AP profile
#define RSI_WLAN_PROFILE_ALL                      0xFF  //! All profiles


/*==============================================================================*/
//! AP Config Profile Parameters
/*==============================================================================*/
#define RSI_CONFIG_AP_DATA_RATE                   RSI_DATA_RATE_AUTO             //! Transmission data rate. Physical rate at which data has to be transmitted.
#define RSI_CONFIG_AP_WLAN_FEAT_BIT_MAP           (FEAT_SECURITY_PSK)            //! To set WLAN feature select bit map
#define RSI_CONFIG_AP_TCP_IP_FEAT_BIT_MAP         (TCP_IP_FEAT_DHCPV4_SERVER)    //! TCP/IP feature select bitmap for selecting TCP/IP features
#define RSI_CONFIG_AP_CUSTOM_FEAT_BIT_MAP         0                              //! To set custom feature select bit map
#define RSI_CONFIG_AP_BAND                        RSI_BAND_2P4GHZ                //! RSI_BAND_2P4GHZ(2.4GHz) or RSI_BAND_5GHZ(5GHz) or RSI_DUAL_BAND
#define RSI_CONFIG_AP_TX_POWER                    RSI_POWER_LEVEL_HIGH           //! Tx power level
#define RSI_CONFIG_AP_SSID                        "REDPINE_AP"                   //! AP SSID
#define RSI_CONFIG_AP_BAND                        RSI_BAND_2P4GHZ                //! RSI_BAND_2P4GHZ(2.4GHz) or RSI_BAND_5GHZ(5GHz) or RSI_DUAL_BAND
#define RSI_CONFIG_AP_CHANNEL                     6                              //! To configure AP channel number
#define RSI_CONFIG_AP_SECURITY_TYPE               RSI_WPA                        //! To configure security type
#define RSI_CONFIG_AP_ENCRYPTION_TYPE             1                              //! To configure encryption type
#define RSI_CONFIG_AP_PSK                         "1234567890"                   //! To configure PSK
#define RSI_CONFIG_AP_BEACON_INTERVAL             100                            //! To configure beacon interval
#define RSI_CONFIG_AP_DTIM                        2                              //! To configure DTIM period
#define RSI_CONFIG_AP_KEEP_ALIVE_TYPE             0                              //! RSI_NULL_BASED_KEEP_ALIVE ;//! This parameter is used to configure keep alive type
#define RSI_CONFIG_AP_KEEP_ALIVE_COUNTER          0                              //! 100
#define RSI_CONFIG_AP_KEEP_ALIVE_PERIOD           100                            //! This parameter is used to configure keep alive period
#define RSI_CONFIG_AP_MAX_STATIONS_COUNT          4                              //! This parameter is used to configure maximum stations supported
#define RSI_CONFIG_AP_TCP_STACK_USED              BIT(0)                         //! P2P Network parameters //! TCP_STACK_USED BIT(0) - IPv4, BIT(1) -IPv6, (BIT(0) | BIT(1)) - Both IPv4 and IPv6
#define RSI_CONFIG_AP_IP_ADDRESS                  0x0A0AA8C0                     //! IP address of the module //! E.g: 0x0A0AA8C0 == 192.168.10.10
#define RSI_CONFIG_AP_SN_MASK_ADDRESS             0x00FFFFFF                     //! IP address of netmask //! E.g: 0x00FFFFFF == 255.255.255.0
#define RSI_CONFIG_AP_GATEWAY_ADDRESS             0x0A0AA8C0                     //! IP address of Gateway //! E.g: 0x0A0AA8C0 == 192.168.10.10


/*======================================================================================*/
//! Client Profile Parameters
/* ===================================================================================== */
#define RSI_CONFIG_CLIENT_DATA_RATE               		RSI_DATA_RATE_AUTO    //! To configure data rate
#define RSI_CONFIG_CLIENT_WLAN_FEAT_BIT_MAP       		0                     //! To configure WLAN feature bitmap
#define RSI_CONFIG_CLIENT_TCP_IP_FEAT_BIT_MAP     		BIT(2)                //! To configure tcp/ip feature bitmap
#define RSI_CONFIG_CLIENT_CUSTOM_FEAT_BIT_MAP     		0                     //! To configure custom feature bit map
#define RSI_CONFIG_CLIENT_TX_POWER                		RSI_POWER_LEVEL_HIGH  //! To configure TX power
#define RSI_CONFIG_CLIENT_LISTEN_INTERVAL         		0                     //! To configure listen interval
#define RSI_CONFIG_CLIENT_SSID                    		"REDPINE_AP"          //! To configure SSID
#define RSI_CONFIG_CLIENT_BAND                    		RSI_BAND_2P4GHZ       //! RSI_BAND_2P4GHZ(2.4GHz) or RSI_BAND_5GHZ(5GHz) or RSI_DUAL_BAND
#define RSI_CONFIG_CLIENT_CHANNEL                 		0                     //! To configure channel number
#define RSI_CONFIG_CLIENT_SECURITY_TYPE           		RSI_WPA               //! To configure security type
#define RSI_CONFIG_CLIENT_ENCRYPTION_TYPE         		0                     //! To configure encryption type
#define RSI_CONFIG_CLIENT_PSK                     		"1234567890"          //! To configure PSK
#define RSI_CONFIG_CLIENT_PMK                     		""                    //! To configure PMK
#define RSI_CONFIG_CLIENT_TCP_STACK_USED          		BIT(0)                //! Client Network parameters //! TCP_STACK_USED BIT(0) - IPv4, BIT(1) -IPv6, (BIT(0) | BIT(1)) - Both IPv4 and IPv6
#define RSI_CONFIG_CLIENT_DHCP_MODE               		RSI_DHCP              //! DHCP mode 1- Enable 0- Disable //! If DHCP mode is disabled given IP statically
#define RSI_CONFIG_CLIENT_IP_ADDRESS              		0x0A0AA8C0            //! IP address of the module //! E.g: 0x0A0AA8C0 == 192.168.10.10
#define RSI_CONFIG_CLIENT_SN_MASK_ADDRESS         		0x00FFFFFF            //! IP address of netmask //! E.g: 0x00FFFFFF == 255.255.255.0
#define RSI_CONFIG_CLIENT_GATEWAY_ADDRESS         		0x010AA8C0            //! IP address of Gateway //! E.g: 0x010AA8C0 == 192.168.10.1
#define RSI_CONFIG_CLIENT_SCAN_FEAT_BITMAP        		0                     //! scan channel bit map in 2.4GHz band,valid if given channel to scan is 0
#define RSI_CONFIG_CLIENT_MAGIC_CODE              		0x4321                //! Scan channel magic code
#define RSI_CONFIG_CLIENT_SCAN_CHAN_BITMAP_2_4_GHZ    	0             		  //! scan channel bit map in 2.4GHz band,valid if given channel to scan is 0
#define RSI_CONFIG_CLIENT_SCAN_CHAN_BITMAP_5_0_GHZ      0                     //! scan channel bit map in 5GHz band ,valid if given channel to scan is 0


/*======================================================================================*/
//! EAP Client Profile Parameters
/* ===================================================================================== */
#define RSI_CONFIG_EAP_DATA_RATE                  RSI_DATA_RATE_AUTO                 //! To configure data rate
#define RSI_CONFIG_EAP_WLAN_FEAT_BIT_MAP          0                                  //! To configure WLAN feature bitmap
#define RSI_CONFIG_EAP_TCP_IP_FEAT_BIT_MAP        BIT(2)                             //! To configure tcp/ip feature bitmap
#define RSI_CONFIG_EAP_CUSTOM_FEAT_BIT_MAP        0                                  //! To configure custom feature bit map
#define RSI_CONFIG_EAP_TX_POWER                   RSI_POWER_LEVEL_HIGH               //! To configure EAP TX power
#define RSI_CONFIG_EAP_SCAN_FEAT_BITMAP           0                                  //! To Configure scan channel feature bitmap
#define RSI_CONFIG_EAP_CHAN_MAGIC_CODE            0                                  //0x4321 //! scan channel bit map in 2.4GHz band,valid if given channel to scan is 0
#define RSI_CONFIG_EAP_SCAN_CHAN_BITMAP_2_4_GHZ   0                                  //! scan channel bit map in 2.4GHz band,valid if given channel to scan is 0
#define RSI_CONFIG_EAP_SCAN_CHAN_BITMAP_5_0_GHZ   0                                  //! scan channel bit map in 5GHz band ,valid if given channel to scan is 0
#define RSI_CONFIG_EAP_SSID                       "REDPINE_AP"                       //! To configure SSID
#define RSI_CONFIG_EAP_BAND                       RSI_BAND_2P4GHZ                    //! RSI_BAND_2P4GHZ(2.4GHz) or RSI_BAND_5GHZ(5GHz) or RSI_DUAL_BAND
#define RSI_CONFIG_EAP_SECURITY_TYPE              RSI_WPA2_EAP                       //! To set security type
#define RSI_CONFIG_EAP_ENCRYPTION_TYPE            0                                  //! To set encryption type
#define RSI_CONFIG_EAP_CHANNEL                    0                                  //! To set channel number
#define RSI_CONFIG_EAP_METHOD                     "TLS"                              //! Enterprise method ,should be one of among TLS, TTLS, FAST or PEAP
#define RSI_CONFIG_EAP_INNER_METHOD               "\"auth=MSCHAPV2\""                //! This parameter is used to configure the module in Enterprise security mode
#define RSI_CONFIG_EAP_USER_IDENTITY              "\"user1\""                        //! To configure EAP user identity
#define RSI_CONFIG_EAP_PASSWORD                   "\"test123\""                      //! TO configure EAP password
#define RSI_CONFIG_EAP_TCP_STACK_USED             BIT(0)                             //! EAP Network parameters //! TCP_STACK_USED BIT(0) - IPv4, BIT(1) -IPv6, (BIT(0) | BIT(1)) - Both IPv4 and IPv6
#define RSI_CONFIG_EAP_DHCP_MODE                  RSI_DHCP                           //! DHCP mode 1- Enable 0- Disable  //! If DHCP mode is disabled given IP statically
#define RSI_CONFIG_EAP_IP_ADDRESS                 0x0A0AA8C0                         //! IP address of the module //! E.g: 0x0A0AA8C0 == 192.168.10.10
#define RSI_CONFIG_EAP_SN_MASK_ADDRESS            0x00FFFFFF                         //! IP address of netmask  //! E.g: 0x00FFFFFF == 255.255.255.0
#define RSI_CONFIG_EAP_GATEWAY_ADDRESS            0x010AA8C0                         //! IP address of Gateway  //! E.g: 0x010AA8C0 == 192.168.10.1


/*======================================================================================*/
//! P2P Profile parameters
/* =====================================================================================*/
#define RSI_CONFIG_P2P_DATA_RATE                  RSI_DATA_RATE_AUTO        //! To configure data rate
#define RSI_CONFIG_P2P_WLAN_FEAT_BIT_MAP          0                         //! To configure wlan feature bitmap
#define RSI_CONFIG_P2P_TCP_IP_FEAT_BIT_MAP        BIT(2)                    //! To configure P2P tcp/ip feature  bitmap
#define RSI_CONFIG_P2P_CUSTOM_FEAT_BIT_MAP        0                         //! To configure P2P custom feature bitmap
#define RSI_CONFIG_P2P_TX_POWER                   RSI_POWER_LEVEL_HIGH      //! TO configure P2P tx power level
#define RSI_CONFIG_P2P_GO_INTNET                  16                        //! Set P2P go intent //! Support only Autonomous GO mode
#define RSI_CONFIG_P2P_DEVICE_NAME               "WSC1.1"                   //! Set device name
#define RSI_CONFIG_P2P_OPERATING_CHANNEL          11                        //! Set device operating channel
#define RSI_CONFIG_P2P_SSID_POSTFIX              "WSC_1_0_0"                //! Set SSID post fix
#define RSI_CONFIG_P2P_JOIN_SSID                 "REDPINE_AP"               //! Set P2P join SSID
#define RSI_CONFIG_P2P_PSK_KEY                   "12345678"                 //! Set psk key
#define RSI_CONFIG_P2P_TCP_STACK_USED             BIT(0)                    //! P2P Network parameters  //! TCP_STACK_USED BIT(0) - IPv4, BIT(1) -IPv6, (BIT(0) | BIT(1)) - Both IPv4 and IPv6
#define RSI_CONFIG_P2P_DHCP_MODE                  1                         //! DHCP mode 1- Enable 0- Disable  //! If DHCP mode is disabled given IP statically
#define RSI_CONFIG_P2P_IP_ADDRESS                 0x0A0AA8C0                //! IP address of the module  //! E.g: 0x0A0AA8C0 == 192.168.10.10
#define RSI_CONFIG_P2P_SN_MASK_ADDRESS            0x00FFFFFF                //! IP address of netmask  //! E.g: 0x00FFFFFF == 255.255.255.0
#define RSI_CONFIG_P2P_GATEWAY_ADDRESS            0x010AA8C0                //! IP address of Gateway //! E.g: 0x010AA8C0 == 192.168.10.1
/***********************************************************************************************************************************************/
#endif
/***********************************************************************************************************************************************/
#endif /* SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_CONFIG_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_H_ */
