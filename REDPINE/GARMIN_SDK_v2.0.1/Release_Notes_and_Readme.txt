======================================================================================================================================================
Release            : RSI_SDK_v1.0
Date               : 30 July 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_SDK_vX/
2. The latest firmware to use with this example project is located at RSI_SDK_vX/resources/firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map SmartStudio's workspace to RSI_SDK_vX/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_SDK_vX/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the apps->demo.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example located in RSI_SDK_vX/doc. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a BT_A2DP_SOURCE_DEMO_5.pdf file in RSI_SDK_vX/doc.
9. For 1.8v, enable 'ENABLE_1P8V' macro in RSI_SDK_vX->apps->demo->inc
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->GNU ARM Cross C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
SmartStudio will not be able to find project files if the path is too long.
3. Refer to SmartStudio QuickStart Guide.pdf for SmartStudio related details.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. Flash for 80MHz support can be configured through application, by default 40MHz configuartion is selected.
2. PTS Stack Enhancements for BT/BLE profiles as per the IXIT file shared by Garmin. Added Linux application for running PTS test cases in below folder
      -- RSI_SDK_vx.x/apps/pts_apps/
3. Added SAPI for BLE Scanning with user configured values.
4. Added support for BLE/ANT 10dBm Tx Power.
5. Added Linux ANT applications in the below folder
   RSI_SDK_vx.x.x/apps/ant_apps
6. Added support in ANT for analysing radio sharing using debug GPIOs

3.0 Changes/Issues fixed in this release:
=========================================
1. For 1.3v Silicon Chip Bydefault UULP_GPIO_3 is used as a wakeup indication to host.
   For 1.4v Silicon Chip UULP_GPIO_0 is used as a wakeup indication to host, user should update following bitmaps to make use of this chip version
   	- BIT(0) should be set in RSI_CONFIG_FEATURE_BIT_MAP &
        - BIT(31) should be set in both TCP_IP_FEATURE_BIT_MAP and EXT_TCP_IP_FEATURE_BIT_MAP. 
        - BIT(23) should be set in RSI_EXT_CUSTOM_FEATURE_BIT_MAP. 	
        - EXT_FEAT_XTAL_CLK_ENABLE (BIT(22)) should be reset in RSI_EXT_CUSTOM_FEATURE_BIT_MAP.

2. Changed Release Folder structure as requested

   GARMIN_SDK_vx.x.x
   -----------------
      |
      |--> RSI_SDK_vx.x
      |     |
      |     |
      |     |--> apps
      |     |--> doc
      |     |--> platforms
      |     |--> resources
      |     |      |
      |     |      |--> firmware/RS9116_WC_SI.rps
      |     |--> sapis
      |        
      |--> RS9116_x.x.x_Build_Env
      |     |
      |     |
      |     |--> escripts
      |     |--> include
      |     |--> src
      |     |--> ..etc

3. Added ANT 4Hz support and fixed BT audio breaks with ANT 4Hz

4.0 Known Issues:
=================
1. Observed Tx throughput issue with WPA2 Security in WIFI_TCP_TX_POWERSAVE_DEMO_16 demo

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - BLE_CENTRAL_ANT_APP
   - BLE_PERIPHERAL_ANT_APP
   - ANT_BLE_CENT_BT_A2DP_SOURCE
   - ANT_BLE_PERI_BT_A2DP_SOURCE
   - DEMO_22 to DEMO_35, DEMO_37, DEMO_39 to DEMO_44
   - DEMO_18 with BR/EDR Adaptation

6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.2.0d.
2. A2DP supported for SBC pass through mode.

======================================================================================================================================================
Release            : RSI_WSDK_v0.9.6b
Date               : 16 July 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_WSDK_vX/
2. The latest firmware to use with this example project is located at RSI_WSDK_vX/resources/firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map SmartStudio's workspace to RSI_WSDK_vX/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_WSDK_vX/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the apps->demo.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example located in RSI_WSDK_vX/doc. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a BT_A2DP_SOURCE_DEMO_5.pdf file in RSI_WSDK_vX/doc.
9. For 1.8v, enable 'ENABLE_1P8V' macro in RSI_WSDK_vX->apps->demo->inc
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->GNU ARM Cross C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
SmartStudio will not be able to find project files if the path is too long.
3. Refer to SmartStudio QuickStart Guide.pdf for SmartStudio related details.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
    
