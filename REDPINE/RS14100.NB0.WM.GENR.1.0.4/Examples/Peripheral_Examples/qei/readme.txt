===============================================================================================
                               QEI  Example Program
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
This example demonstrates the velocity,position count,index count and direction measurement
using QEI module.

===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK 

Assistance software:
---------------------
None
s
===============================================================================================
Usage Procedure:
===============================================================================================
setup requirements :
- Connect GPIO_8 :J1.11 to GPIO_10 :J1.15 this is phase A signal
- Connect GPIO_7 :J1.9  to GPIO_11 :J1.19 this is phase B signal
- Connect GPIO_6 :J1.7  to GPIO_12 :J1.21 this is index signal
- GPIO9 is output from QEI module this outputs the direction of the motor

GPIO_8,ULP_GPIO_2 and GPIO_6 pins are used to emulate the QEI timming wave forms which are feeded to 
QEI module.
This example gives the index position count , velocity of the QEI signals.
===============================================================================================
===============================================================================================
Pass criteria
===============================================================================================
        1] Put break point in 105 line in  file ,in QEI_IRQHandler() this funcation.
	2] This example gives the index position count,velocity of the QEI signals. 
	3] check the value of 'qeiVelocity' in watch wimdow,its around F9 value (approx).



