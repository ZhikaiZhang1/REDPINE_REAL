/**
 * @file       rsi_gspi.c
 * @version    0.1
 * @date       20 dec 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes releated to gspi peripheral
 *
 * @section Description
 * This file contains the list of function prototypes for the gspi and low level function definations
 * Following are list of API's which need to be defined in this file.
 *
 */

#include "rsi_chip.h"

typedef uint16_t (*spiMasterFifoPushFN)(GSPI0_Type *pSPI, void *data, uint32_t sendBytes, uint32_t curIndex);
typedef uint16_t (*spiMasterFifoPopFN)(GSPI0_Type *pSPI, void *data, uint32_t bytes, uint32_t curIndex);


/* Private data structure used for the SPI master driver, holds the driver and
   peripheral context */
typedef struct {
	GSPI0_Type                  *base;				/*!< Base address of SPI peripheral to use */
	uint32_t                    baseClockRate;		/*!< SPI base clock rate in Hz, call Init() again if this rate changes */
	spiMasterXferCSAssertCB     pAssertCb;			/*!< SSEL assertion callback */
	spiMasterTransmitCB         pTranCb;			/*!< Transmit data start callback */
	spiMasterReceiveCB          pRecvCb;			/*!< Receive data start callback */
	spiMMasterXferCSDeAssertCB  pDeassertCb;		/*!< SSEL deassertion callback */
	spiMasterFifoPushFN         pPushFN;			/*!< Pointer to current FIFO push function */
	spiMasterFifoPopFN          pPopFN;				/*!< Pointer to current FIFO pop function */
	GSPI_XFER_T                *pXfer;				/*!< Pointer to current transfer descriptor */
	GSPI_INIT_T                *pConfig;
	GSPI_XFER_CONFIG_T         *pXferConfig;
	uint32_t                    sendIdx;			/*!< Current transmit buffer index */
	uint32_t                    recvIdx;			/*!< Current receive buffer index */
	uint8_t                     pendingStatus;		/*!< Pending transfer status */
	uint8_t                     xmitOn;				/*!< Transfer in progress flag */
	uint8_t                     terminate;			/*!< Terminate transfer flag */
	uint8_t                     reserved[2];
} SPIM_DATACONTEXTMEMORY_T;



/**
* @fn          error_t RSI_Clock_EnablePeriphClock(GSPI_HANDLE_T pHandle,uint8_t fulltime_clk_en)
* @brief	   	 This API is used to enable the GSPI clock with clock configuration modes .
* @param[in]   pHandle : handle of GSPI master.
* @param[in]   fulltime_clk_en : To enable and disable clock 
*                 - 1: Full time clock enable 
*                 - 0: Disable Dynamic clock gating
* @return        returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG
*/
error_t RSI_Clock_EnablePeriphClock(GSPI_HANDLE_T pHandle,uint8_t fulltime_clk_en)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;


	if(fulltime_clk_en < 2)
	{
			/* Full time clock is enabled for GSPI controller. */
			pDrv->base->GSPI_CLK_CONFIG_b.GSPI_CLK_EN = fulltime_clk_en;

	return RSI_OK;
	}	
	else
	{
		return ERROR_GSPI_INVALID_ARG;
	}	
}     

/**
 * @fn           void RSI_GSPI_CsAssert(GSPI_HANDLE_T pHandle , uint8_t  csNum)
 * @brief	       This API is used to assert the GSPI peripheral with chip select signal.
 * @param[in]    pHandle : handle of GSPI master.
 * @param[in]    csNum   : chip select no.(0..4)
 * @return	     none
*/
void RSI_GSPI_CsAssert(GSPI_HANDLE_T pHandle , uint8_t  csNum)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	/* Assert selected CS line */
	pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_CSN_SELECT=csNum;
	
	pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_CSN=1;
	
	while(!(pDrv->base->GSPI_STATUS & GSPI_MAN_CSN)) ;
}

/**
 * @fn           void RSI_GSPI_CsDeAssert(GSPI_HANDLE_T pHandle,uint8_t  csNum)
 * @brief	       This API is used to Deassert the GSPI peripheral with chip select signal.
 * @param[in]    pHandle : handle of GSPI master.
 * @param[in]    csNum   : chip select no.(0..4)
 * @return	     none
 */