3.0 Changes/Issues fixed in this release:
=========================================
1. For 1.3v Silicon Chip Bydefault UULP_GPIO_3 is used as a wakeup indication to host.
   For 1.4v Silicon Chip UULP_GPIO_0 is used as a wakeup indication to host, user should update following bitmaps to make use of this chip version
   	- BIT(0) should be set in RSI_CONFIG_FEATURE_BIT_MAP &
        - BIT(31) should be set in both TCP_IP_FEATURE_BIT_MAP and EXT_TCP_IP_FEATURE_BIT_MAP. 
        - BIT(23) should be set in RSI_EXT_CUSTOM_FEATURE_BIT_MAP. 	
        - EXT_FEAT_XTAL_CLK_ENABLE (BIT(22)) should be reset in RSI_EXT_CUSTOM_FEATURE_BIT_MAP.

2. Added ANT 4Hz support and fixed BT audio breaks with ANT 4Hz

4.0 Known Issues:
=================
1. Flash for 80MHz support is not available for ANT Applications.

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - BLE_CENTRAL_ANT_APP
   - BLE_PERIPHERAL_ANT_APP
   - ANT_BLE_CENT_BT_A2DP_SOURCE
   - ANT_BLE_PERI_BT_A2DP_SOURCE
   - DEMO_22 to DEMO_35, DEMO_37, DEMO_39 to DEMO_44
   - DEMO_18 with BR/EDR Adaptation

6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.2.0b9.
2. A2DP supported for SBC pass through mode.

======================================================================================================================================================
Release            : RSI_WSDK_v0.9.6a
Date               : 09 July 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_WSDK_vX/
2. The latest firmware to use with this example project is located at RSI_WSDK_vX/resources/firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map SmartStudio's workspace to RSI_WSDK_vX/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_WSDK_vX/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the apps->demo.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example located in RSI_WSDK_vX/doc. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a BT_A2DP_SOURCE_DEMO_5.pdf file in RSI_WSDK_vX/doc.
9. For 1.8v, enable 'ENABLE_1P8V' macro in RSI_WSDK_vX->apps->demo->inc
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->GNU ARM Cross C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
SmartStudio will not be able to find project files if the path is too long.
3. Refer to SmartStudio QuickStart Guide.pdf for SmartStudio related details.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. Added ANT build environment in the below folder
   RSI_SDK_vx.x.x/resources/ant_stack
    
3.0 Changes/Issues fixed in this release:
=========================================
1. For 1.3v Silicon Chip Bydefault UULP_GPIO_3 is used as a wakeup indication to host.
   For 1.4v Silicon Chip UULP_GPIO_0 is used as a wakeup indication to host, user should update following bitmaps to make use of this chip version
   	- BIT(0) should be set in RSI_CONFIG_FEATURE_BIT_MAP &
        - BIT(31) should be set in both TCP_IP_FEATURE_BIT_MAP and EXT_TCP_IP_FEATURE_BIT_MAP. 
        - BIT(23) should be set in RSI_EXT_CUSTOM_FEATURE_BIT_MAP. 	
        - EXT_FEAT_XTAL_CLK_ENABLE (BIT(22)) should be reset in RSI_EXT_CUSTOM_FEATURE_BIT_MAP.
2. Fixed the issue of high BLE power number in advertising and connection states.

4.0 Known Issues:
=================
1. For ANT+BT+BLE application with 4Hz, audio breaks are observed.
2. Flash for 80MHz support is not available for ANT Applications.

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - BLE_CENTRAL_ANT_APP
   - BLE_PERIPHERAL_ANT_APP
   - ANT_BLE_CENT_BT_A2DP_SOURCE
   - ANT_BLE_PERI_BT_A2DP_SOURCE
   - DEMO_22 to DEMO_35, DEMO_37, DEMO_39 to DEMO_44
   - DEMO_18 with BR/EDR Adaptation

6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.2.0b8.
2. A2DP supported for SBC pass through mode.
3. Measured BLE Power Consumption with 1.4rev Chip
   ---------                                              --------------------------
   Test Case                                              Current Consumption (1.8v)
   ---------                                              --------------------------
   20ms advertising (20 byte) with 3 channels              ~1.6 mA
   20ms advertising (20 byte) with 1 channe                ~818 uA
   1065ms advertising (20 byte) with 3 channels            ~60 uA
   1065ms advertising (20 byte) with 1 channel             ~43 uA 
   Idle-link connection to a phone, 250ms. (empty packet)  ~103 uA
   
