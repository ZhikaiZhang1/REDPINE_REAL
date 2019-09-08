#include "rsi_chip.h" 
/*==============================================*/
/**
 * @fn          void RSI_ETH_SetVlanTag(RSI_ETH_HANDLE_T pETHHandle, uint16_t VLANTag)
 * @brief       This API is used to set the VLAN Tag to identify the VLAN frames, and is used to
								compare with the 15th and 16th bytes of the receiving frames for VLAN frames.
 * @param[in]    pETHHandle, Ethernet Driver handler  
 * @param[in]    VLANTag, the InEEE 802.1Q VLAN Tag to identify the VLAN frames.
 * @param[out]   none
 * @return       none
 * @section description
 * This API is used to set the VLAN Tag to identify the VLAN frames, and is used to
 * compare with the 15th and 16th bytes of the receiving frames for VLAN frames.
 *
 *
 */

void RSI_ETH_SetVlanTag(RSI_ETH_HANDLE_T pETHHandle, uint16_t VLANTag)
{

	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
   //! Set the  VLAN Tag 
   pETH->VLAN_TAG_REG7 =  VLANTag;
	
}



/*==============================================*/
/**
 * @fn           void RSI_ETH_ResetVlanTag(RSI_ETH_HANDLE_T pETHHandle)
 * @brief        Resets the VLAN Tag register
 * @param[in]    pETHHandle, Ethernet Driver handler  
 * @param[out]   none
 * @return       none
 *
 *
 * @section description
 * This Resets the VLAN Tag register
 *
 *
 */

void RSI_ETH_ResetVlanTag(RSI_ETH_HANDLE_T pETHHandle)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
   //! Reset the VLAN Tag
   pETH->VLAN_TAG_REG7 =  0x0000;
}



/*==============================================*/
/**
 * @fn          void RSI_ETH_TXEnable(RSI_ETH_HANDLE_T pETHHandle)
 * @brief       This API is used to set the transmit state machine of the GMAC
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[out]  none
 * @return      none
 *
 *
 * @section description
 * This API is used to set the transmit state machine of the GMAC
 *
 *
 */

void RSI_ETH_TXEnable(RSI_ETH_HANDLE_T pETHHandle)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
  //! GMAC Transmit Enable  
  pETH->MAC_CONFIG_REG0 |= MAC_ENABLE_TE;

}


/*==============================================*/
/**
 * @fn            void RSI_ETH_TXDisable(RSI_ETH_HANDLE_T pETHHandle)
 * @brief         This API is used to reset the transmit state machine of the GMAC
 * @param[in]     pETHHandle, Ethernet Driver handler   
 * @param[out]    none
 * @return        none
 *
 *
 * @section description
 * This API is used to set the transmit state machine of the GMAC
 *
 *
 */

 void RSI_ETH_TXDisable(RSI_ETH_HANDLE_T pETHHandle)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
  //! GMAC Transmit Disable  
  pETH->MAC_CONFIG_REG0 &= ~MAC_ENABLE_TE;
}


/*==============================================*/
/**
 * @fn          void RSI_ETH_RXEnable(RSI_ETH_HANDLE_T pETHHandle)  
 * @brief 	    This API is used to set the receive state machine of the GMAC
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[out]  none
 * @return      none
 *
 *
 * @section description
 * This API is used to set the receive state machine of the GMAC
 *
 *
 */

 void RSI_ETH_RXEnable(RSI_ETH_HANDLE_T pETHHandle)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	//! GMAC Receiver Enable  
  pETH->MAC_CONFIG_REG0 |= MAC_ENABLE_RE;
}



/*==============================================*/
/**
 * @fn          void RSI_ETH_RXDisable(RSI_ETH_HANDLE_T pETHHandle)  
 * @brief 	    This API is used to reset the receive state machine of the GMAC
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[out]  none
 * @return      none
 *
 *
 * @section description
 * This API is used to reset the receive state machine of the GMAC
 *
 *
 */

 void RSI_ETH_RXDisable(RSI_ETH_HANDLE_T pETHHandle)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	//! GMAC Receiver Disable  
  pETH->MAC_CONFIG_REG0 &= ~MAC_ENABLE_RE;
}


/*==============================================*/
/**
 * @fn          void RSI_ETH_EnableRXFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask) 
 * @brief       This API is used to select the RX filter for receiving frames
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[in]   mask, RX filters each bit enables specific filter
 *               MAC_FILTER_RA   
 *						   MAC_FILTER_HPF         
 *               MAC_FILTER_SAF         
 *               MAC_FILTER_SAIF        
 *               MAC_FILTER_DBF         
 *               MAC_FILTER_PM          
 *               MAC_FILTER_DAIF         
 *               MAC_FILTER_HMC         
 *               MAC_FILTER_HUC         
 *               MAC_FILTER_PR         
 * @param[out]   none
 * @return       none
 *
 *
 * @section description
 * This API is used to select the RX filter for receiving frames
 *
 *
 */

void RSI_ETH_EnableRXFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	pETH->MAC_FRAME_FILTER_REG1 &= ~MAC_FILTER_RA;
	pETH->MAC_FRAME_FILTER_REG1 |=  mask;
}


/*==============================================*/
/**
 * @fn          void RSI_ETH_DisableRXFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask) 
 * @brief       This API is used to select the disable RX filter for receiving frames
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[in]   mask, RX filters each bit disables specific filter
 *               MAC_FILTER_RA   
 *						   MAC_FILTER_HPF         
 *               MAC_FILTER_SAF         
 *               MAC_FILTER_SAIF        
 *               MAC_FILTER_DBF         
 *               MAC_FILTER_PM          
 *               MAC_FILTER_DAIF         
 *               MAC_FILTER_HMC         
 *               MAC_FILTER_HUC         
 *               MAC_FILTER_PR         
 * @param[out]   none
 * @return       none
 *
 *
 * @section description
 * This API is used to disable the selected  RX filter for receiving frames
 *
 *
 */

 void RSI_ETH_DisableRXFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	pETH->MAC_FRAME_FILTER_REG1 &=  mask;
}


