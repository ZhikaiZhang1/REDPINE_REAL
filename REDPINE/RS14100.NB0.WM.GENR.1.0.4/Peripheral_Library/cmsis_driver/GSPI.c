 
#include "GSPI.h"
#include "UDMA.h"

#define ARM_SPI_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0) /* driver version */

extern RSI_UDMA_HANDLE_T udmaHandle0,udmaHandle1;
/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
		ARM_SPI_API_VERSION,
		ARM_SPI_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_SPI_CAPABILITIES DriverCapabilities = {
		1, /* Simplex Mode (Master and Slave) */
		0, /* TI Synchronous Serial Interface */
		0, /* Microwire Interface */
		1  /* Signal Mode Fault event: \ref SPI_EVENT_MODE_FAULT */
};
#if (RTE_GSPI_MASTER)

#define  GSPI_MASTER_IRQHandler     IRQ046_Handler
// GSPI_MASTER Run-Time Information
static GSPI_INFO          GSPI_MASTER_Info         = { 0U };
static GSPI_TRANSFER_INFO GSPI_MASTER_TransferInfo = { 0U };

#if (RTE_GSPI_MASTER_CHNL_UDMA_TX_EN == 1)
void GSPI_MASTER_UDMA_Tx_Event (uint32_t event ,uint8_t dmaCh);
static GSPI_DMA GSPI_MASTER_UDMA_TX_CHNL = {
		{
				UDMA_MODE_BASIC,
				0,
				(RTE_GSPI_DMA_TX_LEN_PER_DES-1),
				ARBSIZE_1,
				0x0,
				0x0,
				SRC_SIZE_8,
				SRC_INC_8,
				DST_SIZE_8,
				DST_INC_NONE
		},
		{
				0,  //channelPrioHigh
				0,  //altStruct
				1,   //burstReq
				0,   //reqMask
				1,    //periphReq
				0,//DMA PERI ACK
				RTE_GSPI_MASTER_CHNL_UDMA_TX_CH    //dmaCh
		},
		RTE_GSPI_MASTER_CHNL_UDMA_TX_CH,
		GSPI_MASTER_UDMA_Tx_Event
};
#endif
#if (RTE_GSPI_MASTER_CHNL_UDMA_RX_EN == 1)
void GSPI_MASTER_UDMA_Rx_Event (uint32_t event ,uint8_t dmaCh);
static GSPI_DMA GSPI_MASTER_UDMA_RX_CHNL = {
		{
				UDMA_MODE_BASIC,
				0,
				(RTE_GSPI_DMA_RX_LEN_PER_DES-1),
				ARBSIZE_1,
				0x0,
				0x0,
				SRC_SIZE_8,
				SRC_INC_NONE,
				DST_SIZE_8,
				DST_INC_8
		},
		{
				0,
				0,
				1,
				0,
				1,
				0,  //DMA ACK
				RTE_GSPI_MASTER_CHNL_UDMA_RX_CH
		},
		RTE_GSPI_MASTER_CHNL_UDMA_RX_CH,
		GSPI_MASTER_UDMA_Rx_Event
};
#endif

// GSPI_MASTER Resources
static const GSPI_RESOURCES GSPI_MASTER_Resources = {
		GSPI0,
		RSI_CLK_GetBaseClock,
		GSPI0_IRQn,

#if(RTE_GSPI_MASTER_CHNL_UDMA_RX_EN)
		&GSPI_MASTER_UDMA_RX_CHNL,
#else
		NULL,
#endif

#if (RTE_GSPI_MASTER_CHNL_UDMA_TX_EN)
		&GSPI_MASTER_UDMA_TX_CHNL,
#else
		NULL,
#endif

		&GSPI_MASTER_Info,
		&GSPI_MASTER_TransferInfo,
};
#endif /* GSPI_Master */

ARM_DRIVER_VERSION GSPI_MASTER_GetVersion(void)
{
	return DriverVersion;
}

ARM_SPI_CAPABILITIES GSPI_MASTER_GetCapabilities(void)
{
	return DriverCapabilities;
}

