===============================================================================================
                               Blinky example program
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
The Blinky example is known as the "Hello, World!" of embedded programming. This
example will toggles the GPIO 10times per second.

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
compile and run the application. on board LED will be toggling 
continuously.

Note:If the companion GPIO board is ported on peripheral board ,then it's not possible to see the 
LED toggling .

Pass criteria:
===============================================================================================
on board LED will be toggling continuousness 
===============================================================================================

===============================================================================================
Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 
2) The print's content can be observed on any serial terminal  through UART to USB(J21) .
3) Please enable macro REV_1P2_CHIPS whenever running this example on Rev1.2 EVKs 
===============================================================================================