void RSI_GSPI_CsDeAssert(GSPI_HANDLE_T pHandle,uint8_t  csNum)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
	/* DeAssert selected CS line */
	pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_CSN_SELECT=csNum;
	
	pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_CSN=0;
	
	while((pDrv->base->GSPI_STATUS & GSPI_MAN_CSN)) ;
	
}


/**
 * @fn           uint32_t RSI_GSPI_GetStatus(GSPI_HANDLE_T pHandle);
 * @brief	       This API is used to get the status of the GSPI controller.
 * @param[in]    pHandle  : handle of GSPI master.
 * @return	     GSPI Status bit flags.
 */
uint32_t RSI_GSPI_GetStatus(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	return pDrv->base->GSPI_STATUS;
}

/**
 * @fn           error_t RSI_GSPI_EnableInts(GSPI_HANDLE_T pHandle, uint8_t intMask)
 * @brief	       This API is used to enable the Interrupts of the GSPI Controller.
 * @param[in]    pHandle  : handle of GSPI master.
 * @param[in]    intMask  : Interrupt mask bit flag to be passed.
 * @return        returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG
 */
 error_t RSI_GSPI_EnableInts(GSPI_HANDLE_T pHandle, uint8_t intMask)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if(intMask <=0x7F)
	{
		pDrv->base->GSPI_INTR_UNMASK =intMask;
		return RSI_OK;
	}
	else
	{
		return ERROR_GSPI_INVALID_ARG;
	}
}


/**
 * @fn           error_t RSI_GSPI_DisableInts(GSPI_HANDLE_T pHandle, uint8_t intMask)
 * @brief	       This API is used to disable the Interrupts of the GSPI Controller.
 * @param[in]    pHandle  : handle of GSPI master.
 * @param[in]    intMask  : Interrupt mask bit flag to be passed.
 * @return        returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG
 */
error_t RSI_GSPI_DisableInts(GSPI_HANDLE_T pHandle, uint8_t intMask)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if(intMask <=0x7F)
	{
		pDrv->base->GSPI_INTR_MASK =  intMask;
		return RSI_OK;
	}
	else
	{
		return ERROR_GSPI_INVALID_ARG;
	}

}

/**
 * @fn           uint8_t RSI_GSPI_GetEnabledInts(GSPI_HANDLE_T pHandle);
 * @brief	       This API is used to get the enabled GSPI controller interrupts.
 * @param[in]    pHandle : handle of GSPI master.
 * @return	     Interrupts values to be receive from GSPI Interrupt Register.  	 
 */
uint8_t RSI_GSPI_GetEnabledInts(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
	return pDrv->base->GSPI_INTR_UNMASK;
}

/**
 * @fn           void RSI_GSPI_FlushFifos(GSPI_HANDLE_T pHandle)
 * @brief	       This API is used to Reset the GSPI FIFO Reg.
 * @param[in]    pHandle : handle of GSPI master.
 * @return	     none.	 
 */
void RSI_GSPI_FlushFifos(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
	
        pDrv->base->GSPI_FIFO_THRLD |= (1 << 8)|(1 << 9);
	      pDrv->base->GSPI_FIFO_THRLD &= 0xFF;
	
}

/**
 * @fn           uint32_t RSI_GSPI_GetMemSize().
 * @brief	       This API is used for Get needed size for driver context memory.
 * @return	      return size of data context memory.  
 */
uint32_t RSI_GSPI_GetMemSize()
{
	return sizeof(SPIM_DATACONTEXTMEMORY_T);
}

/* FIFO push function using standard SPI FIFO for datum >8 bits */
static uint16_t RSI_GSPI_FifoPush16(GSPI_HANDLE_T pHandle, void *data, uint8_t numData, uint32_t curIndex)
{
	uint16_t pushed = 0, *p16 = (uint16_t *) data;
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	/* Push as 16-bit value */
	while ((numData > 0) && (pDrv->base->GSPI_STATUS_b.GSPI_BUSY != 1))
	{
		pDrv->base->GSPI_WRITE_DATA2_b.USE_PREV_LENGTH=1U;
		pDrv->base->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2=pDrv->pXferConfig->dataBits;
		pDrv->base->GSPI_WRITE_FIFO_b[0].WRITE_FIFO = (uint32_t) p16[curIndex];
		/* Trigger Write Enable of GSPI Controller*/
		pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_WR=1;
		/* wait till GSPI becomes idle or until transmit done             */
		while(pDrv->base->GSPI_STATUS & GSPI_BUSY_F);
		numData--;
		curIndex++;
		pushed++;
	}
	return pushed;
}

