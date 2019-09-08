#include "UDMA.h"
#include "rsi_rom_udma.h"


uint32_t udma0_init_flag = 0;
uint32_t udma1_init_flag = 0;
uint32_t udma_instance_flag = 0;
RSI_UDMA_HANDLE_T udmaHandle0,udmaHandle1;
uint32_t dma_rom_buff0[30], dma_rom_buff1[30];

#define CONTROL_STRUCT0   (UDMA_NUMBER_OF_CHANNELS * 2)
#define CONTROL_STRUCT1   (ULP_UDMA_NUMBER_OF_CHANNELS * 2)

#if  ((UDMA0_SRAM_BASE & (~0x3FF)) != UDMA0_SRAM_BASE)
#error "Invalid UDMA0 sram base address"
#endif

#if  ((UDMA1_SRAM_BASE & (~0x3FF)) != UDMA1_SRAM_BASE)
#error "Invalid UDMA1 sram base address"
#endif

/* IAR support */
#if defined(__ICCARM__)
#pragma location = UDMA0_SRAM_BASE
RSI_UDMA_DESC_T UDMA0_Table[CONTROL_STRUCT0];
#pragma location = UDMA1_SRAM_BASE
RSI_UDMA_DESC_T UDMA1_Table[CONTROL_STRUCT1];
#endif

/* DMA descriptors must be aligned to 16 bytes */
#if defined(__CC_ARM)
RSI_UDMA_DESC_T UDMA0_Table[CONTROL_STRUCT0] __attribute__ ((at(UDMA0_SRAM_BASE)));
RSI_UDMA_DESC_T UDMA1_Table[CONTROL_STRUCT1] __attribute__ ((at(UDMA1_SRAM_BASE)));
#endif /* defined (__CC_ARM) */

#if defined( __GNUC__ )
RSI_UDMA_DESC_T UDMA0_Table[CONTROL_STRUCT0] __attribute__ ((at(UDMA0_SRAM_BASE)));
RSI_UDMA_DESC_T UDMA1_Table[CONTROL_STRUCT1] __attribute__ ((at(UDMA1_SRAM_BASE)));
#endif /* defined (__GNUC__) */


typedef struct 
{
  uint32_t            SrcAddr;
  uint32_t            DestAddr;
  uint32_t            Size;
  uint32_t            Cnt;
  UDMA_SignalEvent_t  cb_event;
} UDMA_Channel_Info;


UDMA_Channel_Info udma0_chnl_info[32] = { 0U } ;
UDMA_Channel_Info udma1_chnl_info[12] = { 0U } ;

#ifdef RTE_UDMA0

/* I2C0 Resources */
static UDMA_RESOURCES UDMA0_Resources = {
 (UDMA0_Type *)UDMA0,
  UDMA0_IRQn,
  UDMA0_Table          // SRAM base address
};
#endif /* RTE_UDMA0 */


#ifdef RTE_UDMA1

/* I2C1 Resources */
static UDMA_RESOURCES UDMA1_Resources = {
 (UDMA0_Type *)UDMA1,
  UDMA1_IRQn,
  UDMA1_Table          // SRAM base address
};
#endif /* RTE_UDMA1 */


