===============================================================================================
                                        ADC 100ksps sampling rate example 
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
1.Sampling rate  : 1msps 
2.Ping address   : 0x24060000
3.Ping length    : 1023 
4.Pong address   : 0x240603FF  (0x24060000+(1023))
5.Ping length    : 1023

In this example configure the sampling rate as 1msps and m4 is going to sleep with ADC interrupt.
This example give the 1msps sampling with different clock source.
In 20mrc clock give the better performance but power consumtion is more, 
similarly in 20mro clock has less power consumtion but performace is also less.
To enble 20mrc clock enable 'RC_20MHZ_CLOCK' use this macro.(line number 49).
To select the mode of operation use 'SINGLE_ENDED' or 'DIFFERENTIAL_ENDED' macros.
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
Give the input to ULP_GPIO_4 for single ended or ULP_GPIO4 and ULP_GPIO5 in differential mode
===============================================================================================
Pass criteria:
===============================================================================================
This examples define power consumtion of ADC in 1msps as sampling rate.
1. Power pass criteria : Power consumtion of ADC with different clock source as below
   a.20mhz rc(Input supply for chip 3.2v ):654uA   Disable 'RO_1MHZ_CLOCK' macro(line number 49) 
                                                   and Disable 'ENOB'(line number 50)         
   b.20mhz rc(Input supply for chip 1.8v ):899uA   Disable 'RO_1MHZ_CLOCK' macro(line number 49) 
                                                   and Disable 'ENOB'(line number 50)         
 
   b.20mhz ro(Input supply for chip 3.2v ):577uA   Enable 'RO_1MHZ_CLOCK' macro(line number 49) 
                                                   and Disable 'ENOB'(line number 50) 
   b.20mhz ro(Input supply for chip 1.8v ):728uA   Enable 'RO_1MHZ_CLOCK' macro(line number 49) 
                                                   and Disable 'ENOB'(line number 50) 												   
===============================================================================================