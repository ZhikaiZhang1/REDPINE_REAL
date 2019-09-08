/**
 * @file     rsi_nwk.c
 * @version  0.1
 * @date     15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contians networking functionality
 *
 *  @section Description  This file contains networking functionality
 *
 *
 */
/**
 * Include files
 * */
#include "rsi_driver.h"

/*==============================================*/
/**
 * @fn         int32_t rsi_driver_send_data(uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr)
 * @brief      This function is used to send data packet
 * @param[in]  sockID, socket descriptor on which data needs to be send
 * @param[in]  buffer, pointer to data 
 * @param[in]  length, data length
 * @param[in]  destAddr, pointer to destination socket details 
 * @param[out] None
 * @return    
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This 
 *
 *
 */
int32_t rsi_driver_send_data(uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_driver_send_data(global_cb_p, sockID, buffer, length, destAddr);
#else
  return api_wl->rsi_driver_send_data(global_cb_p, sockID, buffer, length, destAddr);
#endif
}


/*==============================================*/
/**
 * @fn         int32_t rsi_driver_process_recv_data(rsi_pkt_t *pkt)
 * @brief      Processes data receive packet
 * @param[in]  pkt, pointer to data packet  
 * @param[out] None
 * @return     
 *             Non zero - If fails
 *             0 - If success
 *
 * @section description
 * This function processes data receive packet  
 *
 */

int32_t rsi_driver_process_recv_data(rsi_pkt_t *pkt)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_driver_process_recv_data(global_cb_p, pkt);
#else
  return api_wl->rsi_driver_process_recv_data(global_cb_p, pkt);
#endif
}


/*====================================================*/
/**
 * @fn          uint16_t rsi_wlan_register_callbacks(rsi_nwk_callback_id_t callback_id, void (*callback_handler_ptr)(void))
 * @brief       Registers the call backs
 * @param[in]   uint32_t callback_id, call back number which needs to be registered
 * @param[in]   void (*callback_handler_ptr)(void), call back handler which needs to be registered for a given call back
 * @return      
 *                1 -  error
 *                0 - Success
 * @section description 
 * This function is used to register the call back
 */

uint16_t rsi_nwk_register_callbacks(rsi_nwk_callback_id_t callback_id, void (*callback_handler_ptr)(uint8_t command_type , uint32_t status, const uint8_t *buffer, const uint32_t length))
{

  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }
  if(callback_id == RSI_NWK_ERROR_CB)  //! check for NULL or not
  {
    //! Register join fail call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.nwk_error_call_back_handler = callback_handler_ptr;
  }
	
  return 0;
}


/*====================================================*/
/**
 * @fn          uint16_t rsi_wlan_register_callbacks(uint32_t callback_id, void (*callback_handler_ptr)((uint16_t status, const uint8_t *buffer, const uint16_t length))
 * @brief       Registers the call backs
 * @param[in]   uint32_t callback_id, call back number which needs to be registered
 * @param[in]   void (*callback_handler_ptr)(void), call back handler which needs to be registered for a given call back
 * @return      
 *                1 -  error
 *                0 - Success
 * @section description 
 * This function is used to register the call back
 */


uint16_t rsi_wlan_nwk_register_json_update_cb(uint32_t callback_id, void (*callback_handler_ptr)(uint8_t *filename, uint8_t *json_object, uint32_t length, uint32_t status))
{
  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }

  //! Register json update handler
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_json_object_update_handler = callback_handler_ptr;
  
  return 0;
}



uint16_t rsi_wlan_nwk_register_json_event_cb(uint32_t callback_id, void (*callback_handler_ptr)(uint32_t status, uint8_t *json_object, uint32_t length))
{
  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }

  //! Register json update handler
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_json_object_event_handler = callback_handler_ptr;
  
  return 0;
}

uint16_t rsi_wlan_nwk_register_webpage_req_cb(uint32_t callback_id, void (*callback_handler_ptr)(uint8_t type, uint8_t *url_name, uint8_t *post_content_buffer, uint32_t post_content_length, uint32_t status))
{
  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }

  //! Register webpage request handler
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_webpage_request_handler = callback_handler_ptr;

  return 0;
}

uint16_t rsi_wlan_nwk_register_wireless_fw_upgrade_cb(uint32_t callback_id, void (*callback_handler_ptr)(uint8_t type, uint32_t status))
{
  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }

  //! Register wireless firmware upgrade call back handler
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_wireless_fw_upgrade_handler = callback_handler_ptr;
  
  return 0;
}



