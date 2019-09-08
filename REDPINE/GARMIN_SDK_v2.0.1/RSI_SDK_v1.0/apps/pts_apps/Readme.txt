Procedure for usage of PTS apps:
================================


SM Layer Cases:
===============

1. Use PTS_gatt_test_smp app to test the SM layer cases.
2. Compile & run each case at a time based on the Test case id mentioned in "rsi_pts_smp_inc.h" file.

GATT Layer Cases:
=================

1. Use PTS_dual_mode app to test the GATT layer cases.
2. Test all the GATT layer client cases based on Test case id (compile and run).
3. To test the GATT layer server cases, make sure that all the Test case Id's
   are disabled i.e., all defines in "rsi_pts_inc.h" file are assigned to "0".
   and then compile and run the app.


GAP Layer Cases:
================

1. To test the GAP layer cases , We need to use multiple apps based on the
   Test case requirements.
2. For the details understanding, see the "rsi_pts_gap_inc.h" file.
3. Complie & run each case based on the Test case Id as mentioned in respective header files.


A2DP, AVDTP, AVCTP, AVRCP Cases:
================================

1. To test the A2DP, AVDTP, AVCTP, AVRCP profile cases, use PTS_a2dp_source_avrcp app
2. For the details understanding, see the "rsi_bt_config.h" file.
3. Complie & run each case based on the Test case Id as mentioned in respective header files.


SDP Profile Cases:
================================

1. To test the SDP profile cases, use PTS_hid_slave app
2. For the details understanding, see the "rsi_bt_config.h" file.
3. Complie & run each case based on the Test case Id as mentioned in respective header files.


Limitations in this Release:
============================

1. Timeout issues (both in GATT & SM cases), 2 to 4 cases might effect.
2. Complete privacy is not availble.
3. Non-bondable cases issue in BR/EDR.
4. Direct L2CAP PSM connection with out sdp browsing (GAP/SEC/SEM cases are pending).