/* FIFO pop function using standard SPI FIFO for datum >8 bits */
static uint16_t RSI_GSPI_FifoPop16(GSPI_HANDLE_T pHandle, void *data, uint32_t numData, uint32_t curIndex)
{
	uint16_t popped = 0, *p16 = (uint16_t *) data;
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
	
	if(((numData > 0) && (pDrv->base->GSPI_STATUS_b.GSPI_BUSY != 1)))
	{
		/* Read 8bits of data*/
		pDrv->base->GSPI_CONFIG2_b.GSPI_MANUAL_SIZE_FRM_REG =1;
		/* Trigger the Read of GSPI Controller*/
		pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_RD=1;
		/* wait till GSPI becomes idle  or read has been done           */
		if(!(pDrv->base->GSPI_STATUS & GSPI_FIFO_EMPTY_RFIFO_S))
	  {
		  /* Read the data from FIFO*/
		  p16[curIndex] = pDrv->base->GSPI_READ_FIFO_b[0].READ_FIFO;
	  }
		numData--;
		curIndex++;
		popped++;
}	
	return popped;
}

/* FIFO push function using standard SPI FIFO for datum <=8 bits */
static uint16_t RSI_GSPI_FifoPush8(GSPI_HANDLE_T pHandle, void *data, uint32_t numData, uint32_t curIndex)
{

	uint16_t pushed = 0;
	uint8_t *p8 = (uint8_t *) data;

	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	/* Push as 16-bit value */
	while ((numData > 0) && (pDrv->base->GSPI_STATUS_b.GSPI_BUSY != 1))
	{
		pDrv->base->GSPI_WRITE_DATA2_b.USE_PREV_LENGTH=1U;
		pDrv->base->GSPI_CONFIG2_b.TAKE_GSPI_MANUAL_WR_SIZE_FRM_REG=0;
		pDrv->base->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2 =pDrv->pXferConfig->dataBits;
		pDrv->base->GSPI_WRITE_FIFO_b[0].WRITE_FIFO = (uint32_t) p8[curIndex];
		/* Trigger Write Enable of GSPI Controller*/
		pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_WR=1;

		/* wait till GSPI becomes idle or until transmit done             */
		while(pDrv->base->GSPI_STATUS & GSPI_BUSY_F);
		numData--;
		curIndex++;
		pushed++;
	}
	return pushed;
}	

/* FIFO pop function using standard SPI FIFO for datum <=8 bits */
static uint16_t RSI_GSPI_FifoPop8(GSPI_HANDLE_T pHandle, void *data, uint32_t numData, uint32_t curIndex)
{	

	uint16_t popped = 0;
	uint8_t *p8 = (uint8_t *) data;
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if(((numData > 0) && (pDrv->base->GSPI_STATUS_b.GSPI_BUSY != 1)))
	{
		/* Read 8bits of data*/
		pDrv->base->GSPI_CONFIG2_b.GSPI_MANUAL_SIZE_FRM_REG =0;
		/* Trigger the Read of GSPI Controller*/
				pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_RD=1;		/* wait till GSPI becomes idle  or read has been done           */
		if(!(pDrv->base->GSPI_STATUS & GSPI_FIFO_EMPTY_RFIFO_S))
		{
		 /* Read the data from FIFO*/
		 p8[curIndex] = pDrv->base->GSPI_READ_FIFO_b[0].READ_FIFO;	
		}
		numData--;
		curIndex++;
		popped++;
	 }
	return popped;
}

/**
 * @fn           void RSI_GSPI_Close_PendingTransfer(GSPI_HANDLE_T pHandle)
 * @brief	       This API is used to terminate the transfer of sending last byte.
 * @param[in]    pHandle  : handle of GSPI master.
 * @return	     none. 
 */
void RSI_GSPI_Close_PendingTransfer(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
	/* Transfer terminates after this byte */
	pDrv->xmitOn = 0;
	pDrv->terminate = 0;

}

/**
 * @fn           void RSI_GSPI_TransmitHandler(GSPI_HANDLE_T pHandle, GSPI_XFER_T *pXfer)
 * @brief	       This API is used for transmit handle.
 * @param[in]    pHandle : handle of GSPI master.
 * @param[in]    pXfer   : Pointer to the Transfer instance structure area.
 * @return	     none.
 */
