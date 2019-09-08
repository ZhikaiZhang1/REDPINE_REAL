===============================================================================================
                                 I2C Master Example
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
This example I2C will be configured in master mode.Here we will connect SCL and SDA lines 
of master controller to EEPROM's SCL and SDA pins.

EEPROM(AT2408C) is slave device and its address if 0x50.
From master we will send slave address after START on bus and wait till the ACK respose from the slave.
Thereafter write data to EEPROM memory based on its page write sequence to the required offset locations.
In single write operation we can write 16 bytes.After completion of write operation we read the data 
from the required offset based on random read sequence of the slave device.
Both write and read data should get match.
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
complile and run the application 
Setup requirement:
SCL(GPIO_12):J1.21 -> Connect to EEPROM SCL pin
SDA(GPIO_15):J1.27  -> Connect to EEPROM SDA pin
Add rd_buf to watch window  and check the data

===============================================================================================
Pass criteria:
===============================================================================================
Both write and read 16 bits of data  should be same.
===============================================================================================