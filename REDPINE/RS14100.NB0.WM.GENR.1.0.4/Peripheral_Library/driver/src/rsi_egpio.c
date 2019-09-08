/**
 * @file       rsi_egpio.c
 * @version    1.0
 * @date       1 AUG ,2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes related to EGPIO peripheral
 * 
 * @section Description
 * This file contains the list of function prototypes for the EGPIO and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */
#include "rsi_chip.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/


/* This API is used to set the EGPIO direction(Direction of the GPIO pin. '1' for INPUT, '0' for OUTPUT) */
void egpio_set_dir(EGPIO_Type *pEGPIO ,uint8_t port,uint8_t pin, boolean_t dir)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.DIRECTION = dir;
}

/* This API is used to set the GPIO pin value.It Loads 0th bit on to the pin on write & reads the value on pin on read into 0th bit */
void egpio_set_pin(EGPIO_Type *pEGPIO ,uint8_t port,uint8_t pin , uint8_t val)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].BIT_LOAD_REG = val;
}

/* This API is used get the GPIO pin status.*/
boolean_t egpio_get_pin(EGPIO_Type *pEGPIO ,uint8_t port,uint8_t pin)
{
	return pEGPIO->PIN_CONFIG[(port * 16)+ pin].BIT_LOAD_REG;
}

/* This API is used to Get the Direction GPIO(Direction of the GPIO pin. '1' for INPUT,and '0'for OUTPUT) */
boolean_t egpio_get_dir(EGPIO_Type *pEGPIO,uint8_t port ,uint8_t pin)
{
	return pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.DIRECTION;
}

/*  This API is used to select the pin for interrupt generation */
void egpio_pin_int_sel(EGPIO_Type *pEGPIO ,uint8_t intCh ,uint8_t port , uint8_t pin)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.PIN_NUMBER = pin;
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.PORT_NUMBER = port;
}

/*  This API is used to set the pin interrupt mode configuration(enables interrupt generation when falling edge is detected on pin '1' for intr enabled and '0' for disabled) */
void egpio_set_int_fall_edge_enable(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.FALL_EDGE_ENABLE = 1U;
}

/* This API to used to set the pin interrupt mode configuration (enables interrupt generation when falling edge is detected on pin '1' for intr enabled and '0' for disabled) */
void egpio_set_int_fall_edge_disable(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.FALL_EDGE_ENABLE = 0U;
}

/*
 This API to used to set the pin interrupt mode configuration(enables interrupt generation when rising edge is detected on pin '1' for intr enabled and '0' for disabled)
 */
void egpio_set_int_rise_edge_enable(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.RISE_EDGE_ENABLE = 1U;
}

/*This API to used to set the pin interrupt mode configuration (enables interrupt generation when rising edge is detected on pin '1' for intr enabled '0' for disabled) */
void egpio_set_int_rise_edge_disable(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.RISE_EDGE_ENABLE = 0U;
}

/* This API is used to set the pin interrupt mode configuration (enables interrupt generation when pin level is 0, '1' for intr enabled, '0' for disabled)  */
void egpio_set_int_low_level_enable(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.LEVEL_LOW_ENABLE = 1U;
}

/*This API is used to set the pin interrupt mode configuratio(Masks the interrupt. Interrupt will still be seen in status register when enabled '1' for intr masked '0' for intr unmasked) */
void egpio_int_mask(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.MASK = 1U;
}

/*This API is used to used to set the pin interrupt mode configuration
(Masks the interrupt. Interrupt will still be seen in status register when enabled '1' for intr masked '0' for intr unmasked) */
void egpio_int_un_mask(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.MASK = 0U;
}

/* This API is used to set the pin interrupt mode configuration(enables interrupt generation when pin level is 0 ,'1' for intr enabled '0' for disabled) */
void egpio_set_int_low_level_disable(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.LEVEL_LOW_ENABLE = 0U;
}

/*This API used to set the pin interrupt mode configuration(enables interrupt generation when pin level is 1, '1' for intr enabled '0' for disabled) */
void egpio_set_int_high_level_enable(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.LEVEL_HIGH_ENABLE = 1U;
}

/* This API is used to used to set the pin interrupt mode configuration(enables interrupt generation when pin level is 1 ,'1' for intr enabled '0' for disabled) */
void egpio_set_int_high_level_disable(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	pEGPIO->INTR[intCh].GPIO_INTR_CTRL_b.LEVEL_HIGH_ENABLE = 0U;
}