/**
  \fn          int32_t UDMA_Initialize (void)
  \brief       Initialize UDMA peripheral
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t UDMAx_Initialize (UDMA_RESOURCES *udma) 
{
  RSI_UDMA_INIT_T udmaInit0 = {0}, udmaInit1 ={0};
 
  if(udma->reg == UDMA0)
  {
    RSI_PS_M4ssPeriPowerUp(M4SS_PWRGATE_ULP_PERI1);
    RSI_CLK_PeripheralClkEnable(M4CLK,UDMA_CLK,ENABLE_STATIC_CLK);   
  }
  if(udma->reg == UDMA1)
  {
    RSI_PS_UlpssPeriPowerUp(ULPSS_PWRGATE_ULP_UDMA);
    RSI_ULPSS_PeripheralEnable(ULPCLK, ULP_UDMA_CLK, ENABLE_STATIC_CLK);
  }
  
  if(udma->reg == UDMA0)
  {      
    if(!udma0_init_flag)
    {
      if(udma_instance_flag == 0)
      udmaInit0.base = (uint32_t)UDMA0;
      udmaInit0.sramBase = (uint32_t)UDMA0_Table;
      udmaInit0.pUserData = (void *)NULL;
      udmaHandle0 = RSI_UDMA_Init(dma_rom_buff0, &udmaInit0);
      if (udmaHandle0 == NULL) 
      {
        return ARM_DRIVER_ERROR;
      }
    }
  }
  
  if(udma->reg == UDMA1)
  { 
    if(!udma1_init_flag)
    {
      udmaInit1.base = (uint32_t)UDMA1;
      
      if(M4_ULP_SLP_STATUS_REG & ULP_MODE_SWITCHED_NPSS)
      {
        if(((uint32_t)UDMA1_Table >= ULP_SRAM_START_ADDR)&& 
           ((uint32_t)UDMA1_Table <= ULP_SRAM_END_ADDR))
        {
          udmaInit1.sramBase = (uint32_t)UDMA1_Table;
        }
        else
        {
          return ERROR_UDMA_CTRL_BASE_INVALID;
        }
      }
      else
      {
        udmaInit1.sramBase = (uint32_t)UDMA1_Table; 
      }
      udmaInit1.pUserData = (void *)NULL;
      udmaHandle1 = RSI_UDMA_Init(dma_rom_buff1, &udmaInit1);
      if (udmaHandle1 == NULL) 
      {
        return ARM_DRIVER_ERROR;
      }
    }
  }
  // Clear all DMA interrupt flags
  NVIC_ClearPendingIRQ(udma->udma_irq_num);
  NVIC_EnableIRQ(udma->udma_irq_num);
  
  return 0;
}

/**
  \fn          int32_t UDMA_Uninitialize (void)
  \brief       De-initialize GPDMA peripheral
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t UDMAx_Uninitialize (UDMA_RESOURCES *udma) 
{
 // Disable DMA clock,Disable and Clear DMA IRQ
  if(udma->reg == UDMA0) 
  { 
    RSI_PS_M4ssPeriPowerDown(M4SS_PWRGATE_ULP_PERI1);    
    RSI_CLK_PeripheralClkDisable(M4CLK,UDMA_CLK);    
  }
  if(udma->reg == UDMA1) 
  {  
    RSI_PS_UlpssPeriPowerDown(ULPSS_PWRGATE_ULP_UDMA);    
    RSI_ULPSS_PeripheralDisable(ULPCLK,ULP_UDMA_CLK);    
  }
  NVIC_DisableIRQ(udma->udma_irq_num);
  NVIC_ClearPendingIRQ(udma->udma_irq_num);
  return 0;
}


/**
  \fn          int32_t UDMA_ChannelConfigure (uint8_t            ch,
                                              uint32_t           src_addr,
                                              uint32_t           dest_addr,
                                              uint32_t           size,
                                              RSI_UDMA_CHA_CONFIG_DATA_T vsUdmaChaConfigData,
                                              uint32_t           config,
                                              UDMA_SignalEvent_t cb_event)
  \brief       Configure GPDMA channel for next transfer
  \param[in]   ch        Channel number (0..7)
  \param[in]   src_addr  Source address
  \param[in]   dest_addr Destination address
  \param[in]   size      Amount of data to transfer
  \param[in]   control   Channel control
  \param[in]   config    Channel configuration
  \param[in]   cb_event  Channel callback pointer
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t UDMAx_ChannelConfigure (UDMA_RESOURCES *udma, 
                                       uint8_t             ch,
                                       uint32_t            src_addr,
                                       uint32_t            dest_addr,
                                       uint32_t            size,
                                       RSI_UDMA_CHA_CONFIG_DATA_T control,
                                       RSI_UDMA_CHA_CFG_T  *config,
                                       UDMA_SignalEvent_t  cb_event) 
{  
  uint32_t len0 =0,len1 =0;  
  if(udma->reg == UDMA0)
  {
    udma0_chnl_info[ch].cb_event = cb_event;
  }
  else
  {
    udma1_chnl_info[ch].cb_event = cb_event; 
  }
  if(M4_ULP_SLP_STATUS_REG & ULP_MODE_SWITCHED_NPSS) 
  { //PS2 state
    if(control.dstInc == UDMA_DST_INC_NONE)
    {
      if((src_addr >= ULP_SRAM_START_ADDR)&& 
         (src_addr <= ULP_SRAM_END_ADDR))
      { 
        udma1_chnl_info[ch].SrcAddr  = (src_addr);
      }
      else
      {
        return ERROR_UDMA_SRC_ADDR; 
      }
    }
    if(control.srcInc == UDMA_SRC_INC_NONE)
    {
      if((dest_addr >= ULP_SRAM_START_ADDR)&& 
         (dest_addr <= ULP_SRAM_END_ADDR))
      { 
        udma1_chnl_info[ch].DestAddr = (dest_addr);
      }
      else
      {
        return ERROR_UDMA_DST_ADDR;
      }
    }
  }
  if(udma->reg == UDMA0)
  { 
		// Clear DMA interrupts
		RSI_UDMA_InterruptClear(udmaHandle0, ch);
		RSI_UDMA_ErrorStatusClear(udmaHandle0);		
		RSI_UDMA_SetupChannel(udmaHandle0, config); 
  }
	if(udma->reg == UDMA1)
  {
		// Clear DMA interrupts
		RSI_UDMA_InterruptClear(udmaHandle1, ch);
		RSI_UDMA_ErrorStatusClear(udmaHandle1);		
		RSI_UDMA_SetupChannel(udmaHandle1, config); 
	}

	if(udma->reg == UDMA0)
  { 
    RSI_UDMA_SetupChannelTransfer( udmaHandle0,(RSI_UDMA_CHA_CFG_T *) config, 
		                               control,(void *)src_addr,(void *)dest_addr );
	}
	if(udma->reg == UDMA1)
	{
	  RSI_UDMA_SetupChannelTransfer( udmaHandle1,(RSI_UDMA_CHA_CFG_T *) config, 
		                               control,(void *)src_addr,(void *)dest_addr );
	}
 
  // Save channel information
  if(udma->reg == UDMA0)
  {
    len0 = control.totalNumOfDMATrans;
    udma0_chnl_info[ch].SrcAddr  = (src_addr );
    udma0_chnl_info[ch].DestAddr = (dest_addr);
    udma0_chnl_info[ch].Size     =  size;
    udma0_chnl_info[ch].Cnt      = (++len0);   
  }
  if(udma->reg == UDMA1)
  {
    len1 = control.totalNumOfDMATrans;  
    udma1_chnl_info[ch].SrcAddr  = (src_addr);    
    udma1_chnl_info[ch].DestAddr = (dest_addr);
    udma1_chnl_info[ch].Size     =  size;
    udma1_chnl_info[ch].Cnt      = (++len1);
  }
  return 0;
}


/**
  \fn          int32_t UDMA_ChannelEnable (uint8_t ch)
  \brief       Enable GPDMA channel
  \param[in]   ch Channel number (0..7)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t UDMAx_ChannelEnable (uint8_t ch,UDMA_RESOURCES *udma) 
{
  if(udma->reg == UDMA0)
  {
    RSI_UDMA_ChannelEnable(udmaHandle0,ch);
  }
  if(udma->reg == UDMA1)
  {
    RSI_UDMA_ChannelEnable(udmaHandle1,ch);
  }  
  return 0;
}

static int32_t UDMAx_DMAEnable (UDMA_RESOURCES *udma) 
{
 if(udma->reg == UDMA0)
 {   
   RSI_UDMA_UDMAEnable(udmaHandle0);  
 }
 if(udma->reg == UDMA1)
 {   
   RSI_UDMA_UDMAEnable(udmaHandle1);
 }
 return 0; 
}

/**
  \fn          int32_t UDMA_ChannelDisable (uint8_t ch)
  \brief       Disable GPDMA channel
  \param[in]   ch Channel number (0..7)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t UDMAx_ChannelDisable (uint8_t ch, UDMA_RESOURCES *udma) 
{
  if(udma->reg == UDMA0)
  {
    RSI_UDMA_ChannelDisable(udmaHandle0,ch);
  }
  if(udma->reg == UDMA1)
  {
    RSI_UDMA_ChannelDisable(udmaHandle1,ch);
  }
  return 0;
}


/**
  \fn         uint32_t UDMAx_ChannelGetCount ( uint8_t ch, 
                                               RSI_UDMA_CHA_CONFIG_DATA_T control,
                                               RSI_UDMA_CHA_CFG_T  config)                                        
  \brief       Gets number of remaining transfers of a descriptor
  \param[in]   ch Channel number (0..31)
  \param[in]   control control data structure
  \param[in]   config  channel configuration
  \returns     Remaining data lenght
*/
static uint32_t UDMAx_ChannelGetCount ( uint8_t ch, RSI_UDMA_CHA_CONFIG_DATA_T control,
                                        RSI_UDMA_CHA_CFG_T  config, 
                                        UDMA_RESOURCES *udma ) 
{
  if(udma->reg == UDMA0)
  {
    return (udma_get_channel_transfer_length( udmaHandle0, &config,control));
  }
  if(udma->reg == UDMA1)
  {
    return (udma_get_channel_transfer_length( udmaHandle1, &config,control));
  }
  return 0;
}

