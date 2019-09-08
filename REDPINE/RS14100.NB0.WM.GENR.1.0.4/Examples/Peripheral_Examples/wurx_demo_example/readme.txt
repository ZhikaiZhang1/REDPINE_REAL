===============================================================================================
                               wurx demo example program
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
In this example configure the l2 frequency pattern and pattern type1 for pattern detection
following configuration are use in example :
Pattern type                : l2 pattern.
l1 bypass                   : Enable
l2 receive frequency        : 32khz(l1 sel value as 1)
Pattern length for l2       : 64 bits 
Pattern detection threshold : 75 % of for input bits pattern.
Transmitter frequency       : 0x780   /* channel1 frequency */
Pattern detection           : Pattern1 detection enable.
by using above configuration receiving the pattern , after match value detect the ISR will be hit,
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
For the pass criteria in this example after receiving pattern form transmitter its compare 
with existing configured pattern match value , if that value match then wake_rx interrupt is 
occurred and m4 is wake by using wurx as wake up source.In wurx IRQ toggle the LED for each
of wurx interrupt. 
===============================================================================================
===============================================================================================
Notice:
===============================================================================================

===============================================================================================