/* This API is used to get the pin interrupt status register */
uint8_t egpio_get_int_stat(EGPIO_Type *pEGPIO ,uint8_t intCh)
{
	return pEGPIO->INTR[intCh].GPIO_INTR_STATUS;
}

/* This API is used to used to clear the pin interrupt in status register */
void egpio_int_clr(EGPIO_Type *pEGPIO ,uint8_t intCh , uint8_t flags)
{
	pEGPIO->INTR[intCh].GPIO_INTR_STATUS = flags;
}

/*  This API is used to used to set pin multiplexing GPIO Pin Mode. Ranges 000 -> Mode 0 to 111 -> Mode 7 Used for GPIO Pin Muxing) */
void egpio_set_pin_mux(EGPIO_Type *pEGPIO ,uint8_t port , uint8_t pin , uint8_t mux)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.MODE =(mux);
}

/* This API is used set ulp soc gpio mode (Gpio pin mode,ranges 000 -> Mode 0 to 111 -> Mode 7 Used for GPIO Pin Muxing ) */
void egpio_ulp_soc_gpio_mode(ULPClk_Type *pULPCLK,uint8_t gpio,uint8_t mode)
{
	pULPCLK->ULP_SOC_GPIO_MODE_REG[gpio].ULP_SOC_GPIO_0_MODE_REG_b.ULP_SOCGPIO_N_MODE=(mode);	 
}

/* This API is used to set the EGPIO port mask. When set, pin is masked when written/read through PORT MASK REG. */
void egpio_set_port_mask(EGPIO_Type *pEGPIO ,uint8_t port,uint8_t pin)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.PORTMASK = 1U;
}

/*  This API is used to set the EGPIO port unmask. When set, pin is masked when written/read through PORT MASK REG. */
void egpio_set_port_un_mask(EGPIO_Type *pEGPIO ,uint8_t port,uint8_t pin)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.PORTMASK = 0U;
}

/* This API is used to set the EGPIO port mask load. When set, pin is masked when written/read through PORT MASK REG. */
void egpio_port_masked_load(EGPIO_Type *pEGPIO ,uint8_t port,  uint16_t val)
{
	pEGPIO->PORT_CONFIG[port].PORT_MASKED_LOAD_REG_b.PORT_MASKED_LOAD =val;
}

/* This API is used to set the port value Sets the pin when corresponding bit is high. Writing zero has no effect */
void egpio_set_port(EGPIO_Type *pEGPIO ,uint8_t port , uint16_t val)
{
	pEGPIO->PORT_CONFIG[port].PORT_SET_REG = (val);
}

/*  This API is used to set the port value Sets the pin when corresponding bit is high. Writing zero has no effect */
void egpio_port_load(EGPIO_Type *pEGPIO ,uint8_t port , uint16_t val)
{
	pEGPIO->PORT_CONFIG[port].PORT_LOAD_REG = (val);
}

/* This API is used to set the port value Sets the pin when corresponding bit is high. Writing zero has no effect */
void egpio_word_load(EGPIO_Type *pEGPIO ,uint8_t pin , uint16_t val)
{
	pEGPIO->PIN_CONFIG[pin].WORD_LOAD_REG_b.WORD_LOAD = (val);
}

/* This API is used to clear the port value Clears the pin when corresponding bit is high. Writing zero has no effect. */
void egpio_clr_port(EGPIO_Type *pEGPIO ,uint8_t port , uint16_t val)
{
	pEGPIO->PORT_CONFIG[port].PORT_CLEAR_REG = (val);
}

/* This API is used to toggle the port Toggles the pin when corresponding bit is high. Writing zero has not effect. */
void egpio_toggle_port(EGPIO_Type *pEGPIO ,uint8_t port , uint16_t val)
{
	pEGPIO->PORT_CONFIG[port].PORT_TOGGLE_REG = (val);
}

/*This API is used to used to get the EGPIO port value  Reads the value on GPIO pins irrespective of the pin mode. */
uint16_t egpio_get_port(EGPIO_Type *pEGPIO ,uint8_t port)
{
	return (pEGPIO->PORT_CONFIG[port].PORT_READ_REG & 0XFFFF);
}

/* This API is used to enable the group interrupt one ,  When set,the corresponding GPIO pin is selected for group interrupt 1 generation */
void egpio_group_int_one_enable(EGPIO_Type *pEGPIO,uint8_t port,uint8_t pin)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.GROUP_INTERRUPT1_ENABLE = 1U;
}