/*==============================================*/
/**
 * @fn          void RSI_ETH_SetHashFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t HashFilterL, uint32_t HashFilterH) 
 * @brief       This API is used to set the 64-bit Hash table which is used for group address filtering.
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[in]   HashFilterL , the lower 32 bits of the Hash table
 * @param[in]   HashFilterH , the higher 32 bits of the Hash table. 
 * @param[out]  none
 * @return      none  
 *
 *
 * @section description
 * This API is used to set the 64-bit Hash table which is used for group address filtering.
 *
 *
 */

 void RSI_ETH_SetHashFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t HashFilterL, uint32_t HashFilterH)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
   //! Set the  MSB of hash filter
   pETH->HASH_TABLE_HIGH_REG2 =  HashFilterH;

   pETH->HASH_TABLE_LOW_REG3 =  HashFilterL;

}


/*==============================================*/
/**
 * @fn          void RSI_ETH_GMACEnableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
 * @brief       This API is used to enable the GMAC interrupt
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[out]  mask, interupts to be enabled 
								GMAC_PMT_INTR	
 * @return      none  
 *
 *
 * @section description
 * This API is used to enable the GMAC interrupt 
 *
 *
 */
 void RSI_ETH_GMACEnableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	pETH->INTR_MASK_REG15 |= ~mask;
}

/*==============================================*/
/**
 * @fn          void RSI_ETH_GMACDisableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
 * @brief       This API is used to disable the GMAC interrupt
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[in]  mask, interupts to be disabled
								GMAC_PMT_INTR	
 * @return      none  
 *
 *
 * @section description
 * This API is used to disable the GMAC interrupt 
 *
 *
 */
void RSI_ETH_GMACDisableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	pETH->INTR_MASK_REG15 &= mask; 
}



/*==============================================*/
/**
 * @fn          void RSI_ETH_DMA_EnableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask) 
 * @brief       This API Enables the respective DMA interrupt  
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[in]   mask, interrupt mask to enable 
 *								DMA_TIE_INTR
 *                DMA_TSE_INTR
 *								DMA_TUE_INTR
 *								DMA_TJE_INTR
 *								DMA_OVE_INTR	
 *								DMA_UNE_INTR
 *								DMA_RIE_INTR
 *								DMA_RUE_INTR
 *								DMA_RSE_INTR
 *							  DMA_RWE_INTR
 *								DMA_ETE_INTR	
 *								DMA_FBE_INTR	
 *								DMA_ERE_INTR
 *								
 * @param[out]    none
 * @return        none
 *
 *
 * @section description
 * This API Enables the respective DMA interrupt  
 *
 *
 */

void RSI_ETH_DMA_EnableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  pETH->IER_REG7 |= mask;

}

/*==============================================*/
/**
 * @fn          void RSI_ETH_DMA_DisableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
 * @brief       This API disables the respective DMA interrupt  
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[in]   mask, interrupt mask to disable
 *								DMA_TIE_INTR
 *                DMA_TSE_INTR
 *								DMA_TUE_INTR
 *								DMA_TJE_INTR
 *								DMA_OVE_INTR	
 *								DMA_UNE_INTR
 *								DMA_RIE_INTR
 *								DMA_RUE_INTR
 *								DMA_RSE_INTR
 *							  DMA_RWE_INTR
 *								DMA_ETE_INTR	
 *								DMA_FBE_INTR	
 *								DMA_ERE_INTR
 *								
 * @param[out]    none
 * @return        none
 *
 *
 * @section description
 * This API disables the respective DMA interrupt  
 *
 */
void RSI_ETH_DMA_DisableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  pETH->IER_REG7 &= (~mask);
}



/*==============================================*/
/**
 * @fn           RSI_ETH_HANDLE_T RSI_ETH_MAC_Initialize(void *drv_mem, uint32_t base_address, uint32_t mask) 	
 * @brief        This API is used to initialise the MAC layer of ethernet
 * @param[in]    drv_mem, Driver memory used to initialise the 
 * @param[out]   base_address, Ethernet base address
 * @return       driver memory handler 
 * @section description
 * This API is used to initialise the MAC layer of ethernet
 *
 *
 */
RSI_ETH_HANDLE_T RSI_ETH_MAC_Initialize(void *drv_mem, uint32_t base_address) 	
{
	RSI_ETH_DRIVER_T *pdrv = (RSI_ETH_DRIVER_T *)drv_mem;
	
	ETHERNET_Type  *pregs = RSI_NULL;

  if(drv_mem == RSI_NULL)
  {
    return RSI_NULL;
    
  }
  else
  {
    memset((void *)pdrv,0, sizeof(RSI_ETH_DRIVER_T));

    pdrv->pREGS = (ETHERNET_Type *)base_address;

    pregs = pdrv->pREGS;

  }  
	 pregs->INTR_MASK_REG15 = 0xFFFF;
	
	return (RSI_ETH_HANDLE_T *)pdrv;
	
	
}





/*==============================================*/
/**
 * @fn         error_t RSI_ETH_MAC_SetMacAddress(RSI_ETH_HANDLE_T pETHHandle, int8_t *mac) 	
 * @brief      This API is used to set the address of the MAC
 * @param[in]  pETHHandle, Ethernet Driver handler 
 * @param[in]  mac, mac address to assign the MAC layer
 * @param[out] none  
 * @return     0 : success 
 *             error code : in case of error 
 *
 *
 * @section description
 * This API is used to set the address of the MAC 
 *
 *
 */
