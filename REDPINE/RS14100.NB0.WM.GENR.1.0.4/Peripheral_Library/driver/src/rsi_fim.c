/** 
 * @file       rsi_fim.c
 * @version    1.0
 * @date       1 AUG 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes related to FIM
 *
 * @section Description
 * This file contains the list of function prototypes for the FIM and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */




/**
 * Includes
 */
#include "rsi_chip.h"

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_FIM_DRIVERS RSI:RS1xxxx FIM 
 *  @{
 *
 */

/* function definitions of data read and write*/
/**
 *@fn          static void RSI_FIM_InputData(void *pSrcA,uint32_t bank,uint32_t blockSize,uint8_t data_type) 
 *@brief       This API is used to store data in ulp memories
 *@param[in]   :pSrcA is input vector
 *@param[in]   :bank is in which memory data samples are stored
 *@param[in]   :data_type specifies q7,q15 , q31 formats
 *             \ref FORMAT_F32
 *             \ref FORMAT_Q31 
 *             \ref FORMAT_Q7
 *             \ref FORMAT_Q15
 *@param[in]   :blockSize is size of the input array
**/
static void RSI_FIM_InputData(void *pSrcA,uint32_t bank,uint32_t blockSize,uint8_t data_type)
{
  uint32_t var;
  q31_t *pSrcA32;
  q15_t *pSrcA16;
  q7_t  *pSrcA8;
  
	switch (data_type)
	{
		case FORMAT_F32:
		case FORMAT_Q31:
      pSrcA32 = (q31_t *)pSrcA;
      for (var = 0; var < blockSize; var++)
      {
        *(volatile uint32_t *)( (MEM_BANK +  (bank<<2))  + (var*4)) = *pSrcA32;
         pSrcA32++;
      }
    break;
		
		case FORMAT_Q7:
      pSrcA8 = (q7_t *)pSrcA;
      for (var = 0; var < blockSize; var++)
      {
       *(volatile uint32_t *)( (MEM_BANK +  (bank<<2))  + (var*4)) = *pSrcA8;
        pSrcA8++;
      }
    break;
		
	 case FORMAT_Q15:
    pSrcA16 = (q15_t *)pSrcA;
	
    for (var = 0; var < blockSize; var++)
    {
     *(volatile uint32_t *)( (MEM_BANK +  (bank<<2))  + (var*4)) = *pSrcA16;
      
      pSrcA16++;
    }
    break;
	}
}

/**
 *@fn          static void RSI_FIM_InputDataCmplx(volatile q15_t *pReal,uint32_t bank,volatile uint32_t var,uint8_t flag)
 *@brief       This API is used to store data in ulp memories
 *@param[in]   :pReal is input vector
 *@param[in]   :bank is in which memory data samples are stored
 *@param[in]   :flag is set depending on complex-real and real-complex values
 *@param[in]   :var is for address increment
**/
static void RSI_FIM_InputDataCmplx(volatile q15_t *pReal,uint32_t bank,volatile uint32_t var,uint8_t flag)
{

  if(flag)
	{
	*(volatile uint16_t *)((MEM_BANK +  (bank<<2))  + ((var +1)*2)  )   = *pReal;
	 pReal++;
	*(volatile uint16_t *)((MEM_BANK +  (bank<<2))  + (var)*2)     = *pReal;
		pReal++;
	}else{
		*(volatile uint16_t *)((MEM_BANK +  (bank<<2))  + ((var +1)*2)  )   = *pReal;
	  *(volatile uint16_t *)((MEM_BANK +  (bank<<2))  + (var)*2)     = 0x0000;
		
	}
}

/**
 *@fn         static void RSI_FIM_ComplxOutputConvert(uint32_t blockSize, uint32_t bank,volatile q15_t *pDst)
 *@brief      This API is used to set the FIM to convert the complex output
 *@param[in]  :bank is output bank address
 *@param[out] :pDst is required output array
 *@param[in]  :blockSize is size of the input array
 *@return     :none
 */
static void RSI_FIM_ComplxOutputConvert(uint32_t blockSize, uint32_t bank,volatile q15_t *pDst)
{
	volatile uint32_t i,a;

	for(i=0;i< blockSize; i++)
	{
	  a = i*2;	
	  *pDst  = *(volatile uint16_t *)( (MEM_BANK +  (bank<<2))  + ((a+1))*2);
		 pDst++;
		*pDst = *(volatile uint16_t *)( (MEM_BANK +  (bank<<2))  + (a)*2) ;
		*pDst++;
	}
}
/**
 * @}
 */ 
/* This API is used to set the FIM to read the output */
void rsi_fim_read_data(uint32_t bank,uint32_t blockSize,volatile void *pDst,uint8_t data_type, typ_data_t type_data)
{
  volatile uint32_t var;
 
	
  if((data_type == FORMAT_F32)||(data_type == FORMAT_Q31))
  {
    for (var = 0; var < blockSize; var++)
    {
      ((q31_t *)pDst)[var]= *(volatile uint32_t *)((MEM_BANK + (bank <<2)) + (var*4));
        
    }
  }

  if(data_type == FORMAT_Q7)
  {
    for (var = 0; var < blockSize; var++)
    {
			((q7_t *)pDst)[var]= *(volatile uint32_t *)((MEM_BANK + (bank <<2)) + (var*4));
     
    }
  }
  if(data_type == FORMAT_Q15)
  {
    if(type_data)
    {
      RSI_FIM_ComplxOutputConvert(blockSize,bank,(q15_t *)pDst); 
    }
    else
    {
      for (var = 0; var < blockSize; var++)
      {
        ((q15_t *)pDst)[var]= *(volatile uint32_t *)((MEM_BANK + (bank <<2)) + (var*4));
      }
    } 
  }
}

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_FIM_DRIVERS RSI:RS1xxxx FIM 
 *  @{
 *
 */
/* function definitions for triggering fim module */

/**
 *@fn         static void RSI_FIM_SetDataLen(uint32_t ColAorfilterLen,uint32_t bufferLen1,uint32_t bufferLen2)
 *@brief      RSI_FIM_SetDataLen API is used to set data blockSize of buffers in the FIM module
 *@param[in]  :ColAorfilterLen is used for configuring number of columns of first matrix or
              \n filter coefficients for fir , iir and fir interpolator filters.
 *@param[in]  :bufferLen1 is the blockSize of data used for input1 
              \n for scalar only bufferLen1 need to be configured.
              \n for filter
 *@param[in]  :bufferLen2 is the blockSize of data used for input2 i.e. only for vector operations this is used.
 **/
static void RSI_FIM_SetDataLen(uint32_t ColAorfilterLen,uint32_t bufferLen1,uint32_t bufferLen2)
{
  /* buf_len 1,2, filter len */
	FIM->FIM_CONFIG_REG1 = ( bufferLen2 << 16) | (bufferLen1 << 6) | ColAorfilterLen;
}

/**
 *@fn         static void RSI_FIM_SetSatTruncRound(uint32_t SatTruncRoundShift)
 *@brief      RSI_FIM_SetSatTruncRound API is used to set sat trunc values to FIM module for corresponding inputs 
 *@param[in]  :SatTruncRoundShift is saturate truncation and round value that need to be to get appropriate output
 *            saturate - to confine msb to limited value
 *            truncate - to confine lsb to limited value
 *
 *            round    - approximating to near value
 */
 /*@example: a = 7.54, b = 2.52,32 bit values (with 1 3 28 fixed point notation) 
         c = a*b = 19.0008 
         output is a 64 bit value
         to confine the ouput to 32bit saturate value need to be set to obtained appropriate value.
         If sat = 1, then MSB bits are given as output.
         If round is set 19.0008 value is changed to 19.001
         If truncation is required even that can be used.
**/
static void RSI_FIM_SetSatTruncRound(uint32_t SatTruncRoundShift)
{
		/* Shift truncate saturate and round */
	FIM->FIM_SAT_SHIFT = SatTruncRoundShift;
}

/**
 *@fn           :static void RSI_FIM_DatTypTrig(uint32_t numRow1, uint32_t numCol2, typ_data_t typData)
 *@brief        :RSI_FIM_DatTypTrig API is used to trigger the FIM module
 *@param[in]    :numRow1 is number of rows of first matrix 
 *@param[in]    :numCol2 is number of columns of second matrix 
 *@param[in]    :typData is to select which type of data is given as input i.e. real-real , real-complex , complex-real and complex-complex
                In case of vectors, for real-complx, data in second memory is considered as real and 1st as cmplx.
                                 for complx-real, data in 1st memory is considered as real and second as cmplx.
                In case of scalar, for real-complx, data in memory is taken as complex and scalar as real.
* */
static void RSI_FIM_DatTypTrig(uint32_t numRow1, uint32_t numCol2, typ_data_t typData)
{
	uint32_t x;
	x = 0x0FC00000&FIM->FIM_CONFIG_REG2;
  switch(typData)
  {
    case ULP_FIM_COP_DATA_REAL_REAL: 
      FIM->FIM_CONFIG_REG2 = x | (numRow1 << 16) | (numCol2 << 10) | typData << 8 | ULP_FIM_COP_START_TRIG;
      break;
    case ULP_FIM_COP_DATA_CPLX_REAL:
      FIM->FIM_CONFIG_REG2 = x | (numRow1 << 16) | (numCol2 << 10) | typData << 8 | ULP_FIM_COP_START_TRIG;
      break;
    case ULP_FIM_COP_DATA_REAL_CPLX:
      FIM->FIM_CONFIG_REG2 = x | (numRow1 << 16) | (numCol2 << 10) | typData << 8 | ULP_FIM_COP_START_TRIG;
      break;
    case ULP_FIM_COP_DATA_CPLX_CPLX:
      FIM->FIM_CONFIG_REG2 = x | (numRow1 << 16) | (numCol2 << 10) | typData << 8 | ULP_FIM_COP_START_TRIG;
      break;
  } 
}

/**
 * @}
 */ 
/* function definitions for fim functionality*/

/* This API is used to set the FIM Scalar Addition for real data */
void rsi_arm_offset_f32_opt(int32_t *pSrc, int32_t scale,int32_t *pDst, uint32_t blockSize, uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scale;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_F32);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
	FIM->FIM_MODE_INTERRUPT    = (( ADD_SCALAR <<1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Scalar Addition for real data */
void rsi_arm_offset_q7_opt( q7_t *pSrc, q7_t scale, q7_t *pDst, uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound( STRS(0x1F,0,0,0));
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scale;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q7);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
	FIM->FIM_MODE_INTERRUPT    = ( ( ADD_SCALAR <<1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);	
}
/* This API is used to set the FIM Scalar Addition for real data */
void rsi_arm_offset_q15_opt( q15_t *pSrc, q15_t scale, q15_t *pDst, uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scale;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q15);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
	FIM->FIM_MODE_INTERRUPT    = ( ( ADD_SCALAR <<1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Scalar Addition for real data */
void rsi_arm_offset_q31_opt( q31_t *pSrc, q31_t scale, q31_t *pDst, uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scale;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q31);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
	FIM->FIM_MODE_INTERRUPT    = ( ( ADD_SCALAR <<1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Scalar Subtraction */
void rsi_fim_scalar_sub_f32(int32_t *pSrc,int32_t scale,int32_t *pDst, uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scale;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_F32);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT = ( (SUB_SCALAR <<1) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Scalar Subtraction */ 
void rsi_fim_scalar_sub_q7(q7_t *pSrc,q7_t scale,q7_t *pDst, uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
  RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q15);
  FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scale;
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT = ( (SUB_SCALAR <<1) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Scalar Subtraction */
void rsi_fim_scalar_sub_q31(q31_t *pSrc,q31_t scale, q31_t *pDst,uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scale;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q31);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT = ( (SUB_SCALAR <<1) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}
  

/* This API is used to set the FIM Scalar Multiplication for real data */
void rsi_arm_scale_f32_opt( int32_t *pSrc, int32_t scale,int32_t *pDst, uint32_t blockSize,uint16_t inBank, uint16_t outBank )
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(STRS(0x9,0,0,0));
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scale;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_F32);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( (MUL_SCALAR << 1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Scalar Multiplication */
void rsi_arm_scale_q7_opt(q7_t *pSrc,q7_t scaleFract,int8_t shift,q7_t *pDst,uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(shift+0x19);
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scaleFract;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q7);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( (MUL_SCALAR << 1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}


/*This API is used to set the FIM Scalar Multiplication */
void rsi_arm_scale_q15_opt(q15_t *pSrc,q15_t scaleFract,int8_t shift,q15_t *pDst,uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(shift+0x11);
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scaleFract;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q15);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR   =  outBank ;
	FIM->FIM_MODE_INTERRUPT        = ( (MUL_SCALAR << 1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Scalar Multiplication */
void rsi_arm_scale_q31_opt(q31_t *pSrc,q31_t scaleFract,int8_t shift,q31_t * pDst,uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,0);
	RSI_FIM_SetSatTruncRound(shift+0x1);
	FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 =  scaleFract;
	RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q31);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( (MUL_SCALAR << 1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}


/*This API is used to set the FIM Scalar Multiplication */
void rsi_fim_scalar_mul_q15(q15_t *pSrc,q15_t *scale,uint32_t blockSize, typ_data_t typ_data,uint16_t inBank, uint16_t outBank )
{
	uint32_t i;
	q15_t scalarReal;
	RSI_FIM_SetDataLen(0, blockSize/2,0);
	RSI_FIM_SetSatTruncRound(STRS(0x11,0,0,0));
	
		
	switch(typ_data)
	{
		case ULP_FIM_COP_DATA_CPLX_REAL:
	       for(i=0;i< blockSize/2; i++)
	       {
					RSI_FIM_InputDataCmplx(pSrc,inBank,i*2,1);
					pSrc++;
					pSrc++;
	       }
         scalarReal = *scale;
				 FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 = (scalarReal << 16 )| 0x0000;				 
		break;
		
		case ULP_FIM_COP_DATA_REAL_CPLX:
	       for(i=0;i< blockSize/2; i++)
	       {
	        RSI_FIM_InputDataCmplx(pSrc,inBank,i*2,0);
			    pSrc++;
	        }
				  scalarReal = *scale;
	        scale++;
          FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 = (scalarReal << 16 )| *scale;				 
		break;
			
		case ULP_FIM_COP_DATA_CPLX_CPLX:
			   for(i=0;i< blockSize/2; i++)
	       {
	       RSI_FIM_InputDataCmplx(pSrc,inBank,i*2,1);
		     pSrc++;
		     pSrc++;
	       }
				 scalarReal = *scale;
				 scale++;
				 FIM->FIM_SCALAR_POLE_DATA1_b.SCALAR_POLE_DATA1 = (scalarReal << 16 )| *scale;
			
		break;

    default:
    break;
  }
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank ;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( (MUL_SCALAR << 1 ) | 1);
  RSI_FIM_DatTypTrig(0,0,typ_data);
}

/*This API is used to set the FIM Vector Addition for real data */
void rsi_arm_add_f32_opt(int32_t *pSrcA, int32_t *pSrcB,uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank )
{
  RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_F32);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_F32);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank;
	FIM->FIM_MODE_INTERRUPT    = ( ( ADD_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Vector Addition for real data */
void rsi_arm_add_q7_opt(q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_Q7);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_Q7);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( ADD_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Vector Addition for real data */
void rsi_arm_add_q15_opt(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_Q15);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_Q15);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( ADD_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Vector Addition for real data */
void rsi_arm_add_q31_opt(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_Q31);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_Q31);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( ADD_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Vector Addition */
void rsi_fim_vector_add_q15(q15_t *pIn1, q15_t *pIn2,q15_t *pDst, uint32_t blockSize, typ_data_t typ_data,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	uint32_t i;
	RSI_FIM_SetDataLen(0, blockSize/2,blockSize/2);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  
	
	switch(typ_data)
	{
		case ULP_FIM_COP_DATA_CPLX_REAL:
	  for(i=0;i< blockSize/2; i++)
	  {
	   RSI_FIM_InputDataCmplx(pIn2,inBank2,i*2,0);
		 pIn2++;
		 RSI_FIM_InputDataCmplx(pIn1,inBank1,i*2,1);
		 pIn1++;
		 pIn1++;
	  }	
		break;
		
		case ULP_FIM_COP_DATA_REAL_CPLX:
	    for(i=0;i< blockSize/2; i++)
	    {
	     RSI_FIM_InputDataCmplx(pIn1,inBank1,i*2,0);
			 pIn1++;
			 RSI_FIM_InputDataCmplx(pIn2,inBank2,i*2,1);
			 pIn2++;
			 pIn2++;
	    }		
		break;
			
		case ULP_FIM_COP_DATA_CPLX_CPLX:
			   for(i=0;i< blockSize/2; i++)
	       {
	       RSI_FIM_InputDataCmplx(pIn1,inBank1,i*2,1);
				 pIn1++;
		     pIn1++;
	       RSI_FIM_InputDataCmplx(pIn2,inBank2,i*2,1);
				 pIn2++;
		     pIn2++;
	       }
			
		break;
  
    default:
    break;
  }
	
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( ADD_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,typ_data);
}

/* This API is used to set the FIM Vector Subtraction for real data */
void rsi_arm_sub_f32_opt(int32_t *pSrcA, int32_t *pSrcB,int32_t *pDst,uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
  RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_F32);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_F32);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( SUB_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Vector Subtraction for real data */
void rsi_arm_sub_q7_opt( q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_Q7);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_Q7);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( SUB_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Vector Subtraction for real data */
void rsi_arm_sub_q15_opt( q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_Q15);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_Q15);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( SUB_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Vector Subtraction for real data */
void rsi_arm_sub_q31_opt( q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1, blockSize, FORMAT_Q31);
	RSI_FIM_InputData(pSrcB,inBank2, blockSize, FORMAT_Q31);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( SUB_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Vector Subtraction */
void rsi_fim_vector_sub_q15( q15_t *pIn1, q15_t *pIn2,q15_t *pDst,uint32_t blockSize,typ_data_t typ_data,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	uint32_t i;
	RSI_FIM_SetDataLen(0, blockSize/2,blockSize/2);
	RSI_FIM_SetSatTruncRound(STRS(0x1F,0,0,0));

	switch(typ_data)
	{
		case ULP_FIM_COP_DATA_CPLX_REAL:
	
	  for(i=0;i< (blockSize/2); i++)
	  {
	   RSI_FIM_InputDataCmplx(pIn2,inBank2,i*2,0);
	   pIn2++;
		 RSI_FIM_InputDataCmplx(pIn1,inBank1,i*2,1);
		 pIn1++;
		 pIn1++;
	  }	
		break;
		
		case ULP_FIM_COP_DATA_REAL_CPLX:
	    for(i=0;i< (blockSize/2); i++)
	    {
	     RSI_FIM_InputDataCmplx(pIn1,inBank1,i*2,0);
			 pIn1++;
			 RSI_FIM_InputDataCmplx(pIn2,inBank2,i*2,1);
			 pIn2++;
			 pIn2++;
	    }		
		break;
			
		case ULP_FIM_COP_DATA_CPLX_CPLX:
			   for(i=0;i< blockSize/2; i++)
	       {
	       RSI_FIM_InputDataCmplx(pIn1,inBank1,i*2,1);
				 pIn1++;
		     pIn1++;
	       RSI_FIM_InputDataCmplx(pIn2,inBank2,i*2,1);
				 pIn2++;
		     pIn2++;
	       }
			
		break;
    
    default:
    break;
  
  }
	
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( SUB_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,typ_data);
	
}

/* This API is used to set the FIM Vector Multiplication for real data */
void rsi_arm_mult_f32_opt(int32_t *pIn1, int32_t *pIn2,uint32_t SatTruncRound,uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(SatTruncRound);
  RSI_FIM_InputData(pIn1,inBank1,blockSize, FORMAT_F32);
	RSI_FIM_InputData(pIn2,inBank2,blockSize, FORMAT_F32);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( MUL_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used for Q7 vector multiplication  */
void rsi_arm_mult_q7_opt( q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x19,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_Q7);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_Q7);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( MUL_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used for Q15 vector multiplication */
void rsi_arm_mult_q15_opt( q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x11,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_Q15);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_Q15);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank;
	FIM->FIM_MODE_INTERRUPT    = ( ( MUL_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used for Q31 vector multiplication. */
void rsi_arm_mult_q31_opt(q31_t *pSrcA,q31_t *pSrcB,q31_t *pDst,uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0, blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1,0,0,0));
  RSI_FIM_InputData(pSrcA,inBank1,blockSize, FORMAT_Q31);
	RSI_FIM_InputData(pSrcB,inBank2,blockSize, FORMAT_Q31);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( MUL_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used for Vector Multiplication for complex-real data */
void rsi_fim_vector_mul_q15( q15_t *pIn1, q15_t *pIn2,q15_t *pDst,uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	uint32_t i;
	RSI_FIM_SetDataLen(0, blockSize/2,blockSize/2);
	RSI_FIM_SetSatTruncRound(STRS(0x11,0,0,0));
	for(i=0;i< (blockSize/2); i++)
	{
	RSI_FIM_InputDataCmplx(pIn2,inBank2,i*2,1);
	pIn2++;
	pIn2++;
	RSI_FIM_InputDataCmplx(pIn1,inBank1,i*2,0);
	pIn1++;
	}
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( MUL_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_CPLX);
	
}

/* This API is used to set the FIM Vector Multiplication for complex-real date */
void rsi_arm_cmplx_mult_real_q15_opt(q15_t *pSrcCmplx,q15_t *pSrcReal,q15_t *pDst,uint32_t numSamples,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	uint32_t i;
	RSI_FIM_SetDataLen(0, numSamples/2,numSamples/2);
	RSI_FIM_SetSatTruncRound(STRS(0x11,0,0,0));
	for(i=0;i< (numSamples/2); i++)
	{
	RSI_FIM_InputDataCmplx(pSrcCmplx,inBank1,i*2,1);
	pSrcCmplx++;
	pSrcCmplx++;
	RSI_FIM_InputDataCmplx(pSrcReal,inBank2,i*2,0);
	pSrcReal++;	
	}
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT    = ( ( MUL_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_CPLX_REAL);
} 

/* This API is used for Q15 complex-by-complex multiplication */
void rsi_arm_cmplx_mult_cmplx_q15_opt(q15_t * pSrcA,q15_t * pSrcB,q15_t *pDst,uint32_t numSamples,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	uint32_t i;
	RSI_FIM_SetDataLen(0, numSamples/2,numSamples/2);
	RSI_FIM_SetSatTruncRound(STRS(0x10,0,0,1));
  for(i=0;i< (numSamples/2); i++)
	{
	RSI_FIM_InputDataCmplx(pSrcA,inBank1,i*2,ULP_FIM_COP_DATA_CPLX_CPLX);
	pSrcA++;
	pSrcA++;
	RSI_FIM_InputDataCmplx(pSrcB,inBank2,i*2,ULP_FIM_COP_DATA_CPLX_CPLX);
	pSrcB++;
	pSrcB++;
	}
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank;
	FIM->FIM_MODE_INTERRUPT    = ( ( MUL_VECTOR << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_CPLX_CPLX);
}

/*This API is used for Q15 complex magnitude squared */
void rsi_arm_cmplx_mag_squared_q15_opt(q15_t *pSrc,q15_t *pDst,uint32_t numSamples,uint16_t inBank, uint16_t outBank)
{
	uint32_t i;
	RSI_FIM_SetDataLen(0,numSamples/2,numSamples/2);
	RSI_FIM_SetSatTruncRound(STRS(0x10,0,0,1));
	for(i=0;i<(numSamples/2);i++)
	{
	RSI_FIM_InputDataCmplx(pSrc,inBank,i*2,ULP_FIM_COP_DATA_CPLX_CPLX);
	pSrc++;
	pSrc++;
	}
	
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank ;
	FIM->FIM_MODE_INTERRUPT = ( ( NORM_SQUARE << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_CPLX_CPLX);
}

/* This API is used to set the FIM Absolute Squaring for real number */
void rsi_fim_absSqr_q7(q7_t *pSrc,uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0,blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x19,0,0,0));
  RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q7);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank ;
	FIM->FIM_MODE_INTERRUPT = ( ( NORM_SQUARE << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Absolute Squaring for real number */
void rsi_fim_absSqr_q15(q15_t *pSrc,uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0,blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x11,0,0,0));
  RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q15);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
	FIM->FIM_MODE_INTERRUPT = ( ( NORM_SQUARE << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/*This API is used to set the FIM Absolute Squaring for real number */
void rsi_fim_absSqr_q31(q31_t *pSrc,uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0,blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x1,0,0,0));
  RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_Q31);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank ;
	FIM->FIM_MODE_INTERRUPT = ( ( NORM_SQUARE << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Absolute Squaring for real number  */
void rsi_fim_absSqr_f32(int32_t *pSrc,uint32_t blockSize,uint16_t inBank, uint16_t outBank)
{
	RSI_FIM_SetDataLen(0,blockSize,blockSize);
	RSI_FIM_SetSatTruncRound(STRS(0x9,0,0,0));
  RSI_FIM_InputData(pSrc,inBank,blockSize, FORMAT_F32);
	FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
	FIM->FIM_MODE_INTERRUPT = ( ( NORM_SQUARE << 1 ) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to set the FIM Matrix Multiplication for real numbers */
error_t rsi_arm_mat_mult_f32_opt( const arm_matrix_instance_f32_opt * pSrcA,const arm_matrix_instance_f32_opt * pSrcB,arm_matrix_instance_f32_opt * pDst,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	/* number of rows of input matrix A    */
	uint16_t numRowsA = pSrcA->nRows;     
  /* number of columns of input matrix B */	 
	uint16_t numColsB = pSrcB->nColumns;    
  /* number of columns of input matrix A */	 
	uint16_t numColsA = pSrcA->nColumns;
  if((pSrcA != NULL) && (pSrcB != NULL) )
  {	
    if(pSrcA->nColumns == pSrcB->nRows)
    {           
      RSI_FIM_SetDataLen(numColsA,0,0);
      RSI_FIM_SetSatTruncRound(STRS(0x9,0,0,0));
      RSI_FIM_InputData((pSrcA->pData), inBank1, ((pSrcA->nRows)*(pSrcA->nColumns)), FORMAT_F32);
      RSI_FIM_InputData((pSrcB->pData), inBank2, ((pSrcB->nRows)*(pSrcB->nColumns)), FORMAT_F32);
      FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank1;
      FIM->FIM_INP2_ADDR_b.INP2_ADDR = inBank2;
      FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
      FIM->FIM_MODE_INTERRUPT = ( ( MUL_MAT << 1 ) | 1);
      RSI_FIM_DatTypTrig(numRowsA,numColsB,ULP_FIM_COP_DATA_REAL_REAL);
    }
    else
    {
      return ERROR_FIM_MATRIX_INVALID_ARG;
    }
 } 
 return RSI_OK;
}

/* This API is used to set the FIM Matrix Multiplication for real numbers */
error_t rsi_arm_mat_mult_q31_opt(const arm_matrix_instance_q31_opt * pSrcA,const arm_matrix_instance_q31_opt * pSrcB,arm_matrix_instance_q31_opt * pDst,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	/* number of rows of input matrix A    */
	uint16_t numRowsA = pSrcA->nRows;     
  /* number of columns of input matrix B */	 
	uint16_t numColsB = pSrcB->nColumns;    
  /* number of columns of input matrix A */	 
	uint16_t numColsA = pSrcA->nColumns;
  if((pSrcA != NULL) && (pSrcB != NULL) )
  {	
    if(pSrcA->nColumns == pSrcB->nRows)
    {           
      RSI_FIM_SetDataLen(numColsA,0,0);
      RSI_FIM_SetSatTruncRound(STRS(0x1,0,0,0));
      RSI_FIM_InputData((pSrcA->pData), inBank1, ((pSrcA->nRows)*(pSrcA->nColumns)), FORMAT_Q31);
      RSI_FIM_InputData((pSrcB->pData), inBank2, ((pSrcB->nRows)*(pSrcB->nColumns)), FORMAT_Q31);
      FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank1;
      FIM->FIM_INP2_ADDR_b.INP2_ADDR = inBank2;
      FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
      FIM->FIM_MODE_INTERRUPT = ( ( MUL_MAT << 1 ) | 1);
      RSI_FIM_DatTypTrig(numRowsA,numColsB,ULP_FIM_COP_DATA_REAL_REAL);
    }
    else
    {
      return ERROR_FIM_MATRIX_INVALID_ARG;
    }
 } 
 return RSI_OK;
}

/* This API is used to set the FIM Matrix Multiplication for real numbers */
error_t rsi_arm_mat_mult_q15_opt(const arm_matrix_instance_q15_opt * pSrcA,const arm_matrix_instance_q15_opt * pSrcB,arm_matrix_instance_q15_opt * pDst,q15_t * pState,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	/* number of rows of input matrix A    */
	uint16_t numRowsA = pSrcA->nRows;     
  /* number of columns of input matrix B */	 
	uint16_t numColsB = pSrcB->nColumns;    
  /* number of columns of input matrix A */	 
	uint16_t numColsA = pSrcA->nColumns;
  if((pSrcA != NULL) && (pSrcB != NULL) )
  {	
    if(pSrcA->nColumns == pSrcB->nRows)
    {           
      RSI_FIM_SetDataLen(numColsA,0,0);
      RSI_FIM_SetSatTruncRound(STRS(0x11,0,0,0));
      RSI_FIM_InputData((pSrcA->pData), inBank1, ((pSrcA->nRows)*(pSrcA->nColumns)), FORMAT_Q15);
      RSI_FIM_InputData((pSrcB->pData), inBank2, ((pSrcB->nRows)*(pSrcB->nColumns)), FORMAT_Q15);
      FIM->FIM_INP1_ADDR_b.INP1_ADDR = inBank1;
      FIM->FIM_INP2_ADDR_b.INP2_ADDR = inBank2;
      FIM->FIM_OUT_ADDR_b.OUT_ADDR  = outBank;
      FIM->FIM_MODE_INTERRUPT = ( ( MUL_MAT << 1 ) | 1);
      RSI_FIM_DatTypTrig(numRowsA,numColsB,ULP_FIM_COP_DATA_REAL_REAL);
    }
    else
    {
      return ERROR_FIM_MATRIX_INVALID_ARG;
    }
 } 
 return RSI_OK;
}

/* This API is used to initialize the FIM  filters */
void rsi_arm_fir_init_f32_opt( arm_fir_instance_f32_opt *S,uint16_t numTaps, int32_t *pCoeffs,int32_t *pState, uint32_t blockSize)
{
 /* Assign filter taps */
 S->numTaps = numTaps;
	
 /* Assign coefficient pointer */
 S->pCoeffs = pCoeffs;
 /* Assign state pointer */
 S->pState = pState;

}

/* This API is used to set the FIM FIR Filter */
void rsi_arm_fir_f32_opt( arm_fir_instance_f32_opt *S, int32_t *pSrc,int32_t *pDst,uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	if(S->numTaps > blockSize)
	{
	 RSI_FIM_SetDataLen(blockSize,blockSize,(S->numTaps));
	 RSI_FIM_InputData( pSrc,inBank1,blockSize, FORMAT_F32);
	 RSI_FIM_InputData( S->pCoeffs,inBank2,S->numTaps, FORMAT_F32);	
	}
	else
	{
   RSI_FIM_SetDataLen((S->numTaps),(S->numTaps),blockSize );
	 RSI_FIM_InputData( pSrc,inBank2,blockSize, FORMAT_F32);
	 RSI_FIM_InputData( S->pCoeffs,inBank1,S->numTaps, FORMAT_F32);
	}
	RSI_FIM_SetSatTruncRound(STRS(0x9,0,0,0));
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1 ;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT  = ( (FIR <<1) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to initialize the FIM  filters */
void rsi_arm_fir_init_q31_opt( arm_fir_instance_q31_opt *S,uint16_t numTaps,q31_t *pCoeffs,q31_t *pState, uint32_t blockSize)
{	
 /* Assign filter taps */
 S->numTaps = numTaps;
 /* Assign coefficient pointer */
 S->pCoeffs = pCoeffs;
 /* Assign state pointer */
 S->pState = pState;
}

/* This API is used to set the FIM FIR Filter */
void rsi_arm_fir_q31_opt( arm_fir_instance_q31_opt *S, q31_t *pSrc,q31_t *pDst,uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	if(S->numTaps > blockSize)
	{
	 RSI_FIM_SetDataLen(blockSize,blockSize,(S->numTaps));
	 RSI_FIM_InputData( pSrc,inBank1,blockSize, FORMAT_Q31);
	 RSI_FIM_InputData( S->pCoeffs,inBank2,S->numTaps, FORMAT_Q31);	
	}
	else
	{
   RSI_FIM_SetDataLen((S->numTaps),(S->numTaps),blockSize );
	 RSI_FIM_InputData( pSrc,inBank2,blockSize, FORMAT_Q31);
	 RSI_FIM_InputData( S->pCoeffs,inBank1,S->numTaps, FORMAT_Q31);
	}
	RSI_FIM_SetSatTruncRound(STRS(0x1,0,0,0));
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1 ;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank;
	FIM->FIM_MODE_INTERRUPT  = ( (FIR <<1) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to initialize the FIM  filters */
void rsi_arm_fir_init_q15_opt( arm_fir_instance_q15_opt *S,uint16_t numTaps,q15_t *pCoeffs,q15_t *pState, uint32_t blockSize)
{
 /* Assign filter taps */
 S->numTaps = numTaps;

 /* Assign coefficient pointer */
 S->pCoeffs = pCoeffs;
 /* Assign state pointer */
 S->pState = pState;
}

/* This API is used to set the FIM FIR Filter */
void rsi_arm_fir_q15_opt( arm_fir_instance_q15_opt *S, q15_t *pSrc,q15_t *pDst,uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
  
  if(S->numTaps > blockSize)
	{
	 RSI_FIM_SetDataLen(blockSize,blockSize,(S->numTaps));
	 RSI_FIM_InputData( pSrc,inBank1,blockSize, FORMAT_Q15);
	 RSI_FIM_InputData( S->pCoeffs,inBank2,S->numTaps, FORMAT_Q15);	
	}
	else
	{
   RSI_FIM_SetDataLen((S->numTaps),(S->numTaps),blockSize );
	 RSI_FIM_InputData( pSrc,inBank2,blockSize, FORMAT_Q15);
	 RSI_FIM_InputData( S->pCoeffs,inBank1,S->numTaps, FORMAT_Q15);
	}
	RSI_FIM_SetSatTruncRound(STRS(0x11,0,0,0));
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1 ;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank;
	FIM->FIM_MODE_INTERRUPT  = ((FIR <<1) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}

/* This API is used to initialize the FIM  filters */
void rsi_arm_fir_init_q7_opt( arm_fir_instance_q7_opt *S,uint16_t numTaps,q7_t *pCoeffs,q7_t *pState, uint32_t blockSize)
{
 /* Assign filter taps */
 S->numTaps = numTaps;	
 /* Assign coefficient pointer */
 S->pCoeffs = pCoeffs;
 /* Assign state pointer */
 S->pState = pState;
}

/* This API is used to set the FIM FIR Filter */
void rsi_arm_fir_q7_opt( arm_fir_instance_q7_opt *S, q7_t *pSrc,q7_t *pDst, uint32_t blockSize,uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
	if(S->numTaps > blockSize)
	{
	 RSI_FIM_SetDataLen(blockSize,blockSize,(S->numTaps));
	 RSI_FIM_InputData( pSrc,inBank1,blockSize, FORMAT_Q7);
	 RSI_FIM_InputData( S->pCoeffs,inBank2,S->numTaps, FORMAT_Q7);	
	}
	else
	{
   RSI_FIM_SetDataLen((S->numTaps),(S->numTaps),blockSize );
	 RSI_FIM_InputData( pSrc,inBank2,blockSize, FORMAT_Q7);
	 RSI_FIM_InputData( S->pCoeffs,inBank1,S->numTaps, FORMAT_Q7);
	}
	RSI_FIM_SetSatTruncRound(STRS(0x19,0,0,0));
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1 ;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank;
	FIM->FIM_MODE_INTERRUPT  = ( (FIR <<1) | 1);
	RSI_FIM_DatTypTrig(0,0,ULP_FIM_COP_DATA_REAL_REAL);
}



/************ FIR Interpolate API ***********/

/* This API is used to initialization function for the floating-point FIR interpolator. */ 
arm_status rsi_arm_fir_interpolate_init_f32_opt( arm_fir_interpolate_instance_f32_opt * S,
                                             uint8_t L, uint16_t numTaps,
                                             int32_t * pCoeffs,
                                             int32_t * pState,
                                             uint32_t blockSize )
{
  arm_status status;

  /* The filter length must be a multiple of the interpolation factor */
  if((numTaps % L) != 0u)
  {
    /* Set status as ARM_MATH_LENGTH_ERROR */
    status = ARM_MATH_LENGTH_ERROR;
  }
  else
  {
    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;
    /* Assign Interpolation factor */
    S->L = L;
    /* Assign polyPhaseLength */
    S->phaseLength = numTaps / L;
    /* Assign state pointer */
    S->pState = pState;
    status = ARM_MATH_SUCCESS;
  }
  return (status);
}

/*This API is used Processing function for the floating-point FIR interpolator */
void rsi_arm_fir_interpolate_f32_opt( const arm_fir_interpolate_instance_f32_opt * S,
                                  int32_t * pSrc,
                                  int32_t * pDst,
                                  uint32_t blockSize,
                                  uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
 
  uint32_t numTaps; 
  numTaps = (S->phaseLength)*(S->L);
  
	RSI_FIM_SetDataLen(numTaps, blockSize, numTaps);
	RSI_FIM_InputData(pSrc,inBank1, blockSize, FORMAT_F32);
  RSI_FIM_InputData(S->pCoeffs,inBank2,numTaps, FORMAT_F32);
	RSI_FIM_SetSatTruncRound(STRS(0x9,0,0,0));
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1 ;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
  FIM->FIM_CONFIG_REG2 |= (S->L) << 22 ;
  FIM->FIM_MODE_INTERRUPT  = ( (INTERPOLATE <<1) | 1);
	RSI_FIM_DatTypTrig(0, 0, ULP_FIM_COP_DATA_REAL_REAL);  
}

/* This API is used to initialization function for the Q15 FIR interpolator */
arm_status rsi_arm_fir_interpolate_init_q15_opt( arm_fir_interpolate_instance_q15_opt * S,
                                             uint8_t L,
                                             uint16_t numTaps,
                                             q15_t * pCoeffs,
                                             q15_t * pState,
                                             uint32_t blockSize )
{
  arm_status status;

  /* The filter length must be a multiple of the interpolation factor */
  if((numTaps % L) != 0u)
  {
    /* Set status as ARM_MATH_LENGTH_ERROR */
    status = ARM_MATH_LENGTH_ERROR;
  }
  else
  {
    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;
    
    /* Assign Interpolation factor */
    S->L = L;

    /* Assign polyPhaseLength */
    S->phaseLength = numTaps / L;

    /* Assign state pointer */
    S->pState = pState;
    status = ARM_MATH_SUCCESS;
  }
  return (status);
}

/* This API is used to Processing function for the Q15 FIR interpolator */
void rsi_arm_fir_interpolate_q15_opt( arm_fir_interpolate_instance_q15_opt * S,
                                  q15_t * pSrc,q15_t *pDst,
                                  uint32_t blockSize,
                                  uint16_t inBank1, uint16_t inBank2, uint16_t outBank)
{
  
  uint32_t numTaps; 
  numTaps = (S->phaseLength)*(S->L);
  
	RSI_FIM_SetDataLen(numTaps, blockSize, numTaps);
	RSI_FIM_InputData( pSrc,inBank1,blockSize, FORMAT_Q15);
  RSI_FIM_InputData( S->pCoeffs,inBank2,numTaps, FORMAT_Q15);		
  RSI_FIM_SetSatTruncRound(STRS(0x11,0,0,0));
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1 ;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT  = ( (INTERPOLATE <<1) | 1);
  FIM->FIM_CONFIG_REG2 |= (S->L) << 22;
	RSI_FIM_DatTypTrig(0, 0, ULP_FIM_COP_DATA_REAL_REAL);  
}

/*This API  is used to  initialization function for the Q31 FIR interpolator */
arm_status rsi_arm_fir_interpolate_init_q31_opt( arm_fir_interpolate_instance_q31_opt *S,
                                             uint8_t L,
                                             uint16_t numTaps,
                                             q31_t * pCoeffs,
                                             q31_t * pState,
                                             uint32_t blockSize )
{
  arm_status status;
  /* The filter length must be a multiple of the interpolation factor */
  if((numTaps % L) != 0u)
  {
    /* Set status as ARM_MATH_LENGTH_ERROR */
    status = ARM_MATH_LENGTH_ERROR;
  }
  else
  {
    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;

    /* Assign Interpolation factor */
    S->L = L;

    /* Assign polyPhaseLength */
    S->phaseLength = numTaps / L;
		
    /* Assign state pointer */
    S->pState = pState;
		
    status = ARM_MATH_SUCCESS;
  }
	

  return (status); 
}

/* This  API is used  to Processing function for the Q31 FIR interpolator */
void rsi_arm_fir_interpolate_q31_opt( const arm_fir_interpolate_instance_q31_opt * S,
                                  q31_t * pSrc,
                                  q31_t * pDst,
                                  uint32_t blockSize,
                                  uint16_t inBank1, uint16_t inBank2, uint16_t outBank )
{
  uint32_t numTaps; 
  numTaps = (S->phaseLength)*(S->L);
  
	RSI_FIM_SetDataLen(numTaps, blockSize, numTaps);
	RSI_FIM_InputData(pSrc,inBank1, blockSize, FORMAT_Q31);
  RSI_FIM_InputData(S->pCoeffs,inBank2,numTaps, FORMAT_Q31);
	RSI_FIM_SetSatTruncRound(STRS(0x1,0,0,0));
	FIM->FIM_INP1_ADDR_b.INP1_ADDR =  inBank1 ;
	FIM->FIM_INP2_ADDR_b.INP2_ADDR =  inBank2;
	FIM->FIM_OUT_ADDR_b.OUT_ADDR  =  outBank ;
	FIM->FIM_MODE_INTERRUPT  = ( (INTERPOLATE <<1) | 1);
  FIM->FIM_CONFIG_REG2 |= (S->L) << 22;
	RSI_FIM_DatTypTrig(0, 0, ULP_FIM_COP_DATA_REAL_REAL);   
}
/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_FIM_DRIVERS RSI:RS1xxxx FIM 
 *  @{
 *
 */
/**
 *@brief     This API Clears interrupt status of fim
 *@param[in]  ptFim is pointer to the FIM register instance
 *@return    none
 */
void rsi_fim_interrupt_handler(volatile FIM_Type *ptFim)
{
	ptFim->FIM_MODE_INTERRUPT_b.INTR_CLEAR = 0x1;
}

/**
 * @}
 */ 
