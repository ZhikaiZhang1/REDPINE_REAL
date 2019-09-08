/**
 * @file     rsi_pts_privacy_inc.h
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
#ifndef RSI_PTS_PRIVACY_INC_H
#define RSI_PTS_PRIVACY_INC_H

/*************************************************************
 * ***********         PRIVACY Defines         ***************
 * **********************************************************/

#define GAP_BROB_BCST_BV_03_C                        0
#define GAP_BROB_OBSV_BV_05_C                        0
#define GAP_BROB_OBSV_BV_06_C                        0

#define GAP_CONN_UCON_BV_06_C                        0   /* change the addr_resolution_privacy_time_out value in PTS ixit to 60000 */
#define GAP_CONN_PRDA_BV_01_C                        0

#define GAP_PRIV_CONN_BV_10_C                        0
#define GAP_PRIV_CONN_BI_01_C                        0  /* Test case issue  Please Check the Test SPec Erreta*/

#endif