/**
  \fn          void DMA_IRQHandler (void)
  \brief       DMA interrupt handler
*/
void UDMAx_IRQHandler (UDMA_RESOURCES *udma) 
{
  uint32_t ch = 0, size = 0;
  uint32_t intr = 0,src_inc = 0,dst_inc = 0;
  
  for (ch = 0; ch < UDMA_NUMBER_OF_CHANNELS; ch++) 
  {
    intr = udma->reg->UDMA_DONE_STATUS_REG;
    if (intr & (1U << ch)) 
    {
      // Clear interrupt flag
      udma->reg->UDMA_DONE_STATUS_REG = (1U << ch);
      
      if((udma0_chnl_info[ch].Cnt != udma0_chnl_info[ch].Size)||
         (udma1_chnl_info[ch].Cnt != udma1_chnl_info[ch].Size))        
      {
        if(udma->reg == UDMA0)
        {
          // Data waiting to transfer
          size = udma0_chnl_info[ch].Size - (udma0_chnl_info[ch].Cnt);
          // Max DMA transfer size = 4k
          if (size > DESC_MAX_LEN) 
          { 
            size = DESC_MAX_LEN;
            udma0_chnl_info[ch].Cnt += DESC_MAX_LEN;					
          }
          else
          {
            udma0_chnl_info[ch].Cnt += size;
          }
        }
        if(udma->reg == UDMA1)
        {
          // Data waiting to transfer
          size = udma1_chnl_info[ch].Size - (udma1_chnl_info[ch].Cnt);
          // Max DMA transfer size = 4k
          if (size > DESC_MAX_LEN) 
          { 
            size = DESC_MAX_LEN;
            udma1_chnl_info[ch].Cnt += DESC_MAX_LEN;					
          }
          else
          {
            udma1_chnl_info[ch].Cnt += size;
          }
        }
        // Source Address Increment      
        if(udma->reg == UDMA0)
        {
          src_inc  =  UDMA0_Table[ch].vsUDMAChaConfigData1.srcInc; 
        }
        if(udma->reg == UDMA1)
        {
          src_inc  =  UDMA1_Table[ch].vsUDMAChaConfigData1.srcInc; 
        }       
        if (src_inc != UDMA_SRC_INC_NONE) 
        {					   
          if(udma->reg == UDMA0)
          {
            UDMA0_Table[ch].pSrcEndAddr = (void *)((uint32_t)UDMA0_Table[ch].pSrcEndAddr + (size << src_inc)) ;
            UDMA0_Table[ch].vsUDMAChaConfigData1.totalNumOfDMATrans = (size);
            UDMA0_Table[ch].vsUDMAChaConfigData1.transferType = UDMA_MODE_BASIC;
          }
          if(udma->reg == UDMA1)
          {
            UDMA1_Table[ch].pSrcEndAddr = (void *)((uint32_t)UDMA1_Table[ch].pSrcEndAddr + (size << src_inc)) ;
            UDMA1_Table[ch].vsUDMAChaConfigData1.totalNumOfDMATrans = (size);
            UDMA1_Table[ch].vsUDMAChaConfigData1.transferType = UDMA_MODE_BASIC;
          }
        }
        else
        {
          if(udma->reg == UDMA0)
          {
            UDMA0_Table[ch].pSrcEndAddr = (void *)((uint32_t)UDMA0_Table[ch].pSrcEndAddr) ;
            UDMA0_Table[ch].vsUDMAChaConfigData1.totalNumOfDMATrans = (size);
            UDMA0_Table[ch].vsUDMAChaConfigData1.transferType = UDMA_MODE_BASIC;
          }
          if(udma->reg == UDMA1)
          {
            UDMA1_Table[ch].pSrcEndAddr = (void *)((uint32_t)UDMA1_Table[ch].pSrcEndAddr) ;
            UDMA1_Table[ch].vsUDMAChaConfigData1.totalNumOfDMATrans = (size);
            UDMA1_Table[ch].vsUDMAChaConfigData1.transferType = UDMA_MODE_BASIC;
          } 
        }
        // Destination Address Increment
        if(udma->reg == UDMA0)
        {
          dst_inc = UDMA0_Table[ch].vsUDMAChaConfigData1.dstInc;
        }
        if(udma->reg == UDMA1)
        {
          dst_inc = UDMA1_Table[ch].vsUDMAChaConfigData1.dstInc;
        }      
        if (dst_inc != UDMA_DST_INC_NONE) 
        {         
          if(udma->reg == UDMA0)
          {
            UDMA0_Table[ch].pDstEndAddr = (void *)((uint32_t)UDMA0_Table[ch].pDstEndAddr +(size << dst_inc));
            UDMA0_Table[ch].vsUDMAChaConfigData1.totalNumOfDMATrans = size;
            UDMA0_Table[ch].vsUDMAChaConfigData1.transferType = UDMA_MODE_BASIC;					 
          }
          if(udma->reg == UDMA1)
          {
            UDMA1_Table[ch].pDstEndAddr = (void *)((uint32_t)UDMA1_Table[ch].pDstEndAddr +(size << dst_inc));
            UDMA1_Table[ch].vsUDMAChaConfigData1.totalNumOfDMATrans = (size);
            UDMA1_Table[ch].vsUDMAChaConfigData1.transferType = UDMA_MODE_BASIC;						 
          }
        }
        else
        {
          if(udma->reg == UDMA0)
          {
            UDMA0_Table[ch].pDstEndAddr = (void *)((uint32_t)UDMA0_Table[ch].pDstEndAddr);
            UDMA0_Table[ch].vsUDMAChaConfigData1.totalNumOfDMATrans = size;
            UDMA0_Table[ch].vsUDMAChaConfigData1.transferType = UDMA_MODE_BASIC;					 
          }
          if(udma->reg == UDMA1)
          {
            UDMA1_Table[ch].pDstEndAddr = (void *)((uint32_t)UDMA1_Table[ch].pDstEndAddr);
            UDMA1_Table[ch].vsUDMAChaConfigData1.totalNumOfDMATrans = (size);
            UDMA1_Table[ch].vsUDMAChaConfigData1.transferType = UDMA_MODE_BASIC;						 
          } 
        }
        // Enable DMA Channel
        udma->reg->CHNL_ENABLE_SET = (1U << ch);
      } 
      else 
      {
        // All Data has been transferred
        // Clear Channel active flag
        //Clear_Channel_active_flag (ch);
        // Signal Event
        if(udma->reg == UDMA0)
        {
          if (udma0_chnl_info[ch].cb_event) 
          {
            udma0_chnl_info[ch].cb_event(UDMA_EVENT_XFER_DONE,ch);
          }
        }
        if(udma->reg == UDMA1)
        {
          if (udma1_chnl_info[ch].cb_event) 
          {
            udma1_chnl_info[ch].cb_event(UDMA_EVENT_XFER_DONE, ch);
          }         
        }      
      }
    } 
    else 
    {
       // DMA error interrupt
       if (udma->reg->ERR_CLR & (1U << ch)) 
       {
         udma->reg->ERR_CLR_b.ERR_CLR = 0x1;
         // Clear Channel active flag
         //Clear_Channel_active_flag (ch);
         // Clear interrupt flag
         udma->reg->UDMA_DONE_STATUS_REG = (1U << ch);
         // Signal Event
         if (udma0_chnl_info[ch].cb_event) 
         {
           udma0_chnl_info[ch].cb_event(UDMA_EVENT_ERROR,ch);
         }
         if (udma1_chnl_info[ch].cb_event) 
         {
           udma1_chnl_info[ch].cb_event(UDMA_EVENT_ERROR, ch);
         }
       }
    }
  }
}

