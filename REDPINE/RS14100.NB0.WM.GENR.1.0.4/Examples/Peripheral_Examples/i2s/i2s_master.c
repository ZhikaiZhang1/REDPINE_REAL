 /**
 * @file       i2s_master.c
 * @version    1.0
 * @date       18 Dec 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example describing usage of CMSIS I2S API.
 *
 * @section Description
 * I2S Master mode data loop back example
 */
 

/* Includes ------------------------------------------------------------------*/
#include "SAI.h"
#include "rsi_board.h"


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define  BUFFER_SIZE       2048

/* Private define ------------------------------------------------------------*/
#define SOC_PLL_REF_FREQUENCY           40000000  /*<! PLL input REFERENCE clock 40MHZ */


//Note: Change this macro to required PLL frequency in hertz
#define PS4_SOC_FREQ                    180000000 /*<! PLL out clock 200MHz            */
/* Private variables ---------------------------------------------------------*/
uint16_t tx_buf[BUFFER_SIZE];
uint16_t rx_buf[BUFFER_SIZE];
uint32_t tx_cnt = 0;
uint32_t rx_cnt = 0;
volatile uint8_t ret =1;
volatile uint32_t tx_done = 0,rx_done = 0;

/* SAI Driver */
extern ARM_DRIVER_SAI Driver_SAI0;
static ARM_DRIVER_SAI *SAIdrv = &Driver_SAI0;
void ARM_SAI_SignalEvent(uint32_t event);
ARM_DRIVER_SAI *drv_info;
ARM_SAI_CAPABILITIES drv_capabilities;
 
/* Private function prototypes -----------------------------------------------*/
void ARM_SAI_SignalEvent(uint32_t event);
void setup_sai (void); 
void read_capabilities (void); 
void BufferInit(uint16_t seed);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief	 SAI event handler
 * @param  event SAI transmit and receive events
 * @return	none
 */
void ARM_SAI_SignalEvent(uint32_t event)
{  
  if(event & ARM_SAI_EVENT_SEND_COMPLETE)
  {
    tx_done = 1;
  }
  if(event & ARM_SAI_EVENT_RECEIVE_COMPLETE)
  {
    rx_done = 1;
  }
  if(event & ARM_SAI_EVENT_TX_UNDERFLOW)
  {
  }
  if(event & ARM_SAI_EVENT_RX_OVERFLOW)
  {
  }
  if(event & ARM_SAI_EVENT_FRAME_ERROR)
  {
  }    
}

/**
 * @brief	 SAI driver version check
 * @return	none
 */
void setup_sai (void)  
{
  ARM_DRIVER_VERSION  version;
 
  drv_info = &Driver_SAI0;  
  version = drv_info->GetVersion ();
  if (version.api < 0x10A)  
  {   
    // error handling
    return;
  }
}

/**
 * @brief	 Capabilities of SAI driver
 * @param  none   
 * @return none
 */
void read_capabilities (void)  
{
  drv_info = &Driver_SAI0;  
  drv_capabilities = drv_info->GetCapabilities ();
  // interrogate capabilities
}


/**
 * @brief	 Fills transmit buffer data
 * @param  seed  Inital value of a transmit buffer
 * @return none
 */
void BufferInit(uint16_t seed)
{
	uint32_t loop;

	for (loop = 0; loop < BUFFER_SIZE; loop++) 
	{
		/* Clear RX buffers, so we know something was received */
		rx_buf[loop] = 0;

		/* Seed data for master transmit buffers */
		tx_buf[loop] = seed ;					
		
		seed++;
	}
	return;
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main()
{
  volatile uint32_t status;
	
 /*Configures the system default clock and power configurations*/
  SystemCoreClockUpdate();
	/* Fills the transmit buffer with data */
  BufferInit(1);
  
	/* checks the SAI driver version  */
  setup_sai();
	
	/*Read capabilities of SAI driver*/
  read_capabilities();
  
	/*Switch M4 SOC clock to Reference clock*/
  /*Default keep M4 in reference clock*/
  RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);

  /*Configure the PLL frequency*/
  RSI_CLK_SetSocPllFreq(M4CLK , PS4_SOC_FREQ , SOC_PLL_REF_FREQUENCY);
    
	/*Switch M4 clock to PLL clock for speed operations*/
  RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);

  //RSI_CLK_QspiClkConfig(M4CLK,QSPI_SOCPLLCLK,0,0,0); 
	ROMAPI_M4SS_CLK_API->clk_qspi_clk_config(M4CLK,QSPI_SOCPLLCLK,0,0,0); 
	 
  /* Initializes SAI interface */ 
  SAIdrv->Initialize(ARM_SAI_SignalEvent);
	
	/* Control the power modes of the SAI interface. */
  SAIdrv->PowerControl (ARM_POWER_FULL);
   
  /* configure Transmitter to Asynchronous Master: I2S Protocol, 16-bit data, 8kHz Audio frequency */
  status = SAIdrv->Control(ARM_SAI_CONFIGURE_TX | 
                               ARM_SAI_MODE_MASTER  | 
                               ARM_SAI_ASYNCHRONOUS | 
                               ARM_SAI_PROTOCOL_I2S | 
                               ARM_SAI_DATA_SIZE(16), 32, 8000);
 
 /* configure Receiver to Asynchronous Master: I2S Protocol, 16-bit data, 8kHz Audio frequency */
  status = SAIdrv->Control(ARM_SAI_CONFIGURE_RX | 
                               ARM_SAI_MODE_MASTER  | 
                               ARM_SAI_ASYNCHRONOUS | 
                               ARM_SAI_PROTOCOL_I2S | 
                               ARM_SAI_DATA_SIZE(16), 32,8000);

  /* enable Transmitter */
  status = SAIdrv->Control(ARM_SAI_CONTROL_TX, 1, 0);
 
  /* enable Receiver */
  status = SAIdrv->Control(ARM_SAI_CONTROL_RX, 1, 0);

  /* Sends data to the SAI transmitter. */
  SAIdrv->Send(tx_buf, BUFFER_SIZE);
	
	/*Start receiving data from SAI receiver */
  SAIdrv->Receive(rx_buf, BUFFER_SIZE);
  
	/* wait until tx_done=0  */
  while(!tx_done);
	
	/* Get transmitted data count. */
	tx_cnt = SAIdrv->GetTxCount();	
  if (tx_cnt != (BUFFER_SIZE))
  {		
     while(1);
	}
 
	/* wait until rx_done=0  */
  while(!rx_done);
  /* Get received data count. */	
	rx_cnt = SAIdrv->GetRxCount();
  if (rx_cnt != (BUFFER_SIZE))
  {		
     while(1);
	}
	 ret = memcmp(tx_buf, rx_buf,sizeof(tx_buf));
  while(1);
}