/* This API is used to used to disable the group interrupt one */
void egpio_group_int_one_disable(EGPIO_Type *pEGPIO,uint8_t port,uint8_t pin)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.GROUP_INTERRUPT1_ENABLE = 0U;
}

/* This API is used to enable the group interrupt Two ,  When set,the corresponding GPIO pin is selected for group interrupt 2 generation */
void egpio_group_int_two_enable(EGPIO_Type *pEGPIO,uint8_t port,uint8_t pin)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.GROUP_INTERRUPT2_ENABLE = 1U;
}

/* This API is used to configure the group interrupts(1-mask,0-unmask) */
void egpio_group_int_mask(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.MASK = 1u;
}

/* This API is used to configure the group interrupts(1-mask,0-unmask) */
void egpio_group_int_un_Mask(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.MASK = 0u;
}

/* This API is used to configure the group interrupts(1-enable, 0-disable) */
void egpio_group_int_enable(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.ENABLE_INTERRUPT = 1u;
}

/* This API is used to configure the group interrupts(1-enable, 0-disable) */
void egpio_group_int_disable(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.ENABLE_INTERRUPT = 0u;
}

/* This API is used to configure the group interrupts(0-level,1-edge) */
void egpio_group_int_level(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.LEVEL_EDGE = 0u;
}

/* This API is used to configure the group interrupts(0-level,1-edge) */
void egpio_group_int_edge(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.LEVEL_EDGE = 1u;
}

/* This API is used to configure the group interrupts(0-AND ,1-Or) */
void egpio_group_int_and(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.AND_OR = 0u;
}

/* This API is used to configure the group interrupts(0- AND , 1-Or) */
void egpio_group_int_or(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.AND_OR = 1u;
}

/* This API to used to get the group interrupt status */
uint32_t egpio_group_int_stat(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	return pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_STS;
}

/* This API to used to Enable the group interrupt wakeup interrupt */
void egpio_group_int_wkeup_Enable(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.ENABLE_WAKEUP = 1;
}

/* This API to used to Disable the group interrupt wakeup interrupt */
void egpio_group_int_wkeup_disable(EGPIO_Type *pEGPIO ,uint8_t grpInt)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_CTRL_REG_b.ENABLE_WAKEUP = 0;
}

/* This API is used to used to clear the group interrupt status */
void egpio_group_int_clr(EGPIO_Type *pEGPIO ,uint8_t grpInt , uint8_t u8ClrFlags)
{
	pEGPIO->GPIO_GRP_INTR[grpInt].GPIO_GRP_INTR_STS = (u8ClrFlags);
}

/* This API is used to used to disable the group interrupt two */
void egpio_group_int_two_disable(EGPIO_Type *pEGPIO ,uint8_t port ,uint8_t pin)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.GROUP_INTERRUPT2_ENABLE = 0U;
}

/* This API is used to set the group polarity of interrupt one Decides the active value of the pin to be considered for group interrupt 1 generation when enabled  */
void egpio_set_group_int_one_pol(EGPIO_Type *pEGPIO ,uint8_t port , uint8_t pin , uint8_t pol)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.GROUP_INTERRUPT1_POLARITY = pol;
}

/* This API is used to set the group polarity of interrupt two. Decides the active value of the pin to be considered for group interrupt 2 generation when enabled */
void egpio_set_group_int_two_pol(EGPIO_Type *pEGPIO ,uint8_t port,uint8_t pin , uint8_t pol)
{
	pEGPIO->PIN_CONFIG[(port * 16)+ pin].GPIO_CONFIG_REG_b.GROUP_INTERRUPT2_POLARITY = pol;
}

/* This API is used to select the host pad gpios(26 to 30) */
void egpio_host_pads_gpio_mode_enable(uint8_t u8GpioNum)
{
	if(u8GpioNum==25)
	{
		HOST_PADS_GPIO_MODE |=(0x1<<13);
	}
	else if(u8GpioNum==26)
	{
		HOST_PADS_GPIO_MODE |=(0x1<<14);
	}
	else if(u8GpioNum==27)
	{
		HOST_PADS_GPIO_MODE |=(0x1<<15);
	}
	else if(u8GpioNum==28)
	{
		HOST_PADS_GPIO_MODE |=(0x1<<16);
	}
	else if(u8GpioNum==29)
	{
		HOST_PADS_GPIO_MODE |=(0x1<<17);
	}
	else if(u8GpioNum==30)
	{
		HOST_PADS_GPIO_MODE |=(0x1<<18);
	}
}