// END OF APIs //

#if (RTE_UDMA0)

/* UDMA0 Driver wrapper functions */
int32_t UDMA0_Initialize(void)  
{
  return (UDMAx_Initialize (&UDMA0_Resources));
}

int32_t UDMA0_Uninitialize(void) 
{
  return (UDMAx_Uninitialize (&UDMA0_Resources));
}

int32_t UDMA0_ChannelConfigure ( uint8_t             ch,
                                 uint32_t            src_addr,
                                 uint32_t            dest_addr,
                                 uint32_t            size,
                                 RSI_UDMA_CHA_CONFIG_DATA_T control,
                                 RSI_UDMA_CHA_CFG_T  *config,
                                 UDMA_SignalEvent_t  cb_event )  
{
  return (UDMAx_ChannelConfigure (&UDMA0_Resources, ch, src_addr, dest_addr,
                                   size, control, config, cb_event));
}

int32_t UDMA0_ChannelEnable (uint8_t ch)
{
  return (UDMAx_ChannelEnable (ch, &UDMA0_Resources));  
}

int32_t UDMA0_ChannelDisable (uint8_t ch)
{
  return (UDMAx_ChannelDisable(ch, &UDMA0_Resources));  
}

uint32_t UDMA0_ChannelGetCount (uint8_t ch, RSI_UDMA_CHA_CONFIG_DATA_T control,
                                RSI_UDMA_CHA_CFG_T  config)
{
  return (UDMAx_ChannelGetCount(ch,control,config,&UDMA0_Resources));  
}

