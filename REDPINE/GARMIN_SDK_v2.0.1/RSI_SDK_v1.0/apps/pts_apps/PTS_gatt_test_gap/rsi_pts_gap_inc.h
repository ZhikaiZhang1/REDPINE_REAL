/**
 * @file     rsi_pts_gap_inc.h
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
#ifndef RSI_PTS_GAP_INC_H
#define RSI_PTS_GAP_INC_H
/******************************************************
 *                     GAP Layer Case
 ******************************************************/  
#define GAP_BROB_BCST_BV_01_C                0
#define GAP_BROB_BCST_BV_02_C                0
//#define GAP_BROB_BCST_BV_03_C              use privacy app (PTS_le_privacy app)

#define GAP_BROB_OBSV_BV_01_C                0
#define GAP_BROB_OBSV_BV_02_C                0
//#define GAP_BROB_OBSV_BV_03_C              use privacy app (PTS_le_privacy app)
//#define GAP_BROB_OBSV_BV_05_C              use privacy app (PTS_le_privacy app)
//#define GAP_BROB_OBSV_BV_06_C              use privacy app (PTS_le_privacy app)

#define GAP_DISC_NONM_BV_01_C                0
#define GAP_DISC_NONM_BV_02_C                0

#define GAP_DISC_LIMM_BV_01_C                0
#define GAP_DISC_LIMM_BV_02_C                0

#define GAP_DISC_GENM_BV_01_C                0
#define GAP_DISC_GENM_BV_02_C                0

#define GAP_DISC_LIMP_BV_01_C                0  // verify the adv data flag has set to Limited Discoverable 
#define GAP_DISC_LIMP_BV_02_C                0  // verify the adv data flag not set to Limited Discoverable 
#define GAP_DISC_LIMP_BV_03_C                0  // verify the adv data flag not set to Limited Discoverable 
#define GAP_DISC_LIMP_BV_04_C                0  // verify the adv data flag not set to Limited Discoverable 
#define GAP_DISC_LIMP_BV_05_C                0  // verify the adv data flag not set to Limited Discoverable 

#define GAP_DISC_GENP_BV_01_C                0  // verify the adv data flag has set to General Discoverable
#define GAP_DISC_GENP_BV_02_C                0  // verify the adv data flag has set to Limited Discoverable
#define GAP_DISC_GENP_BV_03_C                0  // verify the adv data flag has not set to Limited/General Discoverable
#define GAP_DISC_GENP_BV_04_C                0  // verify the adv data flag has not set to Limited/General Discoverable
#define GAP_DISC_GENP_BV_05_C                0  // verify the adv data flag has not set to Limited/General Discoverable


#define GAP_IDLE_NAMP_BV_01_C                0
#define GAP_IDLE_NAMP_BV_02_C                0

//#define GAP_IDLE_GIN_BV_01_C               Pending
//#define GAP_IDLE_LIN_BV_01_C               Pending
//#define GAP_IDLE_DED_BV_01_C               Pending

#define GAP_CONN_NCON_BV_01_C                0
#define GAP_CONN_NCON_BV_02_C                0
#define GAP_CONN_NCON_BV_03_C                0

#define GAP_CONN_DCON_BV_01_C                0

#define GAP_CONN_UCON_BV_01_C                0
#define GAP_CONN_UCON_BV_02_C                0
#define GAP_CONN_UCON_BV_03_C                0

#define GAP_CONN_ACEP_BV_01_C                0

#define GAP_CONN_GCEP_BV_01_C                0
#define GAP_CONN_GCEP_BV_02_C                0

#define GAP_CONN_SCEP_BV_01_C                0

#define GAP_CONN_DCEP_BV_01_C                0
#define GAP_CONN_DCEP_BV_03_C                0

#define GAP_CONN_CPUP_BV_01_C                0
#define GAP_CONN_CPUP_BV_02_C                0
#define GAP_CONN_CPUP_BV_03_C                0
#define GAP_CONN_CPUP_BV_04_C                0
#define GAP_CONN_CPUP_BV_05_C                0
#define GAP_CONN_CPUP_BV_06_C                0
#define GAP_CONN_CPUP_BV_08_C                0

#define GAP_CONN_TERM_BV_01_C                0

#define GAP_BOND_NBON_BV_01_C                0
#define GAP_BOND_NBON_BV_02_C                0
#define GAP_BOND_NBON_BV_03_C                0

#define GAP_BOND_BON_BV_01_C                 0
#define GAP_BOND_BON_BV_02_C                 0
#define GAP_BOND_BON_BV_03_C                 0
#define GAP_BOND_BON_BV_04_C                 0

//#define GAP_SEC_AUT_BV_11_C                  use server app (PTS_simple_chat_server app)
//#define GAP_SEC_AUT_BV_12_C                  use server app (PTS_simple_chat_server app)
//#define GAP_SEC_AUT_BV_13_C                  use server app (PTS_simple_chat_server app)
//#define GAP_SEC_AUT_BV_14_C                  use server app (PTS_simple_chat_server app)
#define GAP_SEC_AUT_BV_17_C                  0
#define GAP_SEC_AUT_BV_18_C                  0
#define GAP_SEC_AUT_BV_19_C                  0
#define GAP_SEC_AUT_BV_20_C                  0
#define GAP_SEC_AUT_BV_21_C                  0
#define GAP_SEC_AUT_BV_22_C                  0
//#define GAP_SEC_AUT_BV_23_C                  use server app (PTS_simple_chat_server app)
//#define GAP_SEC_AUT_BV_24_C                  use server app (PTS_simple_chat_server app)

//#define GAP_SEC_SEM_BV_04_C                Pending (use l2cap PSM connection based app)
//#define GAP_SEC_SEM_BV_05_C                Pending (use l2cap PSM connection based app)
//#define GAP_SEC_SEM_BV_06_C                Pending (use l2cap PSM connection based app)
//#define GAP_SEC_SEM_BV_07_C                Pending (use l2cap PSM connection based app)
//#define GAP_SEC_SEM_BV_08_C                Pending (use l2cap PSM connection based app)
//#define GAP_SEC_SEM_BV_09_C                Pending (use l2cap PSM connection based app)
//#define GAP_SEC_SEM_BV_10_C                Pending (use l2cap PSM connection based app)
#define GAP_ADV_BV_01_C                      0
#define GAP_ADV_BV_02_C                      0
#define GAP_ADV_BV_03_C                      0
#define GAP_ADV_BV_04_C                      0
#define GAP_ADV_BV_05_C                      0
#define GAP_ADV_BV_08_C                      0
#define GAP_ADV_BV_10_C                      0

#define GAP_GAT_BV_01_C                      0  /* test as peripheral if you want test as central use server app (simple chat) */
#define GAP_GAT_BV_04_C                      0

//#define GAP_DM_CASES                       use Dual-mode app  (PTS_dual_mode app)
//#define GAP_MOD_CASES                      use Dual-mode app  (PTS_dual_mode app)
#endif
