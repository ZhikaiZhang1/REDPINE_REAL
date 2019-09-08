===============================================================================================
                               IR Decoder Example Program
===============================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 
                                                  
Function description:
===============================================================================================
This example demonstrates the decoding IR signal pattern and read the receiving data pattern.
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
In this example generating IR pulse on GPIO6 by using systic timer and generate pulses on GPIO6
give the input to IR decoder.
If IR decoder recognize the pulse then IR interrupt is hit.Read the pulses data in  'ir_data[]' 
receiver buffer.  
===============================================================================================

===============================================================================================
Setup requirement:
===============================================================================================
In this example loop back the following pins:
ULP_GPIO10 (IR Decoder input) <==> GPIO6 (Systic timer output pin)
===============================================================================================

===============================================================================================
Pass criteria:
===============================================================================================
If Systic timer generate proper waveform then interrupt is hitting.
===============================================================================================