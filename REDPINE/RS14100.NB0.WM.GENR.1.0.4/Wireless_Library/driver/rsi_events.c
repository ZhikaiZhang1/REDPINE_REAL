/**
 * @file       rsi_event.c
 * @version    0.1
 * @date       2015-Aug-15
 *
 * Copyright(C) Redpine Signals 2012
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contains events handling
 *
 * @section Description
 * This file contains events handling
 *
 *
 */

/**
 * Includes
 */

#include "rsi_driver.h"

/**
 * Global Variables
 */

/*====================================================*/
/**
 * @fn          void rsi_set_event(uint32_t event_num)
 * @brief       Sets an event
 * @param[in]   uint32_t event_num, event number to be set
 * @return      void
 * @section description 
 * This function sets an event
 */
void rsi_set_event(uint32_t event_num)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_set_event(global_cb_p, event_num);
#else
  api_wl->rsi_set_event(global_cb_p, event_num);
#endif
}

/*====================================================*/
/**
 * @fn          void rsi_clear_event(uint32_t event_num)
 * @brief       Clears an event
 * @param[in]   uint32_t event_num, event number to clear
 * @return      void
 * @section description 
 * This function clears an event
 */
void rsi_clear_event(uint32_t event_num)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_clear_event(global_cb_p, event_num);
#else
  api_wl->rsi_clear_event(global_cb_p, event_num);
#endif
}

/*====================================================*/
/**
 * @fn          void rsi_mask_event(uint32_t event_num)
 * @brief       Masks an event
 * @param[in]   uint32_t event_num, event number to mask
 * @return      void
 * @section description 
 * This function masks an event
 */
void rsi_mask_event(uint32_t event_num)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_mask_event(global_cb_p, event_num);
#else
  api_wl->rsi_mask_event(global_cb_p, event_num);
#endif
}

/*====================================================*/
/**
 * @fn          void rsi_unmask_event(uint32_t event_num)
 * @brief       Unmasks an event
 * @param[in]   uint32_t event_num, event number to unmask
 * @return      void
 * @section description 
 * This function is used to unmask an event
 */
void rsi_unmask_event(uint32_t event_num)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_unmask_event(global_cb_p, event_num);
#else
  api_wl->rsi_unmask_event(global_cb_p, event_num);
#endif
}

/*====================================================*/
/**
 * @fn          void rsi_find_event(uint32_t event_map)
 * @brief       Returns the event set from the event map
 * @param[in]   uint32_t event_map, event map
 * @return      void
 * @section description 
 * This function is used find the event which is set from the map
 */
uint32_t rsi_find_event(uint32_t event_map)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_find_event(global_cb_p, event_map);
#else
  return api_wl->rsi_find_event(global_cb_p, event_map);
#endif
}

/*====================================================*/
/**
 * @fn          uint16_t rsi_register_event(uint32_t event_id, void (*event_handler_ptr)(void))
 * @brief       Registers the event
 * @param[in]   uint32_t event_id, event number which needs to be registered
 * @param[in]   void (*event_handler_ptr)(void), event handler which needs to be registered for a given event
 * @return      
 *                1 -  error
 *                0 - Success
 * @section description 
 * This function is used to register the event
 */
uint16_t rsi_register_event(uint32_t event_id, void (*event_handler_ptr)(void))
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_register_event(global_cb_p, event_id, event_handler_ptr);
#else
  return api_wl->rsi_register_event(global_cb_p, event_id, event_handler_ptr);
#endif
}


/*====================================================*/
/**
 * @fn          void rsi_events_init(void)
 * @brief       Initializes events
 * @param[in]   None
 * @param[out]  None
 * @return      void
 * @section description 
 * This function is used to initialize the events
 */
void rsi_events_init(void)
{
  //! Initialze RX event
  rsi_register_event(RSI_RX_EVENT, rsi_rx_event_handler);

  //! Initialze TX event
  rsi_register_event(RSI_TX_EVENT, rsi_tx_event_handler);
}