error_t RSI_ETH_MAC_SetMacAddress(RSI_ETH_HANDLE_T pETHHandle, int8_t *mac) 	
{
	
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;

  //! Check if MAC is valid or not
  if(!(mac[0] & mac[1] & mac[2] & mac[3] & mac[4] & mac[5]))
  {
    // return ERROR_ETH_PARAM;
  }
  //!This field contains the upper 16 bits (47:32) of the 6-byte first MAC address
  pETH->MAC_ADDR0_HIGH_REG_b.MAC_ADDR0_HIGH= (uint16_t)(mac[0] | (mac[1] << 8));

  //! This field contains the lower 32 bits of the 6-byte first MAC address
  pETH->MAC_ADDR0_LOW_REG =  (uint32_t)(mac[5] | (mac[4] << 8) | (mac[3] << 16) | (mac[2] << 24));  

  return RSI_OK;

}



/*==============================================*/
/**
 * @fn           error_t RSI_ETH_MAC_GetMacAddress(RSI_ETH_HANDLE_T pETHHandle,  int8_t *mac) 	
 * @brief        Get the MAC address of the Ethernet  
 * @param[in]    pETHHandle, Ethernet Driver handler 
 * @param[in]    mac,pointer to store MAC address of the ethernet interface to get 
 * @param[out]   none
 * @return     0 : success 
 *             error code : in case of error 
 *
 * @section description
 * This API is used to get the address of the MAC 
 *
 *
 */
error_t RSI_ETH_MAC_GetMacAddress(RSI_ETH_HANDLE_T pETHHandle,  int8_t *mac) 	
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
 
  if(mac == 0)
  {
    return ERROR_ETH_PARAM;
  }
  mac[0] = (pETH->MAC_ADDR0_HIGH_REG_b.MAC_ADDR0_HIGH  & 0xFF);

  mac[1] = ((pETH->MAC_ADDR0_HIGH_REG_b.MAC_ADDR0_HIGH  & 0xFF00) >> 8);

  mac[2] = ((pETH->MAC_ADDR0_LOW_REG >> 24)  & 0xFF); 
	
  mac[3] = ((pETH->MAC_ADDR0_LOW_REG >> 16)  & 0xFF); 

	mac[4] = ((pETH->MAC_ADDR0_LOW_REG >> 8)  & 0xFF); 
	
	mac[5] = ((pETH->MAC_ADDR0_LOW_REG )  & 0xFF); 
	
  return RSI_OK;
}



/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_ResetAddressFilter(RSI_ETH_HANDLE_T pETHHandle,uint32_t num_addr) 	
 * @brief       This  API is used to reset or remove MAC filter for that MAC address
 * @param[in]   pETHHandle, Ethernet Driver handler 
 * @param[out]  num_addr, Address index 
 * @return      0 : success 
 *              error code : in case of error 
 *
 *
 * @section description
 * This  API is used to reset or remove MAC filter for that MAC address
 *
 *
 */

error_t RSI_ETH_MAC_ResetAddressFilter(RSI_ETH_HANDLE_T pETHHandle,uint32_t num_addr) 	
{

  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  if((num_addr == 0) || (num_addr > 31))
  {
    return ERROR_ETH_PARAM;
  }
  if(num_addr <= 15)
  {
    //! Disable Address filtering for this mac
    pETH->MAC_16_ADDR[num_addr].MAC_ADDR_HIGH_REG_b.AE = 0;
  }
  else
  { 
    //! Disable Address filtering for this mac
    pETH->MAC_31_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.AE = 0;
  }

  return RSI_OK;
}


/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_SetSourceAddressFilter(RSI_ETH_HANDLE_T pETHHandle, const uint8_t *mac_addr,uint32_t num_addr, uint8_t addr_mask) 	
 * @brief       This API is used to set the MAC Address filter for coming frames on source address  with mask for the bytes
 * @param[in]   pETHHandle, Ethernet Driver handler 
 * @param[in]   mac_addr, MAC address to filter on source address on incoming frames 
 * @param[in]   num_addr, address index MAX 4 address filtering are supported
 * @param[in]     addr_mask, These bits are mask control bits for comparison of each of the MAC Address
 *								bytes. When set high, the GMAC core does not compare the corresponding byte
 *								of received DA/SA with the contents of Mac Address1 registers.
 *								BIT(0) - when set masks/ignores the filtering on the 1st /LSB byte of MAC address
 *                BIT(1) - when set masks/ignores the filtering on the 2nd byte of MAC address       
 *								BIT(2) - when set masks/ignores the filtering on the 3rd byte of MAC address       
 *								BIT(3) - when set masks/ignores the filtering on the 4th byte of MAC address       
 *								BIT(4) - when set masks/ignores the filtering on the 5th byte of MAC address       
 *								BIT(5) - when set masks/ignores the filtering on the 6th/MSB byte of MAC address       
 * @return     0 : success 
 *             error code : in case of error 
 *
 * @section description
 * This API is used to set the MAC Address filter for coming frames on source address  with mask for the bytes
 *
 *
 */

error_t RSI_ETH_MAC_SetSourceAddressFilter(RSI_ETH_HANDLE_T pETHHandle, const uint8_t *mac_addr,uint32_t num_addr, uint8_t addr_mask) 	
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  if((num_addr == 0) || (num_addr > 4))
  {
    return ERROR_ETH_PARAM;
  }
    //! This field contains the lower 32 bits of the 6-byte first MAC address
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_LOW_REG  = (uint32_t)(mac_addr[5] | (mac_addr[4] << 8) | (mac_addr[3] << 16) | (mac_addr[2] << 24));

    //!This field contains the upper 16 bits (47:32) of the 6-byte first MAC address
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.MAC_ADDR1_HIGH = (uint16_t)(mac_addr[0] | (mac_addr[1] << 8));

    //! Enable Source address bit
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.SA = 1;

    //! Enable Address filtering for this mac
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.AE = 1;

    //! Enable Address filtering for this mac
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.MBC_ADDR1_HIGH = addr_mask;
  
  return RSI_OK;
}




