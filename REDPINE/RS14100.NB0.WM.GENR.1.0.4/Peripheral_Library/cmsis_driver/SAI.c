 #include "SAI.h"
// #include "clock_update.h"


#if ((!defined(RTE_I2S0)) || (!defined(RTE_I2S1)))
#error "I2S missing in RTE_Device.h. Please update RTE_Device.h!"
#endif

#if ((defined(RTE_Drivers_SAI0) && !RTE_I2S0) && (defined(RTE_Drivers_SAI1) && !RTE_I2S1))
#error "I2S0/1 not configured in RTE_Device.h!"
#endif

// Definitions

// Frequency tolerance in percentage
#ifndef I2S_FREQ_TOLERANCE
#define I2S_FREQ_TOLERANCE   (1.)
#endif

#define D2F_DOMAIN           (255UL)

#if (RTE_I2S0)

#if ((I2S0_TX_FIFO_LEVEL < 0U) || (I2S0_TX_FIFO_LEVEL > 7U))
#error "Invalid FIFO Level value. FIFO Level can be 0 to 7"
#endif
#if ((I2S0_RX_FIFO_LEVEL < 0U) || (I2S0_RX_FIFO_LEVEL > 7U))
#error "Invalid FIFO Level value. FIFO Level can be 0 to 7"
#endif
#endif

#if (RTE_I2S1)

#if ((I2S1_TX_FIFO_LEVEL < 0U) || (I2S1_TX_FIFO_LEVEL > 7U))
#error "Invalid FIFO Level value. FIFO Level can be 0 to 7"
#endif
#if ((I2S1_RX_FIFO_LEVEL < 0U) || (I2S1_RX_FIFO_LEVEL > 7U))
#error "Invalid FIFO Level value. FIFO Level can be 0 to 7"
#endif
#endif


#define ARM_SAI_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,5)   // driver version
// Driver Version
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_SAI_API_VERSION,
  ARM_SAI_DRV_VERSION
};

// I2S0
#if (RTE_I2S0)

static I2S_INFO I2S0_Info = {0,{0},{0},{0}};
static I2S_CLK  I2S0_Clk = {RTE_I2S0_CLK_SRC, RTE_I2S0_CLK_DIV_FACT};

#if (RTE_I2S0_CHNL_UDMA_TX_EN == 1U)

void I2S0_UDMA_Tx_Event (uint32_t event, uint8_t dmaCh);

static I2S_DMA I2S0_UDMA_TX_CHNL = {
 {
   UDMA_MODE_BASIC,
   0,
   (RTE_I2S0_DMA_TX_LEN_PER_DES -1),
   ARBSIZE_4,
   0x0,
   0x0,
#if (I2S0_TX_RES <= RES_16_BIT )
   SRC_SIZE_16,
   SRC_INC_16,
   DST_SIZE_16,
   DST_INC_NONE
#else
   SRC_SIZE_32,
   SRC_INC_32,
   DST_SIZE_32,
   DST_INC_NONE
#endif
 },  
 RTE_I2S0_CHNL_UDMA_TX_CH,
 I2S0_UDMA_Tx_Event
};
#endif
 
#if (RTE_I2S0_CHNL_UDMA_RX_EN == 1U)

void I2S0_UDMA_Rx_Event (uint32_t event, uint8_t dmaCh);
static I2S_DMA I2S0_UDMA_RX_CHNL = {
 {  
   UDMA_MODE_BASIC,
   0,
   (RTE_I2S0_DMA_RX_LEN_PER_DES -1),
   ARBSIZE_4,
   0x0,
   0x0,
#if (I2S0_RX_RES <= RES_16_BIT)
   SRC_SIZE_16,
   SRC_INC_NONE,
   DST_SIZE_16,
   DST_INC_16
#else
   SRC_SIZE_32,
   SRC_INC_NONE,
   DST_SIZE_32,
   DST_INC_32
#endif
 },  
 RTE_I2S0_CHNL_UDMA_RX_CH,
 I2S0_UDMA_Rx_Event
};
#endif


static I2S_RESOURCES I2S0_Resources = {
  {  // Capabilities
    1,   ///< supports asynchronous Transmit/Receive
    1,   ///< supports synchronous Transmit/Receive
    0,   ///< supports user defined Protocol
    1,   ///< supports I2S Protocol
    0,   ///< supports MSB/LSB justified Protocol
    1,   ///< supports PCM short/long frame Protocol
    0,   ///< supports AC'97 Protocol
    0,   ///< supports Mono mode
    0,   ///< supports Companding
    0,   ///< supports MCLK (Master Clock) pin
    0,   ///< supports Frame error event: \ref ARM_SAI_EVENT_FRAME_ERROR
  },
  I2S0,
  I2S0_IRQn,
#if (RTE_I2S0_CHNL_UDMA_TX_EN == 1U)
  &I2S0_UDMA_TX_CHNL,
#else
  NULL,
#endif
#if (RTE_I2S0_CHNL_UDMA_RX_EN == 1U)
  &I2S0_UDMA_RX_CHNL,
#else
  NULL,
#endif
  I2S0_TX_FIFO_LEVEL,
  I2S0_RX_FIFO_LEVEL,
  0,
  &I2S0_Info,
  0,
  I2S_PROTOCOL,  
  &I2S0_Clk
};
#endif


// I2S1
#if (RTE_I2S1)
static I2S_INFO I2S1_Info = {0,{0},{0},{0}};
static I2S_CLK  I2S1_Clk = {RTE_I2S1_CLK_SRC, I2S1_CLK_DIV_FACT};

#if (RTE_I2S1_CHNL_UDMA_TX_EN == 1U)
void I2S1_UDMA_Tx_Event (uint32_t event, uint8_t dmaCh);
static I2S_DMA I2S1_UDMA_TX_CHNL = {
 {
   UDMA_MODE_BASIC,
   0,
   RTE_I2S1_DMA_TX_LEN_PER_DES-1,
   ARBSIZE_4,
   0x0,
   0x0,
#if (I2S1_CHNL_TX_RES <= 16)
   SRC_SIZE_16,
   SRC_INC_16,
   DST_SIZE_16,
   DST_INC_NONE
#else
   SRC_SIZE_32,
   SRC_INC_32,
   DST_SIZE_32,
   DST_INC_NONE
#endif
 },  
 RTE_I2S1_CHNL_UDMA_TX_CH,
 I2S1_UDMA_Tx_Event
};
#endif
 
#if (RTE_I2S1_CHNL_UDMA_RX_EN == 1U)
void I2S1_UDMA_Rx_Event (uint32_t event, uint8_t dmaCh);
static I2S_DMA I2S1_UDMA_RX_CHNL = {
 {  
   UDMA_MODE_BASIC,
   0,
   RTE_I2S1_DMA_RX_LEN_PER_DES-1,
   ARBSIZE_4,
   0x0,
   0x0,
#if (I2S1_CHNL_RX_RES <= 16)
   SRC_SIZE_16,
   SRC_INC_NONE,
   DST_SIZE_16,
   DST_INC_16
#else
   SRC_SIZE_32,
   SRC_INC_NONE,
   DST_SIZE_32,
   DST_INC_32
#endif
 },  
 RTE_I2S1_CHNL_UDMA_RX_CH,
 I2S1_UDMA_Rx_Event
};
#endif

static I2S_RESOURCES I2S1_Resources = {
  {  // Capabilities
    1,   ///< supports asynchronous Transmit/Receive
    1,   ///< supports synchronous Transmit/Receive
    0,   ///< supports user defined Protocol
    1,   ///< supports I2S Protocol
    0,   ///< supports MSB/LSB justified Protocol
    0,   ///< supports PCM short/long frame Protocol
    0,   ///< supports AC'97 Protocol
    0,   ///< supports Mono mode
    0,   ///< supports Companding
    0,   ///< supports MCLK (Master Clock) pin
    0,   ///< supports Frame error event: \ref ARM_SAI_EVENT_FRAME_ERROR
  },
  I2S1,
  I2S1_IRQn,
#if (RTE_I2S1_CHNL_UDMA_TX_EN == 1U)
  &I2S1_UDMA_TX_CHNL,
#else
  NULL,
#endif
#if (RTE_I2S1_CHNL_UDMA_RX_EN == 1U)
  &I2S1_UDMA_RX_CHNL,
#else
  NULL,
#endif
  I2S1_TX_FIFO_LEVEL,
  I2S1_RX_FIFO_LEVEL,  
  0,
  &I2S1_Info,  
  0,
  I2S_PROTOCOL,
  &I2S1_Clk
};
#endif

