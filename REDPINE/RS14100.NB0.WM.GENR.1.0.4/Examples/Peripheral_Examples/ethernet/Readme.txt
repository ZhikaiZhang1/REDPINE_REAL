============================================================================================================================================================
                                                     Ethernet Example
============================================================================================================================================================



===============================================================================================
                               Ethernet example program
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
The ethernet example illustartes about send and receive the packets from mac in full duplex mode 

===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK board

Assistance tool:
---------------------
none

Assistance software:
---------------------
wireshark software to check packets

===============================================================================================
Usage Procedure:
===============================================================================================
Pin connections to check the packets on wireshark 
 phy       board
MDC     MDC(GPIO_11)   
MDO     MDO(GPIO_12)  
RXD1    RXD1(GPIO_15)  
RXD0    RXD0(GPIO_10) 
RXCLK   RXCLK(GPIO_8) 
TXEN    TXEN(GPIO_9)   
TXD0    TXD0(GPIO_7)   
TXD1    TXD1(GPIO_6)   
CRS_DV  CRS_DV(GPIO_16)

Pass criteria
=============
open the wireshark and check whether the tansmitted packets are coming or not with proper mac 
address i.e 11:22:33:44:55:66 
===============================================================================================
Notice:
===============================================================================================
 

===============================================================================================