/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_SetDestinationAddressFilter(RSI_ETH_HANDLE_T pETHHandle, const uint8_t *mac_addr,uint32_t num_addr, uint8_t addr_mask) 	
 * @brief       This API is used to set the MAC Address filter for coming frames on destination address  with mask for the bytes
 * @param[in]   pETHHandle, Ethernet Driver handler 
 * @param[in]   mac_addr, MAC address to filter on source address on incoming frames 
 * @param[in]   num_addr, address index MAX 4 address filtering are supported
 * @param[in]     addr_mask, These bits are mask control bits for comparison of each of the MAC Address
 *								bytes. When set high, the GMAC core does not compare the corresponding byte
 *								of received DA/SA with the contents of Mac Address1 registers.
 *								BIT(0) - when set masks/ignores the filtering on the 1st /LSB byte of MAC address
 *                BIT(1) - when set masks/ignores the filtering on the 2nd byte of MAC address       
 *								BIT(2) - when set masks/ignores the filtering on the 3rd byte of MAC address       
 *								BIT(3) - when set masks/ignores the filtering on the 4th byte of MAC address       
 *								BIT(4) - when set masks/ignores the filtering on the 5th byte of MAC address       
 *								BIT(5) - when set masks/ignores the filtering on the 6th/MSB byte of MAC address       
 * @return     0 : success 
 *             error code : in case of error 
 *
 * @section description
 * This API is used to set the MAC Address filter for coming frames on destination address  with mask for the bytes
 *
 *
 */
error_t RSI_ETH_MAC_SetDestinationAddressFilter(RSI_ETH_HANDLE_T pETHHandle,const uint8_t *mac_addr, uint32_t 	num_addr, uint8_t addr_mask) 	
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  if((num_addr == 0) || (num_addr > 4))
  {
    return ERROR_ETH_PARAM;
  }

    //! This field contains the lower 32 bits of the 6-byte first MAC address
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_LOW_REG = (uint32_t)(mac_addr[5] | (mac_addr[4] << 8) | (mac_addr[3] << 16) | (mac_addr[2] << 24));

    //!This field contains the upper 16 bits (47:32) of the 6-byte first MAC address
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.MAC_ADDR1_HIGH = (uint16_t)(mac_addr[0] | (mac_addr[1] << 8));

    //! Enable Source address bit
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.SA = 1;

    //! Enable Address filtering for this mac
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.AE = 1;

    //! Enable Address filtering for this mac
    pETH->MAC_16_ADDR[num_addr - 1].MAC_ADDR_HIGH_REG_b.MBC_ADDR1_HIGH = addr_mask;
  
  return RSI_OK;
}








/*==============================================*/
/**
 * @fn           error_t RSI_ETH_MAC_DMAInit(RSI_ETH_HANDLE_T pETHHandle) 
 * @brief        This API is used to initialise the DMA 
 * @param[in]    pETHHandle, Ethernet Driver handler  
 * @param[out]   none
 * @return       success
 * @section description
 * This API is used to initialise the DMA 
 *
 *
 */

error_t RSI_ETH_MAC_DMAInit(RSI_ETH_HANDLE_T pETHHandle) 
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	//! Apply soft reset to DMA
	
	pETH->BUS_MODE_REG0_b.SWR = 1;
	
	//! Wait untill soft reset is done
	while(pETH->BUS_MODE_REG0 & DMA_BUS_MODE_SOFTREST);
	
  //! Hard coded Burst length to 64
  pETH->BUS_MODE_REG0_b.PBL =  1;

	//! Hard coded Burst length to 64
  pETH->BUS_MODE_REG0_b.RPBL =  1;

	pETH->BUS_MODE_REG0_b.PBL_MODE = 1;
	
	pETH->BUS_MODE_REG0_b.DSL = 4;
  //! DMA Arbitration scheme
  //! Rx has priority over Tx
  pETH->BUS_MODE_REG0_b.DA = 1;


  return RSI_OK;

}









/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_BuildRXDescChain(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_RxXferConfig_t *rcvcfg, RSI_ETH_DMA_RXDESC_t  *pDesc, RSI_ETH_DMA_RXDESC_t  *pNextDesc) 
 * @brief       This API is used to build the  descriptor for DMA to trigger RX
 * @param[in]   pETHHandle, Ethernet Driver handler  
 * @param[in]   rcvcfg, DMA configuration structure for the given descriptor 
 * @param[in]   pDesc,  Descriptor pointer which has to be configured
 * @param[in]   pNextDesc, Next descriptor pointer which indicates that the present descriptor is not the end of chain descriptor
 * @param[out]  none
 * @return      0 : success 
 *              error code : in case of error 
 *
 *
 * @section description
 * This API is used to build the  descriptor for DMA to trigger RX
 *
 *
 */
