================================================================================================================
                               Wakeup interrupt example program
================================================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 

================================================================================================================
History 
Date        Ver    
2017-06-27  0.9   
================================================================================================================                                                   
Functional description:
================================================================================================================
RS9116 chip supports GPIO Wakeup interrupt ,This wakeup interrupt example will configures GPIO(6),GPIO(7) are input
pins for the wakeup interrupt generation.It generates interrupt when a pattern is detected on selected pins,We can
configure pattern on group of input pins to generate an interrupt.Here polarity 1 means high state,and polarity 0 
means low state and configure the type of event to be detected(EDGE/LEVEL) with Logical AND or logical OR of pin 
inputs,It will check each input pin status and configured polarity,Logical And event will generates group interrupt 
when each input pin status and configured polarities are true where as Logical OR event will generates group interrupt
for one of the input pins satisfies its pin status and configured polarity.

This example configures these two pins in edge(high polarity) with Logical OR event,interrupt will be generated for every
rise edge on two input pins when Logical OR satisfies,in this example will gives an interrupt any one of the input 
pin matches its status with the configured polarity.Wakeup interrupts can be detected even when clock is not there
for egpio module. 
  
================================================================================================================
Environment
================================================================================================================
Test Board:
---------------------
Smart EVK

Assistance tool:
---------------------
Logic analyzer to check the pin status

Assistance software:
---------------------
None

===============================================================================================================
Usage Procedure:
===============================================================================================================
Configure the GPIO6 and GPIO7  pins for group interrupt edge or level 
with OR or AND events to be detected, and enable wkeup interrupt(Wakeup interrupts can be detected even when
clock is not there for egpio module) and put a break point in the ISR and check the group interrupt status in 
GROUP INTR STATUS regeter. 
===============================================================================================================
Notice:
===============================================================================================================
1) The printf content can be output to hyperterminal through USB to UART converter connected to
   ULPSS-UART pins.
2) Enable DEBUG_UART macro in tool pre-processor window. 

===============================================================================================================