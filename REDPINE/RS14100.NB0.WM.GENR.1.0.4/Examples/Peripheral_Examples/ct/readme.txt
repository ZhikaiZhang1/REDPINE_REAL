===============================================================================================
                               CT Example Program
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
This example, uses the MCPWM driver to create 2 independent PWMs (running at
the same frequency). The PWM1 is used to output a square wave to an Output pin
and PWM2 is used to control the brightness of the LED, the brightness goes
from 0% to 100% in steps of 0.5% every 20 Milliseconds and goes down to 0% in
the same step. Connect oscilloscope to the PWM OUT pin and the waveform
displayed on the scope will have its duty cycle increasing from 0% to 100% in
steps of 10% every second
===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK

Assistance tool:
---------------------
None

Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
- complile and run the application 
setup requirements
==================
GPIO_7  : J1.9  <->  UlP_GPIO_0  :J2.6 (loop back)
GPIO_50 : J2.11 <->  UlP_GPIO_2  :J2.10 (loop back)
===============================================================================================
===============================================================================================
Pass criteria:
===============================================================================================
See the TRI_LED on the board ,if  LEDs brightness goes from 0% to 100% in steps of 0.5% every 20 
Milliseconds and goes down to 0% in the same step then the test is success .
Please observe the output on GPIO_7 and GPIO_50 pins.
================================================================================
