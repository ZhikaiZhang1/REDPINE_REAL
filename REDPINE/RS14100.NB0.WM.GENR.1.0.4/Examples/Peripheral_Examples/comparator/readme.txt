===============================================================================================
                               Comparator Example Program
===============================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 
                                                  
Function description:
===============================================================================================
This example explains how to verify the comparator, its compare two external voltages and gives 
the output in high or in low. if positive voltage is greater than negative voltage then comparator 
output is high and comparator interrupt is hitting.
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
compile and run the application and supply the external voltage to pin by using power supply.
depends on input voltage the comparator output is high or low. 
===============================================================================================

===============================================================================================
Setup requirement:
===============================================================================================
connect external voltage to following pin:
Positive input : (ULP_GPIO_0):J2.6 (Analog pin0)
Negative input : (ULP_GPIO_1):J2.8 (Analog pin1)
===============================================================================================

===============================================================================================
Pass criteria:
===============================================================================================
Positive input > Negative input <==> output high ,interrupt is hitting.
Positive input < Negative input <==> output low  ,interrupt is not hitting.
===============================================================================================