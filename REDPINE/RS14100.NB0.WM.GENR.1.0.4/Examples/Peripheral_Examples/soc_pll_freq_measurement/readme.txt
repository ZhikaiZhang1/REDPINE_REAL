===============================================================================================
                               PLL frequency measurement example program
===============================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 

===============================================================================================
 
===============================================================================================                                                       
Function description:
===============================================================================================
This example explains how to configure SOC-PLL(Phased Lock Loop) in Redpine MCU chip. and explains the 
How to meaure pll-clock from GPIO pin.
===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smark EVK 

Assistance software:
---------------------
Logic analyzer software is required and oscilloscope is also required.

===============================================================================================
Usage Procedure:
===============================================================================================
complile and run the application .To change the required frequency change 'PLL_OUT_FREQUENCY'
macro in soc_pll_freq_measurement.c file. 
===============================================================================================

Setup requirement:
===============================================================================================
GPIO-12:J1.21 <->  Connect the logic Analyzer/ Oscilloscope
===============================================================================================

===============================================================================================
Pass criteria
===============================================================================================
Frequency out on logic Analyzer should match with PLL_OUT_FREQUENCY
===============================================================================================

Notice:
===============================================================================================
1) Use high frequency supported logic analyzers to measure the high frequency clocks.
===============================================================================================