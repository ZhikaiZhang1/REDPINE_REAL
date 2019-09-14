  /* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2016 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * $Date:        1. December 2016
 * $Revision:    V2.4.4
 *
 * Project:      RTE Device Configuration for RS14100
 * -------------------------------------------------------------------------- */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

// <e> CAN1 (Controller Area Network 1) [Driver_CAN1]
// <i> Configuration settings for Driver_CAN1 in component ::CMSIS Driver:CAN
#define RTE_CAN1                        1

//   <o> CAN1_RX Pin <0=>PA11 <1=>PB8 <2=>PD0 <3=>PI9 <4=>PG0
#define RTE_CAN1_RX_PORT_ID             4
#if    (RTE_CAN1_RX_PORT_ID == 0)
#define RTE_CAN1_RX_PORT                0
#define RTE_CAN1_RX_PIN                 16
#define RTE_CAN1_RX_MODE                EGPIO_PIN_MUX_MODE4
#define RTE_CAN1_RX_PAD_SEL             5
#elif  (RTE_CAN1_RX_PORT_ID == 1)
#define RTE_CAN1_RX_PORT                0
#define RTE_CAN1_RX_PIN                 29
#define RTE_CAN1_RX_MODE                1
#define RTE_CAN1_RX_PAD_SEL             0
#elif  (RTE_CAN1_RX_PORT_ID == 2)
#define RTE_CAN1_RX_PORT                0
#define RTE_CAN1_RX_PIN                 46
#define RTE_CAN1_RX_MODE                1
#define RTE_CAN1_RX_PAD_SEL             11
#elif  (RTE_CAN1_RX_PORT_ID == 3)
#define RTE_CAN1_RX_PORT                0
#define RTE_CAN1_RX_PIN                 (4 + 64)
#define RTE_CAN1_RX_MODE                4
#define RTE_CAN1_RX_PAD_SEL             16
#elif  (RTE_CAN1_RX_PORT_ID == 4)
#define RTE_CAN1_RX_PORT                0
#define RTE_CAN1_RX_PIN                 (64+10)
#define RTE_CAN1_RX_MODE                1
#define RTE_CAN1_RX_PAD_SEL             19
#else
#error "Invalid CAN1_RX Pin Configuration!"
#endif

//   <o> CAN1_TX Pin <0=>PA12 <1=>PB9 <2=>PD1 <3=>PH13 <4=>PG1
#define RTE_CAN1_TX_PORT_ID             4
#if    (RTE_CAN1_TX_PORT_ID == 0)
#define RTE_CAN1_TX_PORT                0
#define RTE_CAN1_TX_PIN                 17
#define RTE_CAN1_TX_MODE                4
#define RTE_CAN1_TX_PAD_SEL             5
#elif  (RTE_CAN1_TX_PORT_ID == 1)
#define RTE_CAN1_TX_PORT                0
#define RTE_CAN1_TX_PIN                 30
#define RTE_CAN1_TX_MODE                1
#define RTE_CAN1_TX_PAD_SEL             0
#elif  (RTE_CAN1_TX_PORT_ID == 2)
#define RTE_CAN1_TX_PORT                0
#define RTE_CAN1_TX_PIN                 47
#define RTE_CAN1_TX_MODE                1
#define RTE_CAN1_TX_PAD_SEL             11
#elif  (RTE_CAN1_TX_PORT_ID == 3)
#define RTE_CAN1_TX_PORT                0
#define RTE_CAN1_TX_PIN                 (64 +5)
#define RTE_CAN1_TX_MODE                4
#define RTE_CAN1_TX_PAD_SEL             17
#elif  (RTE_CAN1_TX_PORT_ID == 4)
#define RTE_CAN1_TX_PORT                0
#define RTE_CAN1_TX_PIN                 (64 + 11)
#define RTE_CAN1_TX_MODE                1
#define RTE_CAN1_TX_PAD_SEL             19
#else
#error "Invalid CAN1_TX Pin Configuration!"
#endif

// </e>

// <e> ETH (Ethernet Interface) [Driver_ETH_MAC0]
// <i> Configuration settings for Driver_ETH_MAC0 in component ::CMSIS Driver:Ethernet MAC
#define RTE_ETH                         0
//   </e>
//   <e> RMII (Reduced Media Independent Interface)
#define RTE_ETH_RMII                    1

//     <o> ETH_RMII_TXD0 Pin <0=>PB12 <1=>PG13
#define RTE_ETH_RMII_TXD0_PORT_ID       0
#if    (RTE_ETH_RMII_TXD0_PORT_ID == 0)
#define RTE_ETH_RMII_TXD0_PORT          0
#define RTE_ETH_RMII_TXD0_PIN           7
#define RTE_ETH_RMII_TXD0_MODE          14
#define RTE_ETH_RMII_TXD0_PAD_SEL       1
#elif  (RTE_ETH_RMII_TXD0_PORT_ID == 1)
#define RTE_ETH_RMII_TXD0_PORT          0
#define RTE_ETH_RMII_TXD0_PIN           0
#define RTE_ETH_RMII_TXD0_MODE          0
#define RTE_ETH_RMII_TXD0_PAD_SEL       0
#else
#error "Invalid ETH_RMII_TXD0 Pin Configuration!"
#endif
//     <o> ETH_RMII_TXD1 Pin <0=>PB13 <1=>PG14
#define RTE_ETH_RMII_TXD1_PORT_ID       0
#if    (RTE_ETH_RMII_TXD1_PORT_ID == 0)
#define RTE_ETH_RMII_TXD1_PORT          0
#define RTE_ETH_RMII_TXD1_PIN           6
#define RTE_ETH_RMII_TXD1_MODE          14
#define RTE_ETH_RMII_TXD1_PAD_SEL       1
#elif  (RTE_ETH_RMII_TXD1_PORT_ID == 1)
#define RTE_ETH_RMII_TXD1_PORT          0
#define RTE_ETH_RMII_TXD1_PIN           0
#define RTE_ETH_RMII_TXD1_MODE          0
#define RTE_ETH_RMII_TXD1_PAD_SEL       0
#else
#error "Invalid ETH_RMII_TXD1 Pin Configuration!"
#endif
//     <o> ETH_RMII_TX_EN Pin <0=>PB11 <1=>PG11
#define RTE_ETH_RMII_TX_EN_PORT_ID      0
#if    (RTE_ETH_RMII_TX_EN_PORT_ID == 0)
#define RTE_ETH_RMII_TX_EN_PORT         0
#define RTE_ETH_RMII_TX_EN_PIN          9
#define RTE_ETH_RMII_TX_EN_MODE         14
#define RTE_ETH_RMII_TX_EN_PAD_SEL      1
#elif  (RTE_ETH_RMII_TX_EN_PORT_ID == 1)
#define RTE_ETH_RMII_TX_EN_PORT         0
#define RTE_ETH_RMII_TX_EN_PIN          0
#define RTE_ETH_RMII_TX_EN_MODE         0
#define RTE_ETH_RMII_TX_EN_PAD_SEL      0
#else
#error "Invalid ETH_RMII_TX_EN Pin Configuration!"
#endif
//     <o> ETH_RMII_RXD0 Pin <0=>PC4
#define RTE_ETH_RMII_RXD0_PORT_ID       0
#if    (RTE_ETH_RMII_RXD0_PORT_ID == 0)
#define RTE_ETH_RMII_RXD0_PORT          0
#define RTE_ETH_RMII_RXD0_PIN           15
#define RTE_ETH_RMII_RXD0_MODE          7
#define RTE_ETH_RMII_RXD0_PAD_SEL       5
#else
#error "Invalid ETH_RMII_RXD0 Pin Configuration!"
#endif
//     <o> ETH_RMII_RXD1 Pin <0=>PC5
#define RTE_ETH_RMII_RXD1_PORT_ID       0
#if    (RTE_ETH_RMII_RXD1_PORT_ID == 0)
#define RTE_ETH_RMII_RXD1_PORT          0
#define RTE_ETH_RMII_RXD1_PIN           10
#define RTE_ETH_RMII_RXD1_MODE          7
#define RTE_ETH_RMII_RXD1_PAD_SEL       2
#else
#error "Invalid ETH_RMII_RXD1 Pin Configuration!"
#endif
//     <o> ETH_RMII_REF_CLK Pin <0=>PA1
#define RTE_ETH_RMII_REF_CLK_PORT_ID    0
#if    (RTE_ETH_RMII_REF_CLK_PORT_ID == 0)
#define RTE_ETH_RMII_REF_CLK_PORT       0
#define RTE_ETH_RMII_REF_CLK_PIN        8
#define RTE_ETH_RMII_REF_CLK_MODE       14
#define RTE_ETH_RMII_REF_CLK_PAD_SEL    1
#else
#error "Invalid ETH_RMII_REF_CLK Pin Configuration!"
#endif
//     <o> ETH_RMII_CRS_DV Pin <0=>PA7
#define RTE_ETH_RMII_CRS_DV_PORT_ID     0
#if    (RTE_ETH_RMII_CRS_DV_PORT_ID == 0)
#define RTE_ETH_RMII_CRS_DV_PORT        0
#define RTE_ETH_RMII_CRS_DV_PIN         16
#define RTE_ETH_RMII_CRS_DV_MODE        14
#define RTE_ETH_RMII_CRS_DV_PAD_SEL     5
#else
#error "Invalid ETH_RMII_CRS_DV Pin Configuration!"
#endif
//   </e>

//   <e> Management Data Interface
//     <o> ETH_MDC Pin <0=>PC1
#define RTE_ETH_MDI_MDC_PORT_ID         0
#if    (RTE_ETH_MDI_MDC_PORT_ID == 0)
#define RTE_ETH_MDI_MDC_PORT            0
#define RTE_ETH_MDI_MDC_PIN             11
#define RTE_ETH_MDI_MDC_MODE            7
#define RTE_ETH_MDI_MDC_PAD_SEL         2
#else
#error "Invalid ETH_MDC Pin Configuration!"
#endif
//     <o> ETH_MDIO Pin <0=>PA2
#define RTE_ETH_MDI_MDIO_PORT_ID        0
#if    (RTE_ETH_MDI_MDIO_PORT_ID == 0)
#define RTE_ETH_MDI_MDIO_PORT           0
#define RTE_ETH_MDI_MDIO_PIN            12
#define RTE_ETH_MDI_MDIO_MODE           7
#define RTE_ETH_MDI_MDIO_PAD_SEL        3
#else
#error "Invalid ETH_MDIO Pin Configuration!"
#endif

//</e>



#define RTE_ENABLE_FIFO                    1 
// <e> USART0  [Driver_USART0]
// <i> Configuration settings for Driver_USART0 in component ::CMSIS Driver:USART
#define RTE_USART0                         1

#define RTE_USART0_CLK_SRC                 USART_ULPREFCLK
#define RTE_USART0_CLK_DIV_FACT            1
#define RTE_USART0_FRAC_DIV_EN             0

#define RTE_USART_MODE                     0        //!Usart mode macros
#define RTE_CONTINUOUS_CLOCK_MODE          0

#define RTE_USART0_LOOPBACK                0
#define RTE_USART0_DTR_EANBLE              0

#define RTE_USART0_DMA_MODE1_EN            0        //!dma mode

#define RTE_USART0_TX_FIFO_THRESHOLD       USART_TRIGGER_TX_EMPTY
#define RTE_USART0_RX_FIFO_THRESHOLD       USART_TRIGGER_RX_AEMPTY

#define RTE_USART0_DMA_TX_LEN_PER_DES      1024
#define RTE_USART0_DMA_RX_LEN_PER_DES      1024

#define RTE_USART0_CHNL_UDMA_TX_EN         0   
#define RTE_USART0_CHNL_UDMA_TX_CH         25

#define RTE_USART0_CHNL_UDMA_RX_EN         0 
#define RTE_USART0_CHNL_UDMA_RX_CH         24


//     <o> USART0_CLK <0=>P0_10 <1=>P0_20 <2=>P0_25 <3=>P0_28 <4=>P0_72  
//     <i> CLK of USART0
#define RTE_USART0_CLK_PORT_ID                  0
#if(RTE_USART0_CLK_PORT_ID == 0)
#define RTE_USART0_CLK_PORT                     0
#define RTE_USART0_CLK_PIN                      10
#define RTE_USART0_CLK_MUX                      2
#define RTE_USART0_CLK_PAD                      2
#elif(RTE_USART0_CLK_PORT_ID ==1)
#define RTE_USART0_CLK_PORT                     0
#define RTE_USART0_CLK_PIN                      20
#define RTE_USART0_CLK_MUX                      8
#define RTE_USART0_CLK_PAD                      7
#elif(RTE_USART0_CLK_PORT_ID ==2)
#define RTE_USART0_CLK_PORT                     0
#define RTE_USART0_CLK_PIN                      25
#define RTE_USART0_CLK_MUX                      2
#define RTE_USART0_CLK_PAD                      0//NO PAD
#elif(RTE_USART0_CLK_PORT_ID ==3)
#define RTE_USART0_CLK_PORT                     0
#define RTE_USART0_CLK_PIN                      28
#define RTE_USART0_CLK_MUX                      2
#define RTE_USART0_CLK_PAD                      0//NO PAD
#elif(RTE_USART0_CLK_PORT_ID ==4)
#define RTE_USART0_CLK_PORT                     0
#define RTE_USART0_CLK_PIN                      72
#define RTE_USART0_CLK_MUX                      2
#define RTE_USART0_CLK_PAD                      16
#else
  #error "Invalid USART0 RTE_USART0_CLK_PIN Pin Configuration!"
#endif

//     <o> USART0_TX <0=>P0_9 <1=>P0_13 <2=>P0_18 <3=>P0_26  <4=>P0_30 <5=>P0_75 
//     <i> TX for USART0
#define RTE_USART0_TX_PORT_ID                   0
#if(RTE_USART0_TX_PORT_ID == 0)
#define RTE_USART0_TX_PORT                      0
#define RTE_USART0_TX_PIN                       9
#define RTE_USART0_TX_MUX                       2
#define RTE_USART0_TX_PAD                       1
#elif(RTE_USART0_TX_PORT_ID == 1)
#define RTE_USART0_TX_PORT                      0
#define RTE_USART0_TX_PIN                       13
#define RTE_USART0_TX_MUX                       2
#define RTE_USART0_TX_PAD                       4
#elif(RTE_USART0_TX_PORT_ID ==2)
#define RTE_USART0_TX_PORT                      0
#define RTE_USART0_TX_PIN                       18
#define RTE_USART0_TX_MUX                       8
#define RTE_USART0_TX_PAD                       6
#elif(RTE_USART0_TX_PORT_ID ==3)
#define RTE_USART0_TX_PORT                      0
#define RTE_USART0_TX_PIN                       26
#define RTE_USART0_TX_MUX                       2
#define RTE_USART0_TX_PAD                       0 //no pad
#elif(RTE_USART0_TX_PORT_ID ==4)
#define RTE_USART0_TX_PORT                      0
#define RTE_USART0_TX_PIN                       30
#define RTE_USART0_TX_MUX                       2
#define RTE_USART0_TX_PAD                       0 //no pad
#elif(RTE_USART0_TX_PORT_ID ==5)
#define RTE_USART0_TX_PORT                      0
#define RTE_USART0_TX_PIN                       75
#define RTE_USART0_TX_MUX                       2
#define RTE_USART0_TX_PAD                       19  
#else
  #error "Invalid USART0 RTE_USART0_TX_PIN Pin Configuration!"
