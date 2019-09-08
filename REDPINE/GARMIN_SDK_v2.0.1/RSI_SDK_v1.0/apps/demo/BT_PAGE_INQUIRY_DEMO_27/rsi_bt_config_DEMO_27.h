/**
 * @file     rsi_bt_config_DEMO_27.h
 * @version  0.1
 * @date     03 Feb 2019
 *
 *  Copyright(C) Redpine Signals 2019
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
#ifndef SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_BT_PAGE_INQ_H_
#define SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_BT_PAGE_INQ_H_

#include <rsi_common_app.h>
#if BT_PAGE_INQUIRY
#include<rsi_data_types.h>


/***********************************************************************************************************************************************/
//! GOOGLE_BT_PAGE_INQUIRY APP CONFIG defines
/***********************************************************************************************************************************************/
//! application defines


//! application defines
#define RSI_BT_LOCAL_NAME             "BT_EIR_SNIFF"
#define PIN_CODE                      "4321"

#define SNIFF_SET                     0

#define PSP_TYPE					  RSI_MAX_PSP

//! application events list
#define RSI_APP_EVENT_CONNECTED       1
#define RSI_APP_EVENT_DISCONNECTED    2

//! Inquiry complete event
#define RSI_APP_EVENT_INQ_COMPLT      3
#define RSI_APP_EVENT_INQ_RESP        4


#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 3000

//! BT task priority
#define RSI_BT_TASK_PRIORITY   1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000

void rsi_wireless_driver_task(void);

#endif



/******************************************************
 * *                      Macros
 * ******************************************************/

//! WLAN + BT Classic coex mode
#define RSI_OPERMODE_WLAN_BT_CLASSIC                5

//!Inquiry parameters
#define INQUIRY_DURATION                  0x500
#define MAX_DEVICES                       0x00

//!Enable this macro for BT paging mode
#define BT_PAGE_ENABLE                    0

//! Remote device address to connect
#define REMOTE_BD_ADDR                "FF:1B:DC:07:2C:F0"

/*=======================================================================*/
//! Discovery command parameters
/*=======================================================================*/

//! Discovery mode
//! Start the discoverable mode
#define START_DISCOVERY                             0x01
//! Start the limited discoverable mode
#define START_LIMITED_DISCOVERY                     0x02
//! Stop the discoverable mode
#define STOP_DISCOVERY                              0x00


/*=======================================================================*/
//! Connectability command parameters
/*=======================================================================*/

//! Connectable mode
//! Start the connectable mode
#define CONNECTABLE                                  0x01
//! Stop the connectable mode
#define NON_CONNECTABLE                              0x00


/*=======================================================================*/
//!  Authentication command parameters
/*=======================================================================*/

//! Pair mode
//! Enable authentication
//#define RSI_ENABLE                                  0x01
//! Enable authentication
//#define RSI_DISABLE                                 0x00


/*=======================================================================*/
//!  SSP Confirmation command parameters
/*=======================================================================*/

//! Confimation type
//! Accept confirmation
#define ACCEPT                                  0x01
//! Reject confirmation
#define REJECT                                  0x00


/*=======================================================================*/
//!  BT classic profile enable BITS
/*=======================================================================*/
#define RSI_SPP_PROFILE_BIT                         BIT(0)
#define RSI_A2DP_PROFILE_BIT                        BIT(1)
#define RSI_AVRCP_PROFILE_BIT                       BIT(2)
#define RSI_HFP_PROFILE_BIT                         BIT(3)
#define RSI_PBAP_PROFILE_BIT                        BIT(4)

/*=======================================================================*/
//!  SSP Confirmation command parameters
/*=======================================================================*/

//! A2DP Profile Enable
//#define A2DP_PROFILE_ENABLE
//! A2DP profile enable -> set the 23 bit in bt_feature_bit_map

/*=======================================================================*/

#define RSI_BT_MAX_PAYLOAD_SIZE              310 /* Max supported is 200, but 190 gives optimum Tx throughput */
#define BT_BDR_MODE                           0 // 1 - HP chain

#endif
#endif
