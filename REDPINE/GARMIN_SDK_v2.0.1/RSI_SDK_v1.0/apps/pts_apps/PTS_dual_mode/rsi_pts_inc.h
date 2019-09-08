/**
 * @file     rsi_pts_inc.h
 * @version  0.1  
 * @date     16 Mar 2019
 *
 *  Copyright(C) Redpine Signals 2019
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contain definitions and declarations of PTS test suite.
 *
 *  @section Description  This file contains definitions and declarations required to
 *  configure PTS test suite
 *
 *
 */
#ifndef RSI_PTS_INC_H
#define RSI_PTS_INC_H

/*****************************************************************
 *  *                   GAP DM defines
 *  **************************************************************/

#define GAP_DM_NCON_BV_01_C                       0

#define GAP_DM_CON_BV_01_C                        0

#define GAP_DM_NBON_BV_01_C                       0

#define GAP_DM_BON_BV_01_C                        0

#define GAP_DM_GIN_BV_01_C                        0

#define GAP_DM_LIN_BV_01_C                        0

#define GAP_DM_NAD_BV_01_C                        0
#define GAP_DM_NAD_BV_02_C                        0

#define GAP_DM_LEP_BV_01_C                        0
#define GAP_DM_LEP_BV_02_C                        0
#define GAP_DM_LEP_BV_04_C                        0
#define GAP_DM_LEP_BV_05_C                        0
#define GAP_DM_LEP_BV_06_C                        0
#define GAP_DM_LEP_BV_07_C                        0
#define GAP_DM_LEP_BV_08_C                        0
#define GAP_DM_LEP_BV_09_C                        0
#define GAP_DM_LEP_BV_10_C                        0
#define GAP_DM_LEP_BV_11_C                        0


/*****************************************************************
 *  *                  GAP MOD defines
 *  **************************************************************/

#define GAP_MOD_NDIS_BV_01_C                      0
#define GAP_MOD_LDIS_BV_01_C                      0
#define GAP_MOD_LDIS_BV_02_C                      0
#define GAP_MOD_LDIS_BV_03_C                      0
#define GAP_MOD_GDIS_BV_01_C                      0
#define GAP_MOD_GDIS_BV_02_C                      0
#define GAP_MOD_NCON_BV_01_C                      0
#define GAP_MOD_CON_BV_01_C                       0




/******************************************************************
 *                     GATT CLIENT defines
 *******************************************************************/

#define GATT_CL_GAC_BV_01_C                       0

#define GATT_CL_GAD_BV_01_C                       0
#define GATT_CL_GAD_BV_02_C                       0
#define GATT_CL_GAD_BV_03_C                       0
#define GATT_CL_GAD_BV_04_C                       0
#define GATT_CL_GAD_BV_05_C                       0
#define GATT_CL_GAD_BV_06_C                       0

#define GATT_CL_GAR_BV_01_C                       0
#define GATT_CL_GAR_BI_01_C                       0
#define GATT_CL_GAR_BI_02_C                       0
//#define GATT_CL_GAR_BI_03_C                       0      /* authoriztion not supported */
#define GATT_CL_GAR_BI_04_C                       0
#define GATT_CL_GAR_BI_05_C                       0
#define GATT_CL_GAR_BV_03_C                       0
#define GATT_CL_GAR_BI_06_C                       0
#define GATT_CL_GAR_BI_07_C                       0
//#define GATT_CL_GAR_BI_09_C                       0      /* authoriztion not supported */
#define GATT_CL_GAR_BI_10_C                       0
#define GATT_CL_GAR_BI_11_C                       0
#define GATT_CL_GAR_BV_04_C                       0  // Need to test properly by cacheing the all data//
#define GATT_CL_GAR_BI_12_C                       0
#define GATT_CL_GAR_BI_13_C                       0
#define GATT_CL_GAR_BI_14_C                       0
//#define GATT_CL_GAR_BI_15_C                       0      /* authoriztion not supported */
//#define GATT_CL_GAR_BI_16_C                       0      /* authentication in only le-supported host*/
#define GATT_CL_GAR_BI_17_C                       0
#define GATT_CL_GAR_BV_05_C                       0
#define GATT_CL_GAR_BI_18_C                       0
#define GATT_CL_GAR_BI_19_C                       0
//#define GATT_CL_GAR_BI_20_C                       0      /* authoriztion not supported */
#define GATT_CL_GAR_BI_21_C                       0
#define GATT_CL_GAR_BI_22_C                       0
#define GATT_CL_GAR_BV_06_C                       0
#define GATT_CL_GAR_BV_07_C                       0
#define GATT_CL_GAR_BI_34_C                       0
#define GATT_CL_GAR_BI_35_C                       0

#define GATT_CL_GAW_BV_01_C                       0
//#define GATT_CL_GAW_BV_02_C                       0        /* Signed write not supported */
#define GATT_CL_GAW_BV_03_C                       0
#define GATT_CL_GAW_BI_02_C                       0
#define GATT_CL_GAW_BI_03_C                       0
//#define GATT_CL_GAW_BI_04_C                       0      /* authorization not supported */
#define GATT_CL_GAW_BI_05_C                       0
#define GATT_CL_GAW_BI_06_C                       0
#define GATT_CL_GAW_BV_05_C                       0
#define GATT_CL_GAW_BI_07_C                       0
#define GATT_CL_GAW_BI_08_C                       0
#define GATT_CL_GAW_BI_09_C                       0
//#define GATT_CL_GAW_BI_11_C                       0      /* authorization not supported */
#define GATT_CL_GAW_BI_12_C                       0
#define GATT_CL_GAW_BI_13_C                       0
#define GATT_CL_GAW_BV_06_C                       0
#define GATT_CL_GAW_BV_08_C                       0
#define GATT_CL_GAW_BV_09_C                       0
#define GATT_CL_GAW_BI_32_C                       0
#define GATT_CL_GAW_BI_33_C                       0
#define GATT_CL_GAW_BI_34_C                       0

#define GATT_CL_GAN_BV_01_C                       0

#define GATT_CL_GAI_BV_01_C                       0

#define GATT_CL_GAS_BV_01_C                       0

#define GATT_CL_GAT_BV_01_C                       0

#define GATT_CL_GAT_BV_02_C                       0

//#define GATT_SR_GPM_BV_01_C                     0      /* Not supported */


#endif

