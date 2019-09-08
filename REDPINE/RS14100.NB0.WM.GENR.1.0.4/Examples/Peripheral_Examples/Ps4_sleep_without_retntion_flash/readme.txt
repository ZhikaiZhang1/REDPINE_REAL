===============================================================================================
     MCU PS4 state sleep wake up Example Program without RAM retention with flash 
     based execution					   
===============================================================================================
Copyright(C) Redpine Signals 2017
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 

===============================================================================================
===============================================================================================                                                       
Functional description:
===============================================================================================
This Example configures the processor state from PS4 to PS0 sleep and then wakes up from deep sleep  based on RTC seconds interrupts with flash based execution,without RAM retention mode following mode is there:
1.Using ULPSS RAM -: Here ULPSS RAM address are used as stack pointer so here only ULPSS RAM must
                      be enable
2.Using NPSS battery filpflop : Here M4 RAM address are used for stack pointer so here not required
                                to reten .
===============================================================================================
 
Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
1.Build and run the program 
2.Connect the power meter at J19 in smart EVK to see the sleep current. 
===============================================================================================