#endif

//     <o> USART0_RX <0=>P0_8 <1=>P0_14 <2=>P0_19 <3=>P0_27 <4=>P0_29 <5=>P0_74 
//     <i> RX for USART0
#define RTE_USART0_RX_PORT_ID                   0
#if(RTE_USART0_RX_PORT_ID ==0)
#define RTE_USART0_RX_PORT                      0
#define RTE_USART0_RX_PIN                       8
#define RTE_USART0_RX_MUX                       2
#define RTE_USART0_RX_PAD                       1
#elif(RTE_USART0_RX_PORT_ID ==1)
#define RTE_USART0_RX_PORT                      0
#define RTE_USART0_RX_PIN                       14
#define RTE_USART0_RX_MUX                       2
#define RTE_USART0_RX_PAD                       4
#elif(RTE_USART0_RX_PORT_ID ==2)
#define RTE_USART0_RX_PORT                      0
#define RTE_USART0_RX_PIN                       19
#define RTE_USART0_RX_MUX                       8
#define RTE_USART0_RX_PAD                       6
#elif(RTE_USART0_RX_PORT_ID ==3)
#define RTE_USART0_RX_PORT                      0
#define RTE_USART0_RX_PIN                       27
#define RTE_USART0_RX_MUX                       2
#define RTE_USART0_RX_PAD                       0//no pad
#elif(RTE_USART0_RX_PORT_ID ==4)
#define RTE_USART0_RX_PORT                      0
#define RTE_USART0_RX_PIN                       29
#define RTE_USART0_RX_MUX                       2
#define RTE_USART0_RX_PAD                       0//no pad
#elif(RTE_USART0_RX_PORT_ID ==4)
#define RTE_USART0_RX_PORT                      0
#define RTE_USART0_RX_PIN                       74
#define RTE_USART0_RX_MUX                       2
#define RTE_USART0_RX_PAD                       19 
#else
  #error "Invalid USART0 RTE_USART0_RX_PIN Pin Configuration!"
#endif

//     <o> USART0_CTS <0=>P0_15 <1=>P0_19 <2=>P0_22 <3=>P0_28 <4=>P0_68 <5=>P0_72 
//     <i> CTS for USART0
#define RTE_USART0_CTS_PORT_ID              0
#if(RTE_USART0_CTS_PORT_ID ==0)
#define RTE_USART0_CTS_PORT                     0
#define RTE_USART0_CTS_PIN                      15
#define RTE_USART0_CTS_MUX                      5
#define RTE_USART0_CTS_PAD                      5
#elif(RTE_USART0_CTS_PORT_ID ==1)
#define RTE_USART0_CTS_PORT                     0
#define RTE_USART0_CTS_PIN                      19
#define RTE_USART0_CTS_MUX                      2
#define RTE_USART0_CTS_PAD                      6
#elif(RTE_USART0_CTS_PORT_ID ==2)
#define RTE_USART0_CTS_PORT                     0
#define RTE_USART0_CTS_PIN                      22
#define RTE_USART0_CTS_MUX                      2
#define RTE_USART0_CTS_PAD                      0//NO PAD
#elif(RTE_USART0_CTS_PORT_ID ==3)
#define RTE_USART0_CTS_PORT                     0
#define RTE_USART0_CTS_PIN                      28
#define RTE_USART0_CTS_MUX                      7
#define RTE_USART0_CTS_PAD                      0//NO PAD
#elif(RTE_USART0_CTS_PORT_ID ==4)
#define RTE_USART0_CTS_PORT                     0
#define RTE_USART0_CTS_PIN                      68
#define RTE_USART0_CTS_MUX                      2
#define RTE_USART0_CTS_PAD                      16 
#elif(RTE_USART0_CTS_PORT_ID ==5)
#define RTE_USART0_CTS_PORT                     0
#define RTE_USART0_CTS_PIN                      72
#define RTE_USART0_CTS_MUX                      10
#define RTE_USART0_CTS_PAD                      16 
#else
  #error "Invalid USART0 RTE_USART0_CTS_PIN Pin Configuration!"
#endif


//     <o> USART0_RTS <0=>P0_12 <1=>P0_18 <2=>P0_21 <3=>P0_27 <4=>P0_73  
//     <i> RTS for USART0
#define RTE_USART0_RTS_PORT_ID                  0
#if(RTE_USART0_RTS_PORT_ID   ==0)
#define RTE_USART0_RTS_PORT                     0
#define RTE_USART0_RTS_PIN                      12
#define RTE_USART0_RTS_MUX                      5
#define RTE_USART0_RTS_PAD                      3
#elif(RTE_USART0_RTS_PORT_ID ==1)
#define RTE_USART0_RTS_PORT                     0
#define RTE_USART0_RTS_PIN                      18
#define RTE_USART0_RTS_MUX                      2
#define RTE_USART0_RTS_PAD                      6
#elif(RTE_USART0_RTS_PORT_ID ==2)
#define RTE_USART0_RTS_PORT                     0
#define RTE_USART0_RTS_PIN                      21
#define RTE_USART0_RTS_MUX                      2
#define RTE_USART0_RTS_PAD                      0 //NO PAD
#elif(RTE_USART0_RTS_PORT_ID ==3)
#define RTE_USART0_RTS_PORT                     0
#define RTE_USART0_RTS_PIN                      27
#define RTE_USART0_RTS_MUX                      7
#define RTE_USART0_RTS_PAD                      0 //NO PAD
#elif(RTE_USART0_RTS_PORT_ID ==4)
#define RTE_USART0_RTS_PORT                     0
#define RTE_USART0_RTS_PIN                      73
#define RTE_USART0_RTS_MUX                      2
#define RTE_USART0_RTS_PAD                      17
#else
  #error "Invalid USART0 RTE_USART0_RTS_PIN Pin Configuration!"
#endif

//     <o> USART0_IR_TX <0=>P0_10 <1=>P0_54  
//     <i> IR TX for USART0
#define RTE_IR_TX_PORT_ID                       0
#if(RTE_IR_TX_PORT_ID    ==0)
#define RTE_USART0_IR_TX_PORT                   0
#define RTE_USART0_IR_TX_PIN                    10
#define RTE_USART0_IR_TX_MUX                    1
#define RTE_USART0_IR_TX_PAD                    2
#elif(RTE_IR_TX_PORT_ID  ==1)
#define RTE_USART0_IR_TX_PORT                   0
#define RTE_USART0_IR_TX_PIN                    54
#define RTE_USART0_IR_TX_MUX                    5
#define RTE_USART0_IR_TX_PAD                    13
#else
  #error "Invalid USART0 RTE_USART0_IR_TX_PIN Pin Configuration!"
#endif

//     <o> USART0_IR_RX <0=>P0_12 <1=>P0_53  
//     <i> IR RX for USART0
#define RTE_IR_RX_PORT_ID                       0
#if(RTE_IR_RX_PORT_ID ==0)
#define RTE_USART0_IR_RX_PORT                   0
#define RTE_USART0_IR_RX_PIN                    12
#define RTE_USART0_IR_RX_MUX                    1
#define RTE_USART0_IR_RX_PAD                    3
#elif(RTE_IR_RX_PORT_ID ==1)
#define RTE_USART0_IR_RX_PORT                   0
#define RTE_USART0_IR_RX_PIN                    53
#define RTE_USART0_IR_RX_MUX                    5
#define RTE_USART0_IR_RX_PAD                    13
#else
  #error "Invalid USART0 RTE_USART0_IR_RX_PIN Pin Configuration!"
#endif

//     <o> USART0_RI <0=>P0_52  
//     <i> RI for USART0
#define RTE_USART0_RI_PORT                      0
#define RTE_USART0_RI_PIN                       52
#define RTE_USART0_RI_MUX                       5
#define RTE_USART0_RI_PAD                       13

//     <o> USART0_RI <0=>P0_46  
//     <i> DSR for USART0
#define RTE_USART0_DSR_PORT                     0
#define RTE_USART0_DSR_PIN                      46
#define RTE_USART0_DSR_MUX                      5
#define RTE_USART0_DSR_PAD                      11

//     <o> USART0_RI <0=>P0_47  
//     <i> DCD for USART0
#define RTE_USART0_DCD_PORT                     0
#define RTE_USART0_DCD_PIN                      47
#define RTE_USART0_DCD_MUX                      5
#define RTE_USART0_DCD_PAD                      11

//     <o> USART0_RI <0=>P0_48  
//     <i> DTR for USART0
#define RTE_USART0_DTR_PORT                     0
#define RTE_USART0_DTR_PIN                      48
#define RTE_USART0_DTR_MUX                      5
#define RTE_USART0_DTR_PAD                      11
// </e>

// <e> UART1  [Driver_UART1]
// <i> Configuration settings for Driver_UART1 in component ::CMSIS Driver:USART
#define RTE_UART1                          1

#define RTE_UART1_CLK_SRC                  USART_ULPREFCLK
#define RTE_UART1_CLK_DIV_FACT             1
#define RTE_UART1_FRAC_DIV_EN              0

#define RTE_UART1_LOOPBACK                 0
#define RTE_UART1_DMA_MODE1_EN             0

#define RTE_UART1_TX_FIFO_THRESHOLD        USART_TRIGGER_TX_EMPTY
#define RTE_UART1_RX_FIFO_THRESHOLD        USART_TRIGGER_RX_AEMPTY

#define RTE_UART1_DMA_TX_LEN_PER_DES       1024
#define RTE_UART1_DMA_RX_LEN_PER_DES       1024

#define RTE_UART1_CHNL_UDMA_TX_EN          0
#define RTE_UART1_CHNL_UDMA_TX_CH          27

#define RTE_UART1_CHNL_UDMA_RX_EN          0
#define RTE_UART1_CHNL_UDMA_RX_CH          26

/*UART1 PINS*/
//     <o> UART1_TX <0=>P0_6 <1=>P0_12 <2=>P0_25 <3=>P0_36 <4=>P0_47 <5=>P0_65 <6=>P0_69 <5=>P0_73  
//     <i> TX of UART1
#define RTE_UART1_TX_PORT_ID                0
#if(RTE_UART1_TX_PORT_ID  ==0) 
#define RTE_UART1_TX_PORT                       0
#define RTE_UART1_TX_PIN                        6
#define RTE_UART1_TX_MUX                        2
#define RTE_UART1_TX_PAD                        1
#elif(RTE_UART1_TX_PORT_ID ==1)
#define RTE_UART1_TX_PORT                       0
#define RTE_UART1_TX_PIN                        12
#define RTE_UART1_TX_MUX                        2
#define RTE_UART1_TX_PAD                        3
#elif(RTE_UART1_TX_PORT_ID ==2)
#define RTE_UART1_TX_PORT                       0
#define RTE_UART1_TX_PIN                        25
#define RTE_UART1_TX_MUX                        9
#define RTE_UART1_TX_PAD                        0//NO PAD
#elif(RTE_UART1_TX_PORT_ID ==3)
#define RTE_UART1_TX_PORT                       0
#define RTE_UART1_TX_PIN                        36
#define RTE_UART1_TX_MUX                        2
#define RTE_UART1_TX_PAD                        9
#elif(RTE_UART1_TX_PORT_ID ==4)
#define RTE_UART1_TX_PORT                       0
#define RTE_UART1_TX_PIN                        47
#define RTE_UART1_TX_MUX                        2
#define RTE_UART1_TX_PAD                        11
#elif(RTE_UART1_TX_PORT_ID ==5)
#define RTE_UART1_TX_PORT                       0
#define RTE_UART1_TX_PIN                        65
#define RTE_UART1_TX_MUX                        6
#define RTE_UART1_TX_PAD                        20
#elif(RTE_UART1_TX_PORT_ID ==6)
#define RTE_UART1_TX_PORT                       0
#define RTE_UART1_TX_PIN                        69
#define RTE_UART1_TX_MUX                        1
#define RTE_UART1_TX_PAD                        17
#elif(RTE_UART1_TX_PORT_ID ==7)
#define RTE_UART1_TX_PORT                       0
#define RTE_UART1_TX_PIN                        73
#define RTE_UART1_TX_MUX                        1
#define RTE_UART1_TX_PAD                        17
#else
  #error "Invalid UART1 RTE_UART1_TX_PIN Pin Configuration!"
#endif

//     <o> UART1_RX <0=>P0_7 <1=>P0_15 <2=>P0_26 <3=>P0_37 <4=>P0_46 <5=>P0_64 <6=>P0_68 <5=>P0_72  
//     <i> RX of UART1
#define RTE_UART1_RX_PORT_ID                0
#if(RTE_UART1_RX_PORT_ID  ==0)
#define RTE_UART1_RX_PORT                       0
#define RTE_UART1_RX_PIN                        7
#define RTE_UART1_RX_MUX                        2
#define RTE_UART1_RX_PAD                        1
#elif(RTE_UART1_RX_PORT_ID ==1)
#define RTE_UART1_RX_PORT                       0
#define RTE_UART1_RX_PIN                        15
#define RTE_UART1_RX_MUX                        2
#define RTE_UART1_RX_PAD                        5
#elif(RTE_UART1_RX_PORT_ID ==2)
#define RTE_UART1_RX_PORT                       0
#define RTE_UART1_RX_PIN                        26
#define RTE_UART1_RX_MUX                        9
#define RTE_UART1_RX_PAD                        0//NO PAD
#elif(RTE_UART1_RX_PORT_ID ==3)
#define RTE_UART1_RX_PORT                       0
#define RTE_UART1_RX_PIN                        37
#define RTE_UART1_RX_MUX                        2
#define RTE_UART1_RX_PAD                        9
#elif(RTE_UART1_RX_PORT_ID ==4)
#define RTE_UART1_RX_PORT                       0
#define RTE_UART1_RX_PIN                        46
#define RTE_UART1_RX_MUX                        2
#define RTE_UART1_RX_PAD                        11
#elif(RTE_UART1_RX_PORT_ID ==5)
#define RTE_UART1_RX_PORT                       0
#define RTE_UART1_RX_PIN                        64
#define RTE_UART1_RX_MUX                        6
#define RTE_UART1_RX_PAD                        20
#elif(RTE_UART1_RX_PORT_ID ==6)
#define RTE_UART1_RX_PORT                       0
#define RTE_UART1_RX_PIN                        68
#define RTE_UART1_RX_MUX                        1
#define RTE_UART1_RX_PAD                        16
#elif(RTE_UART1_RX_PORT_ID ==7)
#define RTE_UART1_RX_PORT                       0
#define RTE_UART1_RX_PIN                        72
#define RTE_UART1_RX_MUX                        1
#define RTE_UART1_RX_PAD                        16
#else
  #error "Invalid UART1 RTE_UART1_RX_PIN Pin Configuration!"
