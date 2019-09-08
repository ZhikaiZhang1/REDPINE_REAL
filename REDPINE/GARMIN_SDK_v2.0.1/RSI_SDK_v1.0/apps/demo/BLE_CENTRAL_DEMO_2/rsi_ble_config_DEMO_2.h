/**
 * @file         rsi_ble_config_DEMO_2.h
 * @version      0.1
 * @date         15 Aug 2015 *
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
 */

#ifndef SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BLE_CONFIG_BLE_CENTRAL_H_
#define SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BLE_CONFIG_BLE_CENTRAL_H_
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#include "rsi_common_app.h"
#include "stdint.h"

#if BLE_CENTRAL
//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>

//! Common include file
#include<rsi_common_apis.h>

#include <string.h>

/***********************************************************************************************************************************************/
//! BLE_CENTRAL APP CONFIG defines
/***********************************************************************************************************************************************/

/***********************************************************************************************************************************************/
//! application events list
/***********************************************************************************************************************************************/
#define  RSI_BLE_ADV_REPORT_EVENT       		      1
#define  RSI_BLE_CONN_EVENT             		      2
#define  RSI_BLE_DISCONN_EVENT          		      3
#define  RSI_BLE_SCAN_RESTART_EVENT     		      4
#define  RSI_BLE_PROFILE_LIST_EVENT     		      5
#define  RSI_BLE_GATT_NOTIFY_EVENT      		      6
#define  RSI_BLE_CONN_UPDATE_EVENT     			      7
#define  RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT        8
#define  RSI_BLE_GATT_PROFILE_RESP_EVENT              9
/***********************************************************************************************************************************************/

#define DATA_TRANSFER_ENABLE                          1                               //! To enable data transfer in BLE
#define NO_OF_ADV_REPORTS                             10                              //! Maximum number of advertise reports to hold

#define MAX_NUM_OF_SLAVES                             1                               //! Maximum number of multiple slaves supported

#if(DATA_TRANSFER_ENABLE == 1)
#define MAX_NUM_OF_NOTIFIED_DEVICES                   6                               //! Maximum number of devices to be notified.  
#endif


#define BT_GLOBAL_BUFF_LEN                              10000                   //! Memory length for driver

/***********************************************************************************************************************************************/
//! OS Task defines
/***********************************************************************************************************************************************/
#ifdef RSI_WITH_OS
#define RSI_BT_TASK_STACK_SIZE                          1000                    //! BLE task stack size
#define RSI_BT_TASK_PRIORITY                            1                       //! BT task priority
#define NUMBER_OF_PACKETS                               1000                    //! Number of packet to send or receive
#define RSI_DRIVER_TASK_PRIORITY                        2                       //! Wireless driver task priority
#define RSI_DRIVER_TASK_STACK_SIZE                      3000                    //! Wireless driver task stack size
void rsi_wireless_driver_task(void);
#endif
/***********************************************************************************************************************************************/

/************************************************************************************************************/
//! User configurations
/************************************************************************************************************/
#define RSI_BLE_LOCAL_NAME                              (void *)"BLE_PERIPHERAL"               //! Device Local Name
#define RSI_SEL_ANTENNA                                 RSI_SEL_INTERNAL_ANTENNA //! Antenna Selection - RSI_SEL_EXTERNAL_ANTENNA / RSI_SEL_INTERNAL_ANTENNA
#define ENABLE_POWER_SAVE                               1                       //! Power save enable
#define	PSP_MODE                                        RSI_SLEEP_MODE_2        //! Power Save Profile Mode
#define PSP_TYPE                                        RSI_MAX_PSP             //! Power Save Profile type

#define CONN_BY_ADDR                                    1
#define CONN_BY_NAME                                    2
#define CONNECT_OPTION_APP2                             CONN_BY_NAME

#if(CONNECT_OPTION_APP2 == CONN_BY_NAME)
#define RSI_REMOTE_DEVICE_NAME                           "nexus"                   //! Remote Device Name to connect
#else

#define RSI_BLE_DEV_ADDR_TYPE                            LE_PUBLIC_ADDRESS        //! Address type of the device to connect
#define RSI_BLE_DEV_1_ADDR                               "C2:74:10:B7:52:C6"      //! Address of the devices to connect 
#endif

#if(DATA_TRANSFER_ENABLE == 1)

/***********************************************************************************************************************************************/
//! BLE attribute service types uuid values
/***********************************************************************************************************************************************/
#define  RSI_BLE_CHAR_SERV_UUID                          0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID                        0x2902