======================================================================================================================================================
Release            : RSI_WSDK_v0.9.6
Date               : 28 June 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_WSDK_vX/
2. The latest firmware to use with this example project is located at RSI_WSDK_vX/resources/firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map SmartStudio's workspace to RSI_WSDK_vX/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_WSDK_vX/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the apps->demo.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example located in RSI_WSDK_vX/doc. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a BT_A2DP_SOURCE_DEMO_5.pdf file in RSI_WSDK_vX/doc.
9. For 1.8v, enable 'ENABLE_1P8V' macro in RSI_WSDK_vX->apps->demo->inc
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->GNU ARM Cross C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
SmartStudio will not be able to find project files if the path is too long.
3. Refer to SmartStudio QuickStart Guide.pdf for SmartStudio related details.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================

    
3.0 Changes/Issues fixed in this release:
=========================================
1. For 1.3v Silicon Chip Bydefault UULP_GPIO_3 is used as a wakeup indication to host.
   For 1.4v Silicon Chip UULP_GPIO_0 is used as a wakeup indication to host, user should update following bitmaps to make use of this chip version
   	- BIT(0) should be set in RSI_CONFIG_FEATURE_BIT_MAP &
        - BIT(31) should be set in both TCP_IP_FEATURE_BIT_MAP and EXT_TCP_IP_FEATURE_BIT_MAP. 
        - BIT(23) should be set in RSI_EXT_CUSTOM_FEATURE_BIT_MAP. 	
        - EXT_FEAT_XTAL_CLK_ENABLE (BIT(22)) should be reset in RSI_EXT_CUSTOM_FEATURE_BIT_MAP. 	

4.0 Known Issues:
=================
1. For ANT+BT+BLE application with 4Hz, audio breaks are observed.
2. Flash for 80MHz support is not available for ANT Applications.

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - BLE_CENTRAL_ANT_APP
   - BLE_PERIPHERAL_ANT_APP
   - ANT_BLE_CENT_BT_A2DP_SOURCE
   - ANT_BLE_PERI_BT_A2DP_SOURCE
   - DEMO_22 to DEMO_35, DEMO_37, DEMO_39 to DEMO_44
   - DEMO_18 with BR/EDR Adaptation

6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.2.0b7.
2. A2DP supported for SBC pass through mode.
3. Measured A2DP Power Consumption Sony MDR-XB50XS Headset
   1.8v / 2Mbps
   ------------
   Tx Power 10 dBm - ~26mA
            14 dBm - ~30mA

======================================================================================================================================================
Release            : RSI_WSDK_v0.9.5
Date               : 19 June 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_WSDK_vX/
2. The latest firmware to use with this example project is located at RSI_WSDK_vX/resources/firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map SmartStudio's workspace to RSI_WSDK_vX/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_WSDK_vX/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the apps->demo.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example located in RSI_WSDK_vX/doc. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a BT_A2DP_SOURCE_DEMO_5.pdf file in RSI_WSDK_vX/doc.
9. For 1.8v, enable 'ENABLE_1P8V' macro in RSI_WSDK_vX->apps->demo->inc
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->GNU ARM Cross C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
SmartStudio will not be able to find project files if the path is too long.
3. Refer to SmartStudio QuickStart Guide.pdf for SmartStudio related details.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. BLE_SIMPLE_GATT_DEMO_44        -->   Application for adding GAP & GATT services
2. ANT_BLE_PERI_BT_A2DP_SOURCE    -->   Application for ANT, BLE Peripheral Role & BT A2DP Source
3. ANT_BLE_CENT_BT_A2DP_SOURCE    -->   Application for ANT, BLE Central Role & BT A2DP Source
4. Added the support of Async callback to receive indication Ack/Timeout in SAPI
    
3.0 Changes/Issues fixed in this release:
=========================================
1. In DEMO_1, fixed the issue of less number of notifications per connection interval
2. In DEMOs & SAPIs, removed MAC address strings from API and used binary instead

4.0 Known Issues:
=================
1. For ANT+BT+BLE application with 4Hz, audio breaks are observed.

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - BLE_CENTRAL_ANT_APP
   - BLE_PERIPHERAL_ANT_APP
   - ANT_BLE_CENT_BT_A2DP_SOURCE
   - ANT_BLE_PERI_BT_A2DP_SOURCE
   - DEMO_22 to DEMO_35, DEMO_37, DEMO_39 to DEMO_44
   - DEMO_18 with BR/EDR Adaptation

6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.2.0b6.
2. A2DP supported for SBC pass through mode.
3. Measured A2DP Power Consumption Sony MDR-XB50XS Headset
   1.8v / 2Mbps
   ------------
   Tx Power 10 dBm - ~26mA
            14 dBm - ~30mA

