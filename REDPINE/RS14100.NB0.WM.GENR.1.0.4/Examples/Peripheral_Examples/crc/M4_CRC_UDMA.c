/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "UDMA.h"
#include "rsi_board.h"
#include "sw_crc.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define  CHNL0 
#define  BUFFSIZE              25
#define  CRC_TXDMA_CH          5	
#define  NUMDMADESC            1
/* Private variables ---------------------------------------------------------*/
uint32_t txBuf[BUFFSIZE] = {0x6e95c16f,0x26032526,0x453ea633,0x33301c04,0xa531e3de,0xb040d130,0xabac09c0,
			                      0x539d6754,0x3036bedf,0xa2c998df,0xa7d3bced,0xb93e262e,0xd096b2a7,0xf2b32b1c,
			                      0x5192ff8d,0x6dc046e6,0x2d3256fe,0x73ddaa7c,0x17e73ab6,0xc3ca122a,0xb03f5ba4,
			                      0xb34e87a3,0x9854db27,0x3c5c176a,0x67a367ac };

                            
extern RSI_DRIVER_UDMA Driver_UDMA0;
static RSI_DRIVER_UDMA *UDMAdrv0 = &Driver_UDMA0;
extern RSI_UDMA_HANDLE_T udmaHandle0;
                            
uint32_t tx[BUFFSIZE];
RSI_CRC_PARAMS_T crc_params;
uint32_t hw_crc;

volatile uint8_t dmaDone =0;
extern unsigned int sw_crc;                           
/* Private function prototypes -----------------------------------------------*/
uint32_t ConvertTXBuffer(uint32_t num);
void RSI_CRC_UDMATX(void);
                           
/* Private functions ---------------------------------------------------------*/

/*  CRC interrupt handler */
void CRC_IRQHandler()
{
	/* Clears interrupt */
}


/*  UDMA  controller transfer descriptor chain complete callback  */ 
void udmaTransferComplete(uint32_t event, uint8_t ch)
{
  if(event == UDMA_EVENT_XFER_DONE)
  {
    if(ch == CRC_TXDMA_CH)
    {
      dmaDone = 1;  
    }
  }
} 

void WriteDataGenerate(void)
{
	uint32_t loop;
	
	/* Put some data in the source buffer for */
	for (loop = 0; loop < BUFFSIZE; loop++) 
	{
		txBuf[loop] = (loop + 0x1);
	}	
}


