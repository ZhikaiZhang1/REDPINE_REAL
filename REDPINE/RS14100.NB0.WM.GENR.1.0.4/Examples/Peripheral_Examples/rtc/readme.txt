===============================================================================================
                               Calender Example Program
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
This example explains how to configure the Real Time Clock (RTC) in Redpine MCU chip.This 
explains the periodic alaram configuration for every 5 sec with alarm ISR. And RTC milli seconds
and seconds interrupts are also enabled in this example.
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
complile and run the application watch Real time clock on UART terminal or in debug variables
===============================================================================================
===============================================================================================
Pass criteria
===============================================================================================
On hyperterminal ,Time and date should be displayed 
===============================================================================================
Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 
2)The print's content can be observed on any serial terminal  through UART to USB(J21) . 
===============================================================================================