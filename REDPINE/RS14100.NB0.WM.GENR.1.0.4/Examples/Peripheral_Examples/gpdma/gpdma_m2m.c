/**
 * @file       GPDMA_m2m.c
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
 * @brief This files contains example describing usage of GPDMA APIs.
 *
 * @section Description
 * GPDMA ROM driver memory to memory example. 
 *
 */
 

/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define  GPDMA_IRQHandler    IRQ031_Handler
#define  GPDMA_LASTCHANNEL   8
#define  NUMGPDMADESC        4
#define  XFERSIZE           (TRNSFER_LEN * 4)	
#define  TRNSFER_LEN         1024
/* Private define ------------------------------------------------------------*/

/* DMA descriptors must be aligned to 16 bytes */
#if defined(__CC_ARM)
/* Keil alignment to 1024 bytes */
__align(1024) static RSI_GPDMA_DESC_T GPDMADesc[NUMGPDMADESC];
#endif /* defined (__CC_ARM) */

/* IAR support */
#if defined(__ICCARM__)
#pragma data_alignment=1024
static RSI_GPDMA_DESC_T GPDMADesc[NUMGPDMADESC];
#endif /* defined (__ICCARM__) */

#if defined( __GNUC__ )
__align(1024) static RSI_GPDMA_DESC_T GPDMADesc[NUMGPDMADESC];
#endif /* defined (__GNUC__) */

/* Private variables ---------------------------------------------------------*/
static uint32_t memBuff[30];
uint32_t src[TRNSFER_LEN];
uint32_t dst[TRNSFER_LEN];
uint32_t ret =0;
volatile uint8_t dmaDone=0;
RSI_GPDMA_HANDLE_T GPDMAHandle;
/* Private function prototypes -----------------------------------------------*/
 
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  GPDMA Interrupt Handler
 * @param  None
 * @return None
 */
void GPDMA_IRQHandler(void)
{
	RSI_GPDMA_InterruptHandler(GPDMAHandle);
}

/**
 * @brief   GPDMA controller transfer descriptor chain complete callback
 * @param[in]   GPDMAHandle structure variable to driver context handle
 * @param[in]   pTranDesc   Pointer to transfer descriptor
 * @return  None
 */
void GPDMATransferComplete(RSI_GPDMA_HANDLE_T GPDMAHandle, RSI_GPDMA_DESC_T *pTranDesc)
{}

/**
 * @brief   GPDMA controller transfer descriptor complete callback
 * @param[in]   GPDMAHandle structure variable to driver context handle
 * @param[in]   pTranDesc   Pointer to transfer descriptor
 * @return  None
 */
void GPDMATransferDescComplete(RSI_GPDMA_HANDLE_T GPDMAHandle, RSI_GPDMA_DESC_T *pTranDesc)
{}

/**
 * @brief   GPDMA controller transfer descriptor error callback
 * @param[in]   GPDMAHandle structure variable to driver context handle
 * @param[in]   pTranDesc   Pointer to transfer descriptor
 * @return  None
 */
void GPDMATransferError(RSI_GPDMA_HANDLE_T GPDMAHandle, RSI_GPDMA_DESC_T *pTranDesc)
{}

/**
 * @brief   DMA controller transfer descriptor error callback
 * @param[in]   GPDMAHandle structure variable to driver context handle
 * @param[in]   pTranDesc   Pointer to transfer descriptor
 * @return  None
 */
void GPDMATransferDescFetchComplete(RSI_GPDMA_HANDLE_T GPDMAHandle, RSI_GPDMA_DESC_T *pTranDesc)
{}

