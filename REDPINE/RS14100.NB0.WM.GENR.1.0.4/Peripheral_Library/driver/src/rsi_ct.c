/**
 * @file       rsi_ct.c
 * @version    1.0
 * @date       1 Augest 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes related to CT peripheral
 * 
 * @section Description
 * This file contains the list of function prototypes for the CT and low level function definations 
 * Following are list of API's which need to be defined in this file.
 *
 */

#include "rsi_chip.h"
 
#ifdef __cplusplus
extern "C" {
#endif 
/* This API is used to toggle output state of OCU.*/ 
void ct_ocu_high_Low_toggle_select( RSI_CT_T *pCT, 
                                    boolean_t lowHigh,
																	  boolean_t counterNum, 
                                    uint8_t outputSel )
{	
  if(counterNum == 0)
  {
    if(lowHigh == LOW)
    {
      /* Sets Make output low signal */
      pCT ->CT_OCU_CTRL_REG_b.MAKE_OUTPUT_0_LOW_SEL= outputSel;			
    }
    else
    {
      /* Sets Make output high signal */
      pCT ->CT_OCU_CTRL_REG_b.MAKE_OUTPUT_0_HIGH_SEL = outputSel;			
    }		
  }
  else
  {
    if(lowHigh == LOW)
    {
      /* Sets Make output low signal */
      pCT ->CT_OCU_CTRL_REG_b.MAKE_OUTPUT_1_LOW_SEL= outputSel;			
    }
    else
    {
      /* Sets Make output high signal */
      pCT ->CT_OCU_CTRL_REG_b.MAKE_OUTPUT_1_HIGH_SEL = outputSel;			
    }			
  }
}

/* This API is used to toggle output state of WFG  */ 
error_t ct_wfg_control_config( RSI_CT_T *pCT, 
                               WFG_PARAMS_T ctrlReg )
{
  uint32_t value =0;
  
  if(((ctrlReg.output0_tgl0_sel <= OUTPUT_0_TGL_0_MAX)&&
      (ctrlReg.output0_tgl1_sel <= OUTPUT_0_TGL_1_MAX)&&
      (ctrlReg.tgl_cnt0_peak <= TGL_CNT_0_PEAK))||
     ((ctrlReg.output1_tgl0_sel <= OUTPUT_1_TGL_0_MAX)&&
      (ctrlReg.output1_tgl1_sel <= OUTPUT_1_TGL_1_MAX)&&
      (ctrlReg.tgl_cnt1_peak <= TGL_CNT_1_PEAK)))
  {
    value = ((ctrlReg.output0_tgl0_sel << 0)| 
            (ctrlReg.output0_tgl1_sel << 3)|
            (ctrlReg.tgl_cnt0_peak << 8)|
            (ctrlReg.output1_tgl0_sel << 16)|
            (ctrlReg.output1_tgl1_sel << 19)|
            (ctrlReg.tgl_cnt1_peak << 24));
    
    pCT->CT_WFG_CTRL_REG = value;   
  }
  else
  {
     return ERROR_CT_INVALID_ARG;
  }

  return RSI_OK;  
}

/*  This function controls OCU operation */
error_t ct_ocu_control(RSI_CT_T *pCT, 
                       boolean_t counterNum,
											 boolean_t dmaEn,
                       OCU_PARAMS_T *pOCUparams, 
											 RSI_CT_CALLBACK_T *pCB ) 													
{
   if((pOCUparams) == NULL)
   {
     return ERROR_CT_INVALID_ARG;  
   }
   
   if(counterNum)
	 {
      /* set 2 threshold vlaues for couter 1 */
	    pCT ->CT_OCU_COMPARE2_REG_b.OCU_COMPARE2_0_REG = pOCUparams ->CompareVal1_1;
			pCT ->CT_OCU_COMPARE2_REG_b.OCU_COMPARE2_1_REG = pOCUparams ->CompareVal2_1;
      /* set Sync value */
      pCT ->CT_OCU_SYNC_REG_b.OCU_SYNC_CHANNEL1_REG = (pOCUparams->SyncWith1);		 
	 }	
   else
	 {
		 /* set 2 threshold values for counter 0 */
	   pCT ->CT_OCU_COMPARE_REG_b.OCU_COMPARE_0_REG = pOCUparams ->CompareVal1_0;
		 pCT ->CT_OCU_COMPARE_REG_b.OCU_COMPARE_1_REG = pOCUparams ->CompareVal2_0;
		 /* set Sync value */
		 pCT ->CT_OCU_SYNC_REG_b.OCU_SYNC_CHANNEL0_REG = pOCUparams ->SyncWith0;	
	 }	  
	 /* DMA call back handle */
	 if(dmaEn)
   { 
		 if(counterNum == 0)
		 {
			 if(pCT ->CT_OCU_CTRL_REG_b.OCU_0_DMA_MODE)
			 {
				 if((pCB->cbFunc) != NULL)
				 {
						/* DMA call back event */
					 pCB->cbFunc(CT_OCU_DMA_EN_COUNTER0, counterNum);	
				 }
				 else
				 {	
					 return ERROR_CT_INVALID_ARG;
				 }	
			 }
	   }
		 else
		 {
			 if(pCT ->CT_OCU_CTRL_REG_b.OCU_1_DMA_MODE)
			 {
				 if((pCB->cbFunc) != NULL)
				 {
						/* DMA call back event */
					 pCB->cbFunc(CT_OCU_DMA_EN_COUNTER1,counterNum);	
				 }
				 else
				 {	
					 return ERROR_CT_INVALID_ARG;
				 }	
			 }
		 }
	 }

	 else
   {			
	    if(counterNum == 0)
			{
				/* set compare next value for coutner 0 */
				pCT ->CT_OCU_COMPARE_NXT_REG_b.OCU_COMPARE_NXT_COUNTER0   = pOCUparams ->CompareNextVal1_0;
				pCT ->CT_OCU_COMPARE2_NXT_REG_b.OCU_COMPARE2_NXT_COUNTER0 = pOCUparams ->CompareNextVal2_0;
			}
	    else
			{
       /* set compare next value for counter 1	*/				
				pCT ->CT_OCU_COMPARE_NXT_REG_b.OCU_COMPARE_NXT_COUNTER1 	 = pOCUparams ->CompareNextVal1_1;
				pCT ->CT_OCU_COMPARE2_NXT_REG_b.OCU_COMPARE2_NXT_COUNTER1 = pOCUparams ->CompareNextVal2_1;		
			}
   }
  return RSI_OK;
}

/* This function controls WFG operation */
error_t ct_wfg_comapre_value_set( RSI_CT_T *pCT, 
                                  boolean_t counterNum,
                                  OCU_PARAMS_T *pOCUparams )
{
  if(pOCUparams != NULL)
  {
     if(counterNum == 0)
     {
       /* set 2 threshold vlaues for couter 0 */
       pCT ->CT_OCU_COMPARE_REG_b.OCU_COMPARE_0_REG = pOCUparams ->CompareVal1_0;
       pCT ->CT_OCU_COMPARE_REG_b.OCU_COMPARE_1_REG = pOCUparams ->CompareVal2_0;	
     }	  
     else 
     {
        /* set 2 threshold values for counter 1 */
       pCT ->CT_OCU_COMPARE2_REG_b.OCU_COMPARE2_0_REG = pOCUparams ->CompareVal1_1;
       pCT ->CT_OCU_COMPARE2_REG_b.OCU_COMPARE2_1_REG = pOCUparams ->CompareVal2_1;																	
     }
   }
  else
  {
    return ERROR_CT_INVALID_ARG;
  }
  return RSI_OK;
}

#ifdef __cplusplus
}
#endif	

/************ EOF **************/
