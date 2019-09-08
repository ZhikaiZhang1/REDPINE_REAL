 
===========================================================================================================
                               USART Slave Program
===========================================================================================================
Copyright(C) Redpine Signals 2017
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 
============================================================================================================                                                        
============================================================================================================
Function description:
===============================================================================================
This example is for USART slave configuration,it will sends and receives data in full 
duplex mode(when master generates clock)
===============================================================================================
===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smark EVK 

Assistance software:
---------------------
None
setup requirements :
------------------
1st chip(Master)              2nd chip(Slave)
 USART0_CLK_PIN    <->         USART0_CLK_PIN (GPIO_10):J1.15
 USART0_TX_PIN     <->         USART0_TX_PIN  (GPIO_9) :J1.13
 USART0_RX_PIN     <->         USART0_RX_PIN  (GPIO_8) :J1.11

============================================================================================================
Usage Procedure:
============================================================================================================
->Enable RTE_USART_MODE and RTE_CONTINUOUS_CLOCK_MODE in RTE_Device.h
->Connect master usart TX_GPIO pin to slave RX pin ,master usart RX_GPIO pin to slave TX pin and
  master clock pin to slave clock pin
===============================================================================================
Pass criteria
===============================================================================================
Read the both master and slave receive buffer.check whether the tx data,rx data's are same or not.
============================================================================================================
Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 

2)The print's content can be observed on any serial terminal  through UART to USB(J21) . 
===============================================================================================