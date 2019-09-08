============================================================================================================================================================
                                                     Ethernet Example
============================================================================================================================================================



Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 

===============================================================================================
===============================================================================================                                                       
Function description:
===============================================================================================
The cts example illustartes about sleeps and wakeup from sleep whenever touch happened

===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK board

Assistance tool:
---------------------
none

Assistance software:
---------------------
logic analyzer 
===============================================================================================
Usage Procedure:
===============================================================================================
a.Configure ON_TIME and OFF_TIME
b.Select how many sensor wants to use for scanning(1 to 8)
c.Select Sampling pattern e.g to scan all the sensor set sampling pattern 0x12345678
d.Set the threshold value to which wakeup from sleep  
 
Pass criteria
=============
open the logic analyzer software and check on UULPGPIO_3 for sleep and wakeup wakeforms.
===============================================================================================
Notice:
===============================================================================================
 

===============================================================================================