#endif

//     <o> UART1_CTS <0=>P0_38 <1=>P0_50 <2=>P0_66 <3=>P0_70  
//     <i> CTS of UART1
#define RTE_UART1_CTS_PORT_ID                   0
#if(RTE_UART1_CTS_PORT_ID ==0)
#define RTE_UART1_CTS_PORT                      0
#define RTE_UART1_CTS_PIN                       38
#define RTE_UART1_CTS_MUX                       2
#define RTE_UART1_CTS_PAD                       0//NO PAD
#elif(RTE_UART1_CTS_PORT_ID ==1)
#define RTE_UART1_CTS_PORT                      0
#define RTE_UART1_CTS_PIN                       50
#define RTE_UART1_CTS_MUX                       3
#define RTE_UART1_CTS_PAD                       12
#elif(RTE_UART1_CTS_PORT_ID ==2)
#define RTE_UART1_CTS_PORT                      0
#define RTE_UART1_CTS_PIN                       66
#define RTE_UART1_CTS_MUX                       4
#define RTE_UART1_CTS_PAD                       21
#elif(RTE_UART1_CTS_PORT_ID ==3)
#define RTE_UART1_CTS_PORT                      0
#define RTE_UART1_CTS_PIN                       70
#define RTE_UART1_CTS_MUX                       1
#define RTE_UART1_CTS_PAD                       18
#else
  #error "Invalid UART1 RTE_UART1_CTS_PIN Pin Configuration!"
#endif

//     <o> UART1_RTS <0=>P0_39 <1=>P0_49 <2=>P0_67 <3=>P0_71  
//     <i> RTS of UART1
#define RTE_UART1_RTS_PORT_ID   0
#if(RTE_UART1_RTS_PORT_ID ==0)
#define RTE_UART1_RTS_PORT                      0
#define RTE_UART1_RTS_PIN                       39
#define RTE_UART1_RTS_MUX                       2
#define RTE_UART1_RTS_PAD                       0//NO PAD
#elif(RTE_UART1_RTS_PORT_ID ==1)
#define RTE_UART1_RTS_PORT                      0
#define RTE_UART1_RTS_PIN                       49
#define RTE_UART1_RTS_MUX                       3
#define RTE_UART1_RTS_PAD                       12
#elif(RTE_UART1_RTS_PORT_ID ==2)
#define RTE_UART1_RTS_PORT                      0
#define RTE_UART1_RTS_PIN                       67
#define RTE_UART1_RTS_MUX                       4
#define RTE_UART1_RTS_PAD                       21
#elif(RTE_UART1_RTS_PORT_ID ==3)
#define RTE_UART1_RTS_PORT                      0
#define RTE_UART1_RTS_PIN                       71
#define RTE_UART1_RTS_MUX                       1
#define RTE_UART1_RTS_PAD                       18
#else
  #error "Invalid UART1 RTE_UART1_RTS_PIN Pin Configuration!"
#endif
// </e>

// <e> ULP_UART  [Driver_ULP_UART]
// <i> Configuration settings for Driver_ULP_UART in component ::CMSIS Driver:USART
#define RTE_ULP_UART                       1

#define RTE_ULP_UART_CLK_SRC               ULP_UART_REF_CLK
#define RTE_ULP_UART_CLK_DIV_FACT          0
#define RTE_ULP_UART_FRAC_SEL              0
  
#define RTE_ULP_UART_LOOPBACK              0
#define RTE_ULP_UART_DMA_MODE1_EN          0

#define RTE_ULP_UART_TX_FIFO_THRESHOLD     USART_TRIGGER_TX_EMPTY
#define RTE_ULP_UART_RX_FIFO_THRESHOLD     USART_TRIGGER_RX_AEMPTY

#define RTE_ULP_UART_DMA_TX_LEN_PER_DES    1024
#define RTE_ULP_UART_DMA_RX_LEN_PER_DES    1024

#define RTE_ULPUART_CHNL_UDMA_TX_EN        0 
#define RTE_ULPUART_CHNL_UDMA_TX_CH        1

#define RTE_ULPUART_CHNL_UDMA_RX_EN        0
#define RTE_ULPUART_CHNL_UDMA_RX_CH        0

/*ULPSS UART PINS*/
//     <o> UART1_TX <0=>P0_3 <1=>P0_7 <2=>P0_11  
//     <i> TX of ULPSS UART
#define RTE_ULP_UART_TX_PORT_ID             1
#if(RTE_ULP_UART_TX_PORT_ID   ==0)
#define RTE_ULP_UART_TX_PORT                    0
#define RTE_ULP_UART_TX_PIN                     3
#define RTE_ULP_UART_TX_MUX                     3
#elif(RTE_ULP_UART_TX_PORT_ID ==1)
#define RTE_ULP_UART_TX_PORT                    0
#define RTE_ULP_UART_TX_PIN                     7
#define RTE_ULP_UART_TX_MUX                     3
#elif(RTE_ULP_UART_TX_PORT_ID ==2)
#define RTE_ULP_UART_TX_PORT                    0
#define RTE_ULP_UART_TX_PIN                     11
#define RTE_ULP_UART_TX_MUX                     3
#else
  #error "Invalid ULPSS UART RTE_ULP_UART_TX_PIN Pin Configuration!"
#endif

//     <o> UART1_RX <0=>P0_2 <1=>P0_6 <2=>P0_9  
//     <i> RX of ULPSS UART
#define RTE_ULP_UART_RX_PORT_ID             1
#if(RTE_ULP_UART_RX_PORT_ID   ==0)
#define RTE_ULP_UART_RX_PORT                    0
#define RTE_ULP_UART_RX_PIN                     2
#define RTE_ULP_UART_RX_MUX                     3
#elif(RTE_ULP_UART_RX_PORT_ID ==1)
#define RTE_ULP_UART_RX_PORT                    0
#define RTE_ULP_UART_RX_PIN                     6
#define RTE_ULP_UART_RX_MUX                     3
#elif(RTE_ULP_UART_RX_PORT_ID ==2)
#define RTE_ULP_UART_RX_PORT                    0
#define RTE_ULP_UART_RX_PIN                     9
#define RTE_ULP_UART_RX_MUX                     3
#else
  #error "Invalid ULPSS UART RTE_ULP_UART_RX_PIN Pin Configuration!"
#endif

//     <o> UART1_CTS <0=>P0_1 <1=>P0_5 <2=>P0_8  
//     <i> CTS of ULPSS UART
#define RTE_ULP_UART_CTS_PORT_ID            0
#if(RTE_ULP_UART_CTS_PORT_ID ==0)
#define RTE_ULP_UART_CTS_PORT                   0
#define RTE_ULP_UART_CTS_PIN                    1
#define RTE_ULP_UART_CTS_MUX                    3
#elif(RTE_ULP_UART_CTS_PORT_ID ==1)
#define RTE_ULP_UART_CTS_PORT                   0
#define RTE_ULP_UART_CTS_PIN                    5
#define RTE_ULP_UART_CTS_MUX                    3
#elif(RTE_ULP_UART_CTS_PORT_ID ==2)
#define RTE_ULP_UART_CTS_PORT                   0
#define RTE_ULP_UART_CTS_PIN                    8
#define RTE_ULP_UART_CTS_MUX                    3
#else
  #error "Invalid ULPSS UART RTE_ULP_UART_CTS_PIN Pin Configuration!"
#endif

//     <o> UART1_RTS <0=>P0_0 <1=>P0_4 <2=>P0_10  
//     <i> RTS of ULPSS UART
#define RTE_ULP_UART_RTS_PORT_ID                0
#if(RTE_ULP_UART_RTS_PORT_ID ==0)
#define RTE_ULP_UART_RTS_PORT                   0
#define RTE_ULP_UART_RTS_PIN                    0
#define RTE_ULP_UART_RTS_MUX                    3
#elif(RTE_ULP_UART_RTS_PORT_ID ==1)
#define RTE_ULP_UART_RTS_PORT                   0
#define RTE_ULP_UART_RTS_PIN                    4
#define RTE_ULP_UART_RTS_MUX                    3
#elif(RTE_ULP_UART_RTS_PORT_ID ==2)
#define RTE_ULP_UART_RTS_PORT                   0
#define RTE_ULP_UART_RTS_PIN                    10
#define RTE_ULP_UART_RTS_MUX                    8
#else
  #error "Invalid ULPSS UART RTE_ULP_UART_RTS_PIN Pin Configuration!"
#endif
//	 </e>



// <e> SSI_MASTER (Serial Peripheral Interface 1) [Driver_SSI_MASTER]
// <i> Configuration settings for Driver_SSI_MASTER in component ::CMSIS Driver:SPI
#define RTE_SSI_MASTER                  1


#define RTE_SSI_MASTER_INPUT_CLOCK      SSI_SOCPLLCLK

// <o> SSI_MASTER_MISO Pin <0=>Not Used <1=>GPIO0_7 <2=>GPIO0_15 <3=>GPIO0_27 <4=>GPIO0_37 <5=>GPIO0_71 <6=>GPIO0_75
#define RTE_SSI_MASTER_MISO_PORT_ID           1
#if     (RTE_SSI_MASTER_MISO_PORT_ID == 0)
#define  RTE_SSI_MASTER_MISO                   0
#elif   (RTE_SSI_MASTER_MISO_PORT_ID == 1)
#define  RTE_SSI_MASTER_MISO                   1
#define  RTE_SSI_MASTER_MISO_PORT            0
#define  RTE_SSI_MASTER_MISO_PIN             7
#define  RTE_SSI_MASTER_MISO_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MISO_PADSEL          1
#elif   (RTE_SSI_MASTER_MISO_PORT_ID == 2)
#define  RTE_SSI_MASTER_MISO                   1
#define  RTE_SSI_MASTER_MISO_PORT            0
#define  RTE_SSI_MASTER_MISO_PIN             15
#define  RTE_SSI_MASTER_MISO_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MISO_PADSEL          5
#elif    (RTE_SSI_MASTER_MISO_PORT_ID == 3)
#define  RTE_SSI_MASTER_MISO                   1
#define  RTE_SSI_MASTER_MISO_PORT            0
#define  RTE_SSI_MASTER_MISO_PIN             27
#define  RTE_SSI_MASTER_MISO_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MISO_PADSEL          1
#elif   (RTE_SSI_MASTER_MISO_PORT_ID == 4)
#define  RTE_SSI_MASTER_MISO                   1
#define  RTE_SSI_MASTER_MISO_PORT            0
#define  RTE_SSI_MASTER_MISO_PIN             37
#define  RTE_SSI_MASTER_MISO_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MISO_PADSEL          9
#elif    (RTE_SSI_MASTER_MISO_PORT_ID == 5)
#define  RTE_SSI_MASTER_MISO                   1
#define  RTE_SSI_MASTER_MISO_PORT            0
#define  RTE_SSI_MASTER_MISO_PIN             71
#define  RTE_SSI_MASTER_MISO_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MISO_PADSEL          18
#elif    (RTE_SSI_MASTER_MISO_PORT_ID == 6)
#define  RTE_SSI_MASTER_MISO                   1
#define  RTE_SSI_MASTER_MISO_PORT            0
#define  RTE_SSI_MASTER_MISO_PIN             75
#define  RTE_SSI_MASTER_MISO_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MISO_PADSEL          19
#else
#error "Invalid SSI_MASTER_MISO Pin Configuration!"
#endif

// <o> SSI_MASTER_MOSI Pin <0=>Not Used <1=>GPIO0_6 <2=>GPIO0_12 <3=>GPIO0_26 <4=>GPIO0_36 <5=>GPIO0_72 <6=>GPIO0_74
#define RTE_SSI_MASTER_MOSI_PORT_ID           1
#if    (RTE_SSI_MASTER_MOSI_PORT_ID == 0)
#define RTE_SSI_MASTER_MOSI                   0
#elif    (RTE_SSI_MASTER_MOSI_PORT_ID == 1)
#define  RTE_SSI_MASTER_MOSI                   1
#define  RTE_SSI_MASTER_MOSI_PORT            0
#define  RTE_SSI_MASTER_MOSI_PIN             6
#define  RTE_SSI_MASTER_MOSI_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MOSI_PADSEL          1
#elif    (RTE_SSI_MASTER_MOSI_PORT_ID == 2)
#define  RTE_SSI_MASTER_MOSI                   1
#define  RTE_SSI_MASTER_MOSI_PORT            0
#define  RTE_SSI_MASTER_MOSI_PIN             12
#define  RTE_SSI_MASTER_MOSI_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MOSI_PADSEL          3
#elif    (RTE_SSI_MASTER_MOSI_PORT_ID == 3)
#define  RTE_SSI_MASTER_MOSI                   1
#define  RTE_SSI_MASTER_MOSI_PORT            0
#define  RTE_SSI_MASTER_MOSI_PIN             26
#define  RTE_SSI_MASTER_MOSI_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MOSI_PADSEL          1
#elif    (RTE_SSI_MASTER_MOSI_PORT_ID == 4)
#define  RTE_SSI_MASTER_MOSI                   1
#define  RTE_SSI_MASTER_MOSI_PORT            0
#define  RTE_SSI_MASTER_MOSI_PIN             36
#define  RTE_SSI_MASTER_MOSI_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MOSI_PADSEL          9
#elif    (RTE_SSI_MASTER_MOSI_PORT_ID == 5)
#define  RTE_SSI_MASTER_MOSI                   1
#define  RTE_SSI_MASTER_MOSI_PORT            0
#define  RTE_SSI_MASTER_MOSI_PIN             72
#define  RTE_SSI_MASTER_MOSI_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MOSI_PADSEL          17
#elif    (RTE_SSI_MASTER_MOSI_PORT_ID == 6)
#define  RTE_SSI_MASTER_MOSI                   1
#define  RTE_SSI_MASTER_MOSI_PORT            0
#define  RTE_SSI_MASTER_MOSI_PIN             74
#define  RTE_SSI_MASTER_MOSI_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_MOSI_PADSEL          19
#else
#error "Invalid SSI_MASTER_MOSI Pin Configuration!"
#endif