error_t RSI_ETH_MAC_BuildRXDescChain(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_RxXferConfig_t *rcvcfg, RSI_ETH_DMA_RXDESC_t  *pDesc, RSI_ETH_DMA_RXDESC_t  *pNextDesc) 
{


  if((pDesc == RSI_NULL) || (rcvcfg->dst == RSI_NULL)) 
  {
    return ERROR_ETH_NULL;
  }

  /* Passed descriptor must be correctly aligned */
  if (((uint32_t) pDesc & 0xF) != 0)
  {
    //return ERR_DMA_NOT_ALIGNMENT;
  }
  /* Passed descriptor must be correctly aligned */
  if (((uint32_t) rcvcfg->dst & 0xF) != 0)
  {
    //return ERR_DMA_NOT_ALIGNMENT;
  }
  //! If buffer size is zero
  if((uint32_t)rcvcfg->recvCount == 0)
  {
    return ERROR_ETH_PARAM;
  }
  //! Make DMA own 
  pDesc->RXDESC0 =   (DMA_RXDESC_OWN);
  //! 
  pDesc->RXDESC1 =   (DMA_RXDESC_RCH);
  //! if fire CB is disbled
  if(!rcvcfg->fireDescCB)
  {
    pDesc->RXDESC1  |=   (DMA_RXDESC_DIOC);
  }
  if(pNextDesc == RSI_NULL)
  {
    //!This is the last descriptorin the descriptor chain 
    pDesc->RXDESC1 |= DMA_RXDESC_RER; 

  }
  else
  {
    pDesc->RXDESC3 = (uint32_t)pNextDesc;
  }
	
	 pDesc->RXDESC2 = (uint32_t)rcvcfg->dst;
	
   pDesc->RXDESC1 = 	0x7FF | ((uint32_t) (0x7FF) << 11) | BIT(24);
  return RSI_OK;
}










/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_BuildTXDescChain(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_TxXferConfig_t *xfercfg, RSI_ETH_DMA_TXDESC_t  *pDesc, RSI_ETH_DMA_TXDESC_t  *pNextDesc) 
 * @brief       This API is used to build the  descriptor for DMA to trigger TX
 * @param[in]   pETHHandle, Ethernet Driver handler  
 * @param[in]   xfercfg, DMA configuration structure for the given descriptor 
 * @param[in]   pDesc,  Descriptor pointer which has to be configured
 * @param[in]   pNextDesc, Next descriptor pointer which indicates that the present descriptor is not the end of chain descriptor
 * @param[out]  none
 * @return      0 : success 
 *              error code : in case of error 
 *
 *
 * @section description
 * This API is used to build the  descriptor for DMA to trigger TX
 *
 *
 */
error_t RSI_ETH_MAC_BuildTXDescChain(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_TxXferConfig_t *xfercfg, RSI_ETH_DMA_TXDESC_t  *pDesc, RSI_ETH_DMA_TXDESC_t  *pNextDesc) 
{


  if((pDesc == RSI_NULL) || (xfercfg->src == RSI_NULL)) 
  {
    return ERROR_ETH_NULL;
  }

  /* Passed descriptor must be correctly aligned */
  if (((uint32_t) pDesc & 0xF) != 0)
  {
    //return ERR_DMA_NOT_ALIGNMENT;
  }
  /* Passed descriptor must be correctly aligned */
  if (((uint32_t) xfercfg->src & 0xF) != 0)
  {
     //return ERR_DMA_NOT_ALIGNMENT;
  }
  //! If transfer count is 0 return error  
  if(xfercfg->xferCount == 0)
  {
    return ERROR_ETH_PARAM;
  }
  //!  Fill the transfer size
  pDesc->TXDESC1 = xfercfg->xferCount;

  //! keep the descriptor for DMA own 
  pDesc->TXDESC0 =   (DMA_TXDESC_OWN);

  pDesc->TXDESC1 |=   (DMA_TXDESC_TCH);

  if(xfercfg->fireDescCB)
  {
    //!Raise the interrupt after the completionof this descriptor
    pDesc->TXDESC1 |= DMA_TXDESC_IC;
  }
  if(xfercfg->sof) 
  {
    //!This descriptor contains the start of frame
    pDesc->TXDESC1 |=  DMA_TXDESC_FS;
		pDesc->TXDESC1 |= (uint32_t)(xfercfg->checksum << 27);
  }
  if(xfercfg->eof)
  {
    //!This descriptor contains the end of frame
    pDesc->TXDESC1 |=  DMA_TXDESC_LS;
  }
  if(pNextDesc == RSI_NULL)
  {
    //!This is the last descriptorin the descriptor chain 
    pDesc->TXDESC1 |= DMA_TXDESC_TER; 
  }
  else
  {
    //! else point the next descriptor 
    pDesc->TXDESC3 = (uint32_t)pNextDesc;
  }

  if(xfercfg->dc) 
  {
    //! Disable CRC   
    pDesc->TXDESC1 |= DMA_TXDESC_DC; 
  }
	
	pDesc->TXDESC2 = (uint32_t)xfercfg->src;


  return RSI_OK;

}


/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_StopTx(RSI_ETH_HANDLE_T pETHHandle)  
 * @brief       This API is used to indicate signal to dma to stop the TX transmission of frame 
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[out]  none
 * @return      success
 * @section description
 *  This API is used to indicate signal to dma to stop the TX transmission of frame 
 *
 *
 */
error_t RSI_ETH_MAC_StopTx(RSI_ETH_HANDLE_T pETHHandle)
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;

  //! Stop transmission of this frame
  pETH->OPR_MODE_REG6_b.ST = 0;

	return RSI_OK;

}

/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_StartTx(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_TXDESC_t  *pDesc)
 * @brief       This API is used to indicate signal to dma to start the TX transmission of frame 
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[in]   pDesc, TX descriptor to configure DMA
 * @param[out]  none
 * @return      0 : success 
 *              error code : in case of error 
 * @section description
 *  This API is used to indicate signal to dma to start the TX transmission of frame 
 */
error_t RSI_ETH_MAC_StartTx(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_TXDESC_t  *pDesc)
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  //! check if descriptor is valid or not
  if(pDesc == RSI_NULL) 
  {
    return ERROR_ETH_NULL;
  }

  /* Passed descriptor must be correctly aligned */
  if (((uint32_t) pDesc & 0xF) != 0)
  {
    return ERR_DMA_NOT_ALIGNMENT;
  }
  //! copy the TX descriptor start address 
  pETH->TX_DESC_LIST_ADDR_REG4 = (uint32_t)pDesc;


  //! Start transmission of this frame
  pETH->OPR_MODE_REG6_b.ST = 1;


  return RSI_OK;

}


