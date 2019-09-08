
===============================================================================================
                               UDMA example program
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
This example shows how to setup and use the UDMA controller for a limited software triggered 
memory to memory transfer. Because of the limited burst transfer size of the DMA (1024 transfers max) 
and limited burst size, DMA M2M operations are somewhat specialized, but they do have a good performance
boost over memcpy() and memmove() functions. They would work best for smaller transfers.
===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK

Assistance tool:
---------------------
None

Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
complile and run the application 

===============================================================================================

Pass criteria
===============================================================================================
The src and dst buffer should be same 

===============================================================================================
Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 
  
2)The print's content can be observed on any serial terminal  through UART to USB(J21) . 


