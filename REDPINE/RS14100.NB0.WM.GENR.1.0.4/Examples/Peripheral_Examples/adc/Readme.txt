===============================================================================================
                                        ADC Example 
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
This example explain the ADC functionality,ADC configures as multichannel mode with single 
ended input or Differential and ADC output samples will be stored in PING and PONG ULP memories.
In this example adc_input_pin_mux() function will configure required ULP GPIO pin as Analog pin and 
RSI_ADC_ChannelConfig() will configure that pin as ADC input with POSITIVE_INP_SEL, NEGATIVE_INP_SEL macros.

following configuration used in example : 
1.Sampling rate  : 100ksps 
2.Input type     : Configure the input in single ended mode for ULP_GPIO_4 in analog mode.
3.Ping address   : 0x24060000
4.Ping length    : 1023 
5.Pong address   : 0x240603FF  (0x24060000+(1023))
6.Ping length    : 1023
7.INPUT_MODE     : 0 (0 for single ended and 1 for differential ended)
8.GAIN_OFFSET_CAL_ENABLE : 1 (1 for gain and offset calculation enable and 0 for zero gain and offset calculation disable)  

Various Sampling Rates can be achieved with the following changes:

1Ksps Sampling rate
1.Change SAMPLING_RATE_FAC macro value to 1000

10Ksps Sampling rate
1. Change SAMPLING_RATE_FAC macro value to 100

1Msps Sampling rate
1. Change CLOCK_DIV_FAC macro value to 4
2. Change SAMPLING_RATE_FAC macro value to 4

===============================================================================================
Environment
===============================================================================================
Test Board:
---------------------
Smark EVK Rev1.2

Assistance software:
---------------------
None

===============================================================================================
Usage Procedure:
===============================================================================================
Compile and Run the example.
Give the input to ULP_GPIO_4 for single ended mode.
Give the positive input on ULP_GPIO_4 and negative input on ULP_GPIO_5 in differential mode.  
===============================================================================================
Pass criteria:
===============================================================================================
In uart prints get equivalent ADC input voltage from ADC samples.  
===============================================================================================
===============================================================================================
Added below code in Application 
===============================================================================================
1.Print the equivalent ADC input voltage from obtain ADC samples. 
===============================================================================================

Note
===============================================================================================
1.Add the "DEBUG_UART" macro in follow "option for target -> DEBUG_UART " for UART print purpose.
===============================================================================================