// Extern Function
extern uint32_t GetClockFreq (uint32_t clk_src);



/**
  \fn          ARM_DRIVER_VERSION I2Sx_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION                                */
static ARM_DRIVER_VERSION I2Sx_GetVersion (void) 
{
  return (DriverVersion);
}

/**
  \fn          ARM_SAI_CAPABILITIES I2Sx_GetCapabilities (void)
  \brief       Get driver capabilities.
  \param[in]   i2s       Pointer to I2S resources
  \return      \ref ARM_SAI_CAPABILITIES
*/
static ARM_SAI_CAPABILITIES I2S_GetCapabilities (I2S_RESOURCES *i2s) 
{
  return (i2s->capabilities);
}

static void RSI_I2S_InterruptEnableDisable( I2S_RESOURCES *i2s, 
                                            boolean_t chnlNum,
                                            uint32_t maskFlag, 
                                            boolean_t mask )																				 
{
  if(mask)
  {
    i2s->reg->CHANNEL_CONFIG[chnlNum].I2S_IMR |= maskFlag; 
  }
  else
  {
    i2s->reg->CHANNEL_CONFIG[chnlNum].I2S_IMR  &= ~maskFlag;
  }
}

static void I2S0_Chnl0_PinMux(void)
{
  /* SCK */
  if(RTE_I2S0_SCLK_PIN > 63)
  {
		RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2S0_SCLK_PIN-64));
    RSI_EGPIO_PadSelectionEnable(RTE_I2S0_SCLK_PAD);
		RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2S0_SCLK_PORT,(uint8_t)(RTE_I2S0_SCLK_PIN-64),6);		
	  //configure SCK pin
		RSI_EGPIO_SetPinMux(EGPIO,RTE_I2S0_SCLK_PORT,RTE_I2S0_SCLK_PIN,RTE_I2S0_SCLK_MUX);
	}
  else
  {
    if(RTE_I2S0_SCLK_PIN == 28)
    {
      egpio_host_pads_gpio_mode_enable(RTE_I2S0_SCLK_PIN);
    }
    RSI_EGPIO_PadSelectionEnable(RTE_I2S0_SCLK_PAD);
    RSI_EGPIO_PadReceiverEnable(RTE_I2S0_SCLK_REN);
    RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2S0_SCLK_PORT , RTE_I2S0_SCLK_PIN ,RTE_I2S0_SCLK_MUX);
  }
  
  /* WSCLK */
  if(RTE_I2S0_WSCLK_PIN > 63)
  {
		RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2S0_WSCLK_PIN-64));
    RSI_EGPIO_PadSelectionEnable(RTE_I2S0_WSCLK_PAD);
		RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2S0_WSCLK_PORT,(uint8_t)(RTE_I2S0_WSCLK_PIN-64),6);	
	  //configure WSCLK pin
		RSI_EGPIO_SetPinMux(EGPIO,RTE_I2S0_WSCLK_PORT,RTE_I2S0_WSCLK_PIN,RTE_I2S0_WSCLK_MUX);
	}
  else
  {
    if(RTE_I2S0_WSCLK_PIN == 27)
    {
      egpio_host_pads_gpio_mode_enable(RTE_I2S0_WSCLK_PIN);
    }
    RSI_EGPIO_PadSelectionEnable(RTE_I2S0_WSCLK_PAD);
    RSI_EGPIO_PadReceiverEnable(RTE_I2S0_WSCLK_REN);
    RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2S0_WSCLK_PORT , RTE_I2S0_WSCLK_PIN ,RTE_I2S0_WSCLK_MUX);
  }
 
  if(RTE_I2S0_DOUT0_PIN > 63)
  {
		RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2S0_DOUT0_PIN-64));
		RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2S0_DOUT0_PORT,(uint8_t)(RTE_I2S0_DOUT0_PIN-64),6);
		RSI_EGPIO_PadSelectionEnable(RTE_I2S0_DOUT0_PAD);
	  //configure DOUT0 pin
		RSI_EGPIO_SetPinMux(EGPIO,RTE_I2S0_DOUT0_PORT,RTE_I2S0_DOUT0_PIN,RTE_I2S0_DOUT0_MUX);
	}
  else
  {
    if(RTE_I2S0_DOUT0_PIN == 30)
    {
      egpio_host_pads_gpio_mode_enable(RTE_I2S0_DOUT0_PIN);
    }
    /* DOUT pin */
    RSI_EGPIO_PadSelectionEnable(RTE_I2S0_DOUT0_PAD);
    RSI_EGPIO_PadReceiverEnable(RTE_I2S0_DOUT0_REN);
    RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2S0_DOUT0_PORT , RTE_I2S0_DOUT0_PIN ,RTE_I2S0_DOUT0_MUX);  
  }
  if(RTE_I2S0_DIN0_PIN > 63)
  {
		RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2S0_DIN0_PIN-64));
		RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2S0_DIN0_PORT,(uint8_t)(RTE_I2S0_DIN0_PIN-64),6);
		RSI_EGPIO_PadSelectionEnable(RTE_I2S0_DIN0_PAD);
	  //configure DIN0 pin
		RSI_EGPIO_SetPinMux(EGPIO,RTE_I2S0_DIN0_PORT,RTE_I2S0_DIN0_PIN,RTE_I2S0_DIN0_MUX);
	}
  else
  {
    if(RTE_I2S0_DIN0_PIN == 29)
    {
      egpio_host_pads_gpio_mode_enable(RTE_I2S0_DIN0_PIN);
    }
    /* DIN pin */
    RSI_EGPIO_PadSelectionEnable(RTE_I2S0_DIN0_PAD);
    RSI_EGPIO_PadReceiverEnable(RTE_I2S0_DIN0_REN);
    RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2S0_DIN0_PORT , RTE_I2S0_DIN0_PIN ,RTE_I2S0_DIN0_MUX);   
  }  
}

static void I2S0_Chnl1_PinMux(void)
{
 /* SCK */
  if(RTE_I2S0_SCLK_PIN > 63)
  {
		RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2S0_SCLK_PIN-64));
		RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2S0_SCLK_PORT,(uint8_t)(RTE_I2S0_SCLK_PIN-64),6);
		RSI_EGPIO_PadSelectionEnable(RTE_I2S0_SCLK_PAD);
	  //configure SCK pin
		RSI_EGPIO_SetPinMux(EGPIO,RTE_I2S0_SCLK_PORT,RTE_I2S0_SCLK_PIN,RTE_I2S0_SCLK_MUX);
	}
  else
  {
    if(RTE_I2S0_SCLK_PIN == 28)
    {
      egpio_host_pads_gpio_mode_enable(RTE_I2S0_SCLK_PIN);
    }
    RSI_EGPIO_PadSelectionEnable(RTE_I2S0_SCLK_PAD);
    RSI_EGPIO_PadReceiverEnable(RTE_I2S0_SCLK_REN);
    RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2S0_SCLK_PORT , RTE_I2S0_SCLK_PIN ,RTE_I2S0_SCLK_MUX);
  }
  
  /* WSCLK */
  if(RTE_I2S0_WSCLK_PIN > 63)
  {
		RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2S0_WSCLK_PIN-64));
		RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2S0_WSCLK_PORT,(uint8_t)(RTE_I2S0_WSCLK_PIN-64),6);
		RSI_EGPIO_PadSelectionEnable(RTE_I2S0_WSCLK_PAD);
	  //configure WSCLK pin
		RSI_EGPIO_SetPinMux(EGPIO,RTE_I2S0_WSCLK_PORT,RTE_I2S0_WSCLK_PIN,RTE_I2S0_WSCLK_MUX);
	}
  else
  {
    if(RTE_I2S0_WSCLK_PIN == 27)
    {
      egpio_host_pads_gpio_mode_enable(RTE_I2S0_WSCLK_PIN);
    }
    RSI_EGPIO_PadSelectionEnable(RTE_I2S0_WSCLK_PAD);
    RSI_EGPIO_PadReceiverEnable(RTE_I2S0_WSCLK_REN);
    RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2S0_WSCLK_PORT , RTE_I2S0_WSCLK_PIN ,RTE_I2S0_WSCLK_MUX);
  }
  
  /* TX pin */
  RSI_EGPIO_PadSelectionEnable(RTE_I2S0_DOUT1_PAD);
  RSI_EGPIO_PadReceiverEnable(RTE_I2S0_DOUT1_REN);
  RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2S0_DOUT1_PORT , RTE_I2S0_DOUT1_PIN ,RTE_I2S0_DOUT1_MUX);

  /* RX pin */
  RSI_EGPIO_PadSelectionEnable(RTE_I2S0_DIN1_PAD);
  RSI_EGPIO_PadReceiverEnable(RTE_I2S0_DIN1_REN);
  RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2S0_DIN1_PORT , RTE_I2S0_DIN1_PIN ,RTE_I2S0_DIN1_MUX); 
}

