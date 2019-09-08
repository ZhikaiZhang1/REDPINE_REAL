/**
 * @file    rsi_wlan_powersave_DEMO_13.c
 * @version 0.1
 * @date    13-Nov-2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
*
 */
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#include <rsi_common_app.h>
#if WIFI_CONNECTED_STANDBY
#include "stdlib.h"
#include "rsi_driver.h"


/*=======================================================================*/
//  ! VARIABLES
/*=======================================================================*/
//! application control block
rsi_wlan_app_cb_t rsi_wlan_app_cb;

/*=======================================================================*/
//  ! EXTERN VARIABLES
/*=======================================================================*/
extern uint8_t powersave_d;

/*************************************************************************/
//!  CALLBACK FUNCTIONS
/*************************************************************************/
/*====================================================*/
/**
 * @fn          void rsi_wlan_app_callbacks_init(void)
 * @brief       To initialize WLAN application callback
 * @param[in]   void
 * @return      void
 * @section description
 * This callback is used to initialize WLAN
 * ==================================================*/
void rsi_wlan_app_callbacks_init(void)
{
  rsi_wlan_register_callbacks(RSI_JOIN_FAIL_CB, rsi_join_fail_handler);                                   //! Initialize join fail call back
  rsi_wlan_register_callbacks(RSI_IP_FAIL_CB, rsi_ip_renewal_fail_handler);                               //! Initialize IP renewal fail call back
  rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);       //! Initialize remote terminate call back
  rsi_wlan_register_callbacks(RSI_IP_CHANGE_NOTIFY_CB, rsi_ip_change_notify_handler);                     //! Initialize IP change notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, rsi_stations_connect_notify_handler);       //! Initialize IP change notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, rsi_stations_disconnect_notify_handler); //! Initialize IP change notify call back
}

void  rsi_app_task_wifi_connected_standby(void)
{
  int32_t		status = RSI_SUCCESS;
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
  while(1)	  
#endif
  {
    switch(rsi_wlan_app_cb.state)
    {
      case RSI_WLAN_INITIAL_STATE:
        {
          rsi_wlan_app_callbacks_init();                        //! register call backs
          rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;   //! update WLAN application state

        }
      case RSI_WLAN_UNCONNECTED_STATE:
        {
          status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT("WLAN connection failed\n");
            break;
          }
          else
          {
            LOG_PRINT("wlan connected\n");
            status = rsi_wlan_filter_broadcast(5000, 1, 1);   //!Broadcast filter
            if(status != RSI_SUCCESS)
            {
              LOG_PRINT("Filter broadcast failure\n");
              return;
            }
            status = rsi_wlan_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);      //! Apply power save profile with standby
            if(status != RSI_SUCCESS)
            {
              return;
            }
            LOG_PRINT("In CONNECTED STANDBY mode...\n");
            powersave_d = 1;
#ifdef RSI_WITH_OS
        while(1);
#endif            
            break;
          }
          //! update wlan application state
          rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
          LOG_PRINT("wlan connected state\n");

        }
      case RSI_WLAN_CONNECTED_STATE:
          break;
      default:
          break;
    }
  }
}

/*====================================================*/
/**
 * @fn         void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler in station mode at rejoin failure
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;      //! update WLAN application state
}


/*====================================================*/
/**
 * @fn         void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      IP renewal failure call back handler in station mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  //! update WLAN application state
  rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
}


/*====================================================*/
/**
 * @fn         void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to terminate stations remote socket
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	//! update WLAN application state
	rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
}


/*====================================================*/
/**
 * @fn         void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify IP change in Station mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  //! update WLAN application state
  rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
}


/*====================================================*/
/**
 * @fn         void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify stations connect in AP mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{

}

/*====================================================*/
/**
 * @fn         void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify stations disconnect in AP mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{

}
#endif