/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_StartRx(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_RXDESC_t  *pDesc)
 * @brief       This API is used to indicate signal to dma to start the reception of frame 
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[in]   pDesc, RX descriptor to configure DMA
 * @param[out]  none
 * @return      0 : success 
 *              error code : in case of error 
 * @section description
 * This API is used to indicate signal to dma to start the reception of frame 
 *
 *
 */
error_t RSI_ETH_MAC_StartRx(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_RXDESC_t  *pDesc)
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  //! check if descriptor is valid or not
  if(pDesc == RSI_NULL) 
  {
    return ERROR_ETH_NULL;
  }

  /* Passed descriptor must be correctly aligned */
  if (((uint32_t) pDesc & 0xF) != 0)
  {
    return ERR_DMA_NOT_ALIGNMENT;
  }
  //! copy the TX descriptor start address 
  pETH->RC_DESC_LIST_ADDR_REG3 = (uint32_t)pDesc;

  //! Start transmission of this frame
  pETH->OPR_MODE_REG6_b.SR = 1;
 
	return RSI_OK;


}


/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_StopRx(RSI_ETH_HANDLE_T pETHHandle)  
 * @brief       This API is used to indicate signal to dma to stop the RX
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[out]  none
 * @return      success
 * @section description
 *  This API is used to indicate signal to dma to stop the RX
 *
 *
 */

error_t RSI_ETH_MAC_StopRx(RSI_ETH_HANDLE_T pETHHandle)
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;

  pETH->OPR_MODE_REG6_b.SR = 0;
  
	return RSI_OK;
}



/*==============================================*/
/**
 * @fn           error_t RSI_ETH_MAC_RegisterCallBacks(RSI_ETH_HANDLE_T pETHHandle, uint32_t callback_index, void (*eth_cb)(RSI_ETH_HANDLE_T , uint32_t event , void *arg) )  
 * @brief        This API is used to register the call backs for Ethernet events
 * @param[in]    pETHHandle, Ethernet Driver handler   
 * @param[in]    callback_index, indicates the call back is registered for GMAC interrupts or DMA interrupts
									0 - GMAC_INTERRUPT	
									1 - DMA_INTERRUPT 
 * @param[in]    call back address
 * @param[out]   none
 * @return       none
 *
 *
 * @section description
 * This API is used to register the call backs for Ethernet events
 *
 *
 */

error_t RSI_ETH_MAC_RegisterCallBacks(RSI_ETH_HANDLE_T pETHHandle, uint32_t callback_index, void (*eth_cb)(RSI_ETH_HANDLE_T , uint32_t event , void *arg) )
{
  RSI_ETH_DRIVER_T *pDrv = ((RSI_ETH_DRIVER_T *)pETHHandle);
	
	if(callback_index > MAX_ENET_CALL_BACKS)
  {
    return ERROR_ETH_CALLBACK_ERR;
  }
  else
  { 
    pDrv->cbTable[callback_index] = eth_cb;

    return RSI_OK;
  }
	
}

/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_Control(RSI_ETH_HANDLE_T pETHHandle, uint32_t  control, uint32_t	arg) 		
 * @brief       This API is used to set the MAC Control options 
 * @param[in]    pETHHandle, Ethernet Driver handler   
 * @param[in]   control , this is the argument to control MAC configuration
								RSI_CONFIG_MAC 
								RSI_CONFIG_FLOWCONTRL
 * @param[in]	  arg, argumhent w.r.t the control arguement
 *              RSI_CONFIG_MAC  :  if the control argument is  RSI_CONFIG_MAC ,the following options
								are vaild	
										MAC_DISABLE_WATCHDOG
										MAC_DISABLE_JABBER
										MAC_ENABLE_JUMBO_FRAME
										MAC_DISABLE_DCRS
										MAC_DISABLE_DCRS
										MAC_FES_100
										MAC_DISABLE_DO
										MAC_ENABLE_LM		
										MAC_ENABLE_FULL_DUPLEX
										MAC_ENABLE_IPC
										MAC_DISABLE_RETRY
										MAC_ENABLE_DC	
										MAC_ENABLE_TE	
										MAC_ENABLE_RE	
								RSI_CONFIG_FLOWCONTRL  :  if the control argument is  RSI_CONFIG_FLOWCONTRL ,the following options
								are vaild	
                    GMAC_FLOW_CNTL_DZPQ
	  		     				GMAC_FLOW_CNTL_UP
					      		GMAC_FLOW_CNTL_RFE
							      GMAC_FLOW_CNTL_TFE
					      		GMAC_FLOW_CNTL_FCB
										
							  	
 * @param[out]  none
 * @return      none
 *
 * @section description
 * This 
 *
 *
 */
error_t RSI_ETH_MAC_Control(RSI_ETH_HANDLE_T pETHHandle, uint32_t  control, uint32_t	arg) 		
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  switch(control)
  {

    case RSI_CONFIG_MAC:
      {
        pETH->MAC_CONFIG_REG0 = arg;
      }
      break;
    case RSI_CONFIG_FLOWCONTRL:
      {
        pETH->MAC_FRAME_FILTER_REG1 = arg;
      }
      break;
    default:
      {
         return ERROR_ETH_PARAM;
      }
  }
   return RSI_OK;

}




/*==============================================*/
/**
 * @fn          error_t RSI_ETH_MAC_SetOPerMode(RSI_ETH_HANDLE_T pETHHandle, uint32_t control)
 * @brief       This API is used to to Set operating mode of the DMA 
 * @param[in]   pETHHandle, Ethernet Driver handler 
 * @param[in]   control, opermode options
 * @param[out]  none
 * @return      success
 * @section description
 *  This API is used to to Set operating mode of the DMA
 *
 *
 */

