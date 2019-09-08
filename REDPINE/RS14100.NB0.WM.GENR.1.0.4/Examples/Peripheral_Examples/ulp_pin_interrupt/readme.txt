=========================================================================================================
                               ULP Pin Interrupt
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
it will use the Ulp_GPIO_6:J2.20 for pin interrupt generation with level low event enable,
for level low event on input pin will generates pin interrupt.  
=========================================================================================================
Environment
=========================================================================================================
Assistance tool:
---------------------
Smart EVK

Assistance software:
---------------------
None

Setup requirement:
===============================================================================================
Connect the Ulp_GPIO_6:J2.20 to level low event or ground on smart EVK 

========================================================================================================
Usage Procedure:
========================================================================================================
Feed level low on input pin and put a break point in the ISR and check the pin interrupt 
status in status register. 
========================================================================================================
===============================================================================================
Pass criteria
===============================================================================================
when the ULP_GPIO_6:J2.20 is connected to level low event ISR will happen . 

Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 

2)The print's content can be observed on any serial terminal  through UART to USB(J21) . 
===============================================================================================