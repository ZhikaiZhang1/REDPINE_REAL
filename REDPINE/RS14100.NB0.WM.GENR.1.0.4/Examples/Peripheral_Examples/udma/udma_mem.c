/**
 * @file       udma_m2m.c
 * @version    1.0
 * @date       27 jun 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example describing usage of UDMA APIs.
 *
 * @section Description
 * UDMA ROM driver memory to memory example. 
 *
 */
 
/* Includes ------------------------------------------------------------------*/
#include "UDMA.h"
#include "rsi_board.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define  SIZE_BUFFERS            (1024)
#define  DMA_DESC_LEN            (1024)
#define  CHNL0                    0
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern RSI_DRIVER_UDMA Driver_UDMA0;
static RSI_DRIVER_UDMA *UDMAdrv0 = &Driver_UDMA0;
extern RSI_UDMA_HANDLE_T udmaHandle0;

uint32_t src0[SIZE_BUFFERS];   
uint32_t dst0[SIZE_BUFFERS];

volatile uint32_t done,ret;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief      UDMA  controller transfer descriptor chain complete callback 
 * @param[in]  event dma transfer events
 * @param[in]  ch    dma channel number
 * @return     None
 */ 
void udmaTransferComplete(uint32_t event, uint8_t ch)
{
  if(event == UDMA_EVENT_XFER_DONE)
  {
    if(ch == 0)
    {
      done = 1;  
    }
  }
} 

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main()
{
	/* At this stage the MICROCONTROLLER clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */

   int loop =0;
 
	
   RSI_UDMA_CHA_CONFIG_DATA_T control;
   RSI_UDMA_CHA_CFG_T config;
	 /*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	   /* Initialize UART for debug prints */
   	DEBUGINIT();
   memset(&control, 0, sizeof(RSI_UDMA_CHA_CONFIG_DATA_T));
	 memset(&config, 0, sizeof(RSI_UDMA_CHA_CFG_T));
   
   config.altStruct = 0;
   config.burstReq  = 1;
   config.channelPrioHigh = 0;
   config.dmaCh = CHNL0;
   config.periAck = 0;
   config.periphReq = 0;
   config.reqMask = 0;
   
   control.transferType       = UDMA_MODE_AUTO;
   control.nextBurst					= 0;
   control.totalNumOfDMATrans = (DMA_DESC_LEN-1);
   control.rPower             = ARBSIZE_16;
   control.srcProtCtrl 			  = 0x000;
   control.dstProtCtrl 			  = 0x000;
   control.srcSize            = SRC_SIZE_32;
   control.srcInc             = SRC_INC_32;
   control.dstSize            = DST_SIZE_32;
   control.dstInc             = DST_INC_32;
    
   for (loop = 0; loop < SIZE_BUFFERS; loop++) 
   {
     src0[loop] = loop + 1 ;
     dst0[loop] = 0;
   }
   /*  Prints on hyper-terminal  */
   DEBUGOUT("UDMA Memory to Memory data transfer example\r\n");
   /* Initialize dma */ 
   UDMAdrv0->Initialize();
 
   /* Configure dma channel */ 
   UDMAdrv0->ChannelConfigure( CHNL0,(uint32_t)src0, (uint32_t)dst0, SIZE_BUFFERS, 
                               control, &config, udmaTransferComplete );
   /* Enable dma channel */
   UDMAdrv0->ChannelEnable(CHNL0); 
   
   /* Enable dma controller */  
   UDMAdrv0->DMAEnable();

   /* Wait till dma done */
   while(!done); 
   
   /* Compare data buffers */
   ret = memcmp(dst0, src0, sizeof(src0));
   if(ret)
   {
		 /*  Prints on hyper-terminal  */
     DEBUGOUT("Data comparison fail\r\n");
   }
   else
   {
		 /*  Prints on hyper-terminal  */
     DEBUGOUT("Data comparison success\r\n");
   }
   /* Uninitialize dma */
   UDMAdrv0->Uninitialize();
   
   while(1); 
}