void RSI_GSPI_TransmitHandler(GSPI_HANDLE_T pHandle, GSPI_XFER_T *pXfer)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
				
	/*Sends the User data*/
	if (pDrv->sendIdx < pXfer->txSz) {
	pDrv->sendIdx += pDrv->pPushFN(pHandle, (void *) pXfer->txBuff, 1, pDrv->sendIdx);
	}
	/*Check for the transfer complete */
	if (pDrv->sendIdx >= pXfer->txSz) {
		if (pDrv->pTranCb) {
			/*Close the transfer by asserting the chip select*/
			/*Call user call back*/
			pDrv->pTranCb(pHandle, pXfer);
			/*Close the pending transfer by setting the required bits */
			RSI_GSPI_Close_PendingTransfer(pHandle);			
		RSI_GSPI_CsAssert(pHandle,pDrv->pXfer->sselNum);
		pDrv->pAssertCb(pHandle,pXfer);
		pDrv->pXfer->status =  ERROR_GSPI_TX_DONE;
		}
	}
	return;
}

/**
 * @fn           void RSI_GSPI_ReceiveHandler(GSPI_HANDLE_T pHandle, GSPI_XFER_T *pXfer)
 * @brief	       This API is used for Receive handle.
 * @param[in]    pHandle : handle of GSPI master.
 * @param[in]    pXfer   : Pointer to the Transfer instance structure area.
 * @return	     none.
 */
void RSI_GSPI_ReceiveHandler(GSPI_HANDLE_T pHandle, GSPI_XFER_T *pXfer)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if (pDrv->recvIdx >= pXfer->rxSz) {
		if ((pDrv->recvIdx >= pXfer->rxSz) && (pDrv->pRecvCb)) {
			pDrv->pRecvCb(pHandle, pXfer);
			pDrv->pXfer->status =  ERROR_GSPI_READ_DONE;
		}
		
	}
	else {
		pDrv->recvIdx += pDrv->pPopFN(pHandle, (void *) pXfer->rxBuff, 1, pDrv->recvIdx);

		/* Call callback for more data */		
	}
	return ;
}

/**
 * @fn           error_t RSI_GSPI_SetWriteSwapData(GSPI_HANDLE_T pHandle)
 * @brief	       This API is used for swap the send data bits.
 * @param[in]    pHandle  : handle of GSPI master.
 * @return	     returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG.
 */
error_t RSI_GSPI_SetWriteSwapData(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if(pDrv->pXferConfig->sselNum <4)
	{
		/*Set bit */
		pDrv->base->GSPI_CONFIG2 |=  (1 << (pDrv->pXferConfig->sselNum));
		return RSI_OK;
	}
	else
	{
		return ERROR_GSPI_INVALID_ARG;
	}

}

/**
 * @fn           error_t RSI_GSPI_ClrWriteSwapData(GSPI_HANDLE_T pHandle)
 * @brief	       This API is used to clear the write swapping data.
 * @param[in]    pHandle  : handle of GSPI master.
 * @return	     returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG.     
 */
error_t RSI_GSPI_ClrWriteSwapData(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if(pDrv->pXferConfig->sselNum <4)
	{
		/*clear bit */
		pDrv->base->GSPI_CONFIG2 &=  ~(1 << (pDrv->pXferConfig->sselNum));
		return RSI_OK;
	}
	else
	{
		return ERROR_GSPI_INVALID_ARG;
	}
}

/**
 * @fn           error_t RSI_GSPI_SetReadSwapData(GSPI_HANDLE_T pHandle)
 * @brief	       This API is used for swap the read data bits.
 * @param[in]    pHandle   : handle of GSPI master.
 * @return	     returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG.
 */
error_t RSI_GSPI_SetReadSwapData(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if(pDrv->pXferConfig->sselNum <4)
	{
		/*Set bit */
		pDrv->base->GSPI_CONFIG2 |=  (1 << (pDrv->pXferConfig->sselNum +4));
		return RSI_OK;
	}
	else
	{
		return ERROR_GSPI_INVALID_ARG;
	}

}

/**
 * @fn           error_t RSI_GSPI_ClrReadSwapData(GSPI_HANDLE_T pHandle)
 * @brief	       This API is used to clear the read swapping data.
 * @param[in]    pHandle  : handle of GSPI master.
 * @return	     returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG. 
 */
