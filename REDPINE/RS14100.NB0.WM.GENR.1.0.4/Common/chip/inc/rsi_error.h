/**
 * @file       rsi_error.h
 * @version    0.9
 * @date       20 Dec 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains error codes of all the peripherals
 *
 * @section Description
 * This file contains error codes
 *
 *
 */
#include <stdint.h>

#ifndef __RSI_ERROR_H__
#define __RSI_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum errnoCode {
	RSI_FAIL						    = -1,
	RSI_OK                    		= 0,
	INVALID_PARAMETERS                  ,

	/*USART error codes*/
	ERROR_USART_BASE                  = 0x100,
	ERROR_USART_CALLBACK_ERR          = ERROR_USART_BASE+1,
	ERROR_USART_NOT_SUPPORTED           ,

	/* GPDMA error codes */
	ERROR_GPDMA_BASE     				     = 0x200,
	ERROR_GPDMA_INVALIDCHNLNUM	     = ERROR_GPDMA_BASE+1,
	ERROR_GPDMA_FLW_CTRL			    ,
	ERROR_GPDMA_BURST				    ,
	ERROR_GPDMA_SRC_ADDR			    ,
	ERROR_GPDMA_DST_ADDR				,
	NOERR_GPDMA_FLAG_SET				,
	ERROR_GPDMA_INVALID_EVENT   		,
	ERROR_GPDMA_INVALID_XFERMODE   	    ,
	ERROR_GPDMA_INVALID_TRANS_LEN       ,
	ERROR_GPDMA_INVALID_ARG			    ,
	ERROR_GPDMA_CHNL_BUSY				,
	ERROR_GPDMA_NOT_ALIGNMENT           ,
	ERROR_GPDMA_QUEUE_EMPTY				,
	ERROR_GPDMA_GENERAL	  			    ,

	/* UDMA error codes */
	ERROR_UDMA_BASE      				= 0x300,
	ERROR_UDMA_INVALIDCHNLNUM		=  ERROR_UDMA_BASE+1,
	ERROR_UDMA_CTRL_BASE_INVALID			,
	ERROR_UDMA_INVALID_XFERMODE			,
	ERROR_UDMA_INVALID_TRANS_LEN        ,
	ERROR_UDMA_INVALID_ARG			    ,
	ERROR_UDMA_SRC_ADDR					,
	ERROR_UDMA_DST_ADDR					,
	ERROR_UDMA_CHNL_BUSY				,

	/* I2C error codes */
	ERROR_I2C_BASE       				= 0x400,
	ERROR_I2C_INVALID_ARG  			= ERROR_I2C_BASE+1	,
	ERROR_I2CS_UNKNOWN					,
	ERROR_I2C_SPIKE_LOGIC				,
	ERROR_I2C_IGNORE_GC_OR_START		,
	ERROR_I2C_STATUS_FLAG_NOT_SET		,
	ERROR_I2C_BUSY_FLAG					,
	ERROR_I2C_MST_BUSY_FLAG				,
	ERROR_I2C_SLV_BUSY_FLAG				,
	ERROR_I2C_SLV_DIS_WHILE_BUSY		,
	ERROR_I2C_MST_XFER_ABORT			,
	ERROR_I2C_MST_TX_CMD_BLOCK			,
	ERROR_I2C_SLV_RX_DATA_LOST			,
	ERROR_I2C_NO_TX_DATA				,
	ERROR_I2C_NO_INTR_FLAG				,
	ERROR_I2C_ERROR_FLAG_NONE			,
	ERROR_I2C_INVALID_CB		      	,
	ERROR_I2C_INVALID_POINTER		  	,
	ERROR_I2C_GENERAL_FAILURE           ,
	ERROR_I2C_TXABORT                   ,
	ERROR_I2C_SCL_STUCK_ATLOW           ,
	ERROR_I2C_MST_ON_HOLD               ,
	ERROR_I2C_BUFFER_OVERFLOW           ,
	ERROR_I2C_BUFFER_UNDERFLOW          ,

	/* I2S error codes */
	ERROR_I2S_BASE       				= 0x500,
	ERROR_I2S_INVALID_ARG				= ERROR_I2S_BASE+1,
	ERROR_I2S_INVALID_RES				,
	ERROR_I2S_INVALID_LENGTH		,
	ERROR_I2S_BUSY 				      ,
	ERROR_I2S_TXOVERRUN         ,
	ERROR_I2S_RXOVERRUN         ,
	ERROR_I2S_TXCOMPLETE        ,
	ERROR_I2S_RXCOMPLETE        ,

	/* UART error codes */
	ERROR_UART_BASE       			= 0x600,
	ERROR_UART_INVALID_ARG			= ERROR_UART_BASE+1,
	ERROR_UART_INVALID_RES				,

	/* PWM error codes */
	ERROR_PWM_BASE       				 = 0x700,
	ERROR_PWM_INVALID_CHNLNUM		 = ERROR_PWM_BASE+1,
	ERROR_PWM_INVALID_PWMOUT			,
	ERROR_PWM_NO_INTR					,
	ERROR_PWM_INVALID_ARG				,

	/* Timers error codes */
	ERROR_TIMER_BASE      				= 0x800,
	ERROR_INVAL_TIMER_NUM  				= ERROR_TIMER_BASE+1,
	ERROR_INVAL_TIMER_MODE				,
	ERROR_INVAL_TIMERTYPE				,
  ERROR_INVAL_COUNTER_DIR   ,
  
	/* SCT error codes */
	ERROR_CT_BASE       				  = 0x900,
	ERROR_CT_INVALID_COUNTER_NUM	= ERROR_CT_BASE+1	,
	ERROR_CT_INVALID_ARG				,

	/* EFUSE ERROR CODES */
	ERROR_EFUSE_BASE   					            = 0xA00,
	ERROR_EFUSE_INVALID_WRITE_ADDRESS       = ERROR_EFUSE_BASE+1,
	ERROR_EFUSE_INVALID_WRITE_BIT_POSITION     ,
	ERROR_EFUSE_INVALID_PARAMETERS             ,

	/* CCI ERROR CODES */
	ERROR_CCI_BASE_ADDRESS              = 0xB00,
	ERROR_CCI_INIT_FAIL                 = ERROR_CCI_BASE_ADDRESS+1,
	ERROR_CCI_ADDRESS_ERR               ,

	/* QEI ERROR CODES */
	ERROR_QEI_BASE   					          = 0xC00,
	ERROR_INVALID_WRITE_ADDRESS         = ERROR_QEI_BASE+1,
	ERROR_INVALID_WRITE_BIT_POSITION    ,
	ERROR_INVALID_PARAMETERS            ,

	/* SDIO ERROR CODES */
	ERROR_SSDIO_BASE_ADDRESS            = 0xD00,                                 			/*!< SDIO Error base address */
	ERROR_SSDIO_INIT_FAIL               = ERROR_SSDIO_BASE_ADDRESS+1,
	ERROR_SSDIO_ADDRESS_ERR             ,
	ERROR_SSDIO_INVALID_FN              ,
	ERROR_SSDIO_INVALID_PARAM           ,

	/* SPI ERROR CODES*/
	ERROR_SSPI_BASE_ADDRESS             = 0xE00,
	ERROR_SSPI_INIT_FAIL                = ERROR_SSPI_BASE_ADDRESS+1,
	ERROR_SSPI_ADDRESS_ERR              ,
	ERROR_SSPI_CB_ERROR                 ,

	/* ETHERNET ERROR CODES */
	ERROR_ETH_BASE_ADDRESS              = 0xF00,
	ERROR_ETH_INIT_FAIL                 = ERROR_ETH_BASE_ADDRESS+1,
	ERROR_ETH_PARAM                     ,
	ERROR_ETH_NULL                      ,
	ERR_DMA_NOT_ALIGNMENT               ,
	ERROR_ETH_CALLBACK_ERR              ,

	/*CAN ERROR CODES*/
	ERROR_CAN_BASE                      = 0x1000,
	ERROR_CAN_INVALID_PARAMETERS        = ERROR_CAN_BASE+1,
	ERROR_CAN_INVALID_TIMING_PARAMETERS ,
	ERROR_CAN_OPERATION_IN_PROGRESS     ,

	/*GSPI ERROR CODES*/
	ERROR_GSPI_BASE                    = 0x1100,
	ERROR_GSPI_INVALID_ARG             = ERROR_GSPI_BASE+1,
	ERROR_GSPI_INVALID_LENGTH          = ERROR_GSPI_BASE+2,
	ERROR_GSPI_BUSY                    = ERROR_GSPI_BASE+3,
	ERROR_GSPI_READ_DONE               = ERROR_GSPI_BASE+4,
	ERROR_GSPI_IDLE                    = ERROR_GSPI_BASE+5,
	ERROR_GSPI_TX_DONE                 = ERROR_GSPI_BASE+6,



	/*SSI ERROR CODES*/
	ERROR_SSI_BASE                     = 0x1200,
	ERROR_SSI_INVALID_ARG              = ERROR_SSI_BASE+1,
	ERROR_SSI_BUSY                     = ERROR_SSI_BASE+2,
	ERROR_SSI_IDLE                     = ERROR_SSI_BASE+3,
	TRANSFER_COMPLETE                  = ERROR_SSI_BASE+4,
	READ_COMPLETED                     = ERROR_SSI_BASE+5,

	/*SSI ERROR CODES*/
	ERROR_CRC_BASE                     = 0x1300,
	ERROR_CRC_INVALID_ARG              = ERROR_CRC_BASE+1,

	/*SSI ERROR CODES*/
	ERROR_RNG_BASE                     = 0x1400,
	ERROR_RNG_INVALID_ARG              = ERROR_RNG_BASE+1,

	/*NPSS ERROR CODES*/
	ERROR_BOD_BASE                     = 0x1500,
	ERROR_PS_BASE                      = ERROR_BOD_BASE+1,
	ERROR_BOD_INVALID_PARAMETERS       ,
	ERROR_PS_INVALID_PARAMETERS        ,
	ERROR_PS_INVALID_STATE             ,

	/*TIME PERIOD*/
	ERROR_TIMEPERIOD_BASE               = 0x1600,
	ERROR_TIME_PERIOD_PARAMETERS        = ERROR_TIMEPERIOD_BASE+1,
	ERROR_TIME_PERIOD_RC_CALIB_NOT_DONE ,
	ERROR_CAL_INVALID_PARAMETERS        ,

	/*M4SS CLOCKS */
	ERROR_M4SS_CLK_BASE                 = 0x1700,
	ERROR_CLOCK_NOT_ENABLED             = ERROR_M4SS_CLK_BASE+1,
	ERROR_INVALID_INPUT_FREQUENCY       ,

	/*ULPSS CLOCKS */
	ERROR_ULPCLK_BASE                   = 1800,
	ERROR_ULPCLK_INVALID_PARAMETERS     = ERROR_ULPCLK_BASE+1,        

	ERROR_SIO_BASE                      =0x1900,
	ERROR_SIO_I2C_NO_ACK                =ERROR_SIO_BASE + 1,

	/* ULPSS FIM */
	ERROR_FIM_BASE                      =0x2000,
	ERROR_FIM_MATRIX_INVALID_ARG        =ERROR_FIM_BASE + 1,
	
	/* AUX ADC */
  ERROR_NO_MULTI_CHNL_ENABLE           = 0x2100,
	NO_MODE_SET                          =ERROR_NO_MULTI_CHNL_ENABLE + 1,
	ERROR_PING_PONG_ADDR_MATCH           =ERROR_NO_MULTI_CHNL_ENABLE + 2,
	ERROR_ADC_INVALID_ARG                =ERROR_NO_MULTI_CHNL_ENABLE + 3,  
	/* AUX DAC */
	ERROR_NO_PAD_SEL                     = 0x2200,
	ERROR_FREQ_VAL                       = ERROR_NO_PAD_SEL + 1,
	
	
}error_t;

#ifdef __cplusplus
}
#endif

#endif // __RSI_ERROR_H__

/*END OF FILE */