static void I2S1_PinMux(void)
{
  /* SCK */
  RSI_EGPIO_UlpPadReceiverEnable(RTE_I2S1_SCLK_PIN);
  RSI_EGPIO_SetPinMux(EGPIO1 ,RTE_I2S1_SCLK_PORT , RTE_I2S1_SCLK_PIN ,RTE_I2S1_SCLK_MUX);
  
  /* WSCLK */
  RSI_EGPIO_UlpPadReceiverEnable(RTE_I2S1_WSCLK_PIN);
  RSI_EGPIO_SetPinMux(EGPIO1 ,RTE_I2S1_WSCLK_PORT , RTE_I2S1_WSCLK_PIN ,RTE_I2S1_WSCLK_MUX);
  
  /* TX pin */
  RSI_EGPIO_UlpPadReceiverEnable(RTE_I2S1_DOUT0_PIN);
  RSI_EGPIO_SetPinMux(EGPIO1 ,RTE_I2S1_DOUT0_PORT , RTE_I2S1_DOUT0_PIN ,RTE_I2S1_DOUT0_MUX);

  /* RX pin */
  RSI_EGPIO_UlpPadReceiverEnable(RTE_I2S1_DIN0_PIN);
  RSI_EGPIO_SetPinMux(EGPIO1 ,RTE_I2S1_DIN0_PORT , RTE_I2S1_DIN0_PIN ,RTE_I2S1_DIN0_MUX);   
}

static void i2s_chnl_Init(ARM_SAI_SignalEvent_t cb_event, I2S_RESOURCES *i2s)
{
  // Initialize I2S Run-Time resources
  i2s->info->cb_event             = cb_event;
  i2s->info->status.frame_error   = 0U;
  i2s->info->status.rx_busy       = 0U;
  i2s->info->status.rx_overflow   = 0U;
  i2s->info->status.tx_busy       = 0U;
  i2s->info->status.tx_underflow  = 0U;
#if(RTE_I2S0)
  RSI_PS_M4ssPeriPowerUp(M4SS_PWRGATE_ULP_PERI3);
  if(i2s->reg == I2S0) 
  {
    if(i2s->xfer_chnl)
    {
      I2S0_Chnl1_PinMux();
    }
    else
    {
      I2S0_Chnl0_PinMux(); 
    }   
  }
#endif
#if(RTE_I2S1)
  RSI_PS_UlpssPeriPowerUp(ULPSS_PWRGATE_ULP_I2S);
  if(i2s->reg == I2S1) 
  {
    I2S1_PinMux();  
  }
#endif  
}



static int32_t I2S_Initialize (ARM_SAI_SignalEvent_t cb_event, I2S_RESOURCES *i2s)
{
  if(i2s->flags & I2S_FLAG_INITIALIZED) 
  {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }
  
  i2s_chnl_Init(cb_event, i2s);
 
  // UDMA Initialize
  if ((i2s->dma_tx != NULL) || (i2s->dma_rx != NULL)||
       RTE_I2S0_CHNL_UDMA_TX_EN || RTE_I2S0_CHNL_UDMA_RX_EN||
       RTE_I2S1_CHNL_UDMA_TX_EN || RTE_I2S1_CHNL_UDMA_RX_EN )      
  {
    if(i2s->reg == I2S0)
    {
      UDMA0_Initialize();     
    }
    if(i2s->reg == I2S1)
    {
      UDMA1_Initialize();   
    }
  }
  i2s->flags = I2S_FLAG_INITIALIZED;
  return ARM_DRIVER_OK;
}

static int32_t I2S_Uninitialize (I2S_RESOURCES *i2s)
{
  // DMA Uninitialize
  if ((i2s->dma_tx != NULL) || (i2s->dma_rx != NULL)||
       RTE_I2S0_CHNL_UDMA_TX_EN || RTE_I2S0_CHNL_UDMA_RX_EN||
       RTE_I2S1_CHNL_UDMA_TX_EN || RTE_I2S1_CHNL_UDMA_RX_EN )
  { 
    if(i2s->reg == I2S0)
    {
      RSI_PS_M4ssPeriPowerDown(M4SS_PWRGATE_ULP_PERI3);
      UDMA0_Uninitialize();    
    }
    if(i2s->reg == I2S1)
    {
      RSI_PS_UlpssPeriPowerDown(ULPSS_PWRGATE_ULP_I2S);
      UDMA1_Uninitialize();     
    } 
  }
  // Reset I2S status flags
  i2s->flags = 0U;
  return ARM_DRIVER_OK;
}

static int32_t I2S_PowerControl (ARM_POWER_STATE state, I2S_RESOURCES *i2s)
{
  switch (state)
  {
    case ARM_POWER_OFF:
      // Disable I2S IRQ
      NVIC_DisableIRQ(i2s->irq_num);
    
      if(i2s->xfer_chnl)
      {
        if((i2s->dma_tx == NULL) &&(i2s->info->status.tx_busy != 0U))           
        {
          if((i2s->reg == I2S0)&&(RTE_I2S0_CHNL_UDMA_TX_EN || RTE_I2S0_CHNL_UDMA_RX_EN))
          {     
            UDMA0_ChannelDisable (i2s->dma_tx->channel);         
          }
          if((i2s->reg == I2S1)&&(RTE_I2S1_CHNL_UDMA_TX_EN || RTE_I2S1_CHNL_UDMA_RX_EN))
          {
            return ERROR_I2S_INVALID_ARG;  
          }
        }
      }
      else
      {
        if((i2s->dma_tx == NULL) &&(i2s->info->status.tx_busy != 0U))
        {
          if((i2s->reg == I2S0)&&(RTE_I2S0_CHNL_UDMA_TX_EN || RTE_I2S0_CHNL_UDMA_RX_EN))
          {
            UDMA0_ChannelDisable (i2s->dma_tx->channel); 
          }
          if((i2s->reg == I2S1)&&(RTE_I2S1_CHNL_UDMA_TX_EN || RTE_I2S1_CHNL_UDMA_RX_EN))
          {   
            UDMA1_ChannelDisable (i2s->dma_tx->channel);            
          }
        }
      }
          
      // Disable I2S peripheral
      i2s->reg->I2S_IER_b.IEN  = 0;
      
      // Disable I2S peripheral clock
      if(i2s->reg == I2S0)
      {
        RSI_CLK_PeripheralClkDisable2(M4CLK, I2SM_INTF_SCLK_ENABLE|I2SM_PCLK_ENABLE);        
      }
      if(i2s->reg == I2S1)
      {
        RSI_ULPSS_UlpPeriClkDisable(ULPCLK , CLK_ENABLE_I2S);
      }

      // Clear pending I2S interrupts in NVIC
      NVIC_ClearPendingIRQ(i2s->irq_num);
     
      // Clear driver variables
      i2s->info->status.frame_error   = 0U;
      i2s->info->status.rx_busy       = 0U;
      i2s->info->status.rx_overflow   = 0U;
      i2s->info->status.tx_busy       = 0U;
      i2s->info->status.tx_underflow  = 0U;
     
      i2s->flags &= ~I2S_FLAG_POWERED;
        break;

    case ARM_POWER_LOW:
        break;

    case ARM_POWER_FULL:
      if ((i2s->flags & I2S_FLAG_INITIALIZED) == 0U) 
      { 
        return ARM_DRIVER_ERROR; 
      }
      if ((i2s->flags & I2S_FLAG_POWERED)!= 0U) 
      { 
        return ARM_DRIVER_OK; 
      }
      if(i2s->reg == I2S0)
      { 
        clk_peripheral_clk_enable(M4CLK, I2SM_CLK, ENABLE_STATIC_CLK);
        //RSI_CLK_I2sClkConfig(M4CLK,i2s->clk->clk_src,i2s->clk->div_fact);        
      }
      if(i2s->reg == I2S1)
      {
        ulpss_peripheral_enable(ULPCLK, ULP_I2S_CLK,ENABLE_STATIC_CLK);
//        RSI_ULPSS_UlpI2sClkConfig(ULPCLK ,RTE_I2S1_CLK_SRC,8); 
      }
      // Disable I2S interrupts
      RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, 
                                      F_RXDAM|F_TXFEM|F_RXFOM|F_TXFOM ,INTR_MASK );
        
      // Disable transmitter and receiver blocks
      // Disable all TX channels
     if(i2s->xfer_chnl)
     {
      i2s->reg->CHANNEL_CONFIG[0].I2S_TER_b.TXCHEN = 0x0;
      i2s->reg->CHANNEL_CONFIG[0].I2S_RER_b.RXCHEN = 0x0;
     }
     else
     {
       i2s->reg->CHANNEL_CONFIG[1].I2S_TER_b.TXCHEN = 0x0;
       i2s->reg->CHANNEL_CONFIG[1].I2S_RER_b.RXCHEN = 0x0; 
     }
      i2s->reg->I2S_IRER_b.RXEN = 0;                    
      i2s->reg->I2S_ITER_b.TXEN = 0;
      
      // Clear chnl0 driver variables
      i2s->info->status.frame_error   = 0U;
      i2s->info->status.rx_busy       = 0U;
      i2s->info->status.rx_overflow   = 0U;
      i2s->info->status.tx_busy       = 0U;
      i2s->info->status.tx_underflow  = 0U;

      i2s->info->rx.residue_cnt       = 0U;
      i2s->info->rx.residue_num       = 0U;
      i2s->info->tx.residue_cnt       = 0U;
      i2s->info->tx.residue_num       = 0U;
      i2s->flags = I2S_FLAG_POWERED | I2S_FLAG_INITIALIZED;

      // Clear and Enable SAI IRQ
      NVIC_ClearPendingIRQ(i2s->irq_num);
      //NVIC_EnableIRQ(i2s->irq_num);
      break;

    default:
       return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}