error_t RSI_ETH_MAC_SetOPerMode(RSI_ETH_HANDLE_T pETHHandle, uint32_t control)
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;

  pETH->OPR_MODE_REG6 = control;
	
	return RSI_OK;

}





/*==============================================*/
/**
 * @fn         error_t RSI_ETH_PHY_PowerControl(int8_t enable, int8_t  control) 
 * @brief      This API is used to control the power down mode
 * @param[in]    pETHHandle, Ethernet Driver handler   
 * @param[in]  enable , Enable(1)/Disable(0)the control option
 * @param[in]  control option,   POWER_DOWN                 
 *                               WAKEUP_ON_MAGIC_PACKET    
 *                               WAKEUP_ON_WAKEUP_FRAME    
 *                               WAKEUP_FILTER_RESET      
 * @param[out]                            
 * @section description
 * This API is used to control the power down mode
 *
 *
 */
error_t RSI_ETH_PHY_PowerControl(RSI_ETH_HANDLE_T pETHHandle, int8_t enable, int8_t  control) 
{

  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
  //! If the bit is enabled
  if(enable) 
  {
    pETH->PMT_CTRL_STS_REG |= control;  
  }
  else
  {
   //! To disable that control
   pETH->PMT_CTRL_STS_REG &= ~control;  
    
  }
  return RSI_OK;

}






/*==============================================*/
/**
 * @fn           error_t RSI_ETH_PHY_SetWakeUpFilter(RSI_ETH_HANDLE_T pETHHandle,RSI_ETH_PMT_WakeupFrameFilter_t *filter)  
 * @brief        This API is used to load the Wakeup filter frame
 * @param[in]    pETHHandle, Ethernet Driver handler   
 * @param[in]    filter, Filter frame structure pointer
 * @param[out]   none
 * @return       none
 *
 * @section description
 * This API is used to load the Wakeup filter frame
 *
 *
 */
error_t RSI_ETH_PHY_SetWakeUpFilter(RSI_ETH_HANDLE_T pETHHandle,RSI_ETH_PMT_WakeupFrameFilter_t *filter) 
{
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;

  //! Copy the filter contents into the Wakeup frame Filter register one after the other
  pETH->RMT_WKUP_FMFILTER_REG = filter->byte_mask[0];
  
  pETH->RMT_WKUP_FMFILTER_REG = filter->byte_mask[1];
  
  pETH->RMT_WKUP_FMFILTER_REG = filter->byte_mask[2];
  
  pETH->RMT_WKUP_FMFILTER_REG = filter->byte_mask[3];
  
  pETH->RMT_WKUP_FMFILTER_REG = filter->command[0] | (filter->command[1] << 8)| (filter->command[2] << 16) | (filter->command[3] << 24 );
  
  pETH->RMT_WKUP_FMFILTER_REG = filter->offset[0] | (filter->offset[1] << 8)| (filter->offset[2] << 16) | (filter->offset[3] << 24 );
  
  pETH->RMT_WKUP_FMFILTER_REG = filter->crc[0] | (filter->crc[1] << 16);
  
  pETH->RMT_WKUP_FMFILTER_REG = filter->crc[3] | (filter->crc[2] << 16);

  return RSI_OK;



}



/*==============================================*/
/**
 * @fn          uint32_t RSI_ETH_DMA_GetTXStatus(RSI_ETH_HANDLE_T pETHHandle)
 * @brief       This function returns the TX status of DMA
 * @param[in]   pETHHandle, Ethernet Driver handler 
 * @param[out]  none
 * @return      DMA TX current status
 *							0: Stopped; Reset or Stop Transmit Command issued.
 *              1: Running; Fetching Transmit Transfer Descriptor.
 *						  2: Running; Waiting for status.
 *						  3: Running; Reading Data from host memory buffer and queuing it
 *						     to transmit buffer (Tx FIFO).
 *						  6: Suspended; Transmit Descriptor Unavailable or Transmit Buffer
 *								 Underflow.
 *							7: Running; Closing Transmit Descriptor
 *
 * @section description
 * This API is used to get the current DMA TX Status
 *
 */
uint32_t RSI_ETH_DMA_GetTXStatus(RSI_ETH_HANDLE_T pETHHandle)
{
	
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	return pETH->STATUS_REG5_b.TS;

}



/*==============================================*/
/**
 * @fn           uint32_t RSI_ETH_DMA_GetRXStatus(RSI_ETH_HANDLE_T pETHHandle)
 * @brief        This function returns the RX status of DMA
 * @param[in]    pETHHandle, Ethernet Driver handler  
 * @param[out]   none
 * @return       DMA RX current status
 *								0 : Stopped; Reset or Stop Receive Command issued.
 * 							  1 : Running; Fetching Receive Transfer Descriptor. 
 *                3 : Running; Waiting for receive packet.
 *                4 : Suspended; Receive Descriptor Unavailable.
 *                5 : Running; Closing Receive Descriptor.
 *                7 : Running; Transferring the receive packet data from receive buffer
 * 	                  to host memory.
 *
 * @section description
 * This API is used to get the current DMA RX Status
 *
 *
 */
uint32_t RSI_ETH_DMA_GetRXStatus(RSI_ETH_HANDLE_T pETHHandle)
{
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	return pETH->STATUS_REG5_b.RS;
}



/*==============================================*/
/**
 * @fn          void RSI_ETH_DMAIntrHandler(RSI_ETH_HANDLE_T pETHHandle)
 * @brief       This API is the handler for DMA Interrupt 
 * @param[in]   pETHHandle, Ethernet Driver handler   
 * @param[out]  none
 * @return      none
 *
 *
 * @section description
 * This API is the handler for DMA Interrupt 
 *
 *
 */

