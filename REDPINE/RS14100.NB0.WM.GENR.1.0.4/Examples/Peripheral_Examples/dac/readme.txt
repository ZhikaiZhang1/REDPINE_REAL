===============================================================================================
                               DAC example program
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
This example explain the DAC functionality in FIFO mode.
here input register of DAC are feed by using external UDMA and DAC output play on ULP_GPIO4.
Following configuration use in example.
1. DAC operation mode  : FIFO mode
2. DAC FIFO threshold  : 4
3. DAC output GPIO     : ULP_GPIO4
4. DAC input samples   : Standard sine wave samples are feed to DAC input register. 
===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smark EVK 

Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
compile and run the application.

===============================================================================================
Pass criteria:
===============================================================================================
Check the ULP_GPIO4 status on 'Logic analyzer' or on 'oscilloscope'. 
If on ULP_GPIO4 play the sine wave then DAC output is proper and test case is pass. 
===============================================================================================