/***********************************************************************************************************************************************/
//! BLE characteristic service and attribute uuid
/***********************************************************************************************************************************************/
#define  RSI_BLE_NEW_SERVICE_UUID                        0xAABB
#define  RSI_BLE_ATTRIBUTE_1_UUID                        0x1AA1
#define  RSI_BLE_ATTRIBUTE_2_UUID                        0x1BB1

/***********************************************************************************************************************************************/
//! BLE client characteristic service and attribute uuid.
/***********************************************************************************************************************************************/
#define  RSI_BLE_NEW_CLIENT_SERVICE_UUID                 0x180D                  //! immediate alert service uuid
#define  RSI_BLE_CLIENT_ATTRIBUTE_1_UUID                 0x2A37                  //! Alert level characteristic
/***********************************************************************************************************************************************/

#define  RSI_BLE_MAX_DATA_LEN                            20                      //! max data length

/***********************************************************************************************************************************************/
//! attribute properties
/***********************************************************************************************************************************************/
#define  RSI_BLE_ATT_PROPERTY_READ                       0x02
#define  RSI_BLE_ATT_PROPERTY_WRITE                      0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY                     0x10
/***********************************************************************************************************************************************/

//! user defined structure
/***********************************************************************************************************************************************/
//!Notification enabled devices structure
typedef struct rsi_ble_notified_device_s
{
  uint8_t used;
  uint8_t dev_addr[RSI_DEV_ADDR_LEN];
}rsi_ble_notified_device_t;
#endif

/***********************************************************************************************************************************************/

extern int32_t rsi_bt_power_save_profile(uint8_t psp_mode, uint8_t psp_type);
extern int32_t rsi_ble_conn_params_update(int8_t *remote_dev_address, uint16_t min_int, uint16_t max_int, uint16_t latency, uint16_t timeout);
extern int32_t rsi_ble_notify_value (uint8_t  *dev_addr, uint16_t  handle, uint16_t  data_len, uint8_t  *p_data);

/***********************************************************************************************************************************************/
//! BLE_CENTRAL SAPI CONFIG DEFINES
/***********************************************************************************************************************************************/


#define RSI_BLE_SET_RAND_ADDR                       "00:23:A7:12:34:56"

#define CLEAR_WHITELIST                              0x00
#define ADD_DEVICE_TO_WHITELIST                      0x01
#define DELETE_DEVICE_FROM_WHITELIST                 0x02

#define ALL_PHYS                                     0x00

#define RSI_BLE_DEV_ADDR_RESOLUTION_ENABLE           0

#define RSI_OPERMODE_WLAN_BLE                        13

#define RSI_BLE_MAX_NBR_ATT_REC                      80
#define RSI_BLE_MAX_NBR_ATT_SERV                     10

#define RSI_BLE_MAX_NBR_SLAVES                       3
#define RSI_BLE_MAX_NBR_MASTERS                      1

#define RSI_BLE_GATT_ASYNC_ENABLE                    0

/* Number of BLE notifications */
#define RSI_BLE_NUM_CONN_EVENTS                      20

/* Number of BLE GATT RECORD SIZE IN (n*16 BYTES), eg:(0x40*16)=1024 bytes */
#define RSI_BLE_NUM_REC_BYTES                      0x40


/***********************************************************************************************************************************************/
//! Advertising command parameters
/***********************************************************************************************************************************************/

#define RSI_BLE_ADV_TYPE                             UNDIR_CONN
#define RSI_BLE_ADV_FILTER_TYPE                      ALLOW_SCAN_REQ_ANY_CONN_REQ_ANY
#define RSI_BLE_ADV_DIR_ADDR_TYPE                    LE_PUBLIC_ADDRESS
#define RSI_BLE_ADV_DIR_ADDR                         "00:15:83:6A:64:17"

#define  RSI_BLE_ADV_INT_MIN                         0x100
#define  RSI_BLE_ADV_INT_MAX                         0x200
#define  RSI_BLE_ADV_CHANNEL_MAP                     0x07

#define RSI_BLE_START_ADV                            0x01                        //! Start the advertising process
#define RSI_BLE_STOP_ADV                             0x00                        //! Stop the advertising process
/***********************************************************************************************************************************************/

/***********************************************************************************************************************************************/
//!Advertise types
/***********************************************************************************************************************************************/
/* Advertising will be visible(discoverable) to all the devices.
 * Scanning/Connection is also accepted from all devices
 * */