static int32_t I2S_Send (const void *data, uint32_t num,I2S_RESOURCES *i2s)
{  
  volatile int32_t stat = 0;  
   RSI_UDMA_CHA_CFG_T chnl_cfg = {0};
  
  if ((data == NULL) || (num == 0U)) 
  {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }  
  if ((i2s->flags & I2S_FLAG_CONFIGURED) == 0U) 
  {
    // I2S is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }
  if(i2s->info->status.tx_busy) 
  {
    // Send is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }
  i2s->info->status.tx_busy = 1U;
  i2s->info->status.tx_underflow = 0U;
  i2s->info->tx.buf = (uint8_t *)data;
  i2s->info->tx.cnt = 0U;
  num = num * (i2s->info->tx.data_bits / 8U);
  
  i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TXFCR_b.TXCHET = (i2s->tx_fifo_level);
  
  // Only 32-bit value can be written to FIFO. If there is data left from last send,
  // fill residue buffer to 32bits and write to TX FIFO
  if (i2s->info->tx.residue_cnt != 0U) 
  {
    while ((i2s->info->tx.residue_cnt < 4U) && (i2s->info->tx.cnt < num)) 
    {
      i2s->info->tx.residue_buf[i2s->info->tx.residue_cnt++] = 
                                   i2s->info->tx.buf[i2s->info->tx.cnt++];
    }
    if (i2s->info->tx.residue_cnt == 4U) 
    {
      // Write 32bits to TX FIFO
      i2s->reg->CHANNEL_CONFIG[0].I2S_LTHR = *(__packed uint32_t *)(i2s->info->tx.residue_buf);

      // There is no valid data in residue buffer
      i2s->info->tx.residue_cnt = 0U;
    }
  }
  if (i2s->info->tx.residue_cnt != 0U) 
  {
    while ((i2s->info->tx.residue_cnt < 4U) && (i2s->info->tx.cnt < num)) 
    {
      i2s->info->tx.residue_buf[i2s->info->tx.residue_cnt++] = 
                                    i2s->info->tx.buf[i2s->info->tx.cnt++];
    }
    if (i2s->info->tx.residue_cnt == 4U) 
    {
      // Write 32bits to TX FIFO
      i2s->reg->CHANNEL_CONFIG[1].I2S_LTHR = *(__packed uint32_t *)(i2s->info->tx.residue_buf);

      // There is no valid data in residue buffer
      i2s->info->tx.residue_cnt = 0U;
    }
  }  
  //RSI_I2S_InterruptEnableDisable(i2s,i2s->xfer_chnl,F_TXFEM,INTR_MASK);
  RSI_I2S_InterruptEnableDisable(i2s,i2s->xfer_chnl,F_TXFEM,INTR_UNMASK);
  
  i2s->info->tx.num = num;
  i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TER_b.TXCHEN = 0x1;
  
  // DMA mode for Channel0 
  if (i2s->dma_tx != NULL) 
  {
    num = num - i2s->info->tx.cnt;
    if(num < 4U) 
    {  
      // Enable I2S transmitter interrupt      
      RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_TXFEM ,INTR_UNMASK );
    } 
    else 
    {
      chnl_cfg.altStruct = 0;
      chnl_cfg.burstReq = 1;
      if(i2s->reg == I2S0)
      { 
        chnl_cfg.channelPrioHigh = UDMA0_CHNL_PRIO_LVL;        
        chnl_cfg.dmaCh  = RTE_I2S0_CHNL_UDMA_TX_CH;
      }
      else
      {
        chnl_cfg.channelPrioHigh = UDMA1_CHNL_PRIO_LVL;   
        chnl_cfg.dmaCh  = RTE_I2S1_CHNL_UDMA_TX_CH; 
      }
      chnl_cfg.periAck = 0;
      chnl_cfg.periphReq = 0;
      chnl_cfg.reqMask = 0;
           
      // Configure DMA channel
     if((i2s->reg == I2S0)&&(RTE_I2S0_CHNL_UDMA_TX_EN))
     {
      stat = UDMA0_ChannelConfigure( i2s->dma_tx->channel,
                                   (uint32_t)(i2s->info->tx.buf),
                                   (uint32_t)(&(i2s->reg->I2S_TXDMA)),
                                    num/(i2s->info->tx.data_bits / 8U),                    //num / 4U,
			                              i2s->dma_tx->control,
                                    &chnl_cfg,
                                    i2s->dma_tx->cb_event);
      if (stat == -1) 
      {  
        return ARM_DRIVER_ERROR; 
      }
      UDMA0_ChannelEnable (i2s->dma_tx->channel);
      UDMA0_DMAEnable();
      i2s->reg->I2S_ITER_b.TXEN = 0x1;
      if(i2s->info->tx.master)
      {        
        i2s->reg->I2S_CER_b.CLKEN = ENABLE;
      }        
     }
     if((i2s->reg == I2S1)&&(RTE_I2S1_CHNL_UDMA_TX_EN))
     {
       stat = UDMA1_ChannelConfigure( i2s->dma_tx->channel,
                                    (uint32_t)(i2s->info->tx.buf),
                                   (uint32_t)(&(i2s->reg->I2S_TXDMA)),
                                    num / (i2s->info->tx.data_bits / 8U),
			                              i2s->dma_tx->control,
                                    &chnl_cfg,
                                    i2s->dma_tx->cb_event);
        if (stat == -1) 
        {  
          return ARM_DRIVER_ERROR; 
        }
        UDMA1_ChannelEnable (i2s->dma_tx->channel);
        UDMA1_DMAEnable();
        i2s->reg->I2S_ITER_b.TXEN = 0x1;
        if(i2s->info->tx.master)
        {        
          i2s->reg->I2S_CER_b.CLKEN = ENABLE;
        }        
      }       
    }
  } 
  else 
  {   
    // Enable I2S transmitter interrupt
    RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_TXFEM ,INTR_UNMASK );
  }
  return ARM_DRIVER_OK;
}

