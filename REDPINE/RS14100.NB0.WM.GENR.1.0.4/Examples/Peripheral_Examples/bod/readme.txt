===============================================================================================
                               BOD example program
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
The BOD example will rise interrupt when the supply voltage is less than the BOD threshold .
Note:We can configure the thershold value at the line 37.
     We can select the Auto mode BOD or Manual BOD at line 31 and 32
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
compile and run the application.

===============================================================================================
Pass criteria:
===============================================================================================
Keep a break point at the interrupt,decrease the voltage less than the threshold voltage
then cursor will hit break point .
===============================================================================================