// <o> SSI_MASTER_SCK Pin <0=>Not Used <1=>GPIO0_8 <2=>GPIO0_11 <3=>GPIO0_25 <4=>GPIO0_35 <5=>GPIO0_68 <6=>GPIO0_72
#define RTE_SSI_MASTER_SCK_PORT_ID           1
#if     (RTE_SSI_MASTER_SCK_PORT_ID == 0)
#define  RTE_SSI_MASTER_SCK                   0
#elif    (RTE_SSI_MASTER_SCK_PORT_ID == 1)
#define  RTE_SSI_MASTER_SCK                   1
#define  RTE_SSI_MASTER_SCK_PORT            0
#define  RTE_SSI_MASTER_SCK_PIN             8
#define  RTE_SSI_MASTER_SCK_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_SCK_PADSEL          1
#elif    (RTE_SSI_MASTER_SCK_PORT_ID == 2)
#define  RTE_SSI_MASTER_SCK                   1
#define  RTE_SSI_MASTER_SCK_PORT            0
#define  RTE_SSI_MASTER_SCK_PIN             11
#define  RTE_SSI_MASTER_SCK_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_SCK_PADSEL          2
#elif    (RTE_SSI_MASTER_SCK_PORT_ID == 3)
#define  RTE_SSI_MASTER_SCK                   1
#define  RTE_SSI_MASTER_SCK_PORT            0
#define  RTE_SSI_MASTER_SCK_PIN             25
#define  RTE_SSI_MASTER_SCK_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_SCK_PADSEL          1
#elif    (RTE_SSI_MASTER_SCK_PORT_ID == 4)
#define  RTE_SSI_MASTER_SCK                   1
#define  RTE_SSI_MASTER_SCK_PORT            0
#define  RTE_SSI_MASTER_SCK_PIN             35
#define  RTE_SSI_MASTER_SCK_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_SCK_PADSEL          9
#elif    (RTE_SSI_MASTER_SCK_PORT_ID == 5)
#define  RTE_SSI_MASTER_SCK                   1
#define  RTE_SSI_MASTER_SCK_PORT            0
#define  RTE_SSI_MASTER_SCK_PIN             68
#define  RTE_SSI_MASTER_SCK_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_SCK_PADSEL          16
#elif    (RTE_SSI_MASTER_SCK_PORT_ID == 6)
#define  RTE_SSI_MASTER_SCK                   1
#define  RTE_SSI_MASTER_SCK_PORT            0
#define  RTE_SSI_MASTER_SCK_PIN             72
#define  RTE_SSI_MASTER_SCK_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_SCK_PADSEL          16
#else
#error "Invalid SSI_MASTER_SCK Pin Configuration!"
#endif

// <o> SSI_MASTER_CS Pin <0=>Not Used <1=>GPIO0_9 <2=>GPIO0_16 <3=>GPIO0_28 <4=>GPIO0_42 <5=>GPIO0_73 
#define RTE_SSI_MASTER_CS_PORT_ID           1
#if    (RTE_SSI_MASTER_CS_PORT_ID == 0)
#define RTE_SSI_MASTER_CS                   0
#elif   (RTE_SSI_MASTER_CS_PORT_ID == 1)
#define  RTE_SSI_MASTER_CS                   1
#define  RTE_SSI_MASTER_CS_PORT            0
#define  RTE_SSI_MASTER_CS_PIN             9
#define  RTE_SSI_MASTER_CS_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_CS_PADSEL          1
#elif   (RTE_SSI_MASTER_CS_PORT_ID == 2)
#define  RTE_SSI_MASTER_CS                   1
#define  RTE_SSI_MASTER_CS_PORT            0
#define  RTE_SSI_MASTER_CS_PIN             16
#define  RTE_SSI_MASTER_CS_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_CS_PADSEL          5
#elif   (RTE_SSI_MASTER_CS_PORT_ID == 3)
#define  RTE_SSI_MASTER_CS                   1
#define  RTE_SSI_MASTER_CS_PORT            0
#define  RTE_SSI_MASTER_CS_PIN             28
#define  RTE_SSI_MASTER_CS_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_CS_PADSEL          1
#elif   (RTE_SSI_MASTER_CS_PORT_ID == 4)
#define  RTE_SSI_MASTER_CS                   1
#define  RTE_SSI_MASTER_CS_PORT            0
#define  RTE_SSI_MASTER_CS_PIN             42
#define  RTE_SSI_MASTER_CS_MODE            EGPIO_PIN_MUX_MODE5
#define  RTE_SSI_MASTER_CS_PADSEL          10
#elif   (RTE_SSI_MASTER_CS_PORT_ID == 5)
#define  RTE_SSI_MASTER_CS                   1
#define  RTE_SSI_MASTER_CS_PORT            0
#define  RTE_SSI_MASTER_CS_PIN             73
#define  RTE_SSI_MASTER_CS_MODE            EGPIO_PIN_MUX_MODE3
#define  RTE_SSI_MASTER_CS_PADSEL          17
#else
#error "Invalid SSI_MASTER_CS Pin Configuration!"
#endif

//   <e> DMA Rx
//     <o3> Channel <28=>28
//     <i>  Selects DMA Channel (only Channel 28 can be used)
//   </e>
#define RTE_SSI_MASTER_RX_DMA                1
#define RTE_SSI_MASTER_UDMA_RX_CH	         28
#define RTE_SSI_MASTER_DMA_RX_LEN_PER_DES	 192

//   <e> DMA Tx
//     <o3> Channel <29=>29
//     <i>  Selects DMA Channel (only Channel 29 can be used)
//   </e>
#define RTE_SSI_MASTER_TX_DMA                1
#define RTE_SSI_MASTER_UDMA_TX_CH      		 29
#define RTE_SSI_MASTER_DMA_TX_LEN_PER_DES	 192
// </e>

// <e> SSI_SLAVE (Serial Peripheral Interface 2) [Driver_SSI_SLAVE]
// <i> Configuration settings for Driver_SSI_SLAVE in component ::CMSIS Driver:SPI
#define RTE_SSI_SLAVE                  0


#define RTE_SSI_SLAVE_INPUT_CLOCK                  SSISLAVE_CLK

// <o> SSI_SLAVE_MISO Pin <0=>Not Used <1=>GPIO0_15 <2=>GPIO0_18 <3=>GPIO0_29 <4=>GPIO0_74
#define RTE_SSI_SLAVE_MISO_PORT_ID           1
#if    (RTE_SSI_SLAVE_MISO_PORT_ID == 0)
#define RTE_SSI_SLAVE_MISO                   0
#elif    (RTE_SSI_SLAVE_MISO_PORT_ID == 1)
#define RTE_SSI_SLAVE_MISO                   1
#define  RTE_SSI_SLAVE_MISO_PORT            0
#define  RTE_SSI_SLAVE_MISO_PIN             15
#define  RTE_SSI_SLAVE_MISO_MODE            EGPIO_PIN_MUX_MODE14
#define  RTE_SSI_SLAVE_MISO_PADSEL          5
#elif    (RTE_SSI_SLAVE_MISO_PORT_ID == 2)
#define RTE_SSI_SLAVE_MISO                   1
#define  RTE_SSI_SLAVE_MISO_PORT            0
#define  RTE_SSI_SLAVE_MISO_PIN             18
#define  RTE_SSI_SLAVE_MISO_MODE            EGPIO_PIN_MUX_MODE7
#define  RTE_SSI_SLAVE_MISO_PADSEL          6
#elif    (RTE_SSI_SLAVE_MISO_PORT_ID == 3)
#define RTE_SSI_SLAVE_MISO                   1
#define  RTE_SSI_SLAVE_MISO_PORT            0
#define  RTE_SSI_SLAVE_MISO_PIN             29
#define  RTE_SSI_SLAVE_MISO_MODE            EGPIO_PIN_MUX_MODE9
#define  RTE_SSI_SLAVE_MISO_PADSEL          1
#elif    (RTE_SSI_SLAVE_MISO_PORT_ID == 4)
#define RTE_SSI_SLAVE_MISO                   1
#define  RTE_SSI_SLAVE_MISO_PORT            0
#define  RTE_SSI_SLAVE_MISO_PIN             74
#define  RTE_SSI_SLAVE_MISO_MODE            EGPIO_PIN_MUX_MODE9
#define  RTE_SSI_SLAVE_MISO_PADSEL          19
#else
#error "Invalid SSI_SLAVE_MISO Pin Configuration!"
#endif

// <o> SSI_SLAVE_MOSI Pin <0=>Not Used <1=>GPIO0_12 <2=>GPIO0_19 <3=>GPIO0_30 <4=>GPIO0_75
#define RTE_SSI_SLAVE_MOSI_PORT_ID           1
#if    (RTE_SSI_SLAVE_MOSI_PORT_ID == 0)
#define RTE_SSI_SLAVE_MOSI                   0
#elif    (RTE_SSI_SLAVE_MOSI_PORT_ID == 1)
#define RTE_SSI_SLAVE_MOSI                   1
#define  RTE_SSI_SLAVE_MOSI_PORT            0
#define  RTE_SSI_SLAVE_MOSI_PIN             12
#define  RTE_SSI_SLAVE_MOSI_MODE            EGPIO_PIN_MUX_MODE14
#define  RTE_SSI_SLAVE_MOSI_PADSEL          3
#elif    (RTE_SSI_SLAVE_MOSI_PORT_ID == 2)
#define RTE_SSI_SLAVE_MOSI                   1
#define  RTE_SSI_SLAVE_MOSI_PORT            0
#define  RTE_SSI_SLAVE_MOSI_PIN             19
#define  RTE_SSI_SLAVE_MOSI_MODE            EGPIO_PIN_MUX_MODE7
#define  RTE_SSI_SLAVE_MOSI_PADSEL          6
#elif    (RTE_SSI_SLAVE_MOSI_PORT_ID == 3)
#define RTE_SSI_SLAVE_MOSI                   1
#define  RTE_SSI_SLAVE_MOSI_PORT            0
#define  RTE_SSI_SLAVE_MOSI_PIN             30
#define  RTE_SSI_SLAVE_MOSI_MODE            EGPIO_PIN_MUX_MODE9
#define  RTE_SSI_SLAVE_MOSI_PADSEL          1
#elif    (RTE_SSI_SLAVE_MOSI_PORT_ID == 4)
#define RTE_SSI_SLAVE_MOSI                   1
#define  RTE_SSI_SLAVE_MOSI_PORT            0
#define  RTE_SSI_SLAVE_MOSI_PIN             75
#define  RTE_SSI_SLAVE_MOSI_MODE            EGPIO_PIN_MUX_MODE9
#define  RTE_SSI_SLAVE_MOSI_PADSEL          19
#else
#error "Invalid SSI_SLAVE_MOSI Pin Configuration!"
#endif

// <o> SSI_SLAVE_SCK Pin <0=>Not Used <1=>GPIO0_11 <2=>GPIO0_17 <3=>GPIO0_28 <4=>GPIO0_72
#define RTE_SSI_SLAVE_SCK_PORT_ID           3
#if    (RTE_SSI_SLAVE_SCK_PORT_ID == 0)
#define  RTE_SSI_SLAVE_SCK                 0
#elif    (RTE_SSI_SLAVE_SCK_PORT_ID == 1)
#define  RTE_SSI_SLAVE_SCK                 1
#define  RTE_SSI_SLAVE_SCK_PORT            0
#define  RTE_SSI_SLAVE_SCK_PIN             11
#define  RTE_SSI_SLAVE_SCK_MODE            EGPIO_PIN_MUX_MODE14
#define  RTE_SSI_SLAVE_SCK_PADSEL          2
#elif    (RTE_SSI_SLAVE_SCK_PORT_ID == 2)
#define  RTE_SSI_SLAVE_SCK                 1
#define  RTE_SSI_SLAVE_SCK_PORT            0
#define  RTE_SSI_SLAVE_SCK_PIN             17
#define  RTE_SSI_SLAVE_SCK_MODE            EGPIO_PIN_MUX_MODE8
#define  RTE_SSI_SLAVE_SCK_PADSEL          5
#elif    (RTE_SSI_SLAVE_SCK_PORT_ID == 3)
#define  RTE_SSI_SLAVE_SCK                 1
#define  RTE_SSI_SLAVE_SCK_PORT            0
#define  RTE_SSI_SLAVE_SCK_PIN             28
#define  RTE_SSI_SLAVE_SCK_MODE            EGPIO_PIN_MUX_MODE9
#define  RTE_SSI_SLAVE_SCK_PADSEL          1
#elif    (RTE_SSI_SLAVE_SCK_PORT_ID == 4)
#define  RTE_SSI_SLAVE_SCK                 1
#define  RTE_SSI_SLAVE_SCK_PORT            0
#define  RTE_SSI_SLAVE_SCK_PIN             72
#define  RTE_SSI_SLAVE_SCK_MODE            EGPIO_PIN_MUX_MODE9
#define  RTE_SSI_SLAVE_SCK_PADSEL          16
#else
#error "Invalid SSI_SLAVE_SCK Pin Configuration!"
#endif

// <o> SSI_SLAVE_CS Pin <0=>Not Used <1=>GPIO0_10 <2=>GPIO0_16 <3=>GPIO0_27 <4=>GPIO0_73
#define RTE_SSI_SLAVE_CS_PORT_ID           1
#if    (RTE_SSI_SLAVE_CS_PORT_ID == 0)
#define RTE_SSI_SLAVE_CS                   0
#elif    (RTE_SSI_SLAVE_CS_PORT_ID == 1)
#define RTE_SSI_SLAVE_CS                   1
#define  RTE_SSI_SLAVE_CS_PORT            0
#define  RTE_SSI_SLAVE_CS_PIN             10
#define  RTE_SSI_SLAVE_CS_MODE            EGPIO_PIN_MUX_MODE14
#define  RTE_SSI_SLAVE_CS_PADSEL          2
#elif    (RTE_SSI_SLAVE_CS_PORT_ID == 2)
#define RTE_SSI_SLAVE_CS                   1
#define  RTE_SSI_SLAVE_CS_PORT            0
#define  RTE_SSI_SLAVE_CS_PIN             16
#define  RTE_SSI_SLAVE_CS_MODE            EGPIO_PIN_MUX_MODE8
#define  RTE_SSI_SLAVE_CS_PADSEL          5
#elif    (RTE_SSI_SLAVE_CS_PORT_ID == 3)
#define RTE_SSI_SLAVE_CS                   1
#define  RTE_SSI_SLAVE_CS_PORT            0
#define  RTE_SSI_SLAVE_CS_PIN             27
#define  RTE_SSI_SLAVE_CS_MODE            EGPIO_PIN_MUX_MODE9
#define  RTE_SSI_SLAVE_CS_PADSEL          1
#elif    (RTE_SSI_SLAVE_CS_PORT_ID == 4)
#define RTE_SSI_SLAVE_CS                   1
#define  RTE_SSI_SLAVE_CS_PORT            0
#define  RTE_SSI_SLAVE_CS_PIN             73
#define  RTE_SSI_SLAVE_CS_MODE            EGPIO_PIN_MUX_MODE9
#define  RTE_SSI_SLAVE_CS_PADSEL          17
#else
#error "Invalid SSI_SLAVE_CS Pin Configuration!"
#endif