error_t RSI_GSPI_ClrReadSwapData(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if(pDrv->pXferConfig->sselNum <4)
	{
		/*clear bit */
		pDrv->base->GSPI_CONFIG2 &=  ~(1 << (pDrv->pXferConfig->sselNum +4));
		return RSI_OK;
	}
	else
	{
		return ERROR_GSPI_INVALID_ARG;
	}
}

/**
 * @fn           void RSI_GSPI_SetClockRate(GSPI_HANDLE_T pHandle)
 * @brief	       It enables the GSPI clock with clock configuration modes and division factors.
 * @param[in]    pHandle  : handle of GSPI master.
 * @return	     none.
 */
void RSI_GSPI_SetClockRate(GSPI_HANDLE_T pHandle)
{
	uint32_t clk_div_factor =0,clk_div_modulo = 0 ;

	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	clk_div_factor =  (pDrv->pConfig->baseClockRate /pDrv->pXferConfig->dXferBitRate);

	clk_div_modulo  = (clk_div_factor % 2);

	/*Check for fractional clock rate and update the user with differ baud rate */
	if(clk_div_modulo == 0)
	{
		/*Differed clock rate */
		pDrv->pXferConfig->rXferBitRate = pDrv->pXferConfig->dXferBitRate;
	}
	else
	{
		/*Differed clock rate */
		pDrv->pXferConfig->rXferBitRate = ((pDrv->pConfig->baseClockRate / clk_div_factor) * 1000000);
	}
	//NOTE : division factor is multiples of 2
	clk_div_factor  = (clk_div_factor / 2);
	/*Update the clock rate to hardware */
	pDrv->base->GSPI_CLK_DIV_b.GSPI_CLK_DIV_FACTOR = clk_div_factor;

	return ;
}

/**
 * @fn           GSPI_HANDLE_T RSI_GSPI_Init(void *mem, const GSPI_INIT_T *pInit);
 * @brief	       This API is used to initialize the GSPI Controller.
 * @param[in]    mem    : pointer to the memory 
 * @param[in]    pInit  : pointer to the GSPI master Initialization.
 * @return	     \ref  GSPI_HANDLE_T ,return NULL on error     
 */
GSPI_HANDLE_T RSI_GSPI_Init(void *mem, const GSPI_INIT_T *pInit)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv;

	/* Verify alignment is at least 4 bytes and clock rate is not 0 */
	if ((((uint32_t) mem & 0x3) != 0) || (pInit->baseClockRate == 0)) {
		return NULL;
	}
	pDrv = (SPIM_DATACONTEXTMEMORY_T *) mem;

	/*save the SPI configuration */
	pDrv->pConfig = (GSPI_INIT_T*) pInit;
	pDrv->base = (GSPI0_Type *) pInit->base;
	pDrv->baseClockRate = pInit->baseClockRate;

	/* Enable the high performance Features of SPI Controller*/
	pDrv->base->GSPI_BUS_MODE_b.SPI_HIGH_PERFORMANCE_EN=0x1;

	/*SPI MODE Configuration*/
	/*SPI MODE 3 / MODE 0 configuration */
	/*Update the SPI clock sync */
	pDrv->base->GSPI_CLK_CONFIG_b.GSPI_CLK_SYNC = pDrv->pConfig->clockSync;
	
		/* SPI sample negedge or posedge*/
	pDrv->base->GSPI_BUS_MODE_b.GSPI_DATA_SAMPLE_EDGE=pDrv->pConfig->dataSampleEdge;

		pDrv->base->GSPI_BUS_MODE_b.GSPI_CLK_MODE_CSN0=pDrv->pConfig->spiMode;
		pDrv->base->GSPI_BUS_MODE_b.GSPI_CLK_MODE_CSN1=pDrv->pConfig->spiMode;
		pDrv->base->GSPI_BUS_MODE_b.GSPI_CLK_MODE_CSN2=pDrv->pConfig->spiMode;
		pDrv->base->GSPI_BUS_MODE_b.GSPI_CLK_MODE_CSN3=pDrv->pConfig->spiMode;
		/*Enable Full duplex mode */
		pDrv->base->GSPI_CONFIG1_b.SPI_FULL_DUPLEX_EN = pDrv->pConfig->fullDuplexMode;

	/*Configure the FIFO thresholds*/
	pDrv->base->GSPI_FIFO_THRLD_b.FIFO_AEMPTY_THRLD = pDrv->pConfig->fifoAEmpty;
	pDrv->base->GSPI_FIFO_THRLD_b.FIFO_AFULL_THRLD = pDrv->pConfig->fifoAfull;

	/*return the data context memory */
	return (GSPI_HANDLE_T	) pDrv;
}