int32_t UDMA0_DMAEnable(void)
{
  return (UDMAx_DMAEnable (&UDMA0_Resources));  
}

void IRQ033_Handler (void) 
{
  UDMAx_IRQHandler (&UDMA0_Resources);
}

/* UDMA0 Driver Control Block */
RSI_DRIVER_UDMA Driver_UDMA0 = {
  
  UDMA0_Initialize,
  UDMA0_Uninitialize,
  UDMA0_ChannelConfigure,
  UDMA0_ChannelEnable,
  UDMA0_ChannelDisable,
  UDMA0_ChannelGetCount,
  UDMA0_DMAEnable
};
#endif

#if (RTE_UDMA1)

/* UDMA1 Driver wrapper functions */

int32_t UDMA1_Initialize(void)  
{
  return (UDMAx_Initialize (&UDMA1_Resources));
}

int32_t UDMA1_Uninitialize(void) 
{
  return (UDMAx_Uninitialize (&UDMA1_Resources));
}

int32_t UDMA1_ChannelConfigure ( uint8_t             ch,
                                 uint32_t            src_addr,
                                 uint32_t            dest_addr,
                                 uint32_t            size,
                                 RSI_UDMA_CHA_CONFIG_DATA_T control,
                                 RSI_UDMA_CHA_CFG_T  *config,
                                 UDMA_SignalEvent_t  cb_event )  
{
  return (UDMAx_ChannelConfigure ( &UDMA1_Resources,ch,src_addr,dest_addr,
                                   size,control, config,cb_event ));
}

