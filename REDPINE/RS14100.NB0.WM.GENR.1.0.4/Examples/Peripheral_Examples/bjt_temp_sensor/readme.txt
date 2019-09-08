===============================================================================================
                               MCU PS2 state sleep wake up Example Program
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
This Example reads the temperature at every second and prints the temperature on hyperterminal.
===============================================================================================
 
Assistance software:
---------------------
Hyperterminal.

===============================================================================================
Usage Procedure:
===============================================================================================
1.Build and run the program 
2.Connect the UART_header TX_PIN and RX_PIN to ULP_PIN_6 AND ULP_PIN_7 respectively and set the 
  baudrate to 115200 and see the output. 
  
  
  NOTE:
  Before compiling confirm 
  1.SELECT_UART_INSTANCE is ULP_UART_INSTANCE in rsi_board.c.
   
===============================================================================================