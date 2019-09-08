===============================================================================================
                               CAN Example Program
===============================================================================================
Copyright(C) Redpine Signals 2016
All rights reserved by Redpine Signals.

This program should be used on your own responsibility.
Redpine Signals assumes no responsibility for any losses
incurred by customers or third parties arising from the use of this file. 
                                                  
Function description:
===============================================================================================
This example demonstrates the data transmission with other CAN enabled device without hardware
filter configuration.
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
In this example CAN communicate with other third party board CAN device by using transceiver,
so connect CAN Transmitter and receiver pin to EVK board and similarly connect Transmitter and receiver 
pin to other CAN enabled device.Here communication done by using standard frame format.
===============================================================================================

===============================================================================================
Setup requirement:
===============================================================================================
CAN RX(pin) <==> ULP_GPIO[10]:J2.28
CAN TX(pin) <==> ULP_GPIO[11]:J2.30


===============================================================================================
Pass criteria:
===============================================================================================
1.CAN data transmission
  -CAN data transmission verify by checking receiver buffer of third party CAN enabled device,
   transmission proper when the receiver buffer of third parties board and transmission data is same.   
2.CAN data reception
  -CAN data reception is verify the by checking receiver buffer.receiver data must match transmission
   data of third party board . 
    
===============================================================================================