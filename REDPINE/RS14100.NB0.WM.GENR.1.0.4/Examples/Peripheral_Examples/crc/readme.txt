===============================================================================================
                              CRC Example Program
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
In this example array of input data will be given using DMA to CRC engine.
CRC was feeded with 32 bit standard polynomial(0x04C11DB7).
Read the generated CRC from crc variable after dma done status.
Generated checksum for the array of input data need to be compared with the 
checksum calculated by software CRC API and both the values should get match 
to ensure CRC engine functionality.

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
complile and run the application. 
===============================================================================================
===============================================================================================

Pass criteria
==============
hw_crc and sw_crc value  should be equal. 

===============================================================================================
===============================================================================================
Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 
2)The print's content can be observed on any serial terminal  through UART to USB(J21) . 
===============================================================================================