//   <e> DMA Rx
//     <o3> Channel <22=>22
//     <i>  Selects DMA Channel (only Channel 22 can be used)
//   </e>
#define RTE_SSI_SLAVE_RX_DMA                0
#define RTE_SSI_SLAVE_UDMA_RX_CH	         22
#define RTE_SSI_SLAVE_DMA_RX_LEN_PER_DES	 1024

//   <e> DMA Tx
//     <o3> Channel <23=>23
//     <i>  Selects DMA Channel (only Channel 23 can be used)
//   </e>
#define RTE_SSI_SLAVE_TX_DMA                0
#define RTE_SSI_SLAVE_UDMA_TX_CH      		 23
#define RTE_SSI_SLAVE_DMA_TX_LEN_PER_DES	 1024

// </e>

// <e> SSI_ULP_MASTER (Serial Peripheral Interface 3) [Driver_SSI_ULP_MASTER]
// <i> Configuration settings for Driver_SSI_ULP_MASTER in component ::CMSIS Driver:SPI
#define RTE_SSI_ULP_MASTER                  1


#define RTE_SSI_ULP_MASTER_INPUT_CLOCK            ULP_SSI_ULP_32MHZ_RC_CLK

// <o> SSI_ULP_MASTER_MISO Pin <0=>Not Used <1=>ULP_GPIO0_2 <2=>ULP_GPIO0_9
#define RTE_SSI_ULP_MASTER_MISO_PORT_ID           2
#if    (RTE_SSI_ULP_MASTER_MISO_PORT_ID == 0)
#define RTE_SSI_ULP_MASTER_MISO                  0
#elif    (RTE_SSI_ULP_MASTER_MISO_PORT_ID == 1)
#define RTE_SSI_ULP_MASTER_MISO                  1
#define  RTE_SSI_ULP_MASTER_MISO_PORT            0
#define  RTE_SSI_ULP_MASTER_MISO_PIN             2
#define  RTE_SSI_ULP_MASTER_MISO_MODE            1
#elif    (RTE_SSI_ULP_MASTER_MISO_PORT_ID == 2)
#define RTE_SSI_ULP_MASTER_MISO                  1
#define  RTE_SSI_ULP_MASTER_MISO_PORT            0
#define  RTE_SSI_ULP_MASTER_MISO_PIN             9
#define  RTE_SSI_ULP_MASTER_MISO_MODE            1
#else
#error "Invalid SSI_ULP_MISO Pin Configuration!"
#endif

// <o> SSI_ULP_MASTER_MOSI Pin <0=>Not Used <1=>ULP_GPIO0_1 <2=>ULP_GPIO0_11
#define RTE_SSI_ULP_MASTER_MOSI_PORT_ID           2
#if    (RTE_SSI_ULP_MASTER_MOSI_PORT_ID == 0)
#define RTE_SSI_ULP_MASTER_MOSI                  0
#elif    (RTE_SSI_ULP_MASTER_MOSI_PORT_ID == 1)
#define RTE_SSI_ULP_MASTER_MOSI                  1
#define  RTE_SSI_ULP_MASTER_MOSI_PORT            0
#define  RTE_SSI_ULP_MASTER_MOSI_PIN             1
#define  RTE_SSI_ULP_MASTER_MOSI_MODE            1
#elif    (RTE_SSI_ULP_MASTER_MOSI_PORT_ID == 2)
#define RTE_SSI_ULP_MASTER_MOSI                  1
#define  RTE_SSI_ULP_MASTER_MOSI_PORT            0
#define  RTE_SSI_ULP_MASTER_MOSI_PIN             11
#define  RTE_SSI_ULP_MASTER_MOSI_MODE            1
#else
#error "Invalid SSI_ULP_MOSI Pin Configuration!"
#endif

// <o> SSI_ULP_MASTER_SCK Pin <0=>Not Used <1=>ULP_GPIO0_0 <2=>ULP_GPIO0_8
#define RTE_SSI_ULP_MASTER_SCK_PORT_ID           2
#if    (RTE_SSI_ULP_MASTER_SCK_PORT_ID == 0)
#define RTE_SSI_ULP_MASTER_SCK                  0
#elif    (RTE_SSI_ULP_MASTER_SCK_PORT_ID == 1)
#define RTE_SSI_ULP_MASTER_SCK                  1
#define  RTE_SSI_ULP_MASTER_SCK_PORT            0
#define  RTE_SSI_ULP_MASTER_SCK_PIN             0
#define  RTE_SSI_ULP_MASTER_SCK_MODE            1
#elif    (RTE_SSI_ULP_MASTER_SCK_PORT_ID == 2)
#define RTE_SSI_ULP_MASTER_SCK                  1
#define  RTE_SSI_ULP_MASTER_SCK_PORT            0
#define  RTE_SSI_ULP_MASTER_SCK_PIN             8
#define  RTE_SSI_ULP_MASTER_SCK_MODE            1
#else
#error "Invalid SSI_ULP_SCK Pin Configuration!"
#endif

// <o> SSI_ULP_MASTER_CS Pin <0=>Not Used <1=>ULP_GPIO0_3 <2=>ULP_GPIO0_10
#define RTE_SSI_ULP_MASTER_CS_PORT_ID           2
#if    (RTE_SSI_ULP_MASTER_CS_PORT_ID == 0)
#define RTE_SSI_ULP_MASTER_CS                  0
#elif    (RTE_SSI_ULP_MASTER_CS_PORT_ID == 1)
#define RTE_SSI_ULP_MASTER_CS                  1
#define  RTE_SSI_ULP_MASTER_CS_PORT            0
#define  RTE_SSI_ULP_MASTER_CS_PIN             3
#define  RTE_SSI_ULP_MASTER_CS_MODE            1
#elif    (RTE_SSI_ULP_MASTER_CS_PORT_ID == 2)
#define RTE_SSI_ULP_MASTER_CS                  1
#define  RTE_SSI_ULP_MASTER_CS_PORT            0
#define  RTE_SSI_ULP_MASTER_CS_PIN             10
#define  RTE_SSI_ULP_MASTER_CS_MODE            1
#else
#error "Invalid SSI_ULP_NSS Pin Configuration!"
#endif

//   <e> DMA Rx
//     <o3> Channel <2=>2
//     <i>  Selects DMA Channel (only Channel 2 can be used)
//   </e>
#define RTE_SSI_ULP_MASTER_RX_DMA                1
#define RTE_SSI_ULP_MASTER_UDMA_RX_CH	           2
#define RTE_SSI_ULP_MASTER_DMA_RX_LEN_PER_DES	   96

//   <e> DMA Tx
//     <o3> Channel <3=>3
//     <i>  Selects DMA Channel (only Channel 3 can be used)
//   </e>
#define RTE_SSI_ULP_MASTER_TX_DMA                1
#define RTE_SSI_ULP_MASTER_UDMA_TX_CH      		   3
#define RTE_SSI_ULP_MASTER_DMA_TX_LEN_PER_DES	   96

// </e>
/*===================================================================
                            UDMA Defines
====================================================================*/
// <e> UDMA  [Driver_UDMA]
#define DESC_MAX_LEN                    0x3FF
#define RTE_UDMA0                        1
#define UDMA0_IRQHandler                IRQ033_Handler 
#define CHNL_MASK_REQ0                    0
#define CHNL_PRIORITY0                    0
#define DMA_PERI_ACK0                     0
#define BURST_REQ0_EN                     1
#define UDMA0_CHNL_PRIO_LVL               1
#define UDMA0_SRAM_BASE                 0x1BC00

#define RTE_UDMA1                         1
#define UDMA1_IRQHandler                IRQ010_Handler 
#define CHNL_MASK_REQ1                    0
#define CHNL_PRIORITY1                    0
#define BURST_REQ1_EN                     1
#define CHNL_HIGH_PRIO_EN1                1
#define UDMA1_CHNL_PRIO_LVL               1
#define ULP_SRAM_START_ADDR             0x24060000
#define ULP_SRAM_END_ADDR               0x24063E00
//   <o> RTE_UDMA1_BASE_MEM <0=>PS2 <1=>PS4
#define RTE_UDMA1_BASE_MEM                0
#if(RTE_UDMA1_BASE_MEM == 0)
#define UDMA1_SRAM_BASE                 0x24060000
#elif(RTE_UDMA1_BASE_MEM == 1)
#define UDMA1_SRAM_BASE                 0x1CC00
#else
  #error "Invalid UDMA1 Control Base Address!"
#endif
// </e>

// <e> I2S0  [Driver_I2S0]
// <i> Configuration settings for Driver_I2S0 in component ::CMSIS Driver:I2S

#define RTE_I2S0                               1  
#define I2S0_IRQHandler                 IRQ064_Handler 
/*I2S0 PINS*/

//     <o> I2S0_SCLK <0=>P0_7 <1=>P0_17 <2=>P0_42 <3=>P0_49 <4=>P0_28 <5=>P0_68 <6=>P0_72
//     <i> SCLK of I2S0
#define RTE_I2S0_SCLK_PORT_ID                  3
#if(RTE_I2S0_SCLK_PORT_ID == 0)
#define RTE_I2S0_SCLK_PORT                     0
#define RTE_I2S0_SCLK_PIN                      7
#define RTE_I2S0_SCLK_MUX                      5
#define RTE_I2S0_SCLK_PAD                      1                 
#define RTE_I2S0_SCLK_REN                      7
#elif(RTE_I2S0_SCLK_PORT_ID ==1)
#define RTE_I2S0_SCLK_PORT                     0
#define RTE_I2S0_SCLK_PIN                      17
#define RTE_I2S0_SCLK_MUX                      6
#define RTE_I2S0_SCLK_PAD                      5
#define RTE_I2S0_SCLK_REN                      17
#elif(RTE_I2S0_SCLK_PORT_ID ==2)
#define RTE_I2S0_SCLK_PORT                     0
#define RTE_I2S0_SCLK_PIN                      42
#define RTE_I2S0_SCLK_MUX                      4
#define RTE_I2S0_SCLK_PAD                      10
#define RTE_I2S0_SCLK_REN                      42
#elif(RTE_I2S0_SCLK_PORT_ID ==3)
#define RTE_I2S0_SCLK_PORT                     0
#define RTE_I2S0_SCLK_PIN                      49
#define RTE_I2S0_SCLK_MUX                      5
#define RTE_I2S0_SCLK_PAD                      12
#define RTE_I2S0_SCLK_REN                      49
#elif(RTE_I2S0_SCLK_PORT_ID ==4)
#define RTE_I2S0_SCLK_PORT                     0
#define RTE_I2S0_SCLK_PIN                      28
#define RTE_I2S0_SCLK_MUX                      5
#define RTE_I2S0_SCLK_PAD                      0
#define RTE_I2S0_SCLK_REN                      28
#elif(RTE_I2S0_SCLK_PORT_ID ==5)
#define RTE_I2S0_SCLK_PORT                     0
#define RTE_I2S0_SCLK_PIN                      68
#define RTE_I2S0_SCLK_MUX                      10
#define RTE_I2S0_SCLK_PAD                      16
#define RTE_I2S0_SCLK_REN                      4
#elif(RTE_I2S0_SCLK_PORT_ID ==6)
#define RTE_I2S0_SCLK_PORT                     0
#define RTE_I2S0_SCLK_PIN                      72
#define RTE_I2S0_SCLK_MUX                      5
#define RTE_I2S0_SCLK_PAD                      16
#define RTE_I2S0_SCLK_REN                      8
#else
  #error "Invalid I2S0 RTE_I2S0_SCLK Pin Configuration!"
#endif

//     <o> I2S0_WSCLK <0=>P0_11 <1=>P0_16 <2=>P0_43 <3=>P0_48  <4=>P0_69 <5=>P0_27 <5=>P0_73
//     <i> WSCLK for I2S0
#define RTE_I2S0_WSCLK_PORT_ID                 3
#if(RTE_I2S0_WSCLK_PORT_ID == 0)
#define RTE_I2S0_WSCLK_PORT                    0
#define RTE_I2S0_WSCLK_PIN                     11
#define RTE_I2S0_WSCLK_MUX                     5
#define RTE_I2S0_WSCLK_PAD                     2
#define RTE_I2S0_WSCLK_REN                     11
#elif(RTE_I2S0_WSCLK_PORT_ID == 1)
#define RTE_I2S0_WSCLK_PORT                    0
#define RTE_I2S0_WSCLK_PIN                     16
#define RTE_I2S0_WSCLK_MUX                     6
#define RTE_I2S0_WSCLK_PAD                     5
#define RTE_I2S0_WSCLK_REN                     16
#elif(RTE_I2S0_WSCLK_PORT_ID ==2)
#define RTE_I2S0_WSCLK_PORT                    0
#define RTE_I2S0_WSCLK_PIN                     43
#define RTE_I2S0_WSCLK_MUX                     4
#define RTE_I2S0_WSCLK_PAD                     10
#define RTE_I2S0_WSCLK_REN                     43
#elif(RTE_I2S0_WSCLK_PORT_ID ==3)
#define RTE_I2S0_WSCLK_PORT                    0
#define RTE_I2S0_WSCLK_PIN                     48
#define RTE_I2S0_WSCLK_MUX                     7
#define RTE_I2S0_WSCLK_PAD                     11
#define RTE_I2S0_WSCLK_REN                     48
#elif(RTE_I2S0_WSCLK_PORT_ID ==4)
#define RTE_I2S0_WSCLK_PORT                    0
#define RTE_I2S0_WSCLK_PIN                     69
#define RTE_I2S0_WSCLK_MUX                     10
#define RTE_I2S0_WSCLK_PAD                     17
#define RTE_I2S0_WSCLK_REN                     5
#elif(RTE_I2S0_WSCLK_PORT_ID ==5)
#define RTE_I2S0_WSCLK_PORT                    0
#define RTE_I2S0_WSCLK_PIN                     27
#define RTE_I2S0_WSCLK_MUX                     5
#define RTE_I2S0_WSCLK_PAD                     0
#define RTE_I2S0_WSCLK_REN                     27
#elif(RTE_I2S0_WSCLK_PORT_ID ==6)
#define RTE_I2S0_WSCLK_PORT                    0
#define RTE_I2S0_WSCLK_PIN                     73
#define RTE_I2S0_WSCLK_MUX                     5
#define RTE_I2S0_WSCLK_PAD                     17 
#define RTE_I2S0_WSCLK_REN                     9
#else
  #error "Invalid I2S0 RTE_I2S0_WSCLK Pin Configuration!"
#endif