/**
 * @fn           void RSI_GSPI_RegisterCallback(GSPI_HANDLE_T pHandle, uint32_t cbIndex, void *pCB);
 * @brief	       This API is used for control register call backs.
 * @param[in]    pHandle  : handle of GSPI master.
 * @param[in]    cbIndex  : Register callback index.
 * @param[in]    pCB: pointer to the callback. 
 * @return	     none. 
 */
void RSI_GSPI_RegisterCallback(GSPI_HANDLE_T pHandle, uint32_t cbIndex, void *pCB)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	switch (cbIndex) {
	case GSPIM_ASSERTSSEL_CB:
		pDrv->pAssertCb = (spiMasterXferCSAssertCB) pCB;
		break;

	case GSPIM_DATATRANSMIT_CB:
		pDrv->pTranCb = (spiMasterTransmitCB) pCB;
		break;

	case GSPIM_DATATRECEIVE_CB:
		pDrv->pRecvCb = (spiMasterReceiveCB) pCB;
		break;

	case GSPIM_DEASSERTSSEL_CB:
		pDrv->pDeassertCb = (spiMMasterXferCSDeAssertCB) pCB;
		break;
	}
}

/**
 * @fn           error_t RSI_GSPI_SetUpTransfer(GSPI_HANDLE_T pHandle, GSPI_XFER_CONFIG_T *pCfg)
 * @brief	       This API is used for setup the Transfer.
 * @param[in]    pHandle  : handle of GSPI master.
 * @param[in]    pCfg     : Pointer to the Transfer configuration.
 * @return	    returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG.
 */
error_t RSI_GSPI_SetUpTransfer(GSPI_HANDLE_T pHandle, GSPI_XFER_CONFIG_T *pCfg)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	if((pHandle == NULL) || (pCfg == NULL))
	{
		return ERROR_GSPI_INVALID_ARG;
	}

	pDrv->pXferConfig = pCfg;

	/* Verify data parameters area valid */
	if ((pCfg->dataBits == 0) || (pCfg->dataBits > 16) || (pCfg->dXferBitRate == 0)) {
		return ERROR_GSPI_INVALID_ARG;
	}

		RSI_Clock_EnablePeriphClock(pHandle, pDrv->pConfig->fullTimeClkEn);
	/*Set SPI clock rate */
	RSI_GSPI_SetClockRate(pHandle);
// Use Separate api,s for read and write swapData. Name change to Write swap
	/*Byte swapping configuration*/
	if(pDrv->pXferConfig->writeSwap)
	{
		/*Enable the byte swapping */
		RSI_GSPI_SetWriteSwapData(pHandle);
	}
	else
	{

		/*Disable the byte swapping */
		RSI_GSPI_ClrWriteSwapData(pHandle);
	}
		if(pDrv->pXferConfig->readSwap)
	{
		/*Enable the byte swapping */
		RSI_GSPI_SetReadSwapData(pHandle);
	}
	else
	{

		/*Disable the byte swapping */
		RSI_GSPI_ClrReadSwapData(pHandle);
	}

if(pDrv->pXferConfig->rdDataLen < (2*1024)){
	pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_RD_CNT = pDrv->pXferConfig->rdDataLen;
}else{
  	pDrv->base->GSPI_CONFIG3_b.SPI_MANUAL_RD_LNTH_TO_BC=pDrv->pXferConfig->rdDataLen;
}
	/* Setup FIFO call backs based on the data transfer width */
	if (pCfg->dataBits > 8) {
		pDrv->pPushFN = (spiMasterFifoPushFN)&RSI_GSPI_FifoPush16;
		pDrv->pPopFN = (spiMasterFifoPopFN)&RSI_GSPI_FifoPop16;
	}
	else {
		pDrv->pPushFN = (spiMasterFifoPushFN)&RSI_GSPI_FifoPush8;
		pDrv->pPopFN = (spiMasterFifoPopFN)&RSI_GSPI_FifoPop8;
	}
	return RSI_OK;
}