======================================================================================================================================================
Release            : RSI_WSDK_v0.9.4_ANT
Date               : 29 May 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_WSDK_vX/
2. The latest firmware to use with this example project is located at RSI_WSDK_vX/resources/firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map SmartStudio's workspace to RSI_WSDK_vX/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_WSDK_vX/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the apps->demo.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example located in RSI_WSDK_vX/doc. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a BT_A2DP_SOURCE_DEMO_5.pdf file in RSI_WSDK_vX/doc.
9. For 1.8v, enable 'ENABLE_1P8V' macro in RSI_WSDK_vX->apps->demo->inc
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->GNU ARM Cross C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
SmartStudio will not be able to find project files if the path is too long.
3. Refer to SmartStudio QuickStart Guide.pdf for SmartStudio related details.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. Added applications for testing PTS test cases for BT/BLE profiles, Applications are available in the below folder.
   Please refer to the documentation for PTS application execution 
    RSI_SDK_vX.X/apps/pts_apps/
    
    (a) a2dp_source_avrcp_pts - Application used to test BT profiles A2DP, AVRCP, AVDTP AVCTP test cases
    (b) spp_slave             - Application used to test BT profiles RFCOMM, SDP, SPP test cases
    (c) smp_test              - Application used to test BLE SM (Security Manager) master/slave test cases
    (d) gatt_server           - Application used to test BLE GATT Server test cases
    (e) gatt_client           - Application used to test BLE GATT Client test cases
    (f) gap_test              - Application used to test the GAP layer Master/Slave test cases
    (g) dual_mode             - Application used to test the dual mode test cases
    (h) l2cap_slave           - Application used to test the BT L2CAP slave test cases
    (i) l2cap_ble_peripheral  - Application used to test the BLE L2CAP slave test cases
         -->Following test cases are for BLE L2CAP
              L2CAP_COS_CFC_BV_01
              L2CAP_COS_CFC_BV_02
              L2CAP_COS_CFC_BV_03
              L2CAP_COS_CFC_BV_04

   All the above applications are tested and can be run in linux platform.
2. Support for the ANT Applications.
    
3.0 Changes/Issues fixed in this release:
=========================================
1. In DEMO_1, fixed the issue of pseudo random advertising interval
2. In DEMO_7, fixed the sniff issue
3. PTS confirmation for the current stack functionality for profiles
    L2CAP, SDP, RFCOMM, GAP, GATT, SM, Audio (A2DP1.3.1, AVRCP1.5(ABS VOL), AVDTP1.0, AVCTP)  

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - BLE_CENTRAL_ANT_APP
   - BLE_PERIPHERAL_ANT_APP
   - DEMO_23 to DEMO_35, DEMO_37, DEMO_39 to DEMO_43
   - DEMO_18 with BR/EDR Adaptation