//     <o> I2S0_DOUT0 <0=>P0_6 <1=>P0_30 <2=>P0_45 <3=>P0_71  <4=>P0_75
//     <i> DOUT0 for I2S0
#define RTE_I2S0_DOUT0_PORT_ID                   0
#if(RTE_I2S0_DOUT0_PORT_ID ==0)
#define RTE_I2S0_DOUT0_PORT                      0
#define RTE_I2S0_DOUT0_PIN                       6
#define RTE_I2S0_DOUT0_MUX                       5
#define RTE_I2S0_DOUT0_PAD                       1
#define RTE_I2S0_DOUT0_REN                       6
#elif(RTE_I2S0_DOUT0_PORT_ID ==1)
#define RTE_I2S0_DOUT0_PORT                      0
#define RTE_I2S0_DOUT0_PIN                       30
#define RTE_I2S0_DOUT0_MUX                       5
#define RTE_I2S0_DOUT0_PAD                       0
#define RTE_I2S0_DOUT0_REN                       30
#elif(RTE_I2S0_DOUT0_PORT_ID ==2)
#define RTE_I2S0_DOUT0_PORT                      0
#define RTE_I2S0_DOUT0_PIN                       45
#define RTE_I2S0_DOUT0_MUX                       4
#define RTE_I2S0_DOUT0_PAD                       10
#define RTE_I2S0_DOUT0_REN                       45
#elif(RTE_I2S0_DOUT0_PORT_ID ==3)
#define RTE_I2S0_DOUT0_PORT                      0
#define RTE_I2S0_DOUT0_PIN                       71
#define RTE_I2S0_DOUT0_MUX                       10
#define RTE_I2S0_DOUT0_PAD                       18
#define RTE_I2S0_DOUT0_REN                       7
#elif(RTE_I2S0_DOUT0_PORT_ID ==4)
#define RTE_I2S0_DOUT0_PORT                      0
#define RTE_I2S0_DOUT0_PIN                       75
#define RTE_I2S0_DOUT0_MUX                       5
#define RTE_I2S0_DOUT0_PAD                       19
#define RTE_I2S0_DOUT0_REN                       11
#else
  #error "Invalid I2S0 RTE_I2S0_DOUT0 Pin Configuration!"
#endif

//     <o> I2S0_DIN0 <0=>P0_10 <1=>P0_29 <2=>P0_44 <3=>P0_70  <4=>P0_74
//     <i> DIN0 for I2S0
#define RTE_I2S0_DIN0_PORT_ID                   0
#if(RTE_I2S0_DIN0_PORT_ID ==0)
#define RTE_I2S0_DIN0_PORT                      0
#define RTE_I2S0_DIN0_PIN                       10
#define RTE_I2S0_DIN0_MUX                       5
#define RTE_I2S0_DIN0_PAD                       2
#define RTE_I2S0_DIN0_REN                       10                                                      
#elif(RTE_I2S0_DIN0_PORT_ID ==1)
#define RTE_I2S0_DIN0_PORT                      0
#define RTE_I2S0_DIN0_PIN                       29
#define RTE_I2S0_DIN0_MUX                       5
#define RTE_I2S0_DIN0_PAD                       0
#define RTE_I2S0_DIN0_REN                       29 
#elif(RTE_I2S0_DIN0_PORT_ID ==2)
#define RTE_I2S0_DIN0_PORT                      0
#define RTE_I2S0_DIN0_PIN                       44
#define RTE_I2S0_DIN0_MUX                       4
#define RTE_I2S0_DIN0_PAD                       10
#define RTE_I2S0_DIN0_REN                       44
#elif(RTE_I2S0_DIN0_PORT_ID ==3)
#define RTE_I2S0_DIN0_PORT                      0
#define RTE_I2S0_DIN0_PIN                       70
#define RTE_I2S0_DIN0_MUX                       10
#define RTE_I2S0_DIN0_PAD                       18
#define RTE_I2S0_DIN0_REN                       6
#elif(RTE_I2S0_DIN0_PORT_ID ==4)
#define RTE_I2S0_DIN0_PORT                      0
#define RTE_I2S0_DIN0_PIN                       74
#define RTE_I2S0_DIN0_MUX                       5
#define RTE_I2S0_DIN0_PAD                       19
#define RTE_I2S0_DIN0_REN                       10
#else
  #error "Invalid I2S0 RTE_I2S0_DIN0 Pin Configuration!"
#endif

//     <o> I2S0_DOUT1 <0=>P0_18 <1=>P0_24 <2=>P0_47 
//     <i> DOUT1 for I2S0
#define RTE_I2S0_DOUT1_PORT_ID                   0
#if(RTE_I2S0_DOUT1_PORT_ID ==0)
#define RTE_I2S0_DOUT1_PORT                      0
#define RTE_I2S0_DOUT1_PIN                       18
#define RTE_I2S0_DOUT1_MUX                       6
#define RTE_I2S0_DOUT1_PAD                       6
#define RTE_I2S0_DOUT1_REN                       18
#elif(RTE_I2S0_DOUT1_PORT_ID ==1)
#define RTE_I2S0_DOUT1_PORT                      0
#define RTE_I2S0_DOUT1_PIN                       24
#define RTE_I2S0_DOUT1_MUX                       5
#define RTE_I2S0_DOUT1_PAD                       0
#define RTE_I2S0_DOUT1_REN                       24
#elif(RTE_I2S0_DOUT1_PORT_ID ==2)
#define RTE_I2S0_DOUT1_PORT                      0
#define RTE_I2S0_DOUT1_PIN                       47
#define RTE_I2S0_DOUT1_MUX                       7
#define RTE_I2S0_DOUT1_PAD                       11
#define RTE_I2S0_DOUT1_REN                       47
#else
  #error "Invalid I2S0 RTE_I2S0_DOUT1 Pin Configuration!"
#endif

//     <o> I2S0_DIN1 <0=>P0_19 <1=>P0_23 <2=>P0_46 
//     <i> DIN1 for I2S0
#define RTE_I2S0_DIN1_PORT_ID                   0
#if(RTE_I2S0_DIN1_PORT_ID ==0)
#define RTE_I2S0_DIN1_PORT                      0
#define RTE_I2S0_DIN1_PIN                       19
#define RTE_I2S0_DIN1_MUX                       6
#define RTE_I2S0_DIN1_PAD                       6
#define RTE_I2S0_DIN1_REN                       19
#elif(RTE_I2S0_DIN1_PORT_ID ==1)
#define RTE_I2S0_DIN1_PORT                      0
#define RTE_I2S0_DIN1_PIN                       23
#define RTE_I2S0_DIN1_MUX                       5
#define RTE_I2S0_DIN1_PAD                       0
#define RTE_I2S0_DIN1_REN                       23
#elif(RTE_I2S0_DIN1_PORT_ID ==2)
#define RTE_I2S0_DIN1_PORT                      0
#define RTE_I2S0_DIN1_PIN                       46
#define RTE_I2S0_DIN1_MUX                       7
#define RTE_I2S0_DIN1_PAD                       11
#define RTE_I2S0_DIN1_REN                       46
#else
  #error "Invalid I2S0 RTE_I2S0_DIN1 Pin Configuration!"
#endif
// FIFO level can have value 1 to 7
#define I2S0_TX_FIFO_LEVEL   ( 2U )
#define I2S0_RX_FIFO_LEVEL   ( 2U )

//   <o> I2S0_TX_RES         <0=>12 
//                           <1=>16 
//                           <2=>20 
//                           <3=>24
#define RTE_I2S0_TX_RES         1
#if (RTE_I2S0_TX_RES ==0)
#define I2S0_TX_RES          RES_12_BIT                                                               
#elif(RTE_I2S0_TX_RES ==1)
#define I2S0_TX_RES          RES_16_BIT
#elif(RTE_I2S0_TX_RES ==2)
#define I2S0_TX_RES          RES_20_BIT
#elif(RTE_I2S0_TX_RES ==3)
#define I2S0_TX_RES          RES_24_BIT
#else
  #error "Invalid I2S0 TX channel resolution!"
#endif

//   <o> I2S0_RX_RES         <0=>12 
//                           <1=>16 
//                           <2=>20 
//                           <3=>24
#define RTE_I2S0_RX_RES         1
#if (RTE_I2S0_RX_RES ==0)
#define I2S0_RX_RES          RES_12_BIT                                                               
#elif(RTE_I2S0_RX_RES ==1)
#define I2S0_RX_RES          RES_16_BIT
#elif(RTE_I2S0_RX_RES ==2)
#define I2S0_RX_RES          RES_20_BIT
#elif(RTE_I2S0_RX_RES ==3)
#define I2S0_RX_RES          RES_24_BIT
#else
  #error "Invalid I2S0 RX channel resolution!"
#endif

#define RTE_I2S0_CHNL_UDMA_TX_EN                1    
#define RTE_I2S0_CHNL_UDMA_TX_CH                15

#define RTE_I2S0_CHNL_UDMA_RX_EN                1 
#define RTE_I2S0_CHNL_UDMA_RX_CH                14

#define RTE_I2S0_CLK_SRC                        0
#define RTE_I2S0_CLK_DIV_FACT                   0
#define RTE_I2S_PLL_FREQ                        2000

#define RTE_I2S0_DMA_TX_LEN_PER_DES             1024
#define RTE_I2S0_DMA_RX_LEN_PER_DES             1024

// </e>

// <e> I2S1  [Driver_I2S1]
// <i> Configuration settings for Driver_I2S1 in component ::Drivers:I2S
#define RTE_I2S1                               0 
#define I2S1_IRQHandler                        IRQ014_Handler 

//   <o> I2S1_SCLK Pin <0=>P0_3 <1=>P0_7 <2=>P0_8 <3=>P0_12
/*I2S1 PINS*/
#define RTE_I2S1_SCLK_PORT_ID                  2
#if(RTE_I2S1_SCLK_PORT_ID == 0)
#define RTE_I2S1_SCLK_PORT                     0
#define RTE_I2S1_SCLK_PIN                      3
#define RTE_I2S1_SCLK_MUX                      2
#define RTE_I2S1_SCLK_REN                      3
#elif(RTE_I2S1_SCLK_PORT_ID ==1)
#define RTE_I2S1_SCLK_PORT                     0
#define RTE_I2S1_SCLK_PIN                      7
#define RTE_I2S1_SCLK_MUX                      2
#define RTE_I2S1_SCLK_REN                      7
#elif(RTE_I2S1_SCLK_PORT_ID ==2)
#define RTE_I2S1_SCLK_PORT                     0
#define RTE_I2S1_SCLK_PIN                      8
#define RTE_I2S1_SCLK_MUX                      2
#define RTE_I2S1_SCLK_REN                      8
#elif(RTE_I2S0_SCLK_PORT_ID ==3)
#define RTE_I2S1_SCLK_PORT                     0
#define RTE_I2S1_SCLK_PIN                      12
#define RTE_I2S1_SCLK_MUX                      2
#define RTE_I2S1_SCLK_REN                      12
#else
  #error "Invalid I2S1 RTE_I2S1_SCLK Pin Configuration!"
#endif

//   <o> I2S1_WSCLK Pin <0=>P0_2 <1=>P0_4 <2=>P0_10 <3=>P0_14
#define RTE_I2S1_WSCLK_PORT_ID                 2
#if(RTE_I2S1_WSCLK_PORT_ID == 0)
#define RTE_I2S1_WSCLK_PORT                    0
#define RTE_I2S1_WSCLK_PIN                     2
#define RTE_I2S1_WSCLK_MUX                     2
#define RTE_I2S1_WSCLK_REN                     2
#elif(RTE_I2S0_WSCLK_PORT_ID == 1)
#define RTE_I2S1_WSCLK_PORT                    0
#define RTE_I2S1_WSCLK_PIN                     4
#define RTE_I2S1_WSCLK_MUX                     2
#define RTE_I2S1_WSCLK_REN                     2
#elif(RTE_I2S1_WSCLK_PORT_ID ==2)
#define RTE_I2S1_WSCLK_PORT                    0
#define RTE_I2S1_WSCLK_PIN                     10
#define RTE_I2S1_WSCLK_MUX                     2
#define RTE_I2S1_WSCLK_REN                     10
#elif(RTE_I2S1_WSCLK_PORT_ID ==3)
#define RTE_I2S1_WSCLK_PORT                    0
#define RTE_I2S1_WSCLK_PIN                     14
#define RTE_I2S1_WSCLK_MUX                     2
#define RTE_I2S1_WSCLK_REN                     14
#else
  #error "Invalid I2S1 RTE_I2S1_WSCLK Pin Configuration!"
#endif

//   <o> I2S1_DOUT0 Pin <0=>P0_1 <1=>P0_5 <2=>P0_11 <3=>P0_15
#define RTE_I2S1_DOUT0_PORT_ID                 2
#if(RTE_I2S1_DOUT0_PORT_ID ==0)
#define RTE_I2S1_DOUT0_PORT                    0
#define RTE_I2S1_DOUT0_PIN                     1
#define RTE_I2S1_DOUT0_MUX                     2
#define RTE_I2S1_DOUT0_REN                     1
#elif(RTE_I2S1_DOUT0_PORT_ID ==1)
#define RTE_I2S1_DOUT0_PORT                    0
#define RTE_I2S1_DOUT0_PIN                     5
#define RTE_I2S1_DOUT0_MUX                     2
#define RTE_I2S1_DOUT0_REN                     5
#elif(RTE_I2S1_DOUT0_PORT_ID ==2)
#define RTE_I2S1_DOUT0_PORT                    0
#define RTE_I2S1_DOUT0_PIN                     11
#define RTE_I2S1_DOUT0_MUX                     2
#define RTE_I2S1_DOUT0_REN                     11
#elif(RTE_I2S1_DOUT0_PORT_ID ==3)
#define RTE_I2S1_DOUT0_PORT                    0
#define RTE_I2S1_DOUT0_PIN                     15
#define RTE_I2S1_DOUT0_MUX                     2
#define RTE_I2S1_DOUT0_REN                     15
#else
  #error "Invalid I2S1 RTE_I2S1_DOUT0 Pin Configuration!"
#endif

//   <o> I2S1_DIN0 Pin <0=>P0_0 <1=>P0_6 <2=>P0_9 <3=>P0_13
#define RTE_I2S1_DIN0_PORT_ID                   2
#if(RTE_I2S1_DIN0_PORT_ID ==0)
#define RTE_I2S1_DIN0_PORT                      0
#define RTE_I2S1_DIN0_PIN                       0
#define RTE_I2S1_DIN0_MUX                       2
#define RTE_I2S1_DIN0_REN                       0                                                      
#elif(RTE_I2S1_DIN0_PORT_ID ==1)
#define RTE_I2S1_DIN0_PORT                      0
#define RTE_I2S1_DIN0_PIN                       6
#define RTE_I2S1_DIN0_MUX                       2
#define RTE_I2S1_DIN0_REN                       6 
#elif(RTE_I2S1_DIN0_PORT_ID ==2)
#define RTE_I2S1_DIN0_PORT                      0
#define RTE_I2S1_DIN0_PIN                       9
#define RTE_I2S1_DIN0_MUX                       2
#define RTE_I2S1_DIN0_REN                       9
#elif(RTE_I2S1_DIN0_PORT_ID ==3)
#define RTE_I2S1_DIN0_PORT                      0
#define RTE_I2S1_DIN0_PIN                       13
#define RTE_I2S1_DIN0_MUX                       2
#define RTE_I2S1_DIN0_REN                       13
#else
  #error "Invalid I2S1 RTE_I2S1_DIN0 Pin Configuration!"
