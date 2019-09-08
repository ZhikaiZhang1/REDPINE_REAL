===============================================================================================
                              Motor Control PWM Example Program
===============================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 
                                                  
Function description:
===============================================================================================
This example, uses the MCPWM driver to create 2 independent PWMs (running at the same frequency).
The PWM1 is used to output a square wave to an Output pin and PWM2 is used to control the 
brightness of the LED,the brightness goes from 0% to 100% in steps of 0.5% every 20 Milliseconds 
and goes down to 0% in the same step. 
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

setup requirements 
------------------
Observe PWM output on GPIO_19 and GPIO_11
Connect ( GPIO_8)  :J1.11  <-> (ULP_GPIO_0)  :J2.6 
compile and run the application 
Note : If TriLED is not working connect GPIO_19 and GPIO_11 to logic port to observe the generated PWM signals.
===============================================================================================
Pass criteria:
===============================================================================================
See the TRI_LED on the board ,if  LEDs brightness goes from 0% to 100% in steps of 0.5% every 20 
Milliseconds and goes down to 0% in the same step then the test is success .
===============================================================================================