void SetupChannelDesc()
{
	uint32_t j;
	RSI_GPDMA_DESC_T XferCfg;
	RSI_GPDMA_DESC_T *pPrevDesc;
	RSI_GPDMA_CHA_CFG_T chaCfg;
	
	chaCfg.channelPrio = 3;
	chaCfg.descFetchDoneIntr = 0;
	chaCfg.hrespErr = 1;
	chaCfg.gpdmacErr = 1;
	chaCfg.xferDoneIntr = 1;
  chaCfg.dmaCh = 0;
	
	/* Setup channel 0 for trigger operation and M2M transfer */
	if (RSI_GPDMA_SetupChannel(GPDMAHandle, (RSI_GPDMA_CHA_CFG_T *) &chaCfg) != RSI_OK) 
	{
		DEBUGOUT("Error setting up channel\r\n");
	}

		/* Channel Control Config */
    XferCfg.chnlCtrlConfig.transSize = XFERSIZE/NUMGPDMADESC;
    XferCfg.chnlCtrlConfig.transType = MEMORY_MEMORY;
    XferCfg.chnlCtrlConfig.dmaFlwCtrl = DMA_FLW_CTRL;
    XferCfg.chnlCtrlConfig.mastrIfFetchSel = 1;
    XferCfg.chnlCtrlConfig.mastrIfSendSel = 0;
    XferCfg.chnlCtrlConfig.destDataWidth = DST_32_DATA_WIDTH;
    XferCfg.chnlCtrlConfig.srcDataWidth = SRC_32_DATA_WIDTH;
    XferCfg.chnlCtrlConfig.srcAlign = 0;
    XferCfg.chnlCtrlConfig.linkListOn = 1;
    XferCfg.chnlCtrlConfig.linkListMstrSel = 0;
    XferCfg.chnlCtrlConfig.srcAddContiguous = 1;
    XferCfg.chnlCtrlConfig.dstAddContiguous = 1;
    XferCfg.chnlCtrlConfig.retryOnErr = 0;
    XferCfg.chnlCtrlConfig.linkInterrupt = 1;
    XferCfg.chnlCtrlConfig.srcFifoMode = 0;
    XferCfg.chnlCtrlConfig.dstFifoMode = 0;
	
		/* Misc Channel Config */
		XferCfg.miscChnlCtrlConfig.ahbBurstSize = AHBBURST_SIZE_16;
		XferCfg.miscChnlCtrlConfig.destDataBurst = DST_BURST_SIZE_16;
		XferCfg.miscChnlCtrlConfig.srcDataBurst = DST_BURST_SIZE_16;
		XferCfg.miscChnlCtrlConfig.destChannelId = 0;
		XferCfg.miscChnlCtrlConfig.srcChannelId = 0;
		XferCfg.miscChnlCtrlConfig.dmaProt = 0;
		XferCfg.miscChnlCtrlConfig.memoryFillEn = 0;
		XferCfg.miscChnlCtrlConfig.memoryOneFill = 0;

	  pPrevDesc = NULL;
	for(j = 0; j < NUMGPDMADESC; j++)
  {		
    XferCfg.src  = &src[(XFERSIZE/NUMGPDMADESC) * j];  
    XferCfg.dest = &dst[(XFERSIZE/NUMGPDMADESC) * j]; 
		
    if(RSI_GPDMA_BuildDescriptors(GPDMAHandle,&XferCfg,&GPDMADesc[j],pPrevDesc)!= RSI_OK)
    { 
      DEBUGOUT("Error building descriptor chain (single link)\r\n");
    }  
    pPrevDesc = &GPDMADesc[j];
	}			
	RSI_GPDMA_SetupChannelTransfer( GPDMAHandle,GPDMA_CHNL0, GPDMADesc);
}

int main()
{
	/* At this stage the MICROCONTROLLER clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */
  
	uint32_t memSize, *devMem;
	uint32_t i;
	RSI_GPDMA_INIT_T GPDMAInit;
	volatile bool done;
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	dmaDone = false;

	/* Initialize UART for debug prints*/
	DEBUGINIT();
	
	/*  Prints on hyper-terminal  */
  DEBUGOUT("GPDMA Memory to Memory data transfer example\r\n");
  
	/* clear stack structures before use */
	memset(&GPDMAInit, 0, sizeof(RSI_GPDMA_INIT_T));

	/* Get needed size for driver context memory */
	memSize = RSI_GPDMA_GetMemSize();
	if (memSize > sizeof(memBuff)) 
  {
		/*  Prints on hyper-terminal  */
		DEBUGOUT("Can't allocate memory for driver context\r\n");
	}
	devMem = memBuff;	/* Or just use malloc(memSize) */

	/* Initialize driver */
	GPDMAInit.pUserData = (void *) &done;
	GPDMAInit.baseC = (uint32_t) GPDMA_C;
  GPDMAInit.baseG = (uint32_t) GPDMA_G;
	GPDMAInit.sramBase = (uint32_t) &GPDMADesc;
 
	/* Initialize driver context parameters*/
	GPDMAHandle = RSI_GPDMA_Init(devMem, &GPDMAInit);
	if (GPDMAHandle == NULL) 
  {
		/*  Prints on hyper-terminal  */
		DEBUGOUT("Error initializing GPDMA\r\n");
	}
  /* GPDMA clock enable */
	RSI_CLK_PeripheralClkEnable(M4CLK,RPDMA_CLK,ENABLE_STATIC_CLK);
	
	/* Enable the interrupt for the DMA controller */
	NVIC_EnableIRQ(GPDMA_IRQn);

	/* Register error, descriptor completion, and descriptor chain completion callbacks for channel 0 */
	RSI_GPDMA_RegisterCallback(GPDMAHandle, RSI_GPDMA_XFERCOMPLETE_CB, (void *) GPDMATransferComplete);
	RSI_GPDMA_RegisterCallback(GPDMAHandle, RSI_GPDMA_XFERDESCFETCHCOMPLETE_CB, (void *) GPDMATransferDescFetchComplete);
	RSI_GPDMA_RegisterCallback(GPDMAHandle, RSI_GPDMA_XFERHRESPERROR_CB, (void *) GPDMATransferError);
  RSI_GPDMA_RegisterCallback(GPDMAHandle, RSI_GPDMA_XFERGPDMACERROR_CB, (void *) GPDMATransferError);
 
	/* Populate some data to copy */
	for (i = 0; i < TRNSFER_LEN; i++) 
	{
		src[i] = i + 1;
	}	
  /* Setup descriptor */ 
	SetupChannelDesc();
  
  /* Trigger channel */
	RSI_GPDMA_DMAChannelTrigger( GPDMAHandle,GPDMA_CHNL0);
	
	/* Wait for DMA to fire indicating completion */
	__WFI();
    
  /* Compare source and destination buffers */
	ret = memcmp(src, dst, XFERSIZE);
  
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
	while(1);
}