#endif

// FIFO level can have value 1 to 7
#define I2S1_TX_FIFO_LEVEL   ( 2U )
#define I2S1_RX_FIFO_LEVEL   ( 2U )

//   <o> I2S1_TX_RES         <0=>12 
//                           <1=>16 
//                           <2=>20 
//                           <3=>24
#define RTE_I2S1_TX_RES         1
#if (RTE_I2S1_TX_RES ==0)
#define I2S1_TX_RES          RES_12_BIT                                                               
#elif(RTE_I2S1_TX_RES ==1)
#define I2S1_TX_RES          RES_16_BIT
#elif(RTE_I2S1_TX_RES ==2)
#define I2S1_TX_RES          RES_20_BIT
#elif(RTE_I2S1_TX_RES ==3)
#define I2S1_TX_RES          RES_24_BIT
#else
  #error "Invalid I2S1 TX channel resolution!"
#endif

//   <o> I2S1_RX_RES         <0=>12 
//                           <1=>16 
//                           <2=>20 
//                           <3=>24
#define RTE_I2S1_RX_RES         1
#if (RTE_I2S1_RX_RES ==0)
#define I2S1_RX_RES          RES_12_BIT                                                               
#elif(RTE_I2S1_RX_RES ==1)
#define I2S1_RX_RES          RES_16_BIT
#elif(RTE_I2S1_RX_RES ==2)
#define I2S1_RX_RES          RES_20_BIT
#elif(RTE_I2S1_RX_RES ==3)
#define I2S1_RX_RES          RES_24_BIT
#else
  #error "Invalid I2S1 RX channel resolution!"
#endif

#define RTE_I2S1_CHNL_UDMA_TX_EN                1 
#define RTE_I2S1_CHNL_UDMA_TX_CH                7

#define RTE_I2S1_CHNL_UDMA_RX_EN                1 
#define RTE_I2S1_CHNL_UDMA_RX_CH                6

#define I2S1_CLK_DIV_FACT                       0
//   <o> I2S1_CLK_SRC        <0=>ULP_I2S_REF_CLK 
//                           <1=>ULP_I2S_ULP_32KHZ_RO_CLK 
//                           <2=>ULP_I2S_ULP_32KHZ_RC_CLK 
//                           <3=>ULP_I2S_ULP_32KHZ_XTAL_CLK
//                           <4=>ULP_I2S_ULP_32MHZ_RC_CLK
//                           <5=>ULP_I2S_ULP_20MHZ_RO_CLK
//                           <6=>ULP_I2S_SOC_CLK
//                           <7=>ULP_I2S_ULP_DOUBLER_CLK
//                           <8=>ULP_I2S_PLL_CLK

#define RTE_I2S1_CLK_SEL_ID                     5
#if( RTE_I2S1_CLK_SEL_ID == 0)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_REF_CLK                                                   
#elif(RTE_I2S1_CLK_SEL_ID == 1)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_ULP_32KHZ_RO_CLK   
#elif(RTE_I2S1_CLK_SEL_ID == 2)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_ULP_32KHZ_RC_CLK  
#elif(RTE_I2S1_CLK_SEL_ID == 3)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_ULP_32KHZ_XTAL_CLK
#elif(RTE_I2S1_CLK_SEL_ID == 4)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_ULP_32MHZ_RC_CLK
#elif(RTE_I2S1_CLK_SEL_ID == 5)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_ULP_20MHZ_RO_CLK
#elif(RTE_I2S1_CLK_SEL_ID == 6)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_SOC_CLK
#elif(RTE_I2S1_CLK_SEL_ID == 7)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_ULP_DOUBLER_CLK
#elif(RTE_I2S1_CLK_SEL_ID == 8)
#define RTE_I2S1_CLK_SRC                        ULP_I2S_PLL_CLK
#else
  #error "Invalid I2S1 Clock source selection!"
#endif
#define RTE_I2S1_DMA_TX_LEN_PER_DES             1024
#define RTE_I2S1_DMA_RX_LEN_PER_DES             1024 

// </e> I2S1 [Driver_I2S1]

// <e> I2C0 (Inter-integrated Circuit Interface 0) [Driver_I2C0]
// <i> Configuration settings for Driver_I2C0 in component ::Drivers:I2C

#define RTE_I2C0                        1
#define I2C0_IRQHandler                 IRQ042_Handler

//   <o> I2C0_SCL Pin <0=>P0_7 <1=>P0_12 <2=>P0_25 <3=>P0_50 <4=>P0_68  <5=>P0_72
#define RTE_I2C0_SCL_PORT_ID            1                               
#if    (RTE_I2C0_SCL_PORT_ID == 0)
#define RTE_I2C0_SCL_PORT               0
#define RTE_I2C0_SCL_PIN                7
#define RTE_I2C0_SCL_MUX                4
#define RTE_I2C0_SCL_PAD            1
#define RTE_I2C0_SCL_I2C_REN            7
#elif(RTE_I2C0_SCL_PORT_ID == 1)
#define RTE_I2C0_SCL_PORT               0
#define RTE_I2C0_SCL_PIN                12
#define RTE_I2C0_SCL_MUX                4
#define RTE_I2C0_SCL_PAD            3
#define RTE_I2C0_SCL_I2C_REN            12
#elif(RTE_I2C0_SCL_PORT_ID == 2)
#define RTE_I2C0_SCL_PORT               0
#define RTE_I2C0_SCL_PIN                25
#define RTE_I2C0_SCL_MUX                4
#define RTE_I2C0_SCL_PAD            0
#define RTE_I2C0_SCL_I2C_REN            25
#elif(RTE_I2C0_SCL_PORT_ID == 3)
#define RTE_I2C0_SCL_PORT               0
#define RTE_I2C0_SCL_PIN                50
#define RTE_I2C0_SCL_MUX                5
#define RTE_I2C0_SCL_PAD            12
#define RTE_I2C0_SCL_I2C_REN            50
#elif(RTE_I2C0_SCL_PORT_ID == 4)
#define RTE_I2C0_SCL_PORT               0
#define RTE_I2C0_SCL_PIN                68
#define RTE_I2C0_SCL_MUX                7
#define RTE_I2C0_SCL_PAD                16
#define RTE_I2C0_SCL_I2C_REN            4
#elif(RTE_I2C0_SCL_PORT_ID == 5)
#define RTE_I2C0_SCL_PORT               0
#define RTE_I2C0_SCL_PIN                72
#define RTE_I2C0_SCL_MUX                7
#define RTE_I2C0_SCL_PAD                16
#define RTE_I2C0_SCL_I2C_REN            8
#else
  #error "Invalid I2C0 RTE_I2C0_SCL Pin Configuration!"
#endif

//   <o> I2C0_SCL Pin <0=>P0_7 <1=>P0_12 <2=>P0_25 <3=>P0_50 <4=>P0_68  <5=>P0_72
#define RTE_I2C0_SDA_PORT_ID            1
#if    (RTE_I2C0_SDA_PORT_ID == 0)
#define RTE_I2C0_SDA_PORT               0
#define RTE_I2C0_SDA_PIN                6
#define RTE_I2C0_SDA_MUX                4
#define RTE_I2C0_SDA_PAD                1
#define RTE_I2C0_SDA_I2C_REN            6
#elif(RTE_I2C0_SDA_PORT_ID == 1)
#define RTE_I2C0_SDA_PORT               0
#define RTE_I2C0_SDA_PIN                15
#define RTE_I2C0_SDA_MUX                4
#define RTE_I2C0_SDA_PAD                5
#define RTE_I2C0_SDA_I2C_REN            15
#elif(RTE_I2C0_SCL_PORT_ID == 2)
#define RTE_I2C0_SDA_PORT               0
#define RTE_I2C0_SDA_PIN                26
#define RTE_I2C0_SDA_MUX                4
#define RTE_I2C0_SDA_PAD            0
#define RTE_I2C0_SDA_I2C_REN            26
#elif(RTE_I2C0_SDA_PORT_ID == 3)
#define RTE_I2C0_SDA_PORT               0
#define RTE_I2C0_SDA_PIN                51
#define RTE_I2C0_SDA_MUX                5
#define RTE_I2C0_SDA_PAD            12
#define RTE_I2C0_SDA_I2C_REN            51
#elif(RTE_I2C0_SDA_PORT_ID == 4)
#define RTE_I2C0_SDA_PORT               0
#define RTE_I2C0_SDA_PIN                69
#define RTE_I2C0_SDA_MUX                7
#define RTE_I2C0_SDA_PAD                17
#define RTE_I2C0_SDA_I2C_REN            5
#elif(RTE_I2C0_SDA_PORT_ID == 5)
#define RTE_I2C0_SDA_PORT               0
#define RTE_I2C0_SDA_PIN                73
#define RTE_I2C0_SDA_MUX                7
#define RTE_I2C0_SDA_PAD                17
#define RTE_I2C0_SDA_I2C_REN            9
#else
  #error "Invalid I2C0 RTE_I2C0_SDA Pin Configuration!"
#endif

#define IC_SCL_STUCK_TIMEOUT           20
#define IC_SDA_STUCK_TIMEOUT           20

#define I2C_DMA                         0
#if    (I2C_DMA == 1)            
#define DMA_TX_TL                       1
#define DMA_RX_TL                       1
#endif
// </e> I2C0 [Driver_I2C0]

// <e> I2C1 (Inter-integrated Circuit Interface 1) [Driver_I2C1]
// <i> Configuration settings for Driver_I2C1 in component ::Drivers:I2C

#define RTE_I2C1                        0
#define I2C1_IRQHandler                 IRQ061_Handler 
//   <o> I2C1_SCL Pin <0=>P0_10 <1=>P0_20 <2=>P0_28 <3=>P0_35 <4=>P0_40 <5=>P0_66 <6=>P0_70 <7=>P0_74
#define RTE_I2C1_SCL_PORT_ID            0
#if    (RTE_I2C1_SCL_PORT_ID == 0)
#define RTE_I2C1_SCL_PORT               0
#define RTE_I2C1_SCL_PIN                10
#define RTE_I2C1_SCL_MUX                4
#define RTE_I2C1_SCL_PAD            2
#define RTE_I2C1_SCL_REN            10
#elif(RTE_I2C1_SCL_PORT_ID == 1)
#define RTE_I2C1_SCL_PORT               0
#define RTE_I2C1_SCL_PIN                20
#define RTE_I2C1_SCL_MUX                4
#define RTE_I2C1_SCL_PAD            7
#define RTE_I2C1_SCL_REN            20
#elif(RTE_I2C1_SCL_PORT_ID == 2)
#define RTE_I2C1_SCL_PORT               0
#define RTE_I2C1_SCL_PIN                28
#define RTE_I2C1_SCL_MUX                6
#define RTE_I2C1_SCL_PAD            0
#define RTE_I2C1_SCL_REN            28
#elif(RTE_I2C1_SCL_PORT_ID == 3)
#define RTE_I2C1_SCL_PORT               0
#define RTE_I2C1_SCL_PIN                35
#define RTE_I2C1_SCL_MUX                5
#define RTE_I2C1_SCL_PAD            9
#define RTE_I2C1_SCL_REN            35
#elif(RTE_I2C1_SCL_PORT_ID == 4)
#define RTE_I2C1_SCL_PORT               0
#define RTE_I2C1_SCL_PIN                40
#define RTE_I2C1_SCL_MUX                5
#define RTE_I2C1_SCL_PAD            9
#define RTE_I2C1_SCL_REN            40
#elif(RTE_I2C1_SCL_PORT_ID == 5)
#define RTE_I2C1_SCL_PORT               0     
#define RTE_I2C1_SCL_PIN                66
#define RTE_I2C1_SCL_MUX                5
#define RTE_I2C1_SCL_PAD            21
#define RTE_I2C1_SCL_REN            2
#elif(RTE_I2C1_SCL_PORT_ID == 6)
#define RTE_I2C1_SCL_PORT               0     
#define RTE_I2C1_SCL_PIN                70
#define RTE_I2C1_SCL_MUX                7
#define RTE_I2C1_SCL_PAD            18
#define RTE_I2C1_SCL_REN            6
#elif(RTE_I2C1_SCL_PORT_ID == 7)
#define RTE_I2C1_SCL_PORT               0     
#define RTE_I2C1_SCL_PIN                74
#define RTE_I2C1_SCL_MUX                7
#define RTE_I2C1_SCL_PAD            19
#define RTE_I2C1_SCL_REN            10
#else
#error "Invalid I2C1_SCL Pin Configuration!"
#endif

// <o> I2C1_SCL Pin <0=>P0_10 <1=>P0_20 <2=>P0_28 <3=>P0_35 <4=>P0_40 <5=>P0_66 <6=>P0_70 <7=>P0_74
#define RTE_I2C1_SDA_PORT_ID            0
#if    (RTE_I2C1_SDA_PORT_ID == 0)
#define RTE_I2C1_SDA_PORT               0
#define RTE_I2C1_SDA_PIN                11
#define RTE_I2C1_SDA_MUX                4
#define RTE_I2C1_SDA_PAD            2
#define RTE_I2C1_SDA_REN            11
#elif(RTE_I2C1_SDA_PORT_ID == 1)
#define RTE_I2C1_SDA_PORT               0
#define RTE_I2C1_SDA_PIN                18
#define RTE_I2C1_SDA_MUX                4
#define RTE_I2C1_SDA_PAD            6
#define RTE_I2C1_SDA_REN            18
#elif(RTE_I2C1_SDA_PORT_ID == 2)
#define RTE_I2C1_SDA_PORT               0
#define RTE_I2C1_SDA_PIN                29
#define RTE_I2C1_SDA_MUX                6
#define RTE_I2C1_SDA_PAD            0
#define RTE_I2C1_SDA_REN            29
#elif(RTE_I2C1_SDA_PORT_ID == 3)
#define RTE_I2C1_SDA_PORT               0
#define RTE_I2C1_SDA_PIN                36
#define RTE_I2C1_SDA_MUX                5
#define RTE_I2C1_SDA_PAD            9
#define RTE_I2C1_SDA_REN            36
#elif(RTE_I2C1_SDA_PORT_ID == 4)
#define RTE_I2C1_SDA_PORT               0
#define RTE_I2C1_SDA_PIN                41
#define RTE_I2C1_SDA_MUX                5
#define RTE_I2C1_SDA_PAD            9
#define RTE_I2C1_SDA_REN            41
#elif(RTE_I2C1_SDA_PORT_ID == 5)
#define RTE_I2C1_SDA_PORT               0     
#define RTE_I2C1_SDA_PIN                67
#define RTE_I2C1_SDA_MUX                5
#define RTE_I2C1_SDA_PAD            21
#define RTE_I2C1_SDA_REN            3
#elif(RTE_I2C1_SDA_PORT_ID == 6)
#define RTE_I2C1_SDA_PORT               0     
#define RTE_I2C1_SDA_PIN                71
#define RTE_I2C1_SDA_MUX                7
#define RTE_I2C1_SDA_PAD            18
#define RTE_I2C1_SDA_REN            7
#elif(RTE_I2C1_SDA_PORT_ID == 7)
#define RTE_I2C1_SDA_PORT               0     
#define RTE_I2C1_SDA_PIN                75
#define RTE_I2C1_SDA_MUX                7
#define RTE_I2C1_SDA_PAD            19
#define RTE_I2C1_SDA_REN            11
#else
#error "Invalid I2C1_SDA Pin Configuration!"
#endif


