===============================================================================================
                               SSI Master Loop Back Example
===============================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 

===============================================================================================                                                  
Function description:
===============================================================================================
This example SSI master is configured.Here we will connect MOSI pin  to MISO pin  i.e loop back.
master transmits data on MOSI pin and  receives same data on MISO pin.
The data received  should match with the transmitted data. 

===============================================================================================
Environment
===============================================================================================
Test Board:
Smart EVK 

===============================================================================================
Usage Procedure:
===============================================================================================
compile and then run the application,   
add testdata_out, testdata_in  to watch window.

setup requirements :
------------------
SSIM_CLK (GPIO_8) : J1.11
SSIM_CS0 (GPIO_9 ): J1.13
SSIM_MOSI(GPIO_6) : J1.7 <-> SSIM_MISO(GPIO_7) :J1.9 (MOSI and MISO pin loop back connection)


===============================================================================================
Pass criteria:
===============================================================================================
If the testdata_out is equal to testdata_in then the transmission successful,else failed.







