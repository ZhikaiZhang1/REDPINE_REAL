===============================================================================================
                               Button example program
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
The  Button example will rise the button interrupt  .

===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smark EVK 

Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
compile and run the application.Give the button range voltages to UULPSS_GPIO(2):J2.25.
===============================================================================================
Pass criteria:
===============================================================================================
Keep a break point at Button IRQ (line 44) when the voltage of voltage is in the range of button 
then the button interrupt will rise
===============================================================================================
Note:1) button ranges are as below in this example
        button-1 : 1(1.8v) to 6(2.05v) 
		button-2 : 10(2.25V) to 16(2.55v) 
		button-3 : 22(2.85v) to 28(3.15sv) 
      2)UULP_GPIO_2 is the only pin to conifigure as the input pin for Button. 
	  3)While using the Button ,UULPSS_GPIO_2 should not use for other configurations.
===============================================================================================