static int32_t I2S_Receive (void *data, uint32_t num, I2S_RESOURCES *i2s)
{
  volatile int32_t  stat =0;
  RSI_UDMA_CHA_CFG_T chnl_cfg = {0};
  
  if ((data == NULL) || (num == 0U)) 
  {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  if ((i2s->flags & I2S_FLAG_CONFIGURED) == 0U) 
  {
    // I2S is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }
  if (i2s->info->status.rx_busy) 
  {
    // Receive is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set Receive active flag
  i2s->info->status.rx_busy = 1U;

  // Clear RX overflow flag
  i2s->info->status.rx_overflow = 0U;

  // Save receive buffer info
  i2s->info->rx.buf = (uint8_t *)data;
  i2s->info->rx.cnt = 0U;

  // Convert from number of samples to number of bytes
  num = num * (i2s->info->rx.data_bits / 8U);
  
  // Set FIFO level and enable RX DMA
  i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT = (i2s->rx_fifo_level);
  
  while ((i2s->info->rx.cnt < num) && (i2s->info->rx.residue_cnt < i2s->info->rx.residue_num)) 
  {
    // RX Data available in residue buffer
    i2s->info->rx.buf[i2s->info->rx.cnt++] = i2s->info->rx.residue_buf[i2s->info->rx.residue_cnt++];

    if (i2s->info->rx.residue_cnt == i2s->info->rx.residue_num) 
    {
      // Residue buffer empty
      i2s->info->rx.residue_cnt    = 0U;
      i2s->info->rx.residue_num    = 0U;
    }
  }

  // Disable I2S receive interrupt
  //RSI_I2S_InterruptEnableDisable(i2s,i2s->xfer_chnl,F_RXDAM,INTR_MASK );
  RSI_I2S_InterruptEnableDisable(i2s,i2s->xfer_chnl,F_RXDAM,INTR_UNMASK );
  i2s->info->rx.num = num;

  num = num - i2s->info->rx.cnt;
  // DMA mode
  if ((i2s->dma_rx != NULL) && (num >= 4U)) 
  {
    // Set offset in RX Buffer for DMA transfer
    //offset = i2s->info->rx.cnt;

    // Update RX count
    num = num/ 4;
    i2s->info->rx.cnt = i2s->info->rx.cnt + (num * 4);
      
    chnl_cfg.altStruct = 0;
    chnl_cfg.burstReq = 1;
    if(i2s->reg == I2S0)
    { 
      chnl_cfg.channelPrioHigh = UDMA0_CHNL_PRIO_LVL;        
      chnl_cfg.dmaCh  = RTE_I2S0_CHNL_UDMA_RX_CH;
    }
    else
    {
      chnl_cfg.channelPrioHigh = UDMA1_CHNL_PRIO_LVL;   
      chnl_cfg.dmaCh  = RTE_I2S1_CHNL_UDMA_RX_CH; 
    }
    chnl_cfg.periAck = 0;
    chnl_cfg.periphReq = 0;
    chnl_cfg.reqMask = 0;
    
    // Configure DMA channel
    if((i2s->reg == I2S0)&&(RTE_I2S0_CHNL_UDMA_RX_EN))
    {
      stat = UDMA0_ChannelConfigure(i2s->dma_rx->channel,
                                    (uint32_t)(&(i2s->reg->I2S_RXDMA)),
                                    (uint32_t)(i2s->info->rx.buf),
                                    (i2s->info->rx.cnt) / (i2s->info->rx.data_bits / 8U),
			                              i2s->dma_rx->control,
                                    &chnl_cfg,
                                    i2s->dma_rx->cb_event);
      
      if (stat == -1)  
      { 
       return ARM_DRIVER_ERROR; 
      }
      i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RER_b.RXCHEN = 0x1;      
      i2s->reg->I2S_IRER_b.RXEN = 0x1;        
      UDMA0_ChannelEnable (i2s->dma_rx->channel);
      UDMA0_DMAEnable();
      if(i2s->info->rx.master)
      {        
        i2s->reg->I2S_CER_b.CLKEN = ENABLE;
      }    
    }
    if((i2s->reg == I2S1)&&(RTE_I2S1_CHNL_UDMA_RX_EN))
    {
      stat = UDMA1_ChannelConfigure(i2s->dma_rx->channel,
                                   (uint32_t)(&(i2s->reg->I2S_RXDMA)),
                                   (uint32_t)(i2s->info->rx.buf),
                                   (i2s->info->rx.cnt) / (i2s->info->rx.data_bits / 8U),
			                             i2s->dma_rx->control,
                                   &chnl_cfg,
                                   i2s->dma_rx->cb_event);
      if (stat == -1)  
      { 
       return ARM_DRIVER_ERROR; 
      }
      i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RER_b.RXCHEN = 0x1;      
      i2s->reg->I2S_IRER_b.RXEN = 0x1;        
      UDMA1_ChannelEnable (i2s->dma_rx->channel);
      UDMA1_DMAEnable();
      if(i2s->info->rx.master)
      {        
        i2s->reg->I2S_CER_b.CLKEN = ENABLE;
      }    
    }
  } 
  else 
  {
    RSI_I2S_InterruptEnableDisable(i2s,i2s->xfer_chnl,F_RXDAM,INTR_UNMASK );
  }
  return ARM_DRIVER_OK; 
}

static uint32_t I2S_GetTxCount (I2S_RESOURCES *i2s)
{
  uint32_t cnt =0;
  
  // Convert count in bytes to count of samples
  cnt = i2s->info->tx.cnt / (i2s->info->tx.data_bits / 8U);     
  return (cnt);
}

static uint32_t I2S_GetRxCount (I2S_RESOURCES *i2s)
{
  uint32_t cnt =0;
  
  // Convert count in bytes to count of samples
  cnt = i2s->info->rx.cnt / (i2s->info->rx.data_bits / 8U);
  return (cnt);
}

static int32_t I2S_Control (uint32_t control, uint32_t arg1, uint32_t arg2, I2S_RESOURCES *i2s)
{
  uint32_t  master =0, data_bits =0;
  uint32_t  bit_freq = 0, val =0,freq=0;
	
  if ((i2s->flags & I2S_FLAG_POWERED) == 0U) 
  {
    // I2S not powered
    return ARM_DRIVER_ERROR;
  }

  master      = 0U;
  data_bits   = 0U;
 
  switch (control & ARM_SAI_CONTROL_Msk) 
  {
    case ARM_SAI_CONFIGURE_TX:
      break;      
    case ARM_SAI_CONFIGURE_RX:
      break;      
    case ARM_SAI_CONTROL_TX:
      // I2S Enable
      i2s->reg->I2S_IER_b.IEN  = 1;
      if ((i2s->info->status.tx_busy == 0U) || (i2s->dma_tx)) 
      {
        // Set TX level to 0
        val = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TXFCR_b.TXCHET;
        val|= 0x8;     // max FIFO level for TX overflow
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TXFCR_b.TXCHET = val;
        if (i2s->info->status.tx_busy == 0U) 
        {
          // Ready to detect TX underflow
          i2s->info->tx.num = 0U;
        }
      }
       
      // Mute
      if ((arg1 & 2U) != 0U) 
      { 
        if(i2s->xfer_chnl)
        {
          i2s->reg->CHANNEL_CONFIG[1].I2S_TER_b.TXCHEN = 0x1;
        } 
        else
        {
          i2s->reg->CHANNEL_CONFIG[0].I2S_TER_b.TXCHEN = 0x1;
        }               
      }
      else                   
      {}      
      return ARM_DRIVER_OK;

    case ARM_SAI_CONTROL_RX:
      i2s->reg->I2S_IER_b.IEN  = 1;
      i2s->reg->I2S_IRER_b.RXEN = 0x0;
     
      // Set Stop
      val = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT;  
      if (i2s->info->status.rx_busy == 0U) 
      {
        // Set FIFO level to full, to detect RX overflow
        val |= 0x8;
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT = val;

        // Ready to detect RX overflow
        i2s->info->rx.num = 0U;

        // Enable I2S receive interrupt
        //RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_RXDAM ,INTR_UNMASK );
      } 
      else 
      {
        if(i2s->dma_rx != NULL) 
        {
          // Set user RX FIFO level
          val |= (i2s->rx_fifo_level);

          // Enable I2S receive interrupt
          //RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_RXDAM ,INTR_UNMASK );
        }
      }
      return ARM_DRIVER_OK;

    case ARM_SAI_MASK_SLOTS_TX:
      return ARM_DRIVER_ERROR;

    case ARM_SAI_MASK_SLOTS_RX:
      return ARM_DRIVER_ERROR;

    case ARM_SAI_ABORT_SEND:
      // Disable TX interrupt
      RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_TXFEM ,INTR_MASK );

      if (i2s->dma_tx) 
      {
        if (i2s->info->status.tx_busy != 0U) 
        {
          if((i2s->reg == I2S0)&&(RTE_I2S0_CHNL_UDMA_TX_EN || RTE_I2S0_CHNL_UDMA_RX_EN))
          {         
            // Disable DMA channel
            UDMA0_ChannelDisable (i2s->dma_tx->channel);
          }
          else
          {
            UDMA1_ChannelDisable (i2s->dma_tx->channel);            
          }
        }
      }

      // Reset TX channel FIFO
      i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TFF_b.TXCHFR = 0x1;

      // Reset counters
      i2s->info->tx.cnt = 0U;
      i2s->info->tx.num = 0U;
      i2s->info->tx.residue_cnt = 0U;
      i2s->info->tx.residue_num = 0U;

      // Clear busy flag
      i2s->info->status.tx_busy = 0U;

      if (i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TER_b.TXCHEN) 
      {
        // Transmitter is enabled
      
        // Set FIFO level to full and enable TX interrupt, to detect RX overflow
        val  = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TXFCR_b.TXCHET;
        val |= (8U);
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TXFCR_b.TXCHET = val;
        //RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_TXFOM ,INTR_UNMASK );
      }
      return ARM_DRIVER_OK;

    case ARM_SAI_ABORT_RECEIVE:
      // Disable RX interrupt
      RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_RXDAM ,INTR_MASK );

      if (i2s->dma_rx) 
      {
        if (i2s->info->status.rx_busy != 0U) 
        {
          if((i2s->reg == I2S0)&&(RTE_I2S0_CHNL_UDMA_TX_EN || RTE_I2S0_CHNL_UDMA_RX_EN))
          {            
            // Disable DMA channel
            UDMA0_ChannelDisable (i2s->dma_rx->channel);
          }
          else
          {
            UDMA1_ChannelDisable (i2s->dma_rx->channel);            
          }
        }
      }

      // Reset RX FIFO
      i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFF_b.RXCHFR = 0x1;

      // Reset counters
      i2s->info->rx.cnt = 0U;
      i2s->info->rx.num = 0U;
      i2s->info->rx.residue_cnt = 0U;
      i2s->info->rx.residue_num = 0U;

      // Clear busy flag
      i2s->info->status.rx_busy = 0U;

      if (i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RER_b.RXCHEN) 
      {
        // Receiver is enabled      
        // Set FIFO level to full and enable RX interrupt, to detect RX overflow
        val  = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT;
        val |= (8U);
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT = val;
        RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_RXFOM ,INTR_UNMASK );
      }
      return ARM_DRIVER_OK;

    default: return ARM_DRIVER_ERROR;
  }

  // Protocol
  val = (control & ARM_SAI_PROTOCOL_Msk);
  if ((val != ARM_SAI_PROTOCOL_I2S) && (val != ARM_SAI_PROTOCOL_PCM_SHORT)) 
  {
    // Only I2S protocol is supported
    return ARM_SAI_ERROR_PROTOCOL;
  }
  if(val == ARM_SAI_PROTOCOL_PCM_SHORT)
  {
    i2s->protocol = PCM_PROTOCOL;
    if(i2s->reg == I2S0)
	  {  
			MISC_SOFT_SET_REG_2 |= PCM_ENA;//|PCM_FSYNCSTART ;
	  }
	  if(i2s->reg == I2S1)
	  {
			RSI_ULPSS_UlpPeriClkEnable(ULPCLK , PCM_ENA/*|PCM_FSYNCSTART*/);   
		}  
  } 
  // Mode
  switch (control & ARM_SAI_MODE_Msk) 
  {
    case ARM_SAI_MODE_MASTER:
      master = 1U;
      if(i2s->reg == I2S0)
      {           
       /* sets master mode for I2S0(M4SS) */
       MISC_CFG_MISC_CTRL1 |= (1 << 23);	
      }	
      if(i2s->reg == I2S1)
      {
        /* sets master mode for I2S1(ULPSS)*/
        ULPCLK->ULP_I2S_CLK_GEN_REG_b.ULP_I2S_MASTER_SLAVE_MODE_b = 1;	
      }           
      break;
    case ARM_SAI_MODE_SLAVE:
      // Slave mode
      if(i2s->reg == I2S0)
	    {
        /* sets master mode for I2S0(M4SS) */
        MISC_CFG_MISC_CTRL1 |= (0 << 23);	
      }	
      if(i2s->reg == I2S1)
      {
       /* sets master mode for I2S1(ULPSS)*/
        ULPCLK->ULP_I2S_CLK_GEN_REG_b.ULP_I2S_MASTER_SLAVE_MODE_b = 0;	
      }	 
      break;
    default: 
      return ARM_DRIVER_ERROR;
  }

  // Synchronization
  switch (control & ARM_SAI_SYNCHRONIZATION_Msk) 
  {
    case ARM_SAI_ASYNCHRONOUS:     
      // Asynchronous mode requires SCK and WS pins    
      break;
    case ARM_SAI_SYNCHRONOUS:
      if (master == 1U) 
      {
        // Only Slave can be synchronous
        return ARM_SAI_ERROR_SYNCHRONIZATION;
      }
      // 4-pin mode: SCK and WS signals are shared between
      // I2S transmit and receive blocks
      break;
    default: 
      return ARM_SAI_ERROR_SYNCHRONIZATION;
  }

  // Data size
  switch ((control & ARM_SAI_DATA_SIZE_Msk) >> ARM_SAI_DATA_SIZE_Pos) 
  {
    case 12-1:
      data_bits = 12;
      if(i2s->protocol == PCM_PROTOCOL)
      {
        MISC_SOFT_SET_REG_2 |= (RES_12_BIT << 2);
      }
      else
      {
        // Gate after 12 clock cycles
        i2s->reg->I2S_CCR_b.SCLKG = RES_12_BIT;
        // no of sclk cycles 
        i2s->reg->I2S_CCR_b.WSS   = 0x0;        
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TCR_b.WLEN = RES_12_BIT;
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RCR_b.WLEN = RES_12_BIT;
      }
      break;
    case 16-1:
      data_bits = 16;
      if(i2s->protocol == PCM_PROTOCOL)
      {
        MISC_SOFT_SET_REG_2 |= (RES_16_BIT << 2);
      }
      else
      {
        i2s->reg->I2S_CCR_b.SCLKG = RES_16_BIT;
        i2s->reg->I2S_CCR_b.WSS   = 0x0;   
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TCR_b.WLEN = RES_16_BIT;
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RCR_b.WLEN = RES_16_BIT;
        i2s->reg->I2S_CCR_b.WSS = 0;
      }
      break;
    case 20-1:
      data_bits = 20;
      i2s->reg->I2S_CCR_b.SCLKG = RES_20_BIT;
      i2s->reg->I2S_CCR_b.WSS   = 0x1;
      i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TCR_b.WLEN = RES_20_BIT;
      i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RCR_b.WLEN = RES_20_BIT;          
      break;
    case 24-1:
      data_bits = 24;
      if(i2s->protocol == PCM_PROTOCOL)
      {
        MISC_SOFT_SET_REG_2 |= ((RES_24_BIT-1) << 2);
      }
      else
      {
        i2s->reg->I2S_CCR_b.SCLKG = RES_24_BIT;
        i2s->reg->I2S_CCR_b.WSS   = 0x1;
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_TCR_b.WLEN = RES_24_BIT;
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RCR_b.WLEN = RES_24_BIT;
      }
      break;
    default: 
      return ARM_SAI_ERROR_DATA_SIZE;
  }

  // Companding
  val = control & ARM_SAI_COMPANDING_Msk;
  if (val != ARM_SAI_COMPANDING_NONE)  
  { 
    return ARM_SAI_ERROR_COMPANDING; 
  }

  // Clock polarity
  val = control & ARM_SAI_CLOCK_POLARITY_Msk;
  if (val != ARM_SAI_CLOCK_POLARITY_0) 
  { 
    return ARM_SAI_ERROR_CLOCK_POLARITY; 
  }

  // Master clock pin
  
  // Frame length
  val = ((arg1 & ARM_SAI_FRAME_LENGTH_Msk) >> ARM_SAI_FRAME_LENGTH_Pos);
  if ((val != 0U) && (val != (data_bits * 2))) 
  { 
    return ARM_SAI_ERROR_FRAME_LENGHT; 
  }

  // Audio Frequency
  if (master == 1U) 
  {
// As per standard I2S bit clock frequency calculation   
     bit_freq = (data_bits*2*arg2);    
     if(i2s->reg == I2S0)
     {
       RSI_CLK_SetI2sPllFreq(M4CLK, bit_freq,40000000);
       RSI_CLK_I2sClkConfig(M4CLK ,I2S_PLLCLK,0); 
       clk_peripheral_clk_enable(M4CLK, I2SM_CLK, ENABLE_STATIC_CLK);       
     }
     if(i2s->reg == I2S1)
     {
       if(RTE_I2S1_CLK_SRC == ULP_I2S_REF_CLK)
       {
         val = 32000000/bit_freq;
         RSI_ULPSS_UlpI2sClkConfig(ULPCLK ,RTE_I2S1_CLK_SRC,val/2);         
       } 
       if(RTE_I2S1_CLK_SRC == ULP_I2S_ULP_32MHZ_RC_CLK)
       {
         val = 32000000/bit_freq;
         RSI_ULPSS_UlpI2sClkConfig(ULPCLK ,RTE_I2S1_CLK_SRC,val/2);          
       }  
       if(RTE_I2S1_CLK_SRC == ULP_I2S_ULP_20MHZ_RO_CLK)
       {
         val = 20000000/bit_freq;
         RSI_ULPSS_UlpI2sClkConfig(ULPCLK ,RTE_I2S1_CLK_SRC,val);         
       }
       if(RTE_I2S1_CLK_SRC == ULP_I2S_SOC_CLK)
       {
         // TODO: This source is not working
         //freq = GetSOCClockFreq();
         val = 32000000/bit_freq;
         //RSI_ULPSS_UlpI2sClkConfig(ULPCLK ,RTE_I2S1_CLK_SRC,val/2);         
       }
       if(RTE_I2S1_CLK_SRC == ULP_I2S_PLL_CLK)
       {
         val = 6250000/bit_freq;
         RSI_ULPSS_UlpI2sClkConfig(ULPCLK ,RTE_I2S1_CLK_SRC,val/2);         
       }       
     }     
  }

  // Save values to registers and globals
  if ((control & ARM_SAI_CONTROL_Msk) == ARM_SAI_CONFIGURE_TX) 
  {
    i2s->info->tx.data_bits = data_bits;
    i2s->info->tx.master    = master;
  } 
  else 
  {
    i2s->info->rx.data_bits = data_bits;
    i2s->info->rx.master    = master;
  }

  i2s->flags |= I2S_FLAG_CONFIGURED;

  return ARM_DRIVER_OK;
}