int32_t UDMA1_ChannelEnable (uint8_t ch)
{
  return (UDMAx_ChannelEnable (ch, &UDMA1_Resources));  
}

int32_t UDMA1_ChannelDisable (uint8_t ch)
{
  return (UDMAx_ChannelDisable(ch, &UDMA1_Resources));  
}

uint32_t UDMA1_ChannelGetCount ( uint8_t ch, 
                                 RSI_UDMA_CHA_CONFIG_DATA_T control,
                                 RSI_UDMA_CHA_CFG_T  config) 
                                 
{
  return (UDMAx_ChannelGetCount(ch,control,config,&UDMA1_Resources));  
}

int32_t UDMA1_DMAEnable ()
{
  return (UDMAx_DMAEnable (&UDMA1_Resources));  
}

void IRQ010_Handler (void) 
{
  UDMAx_IRQHandler (&UDMA1_Resources);
}

/* UDMA1 Driver access structure */
RSI_DRIVER_UDMA Driver_UDMA1 = {
  
  UDMA1_Initialize,
  UDMA1_Uninitialize,
  UDMA1_ChannelConfigure,
  UDMA1_ChannelEnable,
  UDMA1_ChannelDisable,
  UDMA1_ChannelGetCount,
  UDMA1_DMAEnable
};

#endif  