int32_t GSPI_Initialize(ARM_SPI_SignalEvent_t cb_event, const GSPI_RESOURCES *gspi)
{

	RSI_PS_M4ssPeriPowerUp(M4SS_PWRGATE_ULP_PERI2);

	if (gspi->info->state & SPI_INITIALIZED)
	{
		return ARM_DRIVER_OK;
	}

	// Initialize SPI Run-Time Resources
	gspi->info->cb_event = cb_event;
	gspi->info->status.busy       = 0U;
	gspi->info->status.data_lost  = 0U;
	gspi->info->status.mode_fault = 0U;

	// Clear transfer information
	memset(gspi->xfer, 0, sizeof(GSPI_TRANSFER_INFO));

	gspi->info->state = SPI_INITIALIZED;

	/*Pin Mux*/
	if((gspi->reg == GSPI0))
	{
		//Configure clock pin
		if(RTE_GSPI_MASTER_CLK_PIN > 63)
		{
			RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_GSPI_MASTER_CLK_PIN -64));
			RSI_EGPIO_SetPinMux(EGPIO1,RTE_GSPI_MASTER_CLK_PORT,(uint8_t)(RTE_GSPI_MASTER_CLK_PIN-64),6);
		}
		RSI_EGPIO_PadSelectionEnable(RTE_GSPI_MASTER_CLK_PAD);
		RSI_EGPIO_PadReceiverEnable(RTE_GSPI_MASTER_CLK_PIN);
		RSI_EGPIO_SetPinMux(EGPIO,RTE_GSPI_MASTER_CLK_PORT,RTE_GSPI_MASTER_CLK_PIN,RTE_GSPI_MASTER_CLK_MUX);

		//configure cs pin
		if(RTE_GSPI_MASTER_CS0_PIN > 63)
		{
			RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_GSPI_MASTER_CS0_PIN -64));
			RSI_EGPIO_SetPinMux(EGPIO1,RTE_GSPI_MASTER_CS0_PORT,(uint8_t)(RTE_GSPI_MASTER_CS0_PIN-64),6);
		}
		RSI_EGPIO_PadSelectionEnable(RTE_GSPI_MASTER_CS0_PAD);
		RSI_EGPIO_PadReceiverEnable(RTE_GSPI_MASTER_CS0_PIN);
		RSI_EGPIO_SetPinMux(EGPIO,RTE_GSPI_MASTER_CS0_PORT,RTE_GSPI_MASTER_CS0_PIN,RTE_GSPI_MASTER_CS0_MUX);

		//configure MOSI pin
		if(RTE_GSPI_MASTER_MOSI_PIN > 63)
		{
			RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_GSPI_MASTER_MOSI_PIN -64));
			RSI_EGPIO_SetPinMux(EGPIO1,RTE_GSPI_MASTER_MOSI_PORT,(uint8_t)(RTE_GSPI_MASTER_MOSI_PIN-64),6);
		}
		RSI_EGPIO_PadSelectionEnable(RTE_GSPI_MASTER_MOSI_PAD);
		RSI_EGPIO_PadReceiverEnable(RTE_GSPI_MASTER_MOSI_PIN);
		RSI_EGPIO_SetPinMux(EGPIO,RTE_GSPI_MASTER_MOSI_PORT,RTE_GSPI_MASTER_MOSI_PIN,RTE_GSPI_MASTER_MOSI_MUX);

		//configure MISO pin
		if(RTE_GSPI_MASTER_MISO_PIN > 63)
		{
			RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_GSPI_MASTER_MISO_PIN -64));
			RSI_EGPIO_SetPinMux(EGPIO1,RTE_GSPI_MASTER_MISO_PORT,(uint8_t)(RTE_GSPI_MASTER_MISO_PIN-64),6);
		}
		RSI_EGPIO_PadSelectionEnable(RTE_GSPI_MASTER_MISO_PAD);
		RSI_EGPIO_PadReceiverEnable(RTE_GSPI_MASTER_MISO_PIN);
		RSI_EGPIO_SetPinMux(EGPIO,RTE_GSPI_MASTER_MISO_PORT,RTE_GSPI_MASTER_MISO_PIN,RTE_GSPI_MASTER_MISO_MUX);
	}

	// DMA Initialize
	if (gspi->tx_dma || gspi->rx_dma)
	{
		if(RTE_GSPI_MASTER_CHNL_UDMA_TX_EN || RTE_GSPI_MASTER_CHNL_UDMA_RX_EN)
		{
			if ((gspi->rx_dma != NULL) || (gspi->tx_dma != NULL))
			{

				// Enable DMA instance
				if ((gspi->reg == GSPI0))
				{
					// DMA0 used for SSI_MASTER and SSI_SLAVE
					UDMA0_Initialize();
				}

			}

		}
	}

	gspi->info->state = SPI_INITIALIZED;

	return ARM_DRIVER_OK;
}