2. PTS Test cases not supported in this release
   RFCOMM/DEVA/RFC/* - All Cases
   RFCOMM/DEVA-DEVB/RFC/BV-04-C
   RFCOMM/DEVA-DEVB/RFC/BV-07-C
   RFCOMM/DEVA-DEVB/RFC/BV-17-C
   RFCOMM/DEVA-DEVB/RFC/BV-19-C
   RFCOMM/DEVA-DEVB/RFC/BV-21-C
   RFCOMM/DEVA-DEVB/RFC/BV-22-C
   RFCOMM/DEVA-DEVB/RFC/BV-25-C
   
3. ANT + BT Applications not yet created.

   
6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.2.0b3.
2. A2DP supported for SBC pass through mode.

======================================================================================================================================================
Release            : RSI_WSDK_v0.9.4
Date               : 24 May 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_WSDK_vX/
2. The latest firmware to use with this example project is located at RSI_WSDK_vX/resources/firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map SmartStudio's workspace to RSI_WSDK_vX/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_WSDK_vX/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the apps->demo.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example located in RSI_WSDK_vX/doc. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a BT_A2DP_SOURCE_DEMO_5.pdf file in RSI_WSDK_vX/doc.
9. For 1.8v, enable 'ENABLE_1P8V' macro in RSI_WSDK_vX->apps->demo->inc
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->GNU ARM Cross C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
SmartStudio will not be able to find project files if the path is too long.
3. Refer to SmartStudio QuickStart Guide.pdf for SmartStudio related details.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. Added applications for testing PTS test cases for BT/BLE profiles, Applications are available in the below folder.
   Please refer to the documentation for PTS application execution 
    RSI_SDK_vX.X/apps/pts_apps/
    
    (a) a2dp_source_avrcp_pts - Application used to test BT profiles A2DP, AVRCP, AVDTP AVCTP test cases
    (b) spp_slave             - Application used to test BT profiles RFCOMM, SDP, SPP test cases
    (c) smp_test              - Application used to test BLE SM (Security Manager) master/slave test cases
    (d) gatt_server           - Application used to test BLE GATT Server test cases
    (e) gatt_client           - Application used to test BLE GATT Client test cases
    (f) gap_test              - Application used to test the GAP layer Master/Slave test cases
    (g) dual_mode             - Application used to test the dual mode test cases
    (h) l2cap_slave           - Application used to test the BT L2CAP slave test cases
    (i) l2cap_ble_peripheral  - Application used to test the BLE L2CAP slave test cases
         -->Following test cases are for BLE L2CAP
              L2CAP_COS_CFC_BV_01
              L2CAP_COS_CFC_BV_02
              L2CAP_COS_CFC_BV_03
              L2CAP_COS_CFC_BV_04

   All the above applications are tested and can be run in linux platform.
    
3.0 Changes/Issues fixed in this release:
=========================================
1. In DEMO_1, fixed the issue of pseudo random advertising interval
2. In DEMO_7, fixed the sniff issue
3. PTS confirmation for the current stack functionality for profiles
    L2CAP, SDP, RFCOMM, GAP, GATT, SM, Audio (A2DP1.3.1, AVRCP1.5(ABS VOL), AVDTP1.0, AVCTP)  

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - DEMO_23 to DEMO_35, DEMO_37, DEMO_39 to DEMO_43
   - DEMO_18 with BR/EDR Adaptation

2. PTS Test cases not supported in this release
   RFCOMM/DEVA/RFC/* - All Cases
   RFCOMM/DEVA-DEVB/RFC/BV-04-C
   RFCOMM/DEVA-DEVB/RFC/BV-07-C
   RFCOMM/DEVA-DEVB/RFC/BV-17-C
   RFCOMM/DEVA-DEVB/RFC/BV-19-C
   RFCOMM/DEVA-DEVB/RFC/BV-21-C
   RFCOMM/DEVA-DEVB/RFC/BV-22-C
   RFCOMM/DEVA-DEVB/RFC/BV-25-C
   

   
6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.2.0b1.
2. A2DP supported for SBC pass through mode.



======================================================================================================================================================
Release            : RSI_WSDK_v0.9.3
Date               : 2 May 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_WSDK_vX/
2. The latest firmware to use with this example project is located at RSI_WSDK_vX/resources/firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map SmartStudio's workspace to RSI_WSDK_vX/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_WSDK_vX/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the apps->demo.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example located in RSI_WSDK_vX/doc. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a BT_A2DP_SOURCE_DEMO_5.pdf file in RSI_WSDK_vX/doc.
9. For 1.8v, enable 'ENABLE_1P8V' macro in RSI_WSDK_vX->apps->demo->inc
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->GNU ARM Cross C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
SmartStudio will not be able to find project files if the path is too long.
3. Refer to SmartStudio QuickStart Guide.pdf for SmartStudio related details.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. BT_A2DP_SOURCE_SBC_CODEC_DEMO_42        ----- Application for A2DP Source Set & Get configuration with Host SBC codec
2. FIRMWARE_UPGRADE_VIA_BOOTLOADER_DEMO_43 ----- Application for upgrading firmware through bootloader
3. Support for BR/EDR Adaptation - DEMO_18
4. Support for AVDTP Stats - DEMO_5
5. A2DP Power Consumption reduction - DEMO_5 

3.0 Changes/Issues fixed in this release:
=========================================
1. Release package structure change.
2. Userguide changes as per latest project structure.

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - DEMO_23 to DEMO_35, DEMO_37, DEMO_39 to DEMO_43
   - DEMO_18 with BR/EDR Adaptation
   
6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.0.4b4.
2. A2DP supported for SBC pass through mode.

======================================================================================================================================================
Release            : RSI_WSDK_v0.9.2
Date               : 20 Mar 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
-----------------------------------------------
1. Extract the release package to local directory, say RSI_WSDK_vX/
2. The latest firmware to use with this example project is located at RSI_WSDK_vX/Firmware.
   Update the Redpine EVK with this firmware version prior to running the applications.
3. Change/map MCUEXPRESSO's workspace to RSI_WSDK_vX/host/sapis/platforms/NXP_FRDM_K28.
4. Import project from 'RSI_WSDK_vX/host/sapis/platforms/NXP_FRDM_K28' into 'Project Explorer'.
5. Once project is imported, all examples are located under the sapi->examples->wsdk_apps.
6. There is a common inc folder, which includes all the common headers.
7. For each example , there is a local header file that needs to be modified before running the example.
8. For each supported and verified example, there is a user guide that provides the steps on how to run this example. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a UserGuide_DEMO_5.txt file under its example folder
9. For 1.8v, enable 'ENABLE_1P8V' macro in sapi->examples->wsdk_apps->inc->rsi_common_app.h
10. To disable RTOS support, remove 'RSI_WITH_OS' in "Project->Properties->C/C++ Build->Settings->Tool Settings->MCU C Compiler->Preprocessor" and click 'Apply and Close'.

NOTE: 
----
1. By default, 'RSI_WITH_OS' is enabled in the project.
2. It is recommended to extract the release package to any high-level directory (like 'C:\' or 'D:\' for Windows and '/root/' in Linux)  instead of using multiple sub-folders. 
MCUEXPRESSO will not be able to find project files if the path is too long.

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. BLE_DATA_TRANSFER_DEMO_37		-------    Application for BLE data transfer
2. BLE_PRIVACY_DEMO_38 	    		-------    Application for BLE privacy demo
3. BLE_LONG_READ_AND_WRITE_DEMO_39 	-------    Application for BLE long read and write demo
4. BLE_L2CAP_DEMO_40	 		-------    Application for BLE l2cap demo
5. BLE_GATT_ASYNC_DEMO_41	 	-------    Application for BLE gatt async demo
6. ANT_APP                    	 	-------    Application for ANT
7. ANT_APP_PWRSAVE      	 	-------    Application for ANT powersave

3.0 Changes/Issues fixed in this release:
=========================================
1. RTOS support for DEMO_1 to DEMO_19, DEMO_22, DEMO_36 and DEMO_38.
2. Release package structure change.
3. Fix in 'simple_http_server.py' for DEMO_19.
4. Fix in rsi_scheduler() for context switching [RTOS].
5. Userguide changes as per latest project structure.
6. Fix in a2dp set/get config() for DEMO_41. 
7. Fixed the issue of GATT callbacks do not contain device address - DEMO_41
8. Added support for Synchronous GATT API calls - DEMO_41
9. Fixed the issue of LESC Pairing Generates Generates the Same Passkey Each Time - DEMO_1

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================
1. RTOS support not available for following examples,
   - BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 
   - BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21
   - ANT_APP
   - ANT_APP_PWRSAVE
   - DEMO_23 to DEMO_35, DEMO_37, DEMO_39 to DEMO_41

6.0 Other Notes
===============
1. WiSeConnect firmware version - v1.0.4b3.
2. A2DP supported for SBC pass through mode.

===============================================================================================================================================================
Release            : RSI_WSDK_0.9
Date               : 26 Feb 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
1. The latest firmware to use with this example project is located at RS9116_Firmware folder.
   Update the Redpine EVK with this firmware version prior to running the applications; 
2. Once project is imported, all examples are located under the source->sapi->examples->rsi_demo_apps
3. There is a common inc folder, which includes all the common headers.
4. For each example , there is a local header file that needs to be modified before running the example.
5. For each supported and verified example, there is a user guide that provides the steps on how to run this example. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a UserGuide_DEMO_5.txt file under its example folder
6. For 1.8v, enable 'ENABLE_1P8V' macro in source->sapi->examples->rsi_demo_apps->inc->rsi_common_app.h

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. BT_INQUIRY_SCAN_DEMO_32      	-------    Application for BT inquiry scan
2. BT_PAGE_SCAN_DEMO_33 	      	-------    Application for BT page scan
3. BT_SPP_MASTER_SNIFF_DEMO_34  	-------    Application for BT SPP master sniff
4. BT_SPP_SLAVE_SNIFF_DEMO_35 		-------    Application for BT SPP slave sniff
5. WIFI_SSL_SERVER_CERT_BYPASS_DEMO_36 	-------    Application for SSL Server cert bypass
6. BLE_PRIVACY_DEMO_38          	-------    Application for BLE Privacy Feature
7. Added A2DP Pause and Resume APIs in following applications,
   - Pause API - rsi_bt_app_a2dp_pause()
   - Resume API - rsi_bt_app_a2dp_resume()
	(i) BLE_ADV_BT_A2DP_SOURCE_DEMO_6,
	(ii) BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20
        (iii) BT_A2DP_SOURCE_DEMO_5
        (iv) BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_18

3.0 Changes/Issues fixed in this release:
=========================================
1. Corrected powersave configuration in BT page inquiry - DEMO_27
2. Fix TCP loopback issue in controlled environment - DEMO_10
3. Corrected delay in HTTP/S RX with powersave - DEMO_19
4. Error handling in TCP RX with powersave - DEMO_12
5. Updated user guides
6. Enabled aggregation in HTTP 5MB RX powersave - DEMO_19
7. Fixed the issue of Address Resolution causes Whitelisting - DEMO_38

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================
1. Sometimes module takes more time to connect to remote BT device due to page timeouts while reconnecting.
This is reduced when the remote BT device is configured in pairing mode.

======================================================================================================================================================
Release            : RSI_WSDK_0.8
Date               : 11 Feb 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
1. The latest firmware to use with this example project is located at RS9116_Firmware folder.
   Update the Redpine EVK with this firmware version prior to running the applications; 
2. Once project is imported, all examples are located under the source->sapi->examples->rsi_demo_apps
3. There is a common inc folder, which includes all the common headers.
4. For each example , there is a local header file that needs to be modified before running the example.
5. For each supported and verified example, there is a user guide that provides the steps on how to run this example. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a UserGuide_DEMO_5.txt file under its example folder
6. For 1.8v, enable 'ENABLE_1P8V' macro in source->sapi->examples->rsi_demo_apps->inc->rsi_common_app.h

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. BT_BLE_IDLE_POWERSAVE_DEMO_30      -------    Application for BT and BLE idle powersave
2. BLE_PER_DEMO_31		      -------    Application for BLE PER test

3.0 Changes/Issues fixed in this release:
=========================================
1. Removed size argument from rsi_ble_add_service API - DEMO_1
2. Added support for MITM Protection - DEMO_1

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================
1. Sometimes module takes more time to connect to remote BT device due to page timeouts while reconnecting.
This is reduced when the remote BT device is configured in pairing mode.

======================================================================================================================================================
Release            : RSI_WSDK_0.7
Date               : 6 Feb 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
1. The latest firmware to use with this example project is located at RS9116_Firmware folder.
   Update the Redpine EVK with this firmware version prior to running the applications; 
2. Once project is imported, all examples are located under the source->sapi->examples->rsi_demo_apps
3. There is a common inc folder, which includes all the common headers.
4. For each example , there is a local header file that needs to be modified before running the example.
5. For each supported and verified example, there is a user guide that provides the steps on how to run this example. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a UserGuide_DEMO_5.txt file under its example folder
6. For 1.8v, enable 'ENABLE_1P8V' macro in source->sapi->examples->rsi_demo_apps->inc->rsi_common_app.h

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. BLE_ADV_DEMO_23                 -------    Application for BLE Advertising
2. BLE_IDLE_POWERSAVE_DEMO_24      -------    Application for BLE idle powersave
3. BLE_SCAN_DEMO_25                -------    Application for BLE scan
4. BT_CONNECTED_SNIFF_DEMO_26      -------    Application for BT connected sniff
5. BT_PAGE_INQUIRY_DEMO_27         -------    Application for BT page inquiry
6. BT_PER_DEMO_28                  -------    Application for BT per test
7. BT_SPP_RX_TX_DEMO_29            -------    Application for BT SPP Rx and Tx

3.0 Changes/Issues fixed in this release:
=========================================
1. Added support for GATT Table Creation with Attributes Stored in Application Layer - DEMO_1
2. Added support for Notify Value API Inquiry and Multiple Notifications Per Connection Interval - DEMO_2
3. Added support for Random Advertising Interval - DEMO_1

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================

======================================================================================================================================================
Release            : RSI_WSDK_0.6
Date               : 8 Jan 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
1. The latest firmware to use with this example project is located at RS9116_Firmware folder.
   Update the Redpine EVK with this firmware version prior to running the applications; 
2. Once project is imported, all examples are located under the source->sapi->examples->rsi_demo_apps
3. There is a common inc folder, which includes all the common headers.
4. For each example , there is a local header file that needs to be modified before running the example.
5. For each supported and verified example, there is a user guide that provides the steps on how to run this example. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a UserGuide_DEMO_5.txt file under its example folder
6. For 1.8v, enable 'ENABLE_1P8V' macro in source->sapi->examples->rsi_demo_apps->inc->rsi_common_app.h

1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================
1. FIRMWARE_UPGRADE_DEMO_22                            -------    Application for Firmware Upgrade

3.0 Changes/Issues fixed in this release:
=========================================

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================


======================================================================================================================================================
Release            : RSI_WSDK_0.5
Date               : 4 Jan 2019
_______________________________________________________________
 	Copyright (C) Redpine Signals, Inc 2019
_______________________________________________________________


Steps to do to run the demo's with this release:
1. The latest firmware to use with this example project is located at RS9116_Firmware folder.
   Update the Redpine EVK with this firmware version prior to running the applications; 
2. Once project is imported, all examples are located under the source->sapi->examples->rsi_demo_apps
3. There is a common inc folder, which includes all the common headers.
4. For each example , there is a local header file that needs to be modified before running the example.
5. For each supported and verified example, there is a user guide that provides the steps on how to run this example. 
   For example, BT_A2DP_SOURCE_DEMO_5 has a UserGuide_DEMO_5.txt file under its example folder
6. For 1.8v, enable 'ENABLE_1P8V' macro in source->sapi->examples->rsi_demo_apps->inc->rsi_common_app.h


1.0 Release Status:
===================

    Test         ( )
    Alpha        (X)
    Beta         ( )
    Production   ( )


2.0 New features supported in this release:
===========================================

 1. BLE_PERIPHERAL_DEMO_1                               -------    Application for BLE Peripheral
 2. BLE_CENTRAL_DEMO_2                                  -------    Application for BLE Central
 3. BLE_DUAL_ROLE_DEMO_3                                -------    Application for BLE dual role
 4. BLE_POWERSAVE_DEMO_4                                -------    Application for Power save measurements
 5. BT_A2DP_SOURCE_DEMO_5                               -------    Application for BT A2DP audio play
 6. BLE_ADV_BT_A2DP_SOURCE_DEMO_6                       -------    Application for BLE advertising and BT A2DP audio play
 7. BT_EIR_SNIFF_SPP_SLAVE_DEMO_7                       -------    Application for EIR, DID and Sniff
 8. BLE_ADV_BT_SPP_THROUGHPUT_DEMO_8                    -------    Application for SPP TX/RX Throughput
 9. WIFI_SSL_RX_THROUGHPUT_DEMO_9                       -------    Application for TCP/SSL RX throughput 
10. WIFI_TCP_BI_DIR_DEMO_10                             -------    Application for TCP RX_TX (loop back)
11. WIFI_SSL_TX_THROUGHPUT_DEMO_11                      -------    Application for TCP/SSL TX throughput
12. WIFI_TCP_RX_POWERSAVE_DEMO_12                       -------    Application for TCP_RX Throughput with Power save 
13. WIFI_CONNECTED_STANDBY_DEMO_13                      -------    Application for STANDBY ASSOCIATED
14. WIFI_STA_BGSCAN_DEMO_14                             -------    Application for BGSCAN with power save
15. WIFI_STA_CONN_DISCONN_SSL_CONN_DISCONN_DEMO_15      -------    Application for WLAN SECURE CONN-DISCONN and TLS SOCKET CONN-DISCONN
16. WIFI_TCP_TX_POWERSAVE_DEMO_16                       -------    Application for TCP TX Throughput with power save
17. WIFI_DEEPSLEEP_STANDBY_DEMO_17                      -------    Application for STANDBY ASSOCIATED, DEEPSLEEP with RAM retention and DEEPSLEEP without RAM retention
18. BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_18               -------    Application for BT(A2DP)+ HTTP download  
19. WIFI_HTTP_S_5MB_RX_WITH_STANDBY_DEMO_19             -------    Application for SSL RX(5 MB) and 5 minutes SLEEP  
20. BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 -------    Application for BT(A2DP)+ HTTP download + BLE (2 Central & 6 Peripheral)
21. BLE_DUAL_MODE_BT_SPP_SLAVE_DEMO_21                  -------    Application for BT(SPP)+ BLE (3 Central & 2 Peripheral)       
22. FIRMWARE_UPGRADE_DEMO_22                            -------    Application for Firmware Upgrade

3.0 Changes/Issues fixed in this release:
=========================================

4.0 Known Issues:
=================

5.0 Limitations/Features not supported:
======================================
1. In BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_18 and BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX_DEMO_20 examples,
   -> If SD_DEMO is enabled
         -> MAX HTTP download size is 16 MB
         -> Continuous download is not supported    
======================================================================================================================================================