#define UNDIR_CONN                                   0x80

/* Advertising will be visible(discoverable) to the particular device
 * mentioned in RSI_BLE_ADV_DIR_ADDR only.
 * Scanning and Connection will be accepted from that device only.
 * */
#define DIR_CONN                                     0x81

/* Advertising will be visible(discoverable) to all the devices.
 * Scanning will be accepted from all the devices.
 * Connection will be not be accepted from any device.
 * */
#define UNDIR_SCAN                                   0x82

/* Advertising will be visible(discoverable) to all the devices.
 * Scanning and Connection will not be accepted from any device
 * */
#define UNDIR_NON_CONN                               0x83

/* Advertising will be visible(discoverable) to the particular device
 * mentioned in RSI_BLE_ADV_DIR_ADDR only.
 * Scanning and Connection will be accepted from that device only.
 * */
 /***********************************************************************************************************************************************/

#define DIR_CONN_LOW_DUTY_CYCLE                      0x84

/***********************************************************************************************************************************************/
//!Advertising flags
/***********************************************************************************************************************************************/
#define LE_LIMITED_DISCOVERABLE                      0x01
#define LE_GENERAL_DISCOVERABLE                      0x02
#define LE_BR_EDR_NOT_SUPPORTED                      0x04

/***********************************************************************************************************************************************/
//!Advertise filters
/***********************************************************************************************************************************************/
#define ALLOW_SCAN_REQ_ANY_CONN_REQ_ANY               0x00
#define ALLOW_SCAN_REQ_WHITE_LIST_CONN_REQ_ANY        0x01
#define ALLOW_SCAN_REQ_ANY_CONN_REQ_WHITE_LIST        0x02
#define ALLOW_SCAN_REQ_WHITE_LIST_CONN_REQ_WHITE_LIST 0x03

/***********************************************************************************************************************************************/
//! Address types
/***********************************************************************************************************************************************/
#define LE_PUBLIC_ADDRESS                            0x00
#define LE_RANDOM_ADDRESS                            0x01
#define LE_RESOLVABLE_PUBLIC_ADDRESS                 0x02
#define LE_RESOLVABLE_RANDOM_ADDRESS                 0x03

/***********************************************************************************************************************************************/
//! Connection parameters
/***********************************************************************************************************************************************/
#define LE_SCAN_INTERVAL                             0x0100
#define LE_SCAN_WINDOW                               0x0050

#define CONNECTION_INTERVAL_MIN                      0x00A0
#define CONNECTION_INTERVAL_MAX                      0x00A0

#define CONNECTION_LATENCY                           0x0000
#define SUPERVISION_TIMEOUT                          0x07D0 //2000
/***********************************************************************************************************************************************/

/***********************************************************************************************************************************************/
//! Scan command parameters
/***********************************************************************************************************************************************/

#define RSI_BLE_SCAN_TYPE                            SCAN_TYPE_ACTIVE
#define RSI_BLE_SCAN_FILTER_TYPE                     SCAN_FILTER_TYPE_ALL

//!Scan status
#define RSI_BLE_START_SCAN                           0x01
#define RSI_BLE_STOP_SCAN                            0x00

//!Scan types
#define SCAN_TYPE_ACTIVE                             0x01
#define SCAN_TYPE_PASSIVE                            0x00

//!Scan filters
#define SCAN_FILTER_TYPE_ALL                         0x00
#define SCAN_FILTER_TYPE_ONLY_WHITE_LIST             0x01
/***********************************************************************************************************************************************/

#define RSI_BLE_PWR_INX                              30                          //! BLE Tx Power Index On Air  
/***********************************************************************************************************************************************/
//! BLE Active H/w Pwr Features
/***********************************************************************************************************************************************/
#define BLE_DISABLE_DUTY_CYCLING                     0
#define BLE_DUTY_CYCLING                             1
#define BLR_DUTY_CYCLING                             2
#define BLE_4X_PWR_SAVE_MODE                         4
#define RSI_BLE_PWR_SAVE_OPTIONS                     BLE_DISABLE_DUTY_CYCLING

#define RSI_SEL_INTERNAL_ANTENNA                     0x00
#define RSI_SEL_EXTERNAL_ANTENNA                     0x01
/***********************************************************************************************************************************************/
#endif
#endif /* SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BLE_CONFIG_BLE_CENTRAL_H_ */
