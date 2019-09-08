===============================================================================================
                              SIO Example Program
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
SIO is simply we are emulating the serial peripheral again,serial peripheral like spi,i2c,i2s,
usart etc, like one of serial peripheral.we are taking try to run our application , in this 
example we are taking example of SPI (SIO-SPI)
===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smart EVK 

Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
compile and run the application 
setup requirements :
GPIO_8(MISO):J1.11<->GPIO_9:J1.13 (MOSI) (loop back the pin's).
===============================================================================================

===============================================================================================
Pass criteria:
===============================================================================================
Check u16SpiRxBuf,u16SpiTxBuf buffer,result  in watch window both buffers should be same.
Put the break point at (cnt++), If result value is 0 then data is correct else not.
===============================================================================================