/**
 * @fn           error_t RSI_GSPI_Transfer(GSPI_HANDLE_T pHandle, GSPI_XFER_T *pXfer)
 * @brief	       This API is used for transfer the data.
 * @param[in]    pHandle : handle of GSPI master.
 * @param[in]    pXfer   : Pointer to the Transfer structure instance area.
 * @return	 returns 0 \ref RSI_OK on success,return error code on  failure. \ref ERROR_GSPI_INVALID_ARG.
 */
error_t RSI_GSPI_Transfer(GSPI_HANDLE_T pHandle, GSPI_XFER_T *pXfer)
{

	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
	uint32_t temp=0x12;

	if((pHandle == NULL) || (pXfer == NULL))
	{
		return ERROR_GSPI_INVALID_ARG;
	}

	/* Is slave select  valid? */
	if (pXfer->sselNum >= GSPIM_MAXSELECTS) {
		pXfer->status = ERROR_GSPI_INVALID_ARG;
		return ERROR_GSPI_INVALID_ARG;
	}

	pDrv->pXfer = pXfer;
	pXfer->status = ERROR_GSPI_BUSY;
	pDrv->pendingStatus = RSI_OK;
	pDrv->sendIdx = pDrv->recvIdx = 0;

	/* Transmit start, no terminate */
	pDrv->xmitOn = 1;
	pDrv->terminate = 0;
	
  //Deassert the chipselect signal.
	RSI_GSPI_CsDeAssert(pHandle,pDrv->pXferConfig->sselNum);
	if(pXfer->flags & GSPI_FLAG_DMATX){

		pDrv->base->GSPI_WRITE_DATA2_b.USE_PREV_LENGTH=1U;
		// Enable manual write of GSPI Controller
		pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_WR=1;		
			/*Call DMA call back tx callback*/
		pDrv->pTranCb(pHandle , pXfer);		

	}

	if(pXfer->flags & GSPI_FLAG_DMARX){
		/*DMA receive setup*/
	//Enable Manual read for GSPI Controller
				pDrv->base->GSPI_CONFIG1_b.GSPI_MANUAL_RD=1;
		/*DMA receive setup*/
		/* wait till GSPI becomes idle  or read has been done*/
		pDrv->pRecvCb(pHandle , pXfer);
	}

	/*Blocking mode transfer enabled */
	if(pXfer->flags & GSPI_FLAG_BLOCKING)
	{
		do{
			/*Call transfer handler in loop if blocking mode is enabled*/
			RSI_GSPI_TransferHandler(pHandle);
			/*Wait till SPI is IDLE*/
		}while(pXfer->status == ERROR_GSPI_BUSY);
	}
	else if(pXfer->flags & GSPI_FLAG_NONBLOCKING) 
	{
		pDrv->base->GSPI_WRITE_DATA2 =pDrv->pXferConfig->dataBits ;
		pDrv->base->GSPI_WRITE_FIFO[0] =temp;
		/*Enable the TX and RX interrupts */
		//just returns the function and transfer is handled by ISR.
	}
		/*Blocking mode transfer enabled */
	if(pXfer->flags & GSPI_FLAG_TXIGNORE)
	{
		do{
			/*Call transfer handler in loop if blocking mode is enabled*/
			RSI_GSPI_ReceiveHandler(pHandle, pXfer);
			/*Wait till SPI is IDLE*/
		}while(pXfer->status == ERROR_GSPI_BUSY);
	}
		/*Blocking mode transfer enabled */
	if(pXfer->flags & GSPI_FLAG_RXIGNORE)
	{
		do{
			/*Call transfer handler in loop if blocking mode is enabled*/
			RSI_GSPI_TransmitHandler(pHandle, pXfer);
			/*Wait till SPI is IDLE*/
		}while(pXfer->status == ERROR_GSPI_BUSY);
	}
	return RSI_OK;
}

/**
 * @fn           void RSI_GSPI_TransferHandler(GSPI_HANDLE_T pHandle);
 * @brief	       This API handle the Transfer.
 * @param[in]    pHandle : handle of GSPI master.
 * @return	     none.
 */
