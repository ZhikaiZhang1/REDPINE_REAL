===============================================================================================
                               GSPI Master Loop Back Example
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
RTE_GSPI_MASTER_CLK_PIN (GPIO_72(ULP_GPIO_8)) : J2.24
RTE_GSPI_MASTER_CS0_PIN (GPIO_73(ULP_GPIO_9)) : J2.26
RTE_GSPI_MASTER_MOSI_PIN(GPIO_75(ULP_GPIO_11)): J2.30  
RTE_GSPI_MASTER_MISO_PIN(GPIO_74(ULP_GPIO_10)): J2.28	

Connect J2.30 to J2.28 (loopback connection)

Pass criteria
===============================================================================================
Open hyper terminal  to see the sent data and  received data.
If the testdata_out and testdata_in both are equal then you will get "Transmission Successful"
else "Transmission failed".
===============================================================================================
===============================================================================================
Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 
2)The print's content can be observed on any serial terminal  through UART to USB(J21) . 
===============================================================================================