#define IC_SCL_STUCK_TIMEOUT           20
#define IC_SDA_STUCK_TIMEOUT           20

#define  DMA_EN                         0
#if     (DMA_EN == 1)            
#define  DMA_TX_TL                      1
#define  DMA_RX_TL                      1
#endif

// </e> I2C1 [Driver_I2C1]

// <e> I2C2 (Inter-integrated Circuit Interface 2) [Driver_I2C2]
// <i> Configuration settings for Driver_I2C2 in component ::Drivers:I2C
#define RTE_I2C2                        1
#define I2C2_IRQHandler                 IRQ013_Handler 

// <o> I2C2_SCL Pin <0=>P0_1 <1=>P0_5 <2=>P0_7 <3=>P0_8 <4=>P0_13
#define RTE_I2C2_SCL_PORT_ID            0
#if    (RTE_I2C2_SCL_PORT_ID == 0)
#define RTE_I2C2_SCL_PORT               0
#define RTE_I2C2_SCL_PIN                1
#define RTE_I2C2_SCL_MUX                4
#define RTE_I2C2_SCL_REN                1
#elif(RTE_I2C2_SCL_PORT_ID == 1)
#define RTE_I2C2_SCL_PORT               0     
#define RTE_I2C2_SCL_PIN                5
#define RTE_I2C2_SCL_MUX                4
#define RTE_I2C2_SCL_REN                5
#elif(RTE_I2C2_SCL_PORT_ID == 2)
#define RTE_I2C2_SCL_PORT               0     
#define RTE_I2C2_SCL_PIN                7
#define RTE_I2C2_SCL_MUX                4
#define RTE_I2C2_SCL_REN                7
#elif(RTE_I2C2_SCL_PORT_ID == 2)
#define RTE_I2C2_SCL_PORT               0     
#define RTE_I2C2_SCL_PIN                8
#define RTE_I2C2_SCL_MUX                4
#define RTE_I2C2_SCL_REN                8
#elif(RTE_I2C2_SCL_PORT_ID == 2)
#define RTE_I2C2_SCL_PORT               0     
#define RTE_I2C2_SCL_PIN                13
#define RTE_I2C2_SCL_MUX                4
#define RTE_I2C2_SCL_REN                13
#else
#error "Invalid I2C2_SCL Pin Configuration!"
#endif

// <o> I2C2_SDA Pin <0=>P0_0 <1=>P0_4 <2=>P0_6 <3=>P0_9 <4=>P0_11 <5=>P0_12
#define RTE_I2C2_SDA_PORT_ID            0
#if    (RTE_I2C2_SDA_PORT_ID == 0)
#define RTE_I2C2_SDA_PORT               0
#define RTE_I2C2_SDA_PIN                0
#define RTE_I2C2_SDA_MUX                4
#define RTE_I2C2_SDA_I2C_REN            0
#elif(RTE_I2C2_SDA_PORT_ID == 1)
#define RTE_I2C2_SDA_PORT               0     
#define RTE_I2C2_SDA_PIN                4
#define RTE_I2C2_SDA_MUX                4
#define RTE_I2C2_SDA_REN                4
#elif(RTE_I2C2_SDA_PORT_ID == 2)
#define RTE_I2C2_SDA_PORT               0     
#define RTE_I2C2_SDA_PIN                6
#define RTE_I2C2_SDA_MUX                4
#define RTE_I2C2_SDA_REN                6
#elif(RTE_I2C2_SDA_PORT_ID == 3)
#define RTE_I2C2_SDA_PORT               0     
#define RTE_I2C2_SDA_PIN                9
#define RTE_I2C2_SDA_MUX                4
#define RTE_I2C2_SDA_I2C_REN            9
#elif(RTE_I2C2_SDA_PORT_ID == 4)
#define RTE_I2C2_SDA_PORT               0     
#define RTE_I2C2_SDA_PIN                11
#define RTE_I2C2_SDA_MUX                4
#define RTE_I2C2_SDA_REN                11
#elif(RTE_I2C2_SDA_PORT_ID == 5)
#define RTE_I2C2_SDA_PORT               0     
#define RTE_I2C2_SDA_PIN                12
#define RTE_I2C2_SDA_MUX                4
#define RTE_I2C2_SDA_REN                12
#else
#error "Invalid I2C2_SDA Pin Configuration!"
#endif

#define IC_SCL_STUCK_TIMEOUT           20
#define IC_SDA_STUCK_TIMEOUT           20

#define DMA_EN                         0
#if    (DMA_EN == 1)            
#define DMA_TX_TL                      1
#define DMA_RX_TL                      1
#endif

// </e> I2C2 [Driver_I2C2]

// <e> GSPI (Generic SPI master) [Driver_GSPI_MASTER]
// <i> Configuration settings for Driver_GSPI_MASTER in component ::Drivers:GSPI
#define RTE_GSPI_MASTER       1

//     <o> GSPI_MASTER_CLK <0=>P0_10 <1=>P0_20 <2=>P0_25 <3=>P0_28 <4=>P0_72  
//     <i> CLK of GSPI0
#define RTE_GSPI_MASTER_CLK_PORT_ID                  4
#if(RTE_GSPI_MASTER_CLK_PORT_ID == 0)
#define RTE_GSPI_MASTER_CLK_PORT                     0
#define RTE_GSPI_MASTER_CLK_PIN                      15
#define RTE_GSPI_MASTER_CLK_MUX                      8
#define RTE_GSPI_MASTER_CLK_PAD                      5
#elif(RTE_GSPI_MASTER_CLK_PORT_ID ==1)
#define RTE_GSPI_MASTER_CLK_PORT                     0
#define RTE_GSPI_MASTER_CLK_PIN                      17
#define RTE_GSPI_MASTER_CLK_MUX                      5
#define RTE_GSPI_MASTER_CLK_PAD                      5
#elif(RTE_GSPI_MASTER_CLK_PORT_ID ==2)
#define RTE_GSPI_MASTER_CLK_PORT                     0
#define RTE_GSPI_MASTER_CLK_PIN                      20
#define RTE_GSPI_MASTER_CLK_MUX                      5
#define RTE_GSPI_MASTER_CLK_PAD                      7
#elif(RTE_GSPI_MASTER_CLK_PORT_ID ==3)
#define RTE_GSPI_MASTER_CLK_PORT                     0
#define RTE_GSPI_MASTER_CLK_PIN                      28
#define RTE_GSPI_MASTER_CLK_MUX                      4
#define RTE_GSPI_MASTER_CLK_PAD                      0//NO PAD
#elif(RTE_GSPI_MASTER_CLK_PORT_ID ==4)
#define RTE_GSPI_MASTER_CLK_PORT                     0
#define RTE_GSPI_MASTER_CLK_PIN                      72
#define RTE_GSPI_MASTER_CLK_MUX                      4
#define RTE_GSPI_MASTER_CLK_PAD                      16
#else
  #error "Invalid GSPI0 RTE_GSPI_MASTER_CLK_PIN Pin Configuration!"
#endif

//     <o> GSPI_MASTER_CS0 <0=>P0_10 <1=>P0_20 <2=>P0_25 <3=>P0_28 <4=>P0_72  
//     <i> CS0 of GSPI0
#define RTE_GSPI_MASTER_CS0_PORT_ID                  4
#if(RTE_GSPI_MASTER_CS0_PORT_ID == 0)
#define RTE_GSPI_MASTER_CS0_PORT                     0
#define RTE_GSPI_MASTER_CS0_PIN                      16
#define RTE_GSPI_MASTER_CS0_MUX                      5
#define RTE_GSPI_MASTER_CS0_PAD                      5
#elif(RTE_GSPI_MASTER_CS0_PORT_ID ==1)
#define RTE_GSPI_MASTER_CS0_PORT                     0
#define RTE_GSPI_MASTER_CS0_PIN                      21
#define RTE_GSPI_MASTER_CS0_MUX                      3
#define RTE_GSPI_MASTER_CS0_PAD                      0//NO PAD
#elif(RTE_GSPI_MASTER_CS0_PORT_ID ==2)
#define RTE_GSPI_MASTER_CS0_PORT                     0
#define RTE_GSPI_MASTER_CS0_PIN                      27
#define RTE_GSPI_MASTER_CS0_MUX                      4
#define RTE_GSPI_MASTER_CS0_PAD                      0//NO PAD
#elif(RTE_GSPI_MASTER_CS0_PORT_ID ==3)
#define RTE_GSPI_MASTER_CS0_PORT                     0
#define RTE_GSPI_MASTER_CS0_PIN                      42
#define RTE_GSPI_MASTER_CS0_MUX                      7
#define RTE_GSPI_MASTER_CS0_PAD                      10
#elif(RTE_GSPI_MASTER_CS0_PORT_ID ==4)
#define RTE_GSPI_MASTER_CS0_PORT                     0
#define RTE_GSPI_MASTER_CS0_PIN                      73
#define RTE_GSPI_MASTER_CS0_MUX                      4
#define RTE_GSPI_MASTER_CS0_PAD                      17
#else
  #error "Invalid GSPI0 RTE_GSPI_MASTER_CS0_PIN Pin Configuration!"
#endif

 //     <o> GSPI_MASTER_MOSI <0=>P0_10 <1=>P0_20 <2=>P0_25 <3=>P0_28 <4=>P0_72  
//     <i> MOSI of GSPI0
#define RTE_GSPI_MASTER_MOSI_PORT_ID                  3
#if(RTE_GSPI_MASTER_MOSI_PORT_ID == 0)
#define RTE_GSPI_MASTER_MOSI_PORT                     0
#define RTE_GSPI_MASTER_MOSI_PIN                      13
#define RTE_GSPI_MASTER_MOSI_MUX                      3
#define RTE_GSPI_MASTER_MOSI_PAD                      4
#elif(RTE_GSPI_MASTER_MOSI_PORT_ID ==1)
#define RTE_GSPI_MASTER_MOSI_PORT                     0
#define RTE_GSPI_MASTER_MOSI_PIN                      18
#define RTE_GSPI_MASTER_MOSI_MUX                      5
#define RTE_GSPI_MASTER_MOSI_PAD                      6
#elif(RTE_GSPI_MASTER_MOSI_PORT_ID ==2)
#define RTE_GSPI_MASTER_MOSI_PORT                     0
#define RTE_GSPI_MASTER_MOSI_PIN                      30
#define RTE_GSPI_MASTER_MOSI_MUX                      4
#define RTE_GSPI_MASTER_MOSI_PAD                      0//NO PAD
#elif(RTE_GSPI_MASTER_MOSI_PORT_ID ==3)
#define RTE_GSPI_MASTER_MOSI_PORT                     0
#define RTE_GSPI_MASTER_MOSI_PIN                      75
#define RTE_GSPI_MASTER_MOSI_MUX                      4
#define RTE_GSPI_MASTER_MOSI_PAD                      19
#else
  #error "Invalid GSPI0 RTE_GSPI_MASTER_MOSI_PIN Pin Configuration!"
#endif

 //     <o> GSPI_MASTER_MISO <0=>P0_10 <1=>P0_20 <2=>P0_25 <3=>P0_28 <4=>P0_72  
//     <i> MISO of GSPI0
#define RTE_GSPI_MASTER_MISO_PORT_ID                  3
#if(RTE_GSPI_MASTER_MISO_PORT_ID == 0)
#define RTE_GSPI_MASTER_MISO_PORT                     0
#define RTE_GSPI_MASTER_MISO_PIN                      14
#define RTE_GSPI_MASTER_MISO_MUX                      3
#define RTE_GSPI_MASTER_MISO_PAD                      4
#elif(RTE_GSPI_MASTER_MISO_PORT_ID ==1)
#define RTE_GSPI_MASTER_MISO_PORT                     0
#define RTE_GSPI_MASTER_MISO_PIN                      19
#define RTE_GSPI_MASTER_MISO_MUX                      5
#define RTE_GSPI_MASTER_MISO_PAD                      6
#elif(RTE_GSPI_MASTER_MISO_PORT_ID ==2)
#define RTE_GSPI_MASTER_MISO_PORT                     0
#define RTE_GSPI_MASTER_MISO_PIN                      29
#define RTE_GSPI_MASTER_MISO_MUX                      4
#define RTE_GSPI_MASTER_MISO_PAD                      0//NO PAD
#elif(RTE_GSPI_MASTER_MISO_PORT_ID ==3)
#define RTE_GSPI_MASTER_MISO_PORT                     0
#define RTE_GSPI_MASTER_MISO_PIN                      74
#define RTE_GSPI_MASTER_MISO_MUX                      4
#define RTE_GSPI_MASTER_MISO_PAD                      19
#else
  #error "Invalid GSPI0 RTE_GSPI_MASTER_MISO_PIN Pin Configuration!"
#endif

#define RTE_GSPI_MASTER_CHNL_UDMA_TX_EN  0
#define RTE_GSPI_MASTER_CHNL_UDMA_TX_CH  11


#define RTE_GSPI_MASTER_CHNL_UDMA_RX_EN  0
#define RTE_GSPI_MASTER_CHNL_UDMA_RX_CH  10

#define RTE_GSPI_CLOCK_SOURCE            GSPI_SOC_PLL_CLK

#define RTE_FIFO_AFULL_THRLD             0xC
#define RTE_FIFO_AEMPTY_THRLD            0x0

#define RTE_GSPI_DMA_TX_LEN_PER_DES      100
#define RTE_GSPI_DMA_RX_LEN_PER_DES      100 

// </e>(Generic SPI master)[Driver_GSPI_MASTER]

#endif  /* __RTE_DEVICE_H */