static ARM_SAI_STATUS I2S_GetStatus (I2S_RESOURCES *i2s)
{
  ARM_SAI_STATUS status;

  status.frame_error  = i2s->info->status.frame_error;
  status.rx_busy      = i2s->info->status.rx_busy;
  status.rx_overflow  = i2s->info->status.rx_overflow;
  status.tx_busy      = i2s->info->status.tx_busy;
  status.tx_underflow = i2s->info->status.tx_underflow;

  return status;
}

/**
  \fn          void I2S_IRQHandler (I2S_RESOURCES *i2s)
  \brief       I2S Interrupt handler.
*/
static void I2S_IRQHandler (I2S_RESOURCES *i2s) 
{
  uint32_t  state, val, event, level;
  uint32_t  i, j;
  uint8_t  *ptr_buf;

  state = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_ISR;
  event = 0U;

  if(state) 
  {
    // Fill TX FIFO if needed    
    // Check for TX underflow
    if (i2s->info->tx.num == 0U) 
    {
      // Set TX underflow event and flag
      i2s->info->status.tx_underflow = 1U;
      event |= ARM_SAI_EVENT_TX_UNDERFLOW;

      // Disable TX interrupt
      //RSI_I2S_InterruptEnableDisable( i2s->reg,i2s->xfer_chnl, F_TXFEM ,INTR_MASK );
    } 
    if(state & I2S_STAT_TRANSMIT_EMPTY) 
    {
      while (((i2s->info->tx.cnt + 4U) <= i2s->info->tx.num)) 
      {
        // Copy all available 32bit data to FIFO, until FIFO is full
        ptr_buf = i2s->info->tx.buf + i2s->info->tx.cnt;
        if((i2s->info->tx.cnt)/2 == 0)
        {
          i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_LTHR = *(__packed uint32_t *)(ptr_buf);
        }
        if((i2s->info->tx.cnt)/2 == 1)
        {
          i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RTHR = *(__packed uint32_t *)(ptr_buf);
        }
        // Update TX buffer info
        i2s->info->tx.cnt += 4U;
        level--;
      }

      if ((i2s->info->tx.cnt < i2s->info->tx.num) && (level != 0U)) 
      {
        while (i2s->info->tx.cnt < i2s->info->tx.num) 
        {
          // Copy remaining data to residue buffer (data < 32bits)
          i2s->info->tx.residue_buf[i2s->info->tx.residue_cnt++] = i2s->info->tx.buf[i2s->info->tx.cnt++];
        }
      }

      if (i2s->info->tx.cnt == i2s->info->tx.num) 
      {
        i2s->info->status.tx_busy = 0U;
        i2s->info->tx.num = 0U;
        event |= ARM_SAI_EVENT_SEND_COMPLETE;
      }
    }
    
   // RX interrupt 
   if(state & I2S_STAT_RXDATA_OVERRUN)
   {     
      // Check for RX overflow
      if (i2s->info->rx.num == 0U) 
      {
        // Flush residue buffer
        i2s->info->rx.residue_cnt = 0U;
        i2s->info->rx.residue_num = 0U;

        i2s->info->status.rx_overflow = 1U;
        event |= ARM_SAI_EVENT_RX_OVERFLOW;
        RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_RXFOM ,INTR_MASK );
      }
    }      
    if(state & I2S_STAT_RXDA) 
    {
      // Get FIFO level
      while (((i2s->info->rx.cnt + 4U) <= i2s->info->rx.num) && (level != 0U)) 
      {
        // Read FIFO
        ptr_buf = i2s->info->rx.buf + i2s->info->rx.cnt;
        
        if((i2s->info->rx.cnt)/2 == 0)
        {
          *(__packed uint32_t *)(ptr_buf) = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_LRBR; 
        }
        if((i2s->info->rx.cnt)/2 == 1)
        {
          *(__packed uint32_t *)(ptr_buf) = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RRBR; 
        }

        i2s->info->rx.cnt += 4U;
        level--;
      }

      if ((i2s->info->rx.cnt < i2s->info->rx.num) && (level != 0U)) 
      {
        if((i2s->info->rx.cnt)/2 == 0)
        {
          // Read FIFO
          val = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_LRBR;
        }
        if((i2s->info->rx.cnt)/2 == 1)
        {
          // Read FIFO
          val = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RRBR;
        }
        j = 0U;
        for (i = 0U; i < 4U; i++) 
        {
          if ((i2s->info->rx.cnt < i2s->info->rx.num)) 
          {
            i2s->info->rx.buf[i2s->info->rx.cnt++] = (uint8_t)(val >> j);
          } 
          else 
          {
            i2s->info->rx.residue_buf[i2s->info->rx.residue_num++] = (uint8_t)(val >> j);
          }
          j += 8U;
        }
      }

      if (i2s->info->rx.cnt == i2s->info->rx.num) 
      {
        i2s->info->status.rx_busy = 0U;
        i2s->info->rx.num = 0U;
        event |= ARM_SAI_EVENT_RECEIVE_COMPLETE;
      }
    }
  }
  if ((event != 0U) && (i2s->info->cb_event != NULL)) 
  {
    i2s->info->cb_event (event);
  }
}

