
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
our smallest size chip with M4  to other chips where M4 may or may not be present.
This application configures AMS (Arbiter Master Slave) in the chip1,and the chip2(slave)configuration is done by 
bootloader.Once the slave is configured properly then the master can acccess the 2nd chip memory by using 
CCI ahb slave address.
============================================================================================================
Environment
============================================================================================================ 
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

1st chip(Master)         2nd chip(Slave)
DATA0             to     DATA0 (GPIO_12)                         
DATA1             to     DATA1 (GPIO_13)
DATA2             to     DATA2 (GPIO_14)
DATA3             to     DATA3 (GPIO_15)
CLOCK             to     CLOCK (GPIO_10)
CS0               to     CS0    (GPIO_53)
RDY0              to     DATAVALID (GPIO_35)
DATAVALID         to     RDY0      (GPIO_36)

-->Run cci_slave code in 2nd chip,now 2nd chip will be configured
   to slave mode
============================================================================================================
Notice:
============================================================================================================
1) The printf content can be output to hyperterminal through USB to UART converter connected to
   ULPSS-UART pins.
2) Enable DEBUG_UART macro in tool pre-processor window. 

============================================================================================================