/*  This API is used to deselect the host pad gpios(26 to 30) */
void egpio_host_pads_gpio_mode_disable(uint8_t u8GpioNum)
{
	if(u8GpioNum==25)
	{
		HOST_PADS_GPIO_MODE &= ~(0x1<<13);
	}
	else if(u8GpioNum==26)
	{
		HOST_PADS_GPIO_MODE &= ~(0x1<<14);
	}
	else if(u8GpioNum==27)
	{
		HOST_PADS_GPIO_MODE &= ~(0x1<<15);
	}
	else if(u8GpioNum==28)
	{
		HOST_PADS_GPIO_MODE &= ~(0x1<<16);
	}
	else if(u8GpioNum==29)
	{
		HOST_PADS_GPIO_MODE &= ~(0x1<<17);
	}
	else if(u8GpioNum==30)
	{
		HOST_PADS_GPIO_MODE &= ~(0x1<<18);
	}
}

/*  This API is used to select the pad(0 to 21) */
void egpio_pad_selection_enable(uint8_t padNum)
{
	/*(tass_m4ss_gpio_sel)PAD selection (0 t0 21) A value of 1 on this gives control to M4SS(by default it is 0 means ta control) */
	PADSELECTION |=(0x1<< padNum);
}

/*  This API is used to deselect the pad(0 to 21) */
void egpio_pad_selection_disable(uint8_t padNum)
{
	/*(tass_m4ss_gpio_sel)PAD selection (0 t0 21) A value of 1 on this gives control to M4SS(by default it is 0 means ta control) */
	PADSELECTION &= ~(0x1<< padNum);
}

/* This API is used to enable the receiver enable bit(REN) */
void egpio_pad_receiver_enable(uint8_t u8GpioNum)
{
	/*REN enable bit(this should be enable)*/
	PAD_CONFIG_REG(u8GpioNum) |= (0x1<<4);
}

/* This API is used to Disable the receiver enable bit(REN) */
void egpio_pad_receiver_disable(uint8_t u8GpioNum)
{
	/*REN enable bit(this should be enable)*/
	PAD_CONFIG_REG(u8GpioNum) &= ~(0x1<<4);
}

/* This API is used to use the SDIO pins(25 to 30) in M4 or TA (0 for M4SS and 1 for TASS) */
void egpio_pad_sdio_connected()
{
	/*sdio connected to tass  (0 for M4SS and 1 for TASS) for SDIO pads(25 to 30)*/
	SDIO_CNTD_TO_TASS  =(0x1 << 5);
}

/* This API is used to control the Driver disabled state control */
void egpio_pad_driver_disable_state(uint8_t u8GpioNum , en_driver_state_t  endstate)
{
	uint16_t reg =0 ;
	reg = PAD_CONFIG_REG(u8GpioNum) ;
	reg &= ~DDS_MASK;
	reg |= (endstate << 6);
	PAD_CONFIG_REG(u8GpioNum)  = reg ;
}

/* This API is used to select Drive strength */
void egpio_pad_driver_strength_select(uint8_t u8GpioNum , en_driver_strength_select_t strength)
{
	uint16_t reg =0 ;
	reg = PAD_CONFIG_REG(u8GpioNum) ;
	reg &= ~DSS_MASK;
	reg |= (strength << 0);
	PAD_CONFIG_REG(u8GpioNum)  = reg ;
}

/*  This API is used to select Power on Start enable
POS = 1 : Enables active pull down for invalid power;
POS = 0 : Active pull down capability disabled .
When one of the power supplies is invalid and active high POS is set to 1,
AD is pulled to weak 0. When POS is set to 0, PAD remains in a high-Z state. : Default 0 */
void egpio_pad_power_on_start_enable(uint8_t u8GpioNum ,uint8_t val )
{
	uint16_t reg =0 ;
	reg = PAD_CONFIG_REG(u8GpioNum) ;
	reg &= ~POS_MASK;
	reg |= (val << 2);
	PAD_CONFIG_REG(u8GpioNum)  = reg ;
}

/* Active high Schmitt trigger (Hysteresis) select;
 SMT=0 for No hysteresis; Default value for reset is 1'b1 and others is 1'b0 */
