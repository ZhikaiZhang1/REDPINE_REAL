====================================================================================================================
                               Group interrupt example program
====================================================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 

====================================================================================================================
History 
Date        Ver    
2017-06-27  0.9   
====================================================================================================================                                                      
Functional description:
====================================================================================================================

This example configures GPIO(6):J1.7 ,GPIO(7):J1.9 are two inputs for group interrupt(GROUP INTR1 , GROUP INTR2) generation,
it configures these two pins in edge(high polarity) with Logical OR event,interrupt will be generated for every
rise edge on two input pins when Logical OR satisfies,in this example will gives an interrupt any one of the input 
pin matches its status with the configured polarity. 
====================================================================================================================


Assistance software:
---------------------
None

====================================================================================================================
Usage Procedure:
====================================================================================================================
Configure the GPIO6):J1.7 and GPIO7:J1.9 pins for group interrupt edge with OR event
to be detected,and put a break point in the ISR and check the group interrupt status in
GROUP INTR STATUS regeter. 
====================================================================================================================
Notice:
====================================================================================================================
1) The printf content can be output to hyperterminal through USB to UART on board.
2) Enable DEBUG_UART macro in tool pre-processor window. 

====================================================================================================================