#if (((RTE_I2S0 != 0U) &&(RTE_I2S0_CHNL_UDMA_TX_EN == 1U)) || \
     ((RTE_I2S1 != 0U) &&(RTE_I2S1_CHNL_UDMA_TX_EN == 1U)))
static void I2S_UDMA_Tx_Event (uint32_t event,uint8_t dmaCh, I2S_RESOURCES *i2s) 
{
  uint32_t evt = 0;

  switch (event) 
  {
    case UDMA_EVENT_XFER_DONE:
      // Update TX buffer info
      i2s->info->tx.cnt += (i2s->info->tx.num / 4U) * 4U;

      while (i2s->info->tx.cnt < i2s->info->tx.num) 
      {
        // Copy remaining data to residue buffer (data < 32bits)
        i2s->info->tx.residue_buf[i2s->info->tx.residue_cnt++] = i2s->info->tx.buf[i2s->info->tx.cnt++];
      }

      // Clear TX busy flag
      i2s->info->status.tx_busy = 0U;

      // Clear TX num and enable TX interrupt to detect TX underflow
      i2s->info->tx.num = 0U;
      
      //i2s->reg->IRQ |= I2S_IRQ_TX_IRQ_ENABLE;
      RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_TXFOM ,INTR_UNMASK );
      
      // Set Send complete event
      evt = ARM_SAI_EVENT_SEND_COMPLETE;
      break;
    case UDMA_EVENT_ERROR:
      break;
  }
  if ((evt != 0U) && (i2s->info->cb_event != NULL)) 
  {
    i2s->info->cb_event (evt);
  }
}
#endif

