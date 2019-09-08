===============================================================================================
                               opamp Example Program
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
The opamp example illustrates about Unity Gain Buffer or Voltage follower by giving input on 
non-inverting terminal of opamp and checking output.

===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK board.

Assistance tool:
---------------------
Logic analyzer to check output voltage

Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
Pin connections to check output voltage and calculating offset voltage.
Apply input voltage on Non-Inverting terminal (ULP_GPIO_7):J2.22 from constant D.C supply source upto
2.4v.Check the output voltage on output pin (ULP_GPIO_4):J2.14.
calculate the offst voltage(offset=Vout-Vin)
===============================================================================================
===============================================================================================
Pass criteria
===============================================================================================
Output voltage should be equal to input voltage.Use Logic analyzer / Oscilloscope  to check output 
voltage

Notice:
===============================================================================================

===============================================================================================