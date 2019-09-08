#ifndef __UDMA_H__
#define __UDMA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rsi_chip.h"
#include "RTE_Device.h"
#include "Driver_Common.h"

#define UDMA_EVENT_XFER_DONE              (1)
#define UDMA_EVENT_ERROR                  (2)

/* UDMA Resource Configuration */
typedef struct 
{
  UDMA0_Type             *reg;               // UDMA register interface
  IRQn_Type              udma_irq_num;       // UDMA Event IRQ Numbe
  RSI_UDMA_DESC_T        *desc;              // Run-Time control information
} UDMA_RESOURCES;

/* Number of UDMA channels */
#define UDMA_NUMBER_OF_CHANNELS           ((uint8_t) 32)
#define ULP_UDMA_NUMBER_OF_CHANNELS       ((uint8_t) 12)

/**
  \fn          void UDMA_SignalEvent_t (uint32_t event)
  \brief       Signal UDMA Events.
  \param[in]   event  UDMA Event mask
  \return      none
*/
typedef void  (*UDMA_SignalEvent_t) (uint32_t event, uint8_t ch);

/* UDMA1 API prototypes */
int32_t  UDMA0_Initialize(void);
int32_t  UDMA0_ChannelEnable (uint8_t ch);
int32_t  UDMA0_ChannelDisable (uint8_t ch);
uint32_t UDMA0_ChannelGetCount (uint8_t ch, RSI_UDMA_CHA_CONFIG_DATA_T control,
                                RSI_UDMA_CHA_CFG_T  config);
int32_t UDMA0_DMAEnable(void);
int32_t UDMA0_ChannelConfigure ( uint8_t             ch,
                                 uint32_t            src_addr,
                                 uint32_t            dest_addr,
                                 uint32_t            size,
                                 RSI_UDMA_CHA_CONFIG_DATA_T control,
                                 RSI_UDMA_CHA_CFG_T  *config,
                                 UDMA_SignalEvent_t  cb_event );
int32_t UDMA0_Uninitialize (void); 

/* UDMA1 API prototypes */
int32_t  UDMA1_Initialize(void);
int32_t  UDMA1_ChannelEnable (uint8_t ch);
int32_t  UDMA1_ChannelDisable (uint8_t ch);
uint32_t UDMA1_ChannelGetCount (uint8_t ch, RSI_UDMA_CHA_CONFIG_DATA_T control,
                                RSI_UDMA_CHA_CFG_T  config);
int32_t UDMA1_DMAEnable(void);
int32_t UDMA1_ChannelConfigure ( uint8_t             ch,
                                 uint32_t            src_addr,
                                 uint32_t            dest_addr,
                                 uint32_t            size,
                                 RSI_UDMA_CHA_CONFIG_DATA_T control,
                                 RSI_UDMA_CHA_CFG_T  *config,
                                 UDMA_SignalEvent_t  cb_event ); 
int32_t UDMA1_Uninitialize (void); 

/**
\brief Access structure of the UDMA Driver.
*/
typedef struct _RSI_DRIVER_UDMA {
                                                             
  int32_t  (*Initialize)      ( void );                                      
  int32_t  (*Uninitialize)    ( void );                                                                
  int32_t  (*ChannelConfigure)( uint8_t                    ch,
                                uint32_t                   src_addr, 
                                uint32_t                   dest_addr,
                                uint32_t                   size,
                                RSI_UDMA_CHA_CONFIG_DATA_T control,
                                RSI_UDMA_CHA_CFG_T         *config,
                                UDMA_SignalEvent_t         cb_event );                                               
  int32_t  (*ChannelEnable)   ( uint8_t ch ); 
  int32_t  (*ChannelDisable)  ( uint8_t ch );                   
  uint32_t (*ChannelGetCount) ( uint8_t ch, 
                                RSI_UDMA_CHA_CONFIG_DATA_T control,
                                RSI_UDMA_CHA_CFG_T         config );
  int32_t  (*DMAEnable)       ( void );
} const RSI_DRIVER_UDMA;

#ifdef __cplusplus
}
#endif
#endif // __UDMA_H__


