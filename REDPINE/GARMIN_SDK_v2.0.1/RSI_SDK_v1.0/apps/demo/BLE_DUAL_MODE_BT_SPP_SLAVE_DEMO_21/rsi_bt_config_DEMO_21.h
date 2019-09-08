/**
 * @file     rsi_bt_config_DEMO_21.h
 * @version  0.1  
 * @date     03 Sep 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contain definitions and declarations of BT APIs.
 *
 *  @section Description  This file contains definitions and declarations required to
 *  configure BT module.
 *
 *
 */

#ifndef SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_BLE_DUAL_MODE_BT_SPP_SLAVE_H_
#define SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_BLE_DUAL_MODE_BT_SPP_SLAVE_H_
#include <rsi_common_app.h>
#if BLE_DUAL_MODE_BT_SPP_SLAVE
#include<rsi_data_types.h>

/***********************************************************************************************************************************************/
//! BLE_DUAL_MODE_BT_SPP_SLAVE APP CONFIG defines
/***********************************************************************************************************************************************/

#define RSI_BT_LOCAL_NAME              	          "BT_COEX_SPP_SLAVE"
#define PIN_CODE                       	          "1234"

#define SPP_TX_SEND                  	          0   
#define THROUGHPUT_EN					          0

/***********************************************************************************************************************************************/
//! application events list
/***********************************************************************************************************************************************/
#define RSI_APP_EVENT_CONNECTED       	1
#define RSI_APP_EVENT_PINCODE_REQ     	2
#define RSI_APP_EVENT_LINKKEY_SAVE    	3
#define RSI_APP_EVENT_AUTH_COMPLT     	4
#define RSI_APP_EVENT_DISCONNECTED    	5
#define RSI_APP_EVENT_LINKKEY_REQ     	6
#define RSI_APP_EVENT_SPP_CONN        	7
#define RSI_APP_EVENT_SPP_DISCONN     	8
#define RSI_APP_EVENT_SPP_RX          	9
/** ssp related defines********/
#define RSI_APP_EVENT_PASSKEY_DISPLAY 	10
#define RSI_APP_EVENT_PASSKEY_REQUEST 	11
#define RSI_APP_EVENT_SSP_COMPLETE    	12
#define RSI_APP_EVENT_CONFIRM_REQUEST 	13


#define DATA_TX  "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN            10000
/***********************************************************************************************************************************************/
//! OS task related defines
/***********************************************************************************************************************************************/
#ifdef RSI_WITH_OS
#define RSI_BT_TASK_STACK_SIZE                    3000                                       //! BLE task stack size
#define RSI_BT_TASK_PRIORITY                      1                                          //! BT task priority
#define NUMBER_OF_PACKETS                         1000                                       //! Number of packet to send or receive
#define RSI_DRIVER_TASK_PRIORITY                  2                                          //! Wireless driver task priority
#define RSI_DRIVER_TASK_STACK_SIZE                3000                                       //! Wireless driver task stack size
void rsi_wireless_driver_task(void);
#endif
/***********************************************************************************************************************************************/

/***********************************************************************************************************************************************/
//! BT SAPI CONFIG DEFINES
/***********************************************************************************************************************************************/
//! WLAN + BT Classic coex mode
#define RSI_OPERMODE_WLAN_BT_CLASSIC                5

#define RSI_BT_BLE_DUAL_MODE                        9

/***********************************************************************************************************************************************/
//! Discovery command parameters
/***********************************************************************************************************************************************/

//! Discovery mode
//! Start the discoverable mode
#define START_DISCOVERY                             0x01
//! Start the limited discoverable mode
#define START_LIMITED_DISCOVERY                     0x02
//! Stop the discoverable mode
#define STOP_DISCOVERY                              0x00


/***********************************************************************************************************************************************/
//! Connectability command parameters
/***********************************************************************************************************************************************/

//! Connectable mode
//! Start the connectable mode
#define CONNECTABLE                                 0x01
//! Stop the connectable mode
#define NON_CONNECTABLE                             0x00


/***********************************************************************************************************************************************/
//!  Authentication command parameters
/***********************************************************************************************************************************************/

//! Pair mode
//! Enable authentication
//#define RSI_ENABLE                                0x01
//! Enable authentication
//#define RSI_DISABLE                               0x00


/***********************************************************************************************************************************************/
//!  SSP Confirmation command parameters
/***********************************************************************************************************************************************/

//! Confimation type
//! Accept confirmation
#define ACCEPT                                      0x01
//! Reject confirmation
#define REJECT                                      0x00


/***********************************************************************************************************************************************/
//!  BT classic profile enable BITS
/***********************************************************************************************************************************************/
#define RSI_SPP_PROFILE_BIT                         BIT(0)
#define RSI_A2DP_PROFILE_BIT                        BIT(1)
#define RSI_AVRCP_PROFILE_BIT                       BIT(2)
#define RSI_HFP_PROFILE_BIT                         BIT(3)
#define RSI_PBAP_PROFILE_BIT                        BIT(4)

/***********************************************************************************************************************************************/
//!  SSP Confirmation command parameters
/***********************************************************************************************************************************************/

//! A2DP Profile Enable
//#define A2DP_PROFILE_ENABLE
//! A2DP profile enable -> set the 23 bit in bt_feature_bit_map

/***********************************************************************************************************************************************/

#define RSI_BT_MAX_PAYLOAD_SIZE              1040 /* Max supported is 200, but 190 gives optimum Tx throughput */
#define BT_BDR_MODE                           0   //! 1 - HP chain
/***********************************************************************************************************************************************/

#endif
#endif /* SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_BLE_DUAL_MODE_BT_SPP_SLAVE_H_ */
