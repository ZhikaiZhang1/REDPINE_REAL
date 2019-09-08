===============================================================================================
                               Efuse Example Program
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
eFUSE programming only onces is possible,by default all locations are zeros. each write operation
sets the bits in particular location, it is one time only programmable.
===============================================================================================
 
Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
Write some bit locations in efuse controller and read those locations ,compare write and read data
===============================================================================================


===============================================================================================

Notice:
===============================================================================================
1) Enable DEBUG_UART macro in tool pre-processor window. 

2)The print's content can be observed on any serial terminal  through UART to USB(J21) . 
===============================================================================================