void RSI_GSPI_TransferHandler(GSPI_HANDLE_T pHandle)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
	GSPI_XFER_T *pXfer = pDrv->pXfer;
	uint32_t status = pDrv->base->GSPI_STATUS;

	/*check for data */
	if(pDrv->base->GSPI_STATUS_b.GSPI_BUSY != 1){
		pDrv->base->GSPI_WRITE_DATA2 =pDrv->pXferConfig->dataBits ;

		if(pDrv->sendIdx >= pDrv->pXfer->txSz){
			/*De-assert the chip select */
			//RSI_GSPI_CsDeAssert(pHandle , pDrv->pXfer->sselNum);
			pDrv->pDeassertCb(pHandle, pDrv->pXfer);
		}
	}
	/* Transmit data handler */
	if (((status & GSPI_FIFO_AFULL_WFIFO_S) != 1) && (pDrv->xmitOn == 1)) {
		if ((pXfer->txSz != 0) || (pXfer->txBuff != NULL)){
			/* Handle transmit */
			RSI_GSPI_TransmitHandler(pHandle, pXfer);
		}
	}

	/* Receive data handler */
	if ((status & GSPI_FIFO_EMPTY_RFIFO_S) != 1) {
		if(pDrv->recvIdx < pDrv->pXfer->rxSz){
			if(pDrv->pXfer->rxBuff !=NULL)
			{
				/* Handle receive */
				RSI_GSPI_ReceiveHandler(pHandle, pXfer);
			}
		}
	}

	if(pDrv->sendIdx >= pDrv->pXfer->txSz)
	{		

		pDrv->xmitOn = 0;
		RSI_GSPI_CsAssert(pHandle,pDrv->pXfer->sselNum);
		pDrv->pAssertCb(pHandle,pXfer);
		pDrv->pXfer->status =  ERROR_GSPI_TX_DONE;

	}
	if(pDrv->recvIdx > pDrv->pXfer->rxSz)
	{				

		pDrv->pXfer->status =  ERROR_GSPI_READ_DONE;

	}
}

/**
 * @fn           void RSI_GSPI_Send(GSPI_HANDLE_T pHandle,uint32_t len_in_bits, uint32_t data, uint8_t cs_no)
 * @brief	       This API used to sends the data through GSPI Controller.
 * @param[in]    pHandle : handle of GSPI master.
 * @param[in]    len_in_bits : length of the send data.
 * @param[in]    data  : data to be send .
 * @param[in]    cs_no : chip select number(0..4).
 * @return	     none.
 */
void RSI_GSPI_Send(GSPI_HANDLE_T pHandle,uint32_t len_in_bits, uint32_t data, uint8_t cs_no)
{
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;

	pDrv->base->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2=len_in_bits;
	pDrv->base->GSPI_WRITE_FIFO_b[0].WRITE_FIFO = data;      /* cmd/data/addr to be written             */
	pDrv->base->GSPI_CONFIG1 = ((pDrv->base->GSPI_CONFIG1 & ~(SPI_CSN_SEL | SPI_RD_LEN| SPI_CSN | SPI_RD )) | (cs_no << SPI_CSN_SEL_POS | SPI_WR) ) ;
	while(pDrv->base->GSPI_STATUS & GSPI_BUSY_F);  /* wait till GSPI becomes idle             */
}

 /**
 * @fn           uint32_t RSI_GSPI_Receive(GSPI_HANDLE_T pHandle, uint32_t len_in_bits, uint8_t cs_no)
 * @brief	       This API used to reads the data through Gspi Controller.
 * @param[in]    pHandle     : handle of GSPI master.
 * @param[in]    len_in_bits : length of the send data.
 * @param[in]    cs_no       : chip select number(0..4)
 * @return	     read data to be returned.
 */
uint32_t RSI_GSPI_Receive(GSPI_HANDLE_T pHandle, uint32_t len_in_bits, uint8_t cs_no)
{

	uint16_t rd_data=0;
	uint32_t  len_in_bytes = len_in_bits >>3 ;
	SPIM_DATACONTEXTMEMORY_T *pDrv = (SPIM_DATACONTEXTMEMORY_T *) pHandle;
	pDrv->base->GSPI_CONFIG1 = ((pDrv->base->GSPI_CONFIG1 & ~(SPI_CSN_SEL | SPI_RD_LEN |  SPI_CSN | SPI_WR | SPI_CSN)) | (cs_no << SPI_CSN_SEL_POS | SPI_RD)  | (len_in_bytes << SPI_RD_LEN_POS));
	while(pDrv->base->GSPI_STATUS & GSPI_FIFO_EMPTY_RFIFO_S);  /* wait till GSPI becomes idle             */
	rd_data = pDrv->base->GSPI_READ_FIFO_b[0].READ_FIFO;
	return rd_data;
}


/******************************************** END ****************************************/
