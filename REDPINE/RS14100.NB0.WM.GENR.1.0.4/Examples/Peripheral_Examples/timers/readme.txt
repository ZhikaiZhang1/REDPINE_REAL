===============================================================================================
                                   TIMERS Example
===============================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 
                                                  
Function description:
===============================================================================================
This Timer example uses two timers to blink two LEDs at different rates. The timers are configured to 
generate interrupts upon expiration which is when the LEDs are toggled. The first timer is Timer 0 
set to expire at 1-second (1Hz) intervals.The second timer is Timer 1 used with a
prescale value to expire at.5-second (2Hz) intervals.
===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK 

---------------------


Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
compile and run the application.


===============================================================================================
Pass criteria:
===============================================================================================
See the TRI_LED on the board ,if  LEDs blinks at different rates then the test is success ,else  fail.
Note:If the companion GPIO board is ported on peripheral board ,then it's not possible to see the 
LED toggling .
===============================================================================================





