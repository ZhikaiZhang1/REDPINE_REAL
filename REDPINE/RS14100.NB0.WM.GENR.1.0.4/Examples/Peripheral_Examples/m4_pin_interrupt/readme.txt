=========================================================================================================
                               M4 Pin Interrupt 
=========================================================================================================
Copyright(C) Redpine Signals 2017
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 

========================================================================================================= 
=========================================================================================================                                                      
Functional description:
=========================================================================================================
This example will configures a pin to contribute for pin interrupt,
This example will use the GPIO(GPIO6):J1.7  for pin interrupt generation with level low event enable,
for level low event on input pin will generates pin interrupt.  
=========================================================================================================
Environment
=========================================================================================================
Test Board:
---------------------
Smark EVK 

Assistance software:
---------------------
None

========================================================================================================
Usage Procedure:
========================================================================================================
Feed level low on input pin and put a break point in the ISR and check the pin interrupt 
status in status register. 
========================================================================================================
===============================================================================================
Pass criteria
===============================================================================================
If the GPIO_6:J1.7 is connected to level low event then ISR will happen or else no.