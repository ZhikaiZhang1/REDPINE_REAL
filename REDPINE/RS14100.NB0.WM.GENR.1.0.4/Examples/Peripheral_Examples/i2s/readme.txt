===============================================================================================
                                 I2S Master Example
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
This example I2S will be configured in master mode.Here we will connect TX pin to RX pin i.e
loopback.Master transmits data on TX pin using DMA and receives same data on RX pin using DMA.
The data received should match with the transmitted data.
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
complile and run the application ,add rx_buf and tx_buf at watch window and check the data 
Setup requirement:
==================
SCLK (GPIO_49)   -> Connect to oscilloscope to check the I2S frequency
WSCLK(GPIO_48)   -> Connect to logic analyser to check the sampling frquency
DOUT (GPIO_6) :J1.7   <-> DIN(GPIO_10):J1.15 (TX and RX pin loopback connection)

===============================================================================================
Pass criteria:
===============================================================================================
Both write and read data should get match.
===============================================================================================