void egpio_pad_active_high_schmitt_trigger(uint8_t u8GpioNum ,uint8_t val )
{
	uint16_t reg =0 ;
	reg = PAD_CONFIG_REG(u8GpioNum) ;
	reg &= ~ST_MASK;
	reg |= (val << 3);
	PAD_CONFIG_REG(u8GpioNum)  = reg ;
}

/* This API is used to control the slew rate */
void egpio_pad_slew_rate_controll(uint8_t u8GpioNum ,uint8_t val )
{
	uint16_t reg =0 ;
	reg = PAD_CONFIG_REG(u8GpioNum) ;
	reg &= ~SR_MASK;
	reg |= (val << 5);
	PAD_CONFIG_REG(u8GpioNum)  =   reg ;
}

/*  This API is used to enable the REN for ULP */
void egpio_ulp_pad_receiver_enable(uint8_t u8GpioNum)
{
	ULP_PAD_CONFIG_REG_2 |=(0x1<<u8GpioNum);
}

/* This API is used to enable the REN for ULP */
void egpio_ulp_pad_receiver_disable(uint8_t u8GpioNum)
{
	ULP_PAD_CONFIG_REG_2 &= ~(0x1<<u8GpioNum);
}

/* This API is used to control the Driver disabled state control */
void egpio_ulp_pad_driver_disable_state(uint8_t u8GpioNum , en_ulp_driver_disable_state_t  disablestate)
{
	uint16_t reg =0 ;
	switch(u8GpioNum)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_DDS_MASK_SET_1_AND_3;
		reg |= (disablestate << 6);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_DDS_MASK_SET_2_AND_4;
		reg |= (disablestate << 14);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_DDS_MASK_SET_1_AND_3;
		reg |= (disablestate << 6);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_DDS_MASK_SET_2_AND_4;
		reg |= (disablestate << 14);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;

	}
}

/* This API is used to select Drive strength */
void egpio_ulp_pad_driver_strength_select(uint8_t u8GpioNum , en_ulp_driver_strength_select_t strength)
{
	uint16_t reg =0 ;
	switch(u8GpioNum)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_DSS_MASK_SET_1_AND_3;
		reg |= (strength << 0);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_DSS_MASK_SET_2_AND_4;
		reg |= (strength << 8);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_DSS_MASK_SET_1_AND_3;
		reg |= (strength << 0);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_DSS_MASK_SET_2_AND_4;
		reg |= (strength << 8);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;
	}
}

/* Power-on-Start enable */
void egpio_ulp_pad_power_on_start_enable(uint8_t u8GpioNum ,uint8_t val )
{
	uint16_t reg =0 ;
	switch(u8GpioNum)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_POS_MASK_SET_1_AND_3;
		reg |= (val << 2);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_POS_MASK_SET_2_AND_4;
		reg |= (val << 10);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_POS_MASK_SET_1_AND_3;
		reg |= (val << 2);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_POS_MASK_SET_2_AND_4;
		reg |= (val << 10);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;

	}
}

/* Active high Schmitt trigger (Hysteresis) select */
void egpio_ulp_pad_active_high_schmitt_trigger(uint8_t u8GpioNum ,uint8_t val )
{
	uint16_t reg =0 ;
	switch(u8GpioNum)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_ST_MASK_SET_1_AND_3;
		reg |= (val << 3);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_ST_MASK_SET_2_AND_4;
		reg |= (val << 11);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_ST_MASK_SET_1_AND_3;
		reg |= (val << 3);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_ST_MASK_SET_2_AND_4;
		reg |= (val << 11);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;

	}
}

/* Slew Rate Control*/
void egpio_ulp_pad_slew_rate_controll(uint8_t u8GpioNum ,uint8_t val )
{
	uint16_t reg =0 ;
	switch(u8GpioNum)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_SR_MASK_SET_1_AND_3;
		reg |= (val << 5);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		reg = ULP_PAD_CONFIG_REG_0;
		reg &= ~ULP_SR_MASK_SET_2_AND_4;
		reg |= (val << 13);
		ULP_PAD_CONFIG_REG_0 = reg ;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_SR_MASK_SET_1_AND_3;
		reg |= (val << 5);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		reg = ULP_PAD_CONFIG_REG_1;
		reg &= ~ULP_SR_MASK_SET_2_AND_4;
		reg |= (val << 13);
		ULP_PAD_CONFIG_REG_1 = reg ;
		break;

	}
}

/*End of file not truncated */