void RSI_ETH_DMAIntrHandler(RSI_ETH_HANDLE_T pETHHandle)
{
  RSI_ETH_DRIVER_T *pDrv =  ((RSI_ETH_DRIVER_T *)pETHHandle);
	
	ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	uint32_t intr_status;
	
	intr_status  = pETH->STATUS_REG5;
	
	 pETH->STATUS_REG5 = intr_status;

	if((intr_status & DMA_TI_INTR_STATUS) == DMA_TI_INTR_STATUS)
	{
		//! Clear the Transmit interrupt bit
		//pETH->STATUS_REG5_b. = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_TI, RSI_NULL);
		}
		
	}
	
	if((intr_status & DMA_TPS_INTR_STATUS) == DMA_TPS_INTR_STATUS)
	{
		//! Clear the Transmit process stopped 
		// pETH->STATUS_REG5_b.TPS = 1;
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_TPS, RSI_NULL);
		}
		
	}
	
	if((intr_status & DMA_TU_INTR_STATUS) == DMA_TU_INTR_STATUS)
	{
		//! Clear the Transmit Buffer Unavailable
		// pETH->STATUS_REG5_b.TU =  1;
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_TU, RSI_NULL);
		}
		
	}
	
	if((intr_status & DMA_TJT_INTR_STATUS) == DMA_TJT_INTR_STATUS)
	{
		//! Clear the Transmit Jabber Timer expired
		// pETH->STATUS_REG5_b.TJT = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_TJT, RSI_NULL);
		}
		
	}
	
	if((intr_status & DMA_OVF_INTR_STATUS) == DMA_OVF_INTR_STATUS)
	{
		//! Clear the Receive Overflow
		// pETH->STATUS_REG5_b.OVF = 1;
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)			
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_OVF, RSI_NULL);
		}
		
	}
	
	if((intr_status & DMA_UNF_INTR_STATUS) == DMA_UNF_INTR_STATUS)
	{
		//! Clear the Transmit Underflow
		// pETH->STATUS_REG5_b.TU = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_UNF, RSI_NULL);
		}
		
	}

	if((intr_status & DMA_RI_INTR_STATUS) == DMA_RI_INTR_STATUS)
	{
		//! Clear the Receive Interrupt
		// pETH->STATUS_REG5_b.RI  = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_RI, RSI_NULL);
		}
		
	}

	if((intr_status & DMA_RU_INTR_STATUS) == DMA_RU_INTR_STATUS)
	{
		//! Clear the Receive Buffer Unavailable
		// pETH->STATUS_REG5_b.RU = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_RU, RSI_NULL);
		}
		
	}
	if((intr_status & DMA_RPS_INTR_STATUS) == DMA_RPS_INTR_STATUS)
	{
		//! Clear the Receive Process Stopped
		// pETH->STATUS_REG5_b.RPS = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_RPS, RSI_NULL);
		}
		
	}

	if((intr_status & DMA_RWT_INTR_STATUS) == DMA_RWT_INTR_STATUS)
	{
		//! Clear the RWT: Receive Watchdog Timeout
		// pETH->STATUS_REG5_b.RWT = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_RWT, RSI_NULL);
		}
		
	}


	if((intr_status & DMA_ETI_INTR_STATUS) == DMA_ETI_INTR_STATUS)
	{
		//! Clear the ETI: Early Transmit Interrupt
		// pETH->STATUS_REG5_b.ETI = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_ETI, RSI_NULL);
		}
		
	}

	if((intr_status & DMA_FBI_INTR_STATUS) == DMA_FBI_INTR_STATUS)
	{
		//!Clear the FBI: Fatal Bus Error Interrupt
		pETH->STATUS_REG5_b.FBI = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{				
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_BUS_ERR, RSI_NULL);
		}
		
	}

	if((intr_status & DMA_ERI_INTR_STATUS) == DMA_ERI_INTR_STATUS)
	{
		//!Clear the ERI: Early Receive Interrupt
		// pETH->STATUS_REG5_b.ERI = 1;
		
		if(pDrv->cbTable[DMA_INTERRUPT] != RSI_NULL)	
		{				
			pDrv->cbTable[DMA_INTERRUPT](pETHHandle, ETH_EVENT_ERI, RSI_NULL);
		}
		
	}	
	
}


/*==============================================*/
/**
 * @fn          void RSI_ETH_PMTIntrHandler(RSI_ETH_HANDLE_T pETHHandle)
 * @brief       This API is the handler for PMT Interrupt 
 * @param[in]   pETHHandle, Ethernet Driver handler     
 * @param[out]  none
 * @return      none 
 *
 *
 * @section description
 * This API is the handler for PMT Interrupt 
 *
 *
 */

void RSI_ETH_PMTIntrHandler(RSI_ETH_HANDLE_T pETHHandle)
{
	RSI_ETH_DRIVER_T *pDrv =  ((RSI_ETH_DRIVER_T *)pETHHandle);
	
  ETHERNET_Type *pETH = ((RSI_ETH_DRIVER_T *)pETHHandle)->pREGS;
	
	
	uint32_t intr_status;
	
	intr_status = pETH->PMT_CTRL_STS_REG;
	
	//! check the interrupt status register 
	//! wakeup frame is received,call the wakeup frame  received handler
	if((intr_status & PMT_WFR_INTR) == PMT_WFR_INTR)
	{
	  if(pDrv->cbTable[GMAC_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[GMAC_INTERRUPT](pETHHandle,ETH_EVENT_WAKEUP_FRAME_RECVD, RSI_NULL);
		}
		
	}
	else if((intr_status & PMT_MPR_INTR) == PMT_MPR_INTR)
	{
		//! Magic packet is received,call the Magic packet received handler
		if(pDrv->cbTable[GMAC_INTERRUPT] != RSI_NULL)	
		{
			pDrv->cbTable[GMAC_INTERRUPT](pETHHandle,ETH_EVENT_MAGIC_PACKET_RECVD, RSI_NULL);
		}
	
	}
		
	
	
	

}