uint32_t ConvertTXBuffer(uint32_t num)
{
 int byte0, byte1, byte2, byte3;
 
 byte0 = (num & 0x000000FF) >> 0 ;
 byte1 = (num & 0x0000FF00) >> 8 ;
 byte2 = (num & 0x00FF0000) >> 16 ;
 byte3 = (num & 0xFF000000) >> 24 ;
 return((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | (byte3 << 0));
}

/* M4SS UDMA test case */
void RSI_CRC_UDMATX()
{	
   RSI_UDMA_CHA_CONFIG_DATA_T vsUDMAChaConfigData;
   RSI_UDMA_CHA_CFG_T config;
   
   memset(&vsUDMAChaConfigData, 0, sizeof(RSI_UDMA_CHA_CONFIG_DATA_T));
	 memset(&config, 0, sizeof(RSI_UDMA_CHA_CFG_T));
   
   config.altStruct       = 0;
	 config.burstReq        = 1;
	 config.channelPrioHigh = 1;
	 config.periAck         = 0;
	 config.periphReq       = 0;
	 config.reqMask         = 0;
	 config.dmaCh           = CRC_TXDMA_CH;
   
   /* Descriptor configurations */
	vsUDMAChaConfigData.transferType       = UDMA_MODE_BASIC;
	vsUDMAChaConfigData.nextBurst					 = 0;
	vsUDMAChaConfigData.totalNumOfDMATrans = (BUFFSIZE-1);
	vsUDMAChaConfigData.rPower             = ARBSIZE_1;
	vsUDMAChaConfigData.srcProtCtrl 			 = 0x000;
	vsUDMAChaConfigData.dstProtCtrl 			 = 0x000;
	vsUDMAChaConfigData.srcSize            = SRC_SIZE_32;
	vsUDMAChaConfigData.srcInc             = SRC_INC_32;
	vsUDMAChaConfigData.dstSize            = DST_SIZE_32;
	vsUDMAChaConfigData.dstInc             = DST_INC_NONE;
  
   /* Initialize dma */ 
   UDMAdrv0->Initialize();
 
   /* Configure dma channel */ 
   UDMAdrv0->ChannelConfigure( CRC_TXDMA_CH,(uint32_t )tx, (uint32_t )&CRC->CRC_DIN_FIFO , (BUFFSIZE), 
                               vsUDMAChaConfigData, &config, udmaTransferComplete );
   /* Enable dma channel */
   UDMAdrv0->ChannelEnable(CRC_TXDMA_CH);
   /* Enable dma controller */  
   UDMAdrv0->DMAEnable();    
}



/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
volatile int x;
int main (void)
{
	/* At this stage the micro-controller clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */
  uint32_t i;
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/*Initialize UART for debug prints*/
	DEBUGINIT();
	/*  Prints on hyper-terminal as "Core is Sleeping...\n" */
	DEBUGOUT("\n*******CRC EXAMPLE*********\n");
	
  /* Software CRC calculation */
  sw_crc = redpineCrcCompute((unsigned char *)0 , 100);
  
  for(i =0; i< BUFFSIZE; i++)
	{
		tx[i] = ConvertTXBuffer(txBuf[i]);
	}
	
  /* CRC_32 polynomial */
	crc_params.polynomial = 0x04C11DB7;
	//crc_params.polynomial = 0x7;
	crc_params.polyWidth  = 31;
	crc_params.lfsrVal    = 0x0000;
	crc_params.widthType  = 0;
	crc_params.dinWidth   = 31;
	crc_params.numBytes   = 3;
	crc_params.aempty     = 6 ;
	crc_params.afull      = 2;
	crc_params.swapDin    = 0;
	crc_params.useUdma    = 1;
	crc_params.swapLfsr   = 1;
	

	/* Clears the FIFO and settles all the state machines to their idle. */
	RSI_CRC_SetGenControl(CRC);
	
	/* Enable the peripheral clocks for SET1 register */
	RSI_CLK_PeripheralClkEnable1(M4CLK , CRC_CLK_ENABLE_M4);
	
	/* Load the polynomial value into the polynomial reg */
	RSI_CRC_Polynomial(CRC, &crc_params );
	
	/*  Load the width of the polynomial*/
	RSI_CRC_Polynomial_Width(CRC, &crc_params );
	
	/* Initialize the  LFSR Value */
	RSI_CRC_LfsrInit(CRC, &crc_params );
	
	/* swapped init value. If this is set bit swapped*/
	RSI_CRC_Use_Swapped_Init(CRC,&crc_params );
	
	/* Set and control the data width types */
	RSI_CRC_Set_DataWidthType(CRC,&crc_params  );
	
	/*  Set the FIFO Threshold Levels  */
	RSI_CRC_SetFifoThresholds(CRC,&crc_params);
  
	/* CRC UDMA transmission */
	RSI_CRC_UDMATX();
	
  while(!dmaDone);
	
	/* Monitor the CRC Calculation status and the returns the CRC Value */
  hw_crc = RSI_Monitor_CRCcalc(CRC,&crc_params);
	
	if(hw_crc == sw_crc)
	{
		/*  Prints on hyper-terminal  */
		DEBUGOUT("\n CRC TEST PASSED \n");
		/*  Prints on hyper-terminal  */
		DEBUGOUT("hw_crc is %x and sw_crc is %x",hw_crc,sw_crc);
		x = 0;
	
	}
	else
	{
		/*  Prints on hyper-terminal as */
	  DEBUGOUT("\n CRC TEST FAILED \n");
		x=1;
	}	
  while(1);
}
 