#if (((RTE_I2S0 != 0U) &&(RTE_I2S0_CHNL_UDMA_RX_EN == 1U)) || \
     ((RTE_I2S1 != 0U) &&(RTE_I2S1_CHNL_UDMA_RX_EN == 1U)))
static void I2S_UDMA_Rx_Event (uint32_t event,uint8_t dmaCh, I2S_RESOURCES *i2s) 
{
  uint32_t evt = 0U;
  uint32_t val;

  switch (event) 
  {
    case UDMA_EVENT_XFER_DONE:
      if (i2s->info->rx.cnt == i2s->info->rx.num) 
      {
        // Clear RX busy flag
        i2s->info->status.rx_busy = 0U;

        // Set receive complete event
        evt = ARM_SAI_EVENT_RECEIVE_COMPLETE;

        // Set FIFO level to full, to detect RX overflow
        val  = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT;
        val |= (8U);
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT = val;
      } 
      else 
      {
        // Set user defined level, to retrieve remaining requested data
        val  = i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT;
        val |= (i2s->rx_fifo_level);
        i2s->reg->CHANNEL_CONFIG[i2s->xfer_chnl].I2S_RFCR_b.RXCHDT = val;      
      }

      RSI_I2S_InterruptEnableDisable( i2s,i2s->xfer_chnl, F_RXFOM ,INTR_UNMASK );;
      break;
    case UDMA_EVENT_ERROR:
      break;
  }
  if ((evt != 0U) && (i2s->info->cb_event != NULL)) 
  {
    i2s->info->cb_event (evt);
  }
}
#endif


#if (RTE_I2S0)
// I2S0 Driver Wrapper functions
static ARM_SAI_CAPABILITIES I2S0_GetCapabilities (void) 
{
  return I2S_GetCapabilities (&I2S0_Resources);
}

static int32_t I2S0_Initialize (ARM_SAI_SignalEvent_t cb_event) 
{
  return I2S_Initialize (cb_event, &I2S0_Resources);
}

static int32_t I2S0_Uninitialize (void) 
{
  return I2S_Uninitialize (&I2S0_Resources);
}

static int32_t I2S0_PowerControl (ARM_POWER_STATE state) 
{
  return I2S_PowerControl (state, &I2S0_Resources);
}

static int32_t I2S0_Send (const void *data, uint32_t num) 
{
  return I2S_Send (data, num, &I2S0_Resources);
}

static int32_t I2S0_Receive (void *data, uint32_t num) 
{
  return I2S_Receive (data, num, &I2S0_Resources);
}

static uint32_t I2S0_GetTxCount (void) 
{
  return I2S_GetTxCount (&I2S0_Resources);
}

static uint32_t I2S0_GetRxCount (void) 
{
  return I2S_GetRxCount (&I2S0_Resources);
}

static int32_t I2S0_Control (uint32_t control, uint32_t arg1, uint32_t arg2) 
{
  return I2S_Control (control, arg1, arg2, &I2S0_Resources);
}

static ARM_SAI_STATUS I2S0_GetStatus (void) 
{
  return I2S_GetStatus (&I2S0_Resources);
}

void IRQ064_Handler (void) 
{
  I2S_IRQHandler (&I2S0_Resources);
}

#if (RTE_I2S0_CHNL_UDMA_TX_EN == 1)
void I2S0_UDMA_Tx_Event (uint32_t event, uint8_t dmaCh) 
{
  I2S_UDMA_Tx_Event (event,dmaCh,&I2S0_Resources);
}
#endif

#if (RTE_I2S0_CHNL_UDMA_RX_EN == 1)
void I2S0_UDMA_Rx_Event (uint32_t event,uint8_t dmaCh) 
{
  I2S_UDMA_Rx_Event (event,dmaCh, &I2S0_Resources);
}
#endif

// SAI0 Driver Control Block
ARM_DRIVER_SAI Driver_SAI0 = {
    I2Sx_GetVersion,
    I2S0_GetCapabilities,
    I2S0_Initialize,
    I2S0_Uninitialize,
    I2S0_PowerControl,
    I2S0_Send,
    I2S0_Receive,
    I2S0_GetTxCount,
    I2S0_GetRxCount,
    I2S0_Control,
    I2S0_GetStatus
};
#endif

#if (RTE_I2S1)
// I2S1 Driver Wrapper functions
static ARM_SAI_CAPABILITIES I2S1_GetCapabilities (void) 
{
  return I2S_GetCapabilities (&I2S1_Resources);
}

static int32_t I2S1_Initialize (ARM_SAI_SignalEvent_t cb_event) 
{
  return I2S_Initialize (cb_event, &I2S1_Resources);
}

static int32_t I2S1_Uninitialize (void) 
{
  return I2S_Uninitialize (&I2S1_Resources);
}

static int32_t I2S1_PowerControl (ARM_POWER_STATE state) 
{
  return I2S_PowerControl (state, &I2S1_Resources);
}

static int32_t I2S1_Send (const void *data, uint32_t num) 
{
  return I2S_Send (data, num, &I2S1_Resources);
}

static int32_t I2S1_Receive (void *data, uint32_t num) 
{
  return I2S_Receive (data, num, &I2S1_Resources);
}

static uint32_t I2S1_GetTxCount (void) 
{
  return I2S_GetTxCount (&I2S1_Resources);
}

static uint32_t I2S1_GetRxCount (void) 
{
  return I2S_GetRxCount (&I2S1_Resources);
}

static int32_t I2S1_Control (uint32_t control, uint32_t arg1, uint32_t arg2) 
{
  return I2S_Control (control, arg1, arg2, &I2S1_Resources);
}

static ARM_SAI_STATUS I2S1_GetStatus (void) 
{
  return I2S_GetStatus (&I2S1_Resources);
}

void I2S1_IRQHandler (void) 
{
  I2S_IRQHandler (&I2S1_Resources);
}

#if (RTE_I2S1_CHNL_UDMA_TX_EN == 1)
void I2S1_UDMA_Tx_Event (uint32_t event,uint8_t dmaCh) 
{
  I2S_UDMA_Tx_Event (event, dmaCh,&I2S1_Resources);
}
#endif

#if (RTE_I2S1_CHNL_UDMA_RX_EN == 1)
void I2S1_UDMA_Rx_Event (uint32_t event, uint8_t dmaCh) 
{
  I2S_UDMA_Rx_Event (event, dmaCh, &I2S1_Resources);
}
#endif

// SAI1 Driver Control Block
ARM_DRIVER_SAI Driver_SAI1 = {
    I2Sx_GetVersion,
    I2S1_GetCapabilities,
    I2S1_Initialize,
    I2S1_Uninitialize,
    I2S1_PowerControl,
    I2S1_Send,
    I2S1_Receive,
    I2S1_GetTxCount,
    I2S1_GetRxCount,
    I2S1_Control,
    I2S1_GetStatus
};
#endif