int32_t GSPI_Uninitialize(const GSPI_RESOURCES *gspi)
{
	// Clear SPI state
	gspi->info->state = 0U;

	RSI_PS_M4ssPeriPowerDown(M4SS_PWRGATE_ULP_PERI2);

	if ((gspi->rx_dma != NULL) || (gspi->tx_dma != NULL))
	{
		if(RTE_GSPI_MASTER_CHNL_UDMA_TX_EN || RTE_GSPI_MASTER_CHNL_UDMA_RX_EN)
		{
			// Diasable DMA instance
			if ((gspi->reg == GSPI0))
			{
				UDMA0_Uninitialize();
			}
			else
			{
				UDMA1_Uninitialize();
			}
		}
	}
	return ARM_DRIVER_OK;
}
int32_t GSPI_PowerControl(ARM_POWER_STATE state, const GSPI_RESOURCES *gspi)
{
	switch (state)
	{
	case ARM_POWER_OFF:

		NVIC_DisableIRQ (gspi->irq_num);   // Disable SPI IRQ in NVIC

		// Reset SPI Run-Time Resources
		gspi->info->status.busy       = 0U;
		gspi->info->status.data_lost  = 0U;
		gspi->info->status.mode_fault = 0U;

		// Clear pending SPI interrupts in NVIC
		NVIC_ClearPendingIRQ(gspi->irq_num);

		// Clear transfer information
		memset(gspi->xfer, 0, sizeof(GSPI_TRANSFER_INFO));

		gspi->info->state &= ~SPI_POWERED; // SPI is not powered

		/*power down*/
		RSI_PS_M4ssPeriPowerDown(M4SS_PWRGATE_ULP_PERI2);
		break;

	case ARM_POWER_FULL:

		if ((gspi->info->state & SPI_INITIALIZED) ==0U) { return ARM_DRIVER_ERROR; }
		if ((gspi->info->state & SPI_POWERED)     !=0U) { return ARM_DRIVER_OK; }

		/*Configure the M4 Clock*/
		RSI_CLK_PeripheralClkEnable3(M4CLK , M4_SOC_CLK_FOR_OTHER_ENABLE);

		/* Configure the GSPI Clocks */
		RSI_CLK_GspiClkConfig(M4CLK,RTE_GSPI_CLOCK_SOURCE);

		gspi->reg->GSPI_BUS_MODE_b.SPI_HIGH_PERFORMANCE_EN = 0x1;

		// Reset SPI Run-Time Resources
		gspi->info->status.busy       = 0U;
		gspi->info->status.data_lost  = 0U;
		gspi->info->status.mode_fault = 0U;
		gspi->info->state |= SPI_POWERED;  // SPI is powered

		NVIC_ClearPendingIRQ (gspi->irq_num);
		NVIC_EnableIRQ (gspi->irq_num);    // Enable SPI IRQ in NVIC
		break;

	case ARM_POWER_LOW:
	default:
		return ARM_DRIVER_ERROR_UNSUPPORTED;
	}
	return ARM_DRIVER_OK;
}
static int32_t GSPI_Control(uint32_t control, uint32_t arg, const GSPI_RESOURCES *gspi)
{
	uint32_t clk_div_factor =0;
	uint32_t data_bits,val=0;
	if (!(gspi->info->state & SPI_POWERED))
	{
		return ARM_DRIVER_ERROR;
	}
	if ((gspi->reg == GSPI0) && ((control & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_SLAVE))
	{
		return ARM_DRIVER_ERROR_UNSUPPORTED;
	}

	if ((control & ARM_SPI_CONTROL_Msk) == ARM_SPI_ABORT_TRANSFER)
	{
		memset(gspi->xfer, 0, sizeof(GSPI_TRANSFER_INFO));
		gspi->info->status.busy = 0U;
		return ARM_DRIVER_OK;
	}
	if (gspi->info->status.busy)
	{
		return ARM_DRIVER_ERROR_BUSY;
	}
	switch (control & ARM_SPI_CONTROL_Msk)
	{
	case ARM_SPI_MODE_INACTIVE:
		gspi->info->mode  &= ~ARM_SPI_CONTROL_Msk;
		gspi->info->mode  |=  ARM_SPI_MODE_INACTIVE;
		gspi->info->state &= ~SPI_CONFIGURED;
		return ARM_DRIVER_OK;

	case ARM_SPI_MODE_MASTER:
		gspi->info->mode  &= ~ARM_SPI_CONTROL_Msk;
		gspi->info->mode  |=  ARM_SPI_MODE_MASTER;
		gspi->info->state |=  SPI_CONFIGURED;
		gspi->info->mode |= ARM_SPI_MODE_MASTER;
		goto set_speed;

	case ARM_SPI_MODE_SLAVE:
		return ARM_SPI_ERROR_MODE;

	case ARM_SPI_MODE_MASTER_SIMPLEX:
		return ARM_SPI_ERROR_MODE;

	case ARM_SPI_MODE_SLAVE_SIMPLEX:
		return ARM_SPI_ERROR_MODE;

	case ARM_SPI_SET_BUS_SPEED:
		set_speed:
		clk_div_factor = (gspi->periph_clock(M4_GSPI)/arg);

	/*NOTE : division factor is multiples of 2*/
	clk_div_factor  = (clk_div_factor / 2);

	if(clk_div_factor < 1)
	{
		gspi->reg->GSPI_CLK_CONFIG_b.GSPI_CLK_EN   = 0x1;

		gspi->reg->GSPI_CLK_CONFIG_b.GSPI_CLK_SYNC = 0x1;
	}
	/*Update the clock rate to hardware */
	gspi->reg->GSPI_CLK_DIV_b.GSPI_CLK_DIV_FACTOR = clk_div_factor;
	break;

	case ARM_SPI_GET_BUS_SPEED:             // Get Bus Speed in bps
		return  gspi->reg->GSPI_CLK_DIV_b.GSPI_CLK_DIV_FACTOR;

	case ARM_SPI_SET_DEFAULT_TX_VALUE:      // Set default Transmit value; arg = value
		gspi->xfer->def_val = (uint16_t)(arg & 0xFFFF);
		return ARM_DRIVER_OK;

	case ARM_SPI_CONTROL_SS:

		if (((gspi->info->mode & ARM_SPI_CONTROL_Msk) != ARM_SPI_MODE_MASTER) )
		{
			return ARM_DRIVER_ERROR;
		}

		val=(gspi->info->mode & ARM_SPI_SS_MASTER_MODE_Msk);

		if (arg == ARM_SPI_SS_INACTIVE)
		{
			if(val==ARM_SPI_SS_MASTER_HW_OUTPUT)
			{
				/* Assert selected CS line */
				gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_CSN_SELECT =0;
				gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_CSN  = 0x1;
				while(!(gspi->reg->GSPI_STATUS & GSPI_MAN_CSN)) ;
			}
			else
			{
				RSI_EGPIO_SetPin(EGPIO,RTE_GSPI_MASTER_CS0_PORT,RTE_GSPI_MASTER_CS0_PIN,1);
			}
		}
		else
		{
			if(val==ARM_SPI_SS_MASTER_HW_OUTPUT)
			{
				/* Assert selected CS line */
				gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_CSN_SELECT=0;

				gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_CSN =0;

				while((gspi->reg->GSPI_STATUS & GSPI_MAN_CSN)) ;
			}
			else
			{
				RSI_EGPIO_SetPin(EGPIO,RTE_GSPI_MASTER_CS0_PORT,RTE_GSPI_MASTER_CS0_PIN,0);
			}
		}
		return ARM_DRIVER_OK;
	}

	if ((gspi->info->mode & ARM_SPI_CONTROL_Msk) ==  ARM_SPI_MODE_MASTER)
	{
		switch (control & ARM_SPI_SS_MASTER_MODE_Msk) {
		case ARM_SPI_SS_MASTER_UNUSED:        // SPI Slave Select when Master: Not used (default)

			//unconfigure the slave selct pin
			RSI_EGPIO_SetPinMux(EGPIO,RTE_GSPI_MASTER_CS0_PORT,RTE_GSPI_MASTER_CS0_PIN,0);

			gspi->info->mode  &= ~ARM_SPI_SS_MASTER_MODE_Msk;
			gspi->info->mode  |=  ARM_SPI_SS_MASTER_UNUSED;
			break;

		case ARM_SPI_SS_MASTER_HW_INPUT:      // SPI Slave Select when Master: Hardware monitored Input
			gspi->info->mode  &= ~ARM_SPI_SS_MASTER_MODE_Msk;
			return ARM_SPI_ERROR_SS_MODE;

		case ARM_SPI_SS_MASTER_SW:            // SPI Slave Select when Master: Software controlled
			//configure Gpio pin output mode
			//configure cs pin
			if(RTE_GSPI_MASTER_CS0_PIN > 63)
			{
				RSI_EGPIO_SetPinMux(EGPIO1,RTE_GSPI_MASTER_CS0_PORT,(uint8_t)(RTE_GSPI_MASTER_CS0_PIN-64),6);
			}
			RSI_EGPIO_PadSelectionEnable(RTE_GSPI_MASTER_CS0_PAD);
			RSI_EGPIO_SetPinMux(EGPIO,RTE_GSPI_MASTER_CS0_PORT,RTE_GSPI_MASTER_CS0_PIN,0);    //gpio mode
			RSI_EGPIO_SetDir(EGPIO,RTE_GSPI_MASTER_CS0_PORT,RTE_GSPI_MASTER_CS0_PIN,0);       //set output direction
			RSI_EGPIO_SetPin(EGPIO,RTE_GSPI_MASTER_CS0_PORT,RTE_GSPI_MASTER_CS0_PIN,1);    //set pin value to 1
			gspi->info->mode  |= ARM_SPI_SS_MASTER_SW;
			break;

		case ARM_SPI_SS_MASTER_HW_OUTPUT:     // SPI Slave Select when Master: Hardware controlled Output
			gspi->info->mode  &= ~ARM_SPI_SS_MASTER_MODE_Msk;
			if (RTE_GSPI_MASTER_CS0_PIN != NULL)
			{
				gspi->info->mode |= ARM_SPI_SS_MASTER_HW_OUTPUT;
			}
			else
			{
				return ARM_SPI_ERROR_SS_MODE;
			}
			break;
		default:
			break;
		}
	}
	// Frame format:
	switch (control & ARM_SPI_FRAME_FORMAT_Msk)
	{
	case ARM_SPI_CPOL0_CPHA0:
		gspi->reg->GSPI_BUS_MODE_b.GSPI_CLK_MODE_CSN0 =0;   //mode 0
		break;

	case ARM_SPI_CPOL0_CPHA1:
		return ARM_SPI_ERROR_FRAME_FORMAT;

	case ARM_SPI_CPOL1_CPHA0:
		return ARM_SPI_ERROR_FRAME_FORMAT;

	case ARM_SPI_CPOL1_CPHA1:
		gspi->reg->GSPI_BUS_MODE_b.GSPI_CLK_MODE_CSN0 =0x1;  //mode 3
		break;

	case ARM_SPI_TI_SSI:
		return ARM_SPI_ERROR_FRAME_FORMAT;

	case ARM_SPI_MICROWIRE:
		return ARM_SPI_ERROR_FRAME_FORMAT;

	default:
		return ARM_SPI_ERROR_FRAME_FORMAT;
	}

	/*Enable Full duplex mode */
	gspi->reg->GSPI_CONFIG1_b.SPI_FULL_DUPLEX_EN = 0x1;

	if ((gspi->rx_dma)||(gspi->tx_dma))
	{
		/*Configure the FIFO thresholds*/
		gspi->reg->GSPI_FIFO_THRLD_b.FIFO_AEMPTY_THRLD = RTE_FIFO_AEMPTY_THRLD;
		gspi->reg->GSPI_FIFO_THRLD_b.FIFO_AFULL_THRLD  = RTE_FIFO_AFULL_THRLD;
	}
	else
	{
		/*Configure the FIFO thresholds in I/0 mode*/
		gspi->reg->GSPI_FIFO_THRLD_b.FIFO_AEMPTY_THRLD = 0x1;
		gspi->reg->GSPI_FIFO_THRLD_b.FIFO_AFULL_THRLD  = 0xC;
	}
	/*Swap the read data inside the GSPI controller it-self.*/
	gspi->reg->GSPI_CONFIG2 &=~BIT(4);

	/* Configure Number of Data Bits */
	data_bits = ((control & ARM_SPI_DATA_BITS_Msk) >> ARM_SPI_DATA_BITS_Pos);
	if ((data_bits == 0) || (data_bits > 16))
	{
		return ARM_SPI_ERROR_DATA_BITS;
	}
	else
	{
		/* Update the number of Data Bits*/
		gspi->reg->GSPI_WRITE_DATA2 =data_bits ;
	}

	return ARM_DRIVER_OK;
}

int32_t GSPI_Send(const void *data, uint32_t num, const GSPI_RESOURCES *gspi)
{
	volatile int32_t stat=0;

	static uint8_t dummy_data;

	static uint8_t data_bits=0;

	gspi->info->status.busy       = 0U;
	if ((data == NULL) || (num == 0U))
	{
		return ARM_DRIVER_ERROR_PARAMETER;
	}
	if (!(gspi->info->state & SPI_CONFIGURED))
	{
		return ARM_DRIVER_ERROR;
	}
	if (  gspi->info->status.busy)
	{
		return ARM_DRIVER_ERROR_BUSY;
	}
	gspi->info->status.busy       = 1U;
	gspi->info->status.data_lost  = 0U;
	gspi->info->status.mode_fault = 0U;

	gspi->xfer->rx_buf = NULL;
	gspi->xfer->num    = num;
	gspi->xfer->rx_cnt = 0U;
	gspi->xfer->tx_cnt = 0U;
	gspi->xfer->tx_buf = (uint8_t *)data;

	data_bits= GSPI0->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2;

	//gspi->rx_dma->control.totalNumOfDMATrans = 1;
	if ((gspi->rx_dma)||(gspi->tx_dma))
	{
		if(RTE_GSPI_MASTER_CHNL_UDMA_RX_EN)
		{
			stat = UDMA0_ChannelConfigure( gspi->rx_dma->channel,
					(uint32_t)&(gspi->reg->GSPI_READ_FIFO),(uint32_t)&dummy_data,
					num,
					gspi->rx_dma->control,
					&gspi->rx_dma->chnl_cfg,
					gspi->rx_dma->cb_event);
			if (stat == -1)
			{
				return ARM_DRIVER_ERROR;
			}
			gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_RD = 0x1;
		}
		else
		{
			return ARM_DRIVER_ERROR;
		}
		if(RTE_GSPI_MASTER_CHNL_UDMA_TX_EN)
		{
			stat = UDMA0_ChannelConfigure( gspi->tx_dma->channel,
					(uint32_t)(gspi->xfer->tx_buf),	(uint32_t)&(gspi->reg->GSPI_WRITE_FIFO),
					num,
					gspi->tx_dma->control,
					&gspi->tx_dma->chnl_cfg,
					gspi->tx_dma->cb_event);
			if (stat == -1)
			{
				return ARM_DRIVER_ERROR;
			}
			GSPI0->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2 =data_bits;
			gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_WR  = 0x1; //write enable
			GSPI0->GSPI_WRITE_DATA2_b.USE_PREV_LENGTH = 0x1;
		}
		else
		{
			return ARM_DRIVER_ERROR;
		}
		UDMA0_ChannelEnable (gspi->tx_dma->channel);   
		UDMA0_ChannelEnable (gspi->rx_dma->channel);
		UDMA0_DMAEnable();
	}
	else
	{
		gspi->reg->GSPI_INTR_UNMASK |=(GSPI_INTR_UNMASK_BIT|GSPI_FIFO_AEMPTY_WFIFO_UNMASK);
		gspi->reg->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2 =data_bits;
		gspi->reg->GSPI_WRITE_FIFO_b->WRITE_FIFO= (uint32_t)gspi->xfer->tx_buf[0];
		gspi->xfer->tx_cnt++;
		gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_WR = 0x1; //write enable
		while(gspi->reg->GSPI_STATUS & GSPI_BUSY_F);
	}
	return ARM_DRIVER_OK;
}
int32_t GSPI_Receive(void *data, uint32_t num, const GSPI_RESOURCES *gspi)
{
	volatile int32_t stat=0;
	static uint8_t dummy_data=0,data_bits=0;
	gspi->info->status.busy       = 0U;

	if ((data == NULL) || (num == 0U))        { return ARM_DRIVER_ERROR_PARAMETER; }
	if (!(gspi->info->state & SPI_CONFIGURED)) { return ARM_DRIVER_ERROR; }

	if (  gspi->info->status.busy)
	{
		return ARM_DRIVER_ERROR_BUSY;
	}
	gspi->info->status.busy       = 1U;
	gspi->info->status.data_lost  = 0U;
	gspi->info->status.mode_fault = 0U;

	gspi->xfer->rx_buf = NULL;
	gspi->xfer->num    = num;
	gspi->xfer->rx_cnt = 0U;
	gspi->xfer->tx_cnt = 0U;

	gspi->xfer->rx_buf = (uint8_t *)data;

	data_bits= GSPI0->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2;

	if ((gspi->rx_dma)||(gspi->tx_dma))
	{
		if(RTE_GSPI_MASTER_CHNL_UDMA_RX_EN)
		{
			stat = UDMA0_ChannelConfigure( gspi->rx_dma->channel,
					(uint32_t)&(gspi->reg->GSPI_READ_FIFO),(uint32_t)(gspi->xfer->rx_buf),
					num,
					gspi->rx_dma->control,
					&gspi->rx_dma->chnl_cfg,
					gspi->rx_dma->cb_event);
			if (stat == -1)
			{
				return ARM_DRIVER_ERROR;
			}
			gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_RD = 0x1;
		}
		else
		{
			return ARM_DRIVER_ERROR;
		}
		if(RTE_GSPI_MASTER_CHNL_UDMA_TX_EN)
		{
			stat = UDMA0_ChannelConfigure( gspi->tx_dma->channel,
					(uint32_t)&(dummy_data),(uint32_t)&(gspi->reg->GSPI_WRITE_FIFO),
					num,
					gspi->tx_dma->control,
					&gspi->tx_dma->chnl_cfg,
					gspi->tx_dma->cb_event);
			if (stat == -1)
			{
				return ARM_DRIVER_ERROR;
			}
			GSPI0->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2 =data_bits;
			gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_WR  = 0x1; //write enable
			GSPI0->GSPI_WRITE_DATA2_b.USE_PREV_LENGTH = 0x1;
		}
		else
		{
			return ARM_DRIVER_ERROR;
		}
		UDMA0_ChannelEnable (gspi->tx_dma->channel);
		UDMA0_ChannelEnable (gspi->rx_dma->channel);
		UDMA0_DMAEnable();
	}
	else
	{
		gspi->reg->GSPI_INTR_UNMASK |=(GSPI_INTR_UNMASK_BIT |GSPI_FIFO_AEMPTY_RFIFO_UNMASK);
	}
	return ARM_DRIVER_OK;
}
static int32_t GSPI_Transfer(const void *data_out, void *data_in, uint32_t num, const GSPI_RESOURCES *gspi)
{
	volatile int32_t stat,data_bits=0;

	if ((data_out == NULL) || (data_in == NULL) || (num == 0U))        { return ARM_DRIVER_ERROR_PARAMETER; }
	if (!(gspi->info->state & SPI_CONFIGURED)) { return ARM_DRIVER_ERROR; }
	if (  gspi->info->status.busy)             { return ARM_DRIVER_ERROR_BUSY; }
	gspi->info->status.busy       = 1U;
	gspi->info->status.data_lost  = 0U;
	gspi->info->status.mode_fault = 0U;

	gspi->xfer->rx_buf = (uint8_t *)data_in;;
	gspi->xfer->tx_buf = (uint8_t *)data_out;

	gspi->xfer->num    = num;
	gspi->xfer->rx_cnt = 0U;
	gspi->xfer->tx_cnt = 0U;

	data_bits= GSPI0->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2;

	if (gspi->tx_dma)
	{
		if(RTE_GSPI_MASTER_CHNL_UDMA_TX_EN)
		{
			stat = UDMA0_ChannelConfigure( gspi->tx_dma->channel,
					(uint32_t)(gspi->xfer->tx_buf),	(uint32_t)&(gspi->reg->GSPI_WRITE_FIFO),
					num,
					gspi->tx_dma->control,
					&gspi->tx_dma->chnl_cfg,
					gspi->tx_dma->cb_event);
			if (stat == -1)
			{
				return ARM_DRIVER_ERROR;
			}
			GSPI0->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2 =data_bits;
			gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_WR = 0x1; //write enable
			GSPI0->GSPI_WRITE_DATA2_b.USE_PREV_LENGTH = 0x1;
		}
	}
	if (gspi->rx_dma)
	{
		if(RTE_GSPI_MASTER_CHNL_UDMA_TX_EN)
		{
			stat = UDMA0_ChannelConfigure( gspi->rx_dma->channel,
					(uint32_t)&(gspi->reg->GSPI_READ_FIFO),(uint32_t)(gspi->xfer->rx_buf),
					num,
					gspi->rx_dma->control,
					&gspi->rx_dma->chnl_cfg,
					gspi->rx_dma->cb_event);
			if (stat == -1)
			{
				return ARM_DRIVER_ERROR;
			}
			gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_RD =0x1;
			UDMA0_ChannelEnable (gspi->tx_dma->channel);
			UDMA0_ChannelEnable (gspi->rx_dma->channel);
			UDMA0_DMAEnable();
		}
	}
	else
	{
		gspi->reg->GSPI_INTR_UNMASK |=(GSPI_INTR_UNMASK_BIT |GSPI_FIFO_AEMPTY_RFIFO_UNMASK |GSPI_FIFO_AEMPTY_WFIFO_UNMASK);
		GSPI0->GSPI_WRITE_DATA2_b.GSPI_MANUAL_WRITE_DATA2 =data_bits;
		gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_WR =0x1; //write enable
		gspi->reg->GSPI_WRITE_FIFO_b->WRITE_FIFO= (uint32_t)gspi->xfer->tx_buf[0];
		while( gspi->reg->GSPI_STATUS & GSPI_BUSY_F);
		gspi->xfer->tx_cnt++;
	}
	return ARM_DRIVER_OK;
}
uint32_t GSPI_GetDataCount(const GSPI_RESOURCES *gspi)
{
	if (!(gspi->info->state & SPI_CONFIGURED)) { return 0U; }

	return gspi->xfer->rx_cnt;
}

ARM_SPI_STATUS GSPI_GetStatus(const GSPI_RESOURCES *gspi)
{
	ARM_SPI_STATUS status;

	return status;
}

void GSPI_IRQHandler (const GSPI_RESOURCES *gspi)
{
	uint32_t  data_bits=0;
	uint32_t  event=0;
	uint32_t gspi_status = 0;
	uint32_t int_status = 0;

	gspi_status= gspi->reg->GSPI_STATUS;

	int_status=gspi->reg->GSPI_INTR_STS;

	data_bits=gspi->reg->GSPI_WRITE_DATA2;

	/*wait for GSPI idle*/
	while (gspi->reg->GSPI_STATUS_b.GSPI_BUSY);

	if(gspi->reg->GSPI_STATUS_b.GSPI_BUSY != 1)
	{
		gspi->reg->GSPI_WRITE_DATA2 = data_bits;
	}

	if (((gspi_status & BIT(3)) ==  BIT(3))&&(int_status &BIT(0)))
	{		
		gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_WR = 0x1;
		if (gspi->xfer->tx_buf)
		{
			gspi->reg->GSPI_WRITE_FIFO_b->WRITE_FIFO =(uint32_t)gspi->xfer->tx_buf[gspi->xfer->tx_cnt] ;
		}
		while (gspi->reg->GSPI_STATUS_b.GSPI_BUSY);

		gspi->xfer->tx_cnt++;

		if(gspi->xfer->num==gspi->xfer->tx_cnt)
		{
			gspi->reg->GSPI_INTR_MASK &=~GSPI_FIFO_AEMPTY_WFIFO_MASK;
			gspi->reg->GSPI_INTR_MASK &=~GSPI_FIFO_AFULL_WFIFO_MASK;
			gspi->info->status.busy = 0U;
		}
	}
	if (((gspi_status & GSPI_FIFO_EMPTY_RFIFO_S) != 1)&&(gspi_status & BIT(8)) &&(int_status & BIT(0)))
	{
		if((gspi->reg->GSPI_STATUS_b.GSPI_BUSY != 1))
		{
			if(!(gspi->reg->GSPI_STATUS & GSPI_FIFO_EMPTY_RFIFO_S))
			{
				if (gspi->xfer->rx_buf)
				{
					/* Trigger the Read of GSPI Controller*/
					gspi->reg->GSPI_CONFIG1_b.GSPI_MANUAL_RD =0x1;

					gspi->xfer->rx_buf[gspi->xfer->rx_cnt]=GSPI0->GSPI_READ_FIFO_b->READ_FIFO;
					while (gspi->reg->GSPI_STATUS_b.GSPI_BUSY);
					gspi->xfer->rx_cnt++;
					if(gspi->xfer->num==gspi->xfer->rx_cnt)
					{
						gspi->reg->GSPI_INTR_MASK &= ~GSPI_FIFO_EMPTY_RFIFO_MASK;
						// Clear busy flag
						gspi->info->status.busy = 0U;
						// Transfer completed
						event |= ARM_SPI_EVENT_TRANSFER_COMPLETE;
						gspi->info->cb_event(event);
					}
				}
				else
				{
					gspi->xfer->rx_cnt++;
					if(gspi->xfer->rx_cnt==((gspi->xfer->num)-1))
					{
						// Clear busy flag
						gspi->info->status.busy = 0U;

						gspi->info->cb_event(ARM_SPI_EVENT_TRANSFER_COMPLETE);
					}
				}
			}
		}
	}
}
#if (RTE_GSPI_MASTER_CHNL_UDMA_TX_EN )
static void GSPI_UDMA_Tx_Event (uint32_t event,uint8_t dmaCh, GSPI_RESOURCES *gspi)
{
	switch (event)
	{
	case UDMA_EVENT_XFER_DONE:
		gspi->info->status.busy       = 0U;
		// Update TX buffer info
		gspi->xfer->tx_cnt = gspi->xfer->num;
		break;
	case UDMA_EVENT_ERROR:
		break;
	}

}
#endif
#if (RTE_GSPI_MASTER_CHNL_UDMA_RX_EN )
static void GSPI_UDMA_Rx_Event (uint32_t event,uint8_t dmaCh, GSPI_RESOURCES *gspi)
{
	switch (event)
	{
	case UDMA_EVENT_XFER_DONE:
		gspi->xfer->rx_cnt    = gspi->xfer->num;
		gspi->info->status.busy = 0U;
		break;
	case UDMA_EVENT_ERROR:
		break;
	}
	if (gspi->info->cb_event != NULL) {
		gspi->info->cb_event(ARM_SPI_EVENT_TRANSFER_COMPLETE);
	}

}
#endif
#if (RTE_GSPI_MASTER_CHNL_UDMA_TX_EN == 1)
void GSPI_MASTER_UDMA_Tx_Event (uint32_t event, uint8_t dmaCh)
{
	GSPI_UDMA_Tx_Event (event,dmaCh, &GSPI_MASTER_Resources);
}
#endif

#if (RTE_GSPI_MASTER_CHNL_UDMA_RX_EN == 1)
void GSPI_MASTER_UDMA_Rx_Event (uint32_t event,uint8_t dmaCh)
{
	GSPI_UDMA_Rx_Event (event,dmaCh, &GSPI_MASTER_Resources);
}
#endif



#if RTE_GSPI_MASTER
static int32_t        GSPI_MASTER_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return GSPI_Initialize (pSignalEvent, &GSPI_MASTER_Resources); }
static int32_t        GSPI_MASTER_Uninitialize        (void)                                              { return GSPI_Uninitialize (&GSPI_MASTER_Resources); }
static int32_t        GSPI_MASTER_PowerControl        (ARM_POWER_STATE state)                             { return GSPI_PowerControl (state, &GSPI_MASTER_Resources); }
static int32_t        GSPI_MASTER_Send                (const void *data, uint32_t num)                    { return GSPI_Send (data, num, &GSPI_MASTER_Resources); }
static int32_t        GSPI_MASTER_Receive             (void *data, uint32_t num)                          { return GSPI_Receive (data, num, &GSPI_MASTER_Resources); }
static int32_t        GSPI_MASTER_Transfer            (const void *data_out, void *data_in, uint32_t num) { return GSPI_Transfer (data_out, data_in, num, &GSPI_MASTER_Resources); }
static uint32_t       GSPI_MASTER_GetDataCount        (void)                                              { return GSPI_GetDataCount (&GSPI_MASTER_Resources); }
static int32_t        GSPI_MASTER_Control             (uint32_t control, uint32_t arg)                    { return GSPI_Control (control, arg, &GSPI_MASTER_Resources); }
static ARM_SPI_STATUS GSPI_MASTER_GetStatus           (void)                                              { return GSPI_GetStatus (&GSPI_MASTER_Resources); }
void          		  GSPI_MASTER_IRQHandler            (void)
{
	GSPI_IRQHandler (&GSPI_MASTER_Resources);
}

// End SPI Interface

ARM_DRIVER_SPI Driver_GSPI_MASTER = {
		GSPI_MASTER_GetVersion,
		GSPI_MASTER_GetCapabilities,
		GSPI_MASTER_Initialize,
		GSPI_MASTER_Uninitialize,
		GSPI_MASTER_PowerControl,
		GSPI_MASTER_Send,
		GSPI_MASTER_Receive,
		GSPI_MASTER_Transfer,
		GSPI_MASTER_GetDataCount,
		GSPI_MASTER_Control,
		GSPI_MASTER_GetStatus
};
#endif
