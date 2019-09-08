===============================================================================================
                               WDT example program
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
The WWDT example demonstrates the WDT warning trigger interrupt handler. 
The watchdog generates a warning WWDT interrupt and then restarts the WWDT
on the warning (the LED0 will toggle on each warning interval cycle). 
After 10 warning interrupts the re-start request is stopped to generates a watchdog reset.


===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK


Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
complile and run the application 
===============================================================================================

Pass criteria:
===============================================================================================
Note:The following are the prints we can seen on serial terminal 
--------------
Main..!
Power on RESET occurred..
WDT handler 1
WDT handler 2
WDT handler 3
WDT handler 4
WDT handler 5
WDT handler 6
WDT handler 7
WDT handler 8
WDT handler 9
WDT handler 10
Main..!
WDT RESET occurred..

===============================================================================================
Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 

2)The print's content can be observed on any serial terminal  through UART to USB(J21) . 
===============================================================================================