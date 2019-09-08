
===========================================================================================================
                               CCI Example Program
===========================================================================================================
Copyright(C) Redpine Signals 2017
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 

============================================================================================================
============================================================================================================                                                       
Function description:
============================================================================================================
The CCI(Companion Chip Interface)example shows how to use the CCI to access the second chip memory.The CCI 
module helps external memories and peripherals to communicate with internal AHB bus.This enables connecting
our smallest size chip with M4 to other chips where M4 may or may not be present.
This application configures AMS (Arbiter Master Slave) in the chip1,and the chip2(slave)configuration is done by 
bootloader.Once the slave is configured properly then the master can acccess the 2nd chip memory by using 
CCI ahb slave address.
============================================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smark EVK 
Assistance tool:
---------------------
Logic analyzer to check the pin toggle status

Assistance software:
---------------------
None

============================================================================================================
Usage Procedure:
============================================================================================================
To use the example,connections from chip1 to chip2 should be done like below

1st chip(Master)              2nd chip(Slave)
DATA0 (GPIO_12)     to     DATA0                          
DATA1 (GPIO_13)     to     DATA1
DATA2 (GPIO_14)     to     DATA2
DATA3 (GPIO_15)     to     DATA3
CLOCK (GPIO_10)     to     CLOCK
CS0   (GPIO_53)     to     CS0
RDY0  (GPIO_35)     to     DATAVALID
DATAVALID (GPIO_36) to     RDY0

-->Run cci_master code(When slave is configured) in 1st chip to access 2nd chip memory
============================================================================================================
Notice:
============================================================================================================
1) The printf content can be output to hyperterminal through USB to UART converter connected to
   ULPSS-UART pins.
2) Enable DEBUG_UART macro in tool pre-processor window. 

============================================================================================================