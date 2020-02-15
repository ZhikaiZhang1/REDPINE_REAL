/**
 * @file    rsi_udp_client.c
 * @version 0.1
 * @date    15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for UDP client socket
 *
 *  @section Description  This file contains example application for UDP client socket
 *
 *
 */

/**
 * Include files
 * */
 
 #include "rsi_board.h"
//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "sys/socket.h"
//#include "select.h"

//spi includes
#include "SPI.h"
#include "RTE_Device.h"
#include "rsi_chip.h"
#include "spi_decode.h"


//! Access point SSID to connect
#define SSID              "Logan_conqueror"
//#define SSID              "CIK1000M_AC2.4G_9145"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE     RSI_WPA2

//! Password
#define PSK               "89706090"
//#define PSK               "d0608c043090"

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE         1

//! If DHCP mode is disabled give IP statically
#if !(DHCP_MODE)

//! IP address of the module
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP         0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF

#endif

#define HOSTNAME "Logan"
//select defines
#define FD_CLR(fd, set)   (((set)->fd_bits[(fd)/8]) &= ~(1 << ((fd) & 7)))
#define FD_SET(fd, set)   (((set)->fd_bits[(fd)/8]) |= (1 << ((fd) & 7)))
#define FD_ISSET(fd, set) (((set)->fd_bits[(fd)/8]) &  (1 << ((fd) & 7)))
#define FD_ZERO(set)      memset(set, 0, sizeof(fd_set))
//! Server port number
#define SERVER_PORT       3490

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x640AA8C0

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Memory length for driver
#define GLOBAL_BUFF_LEN   10000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   1

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//SPI defines
#define  BUFFER_SIZE      1024      //Number of data to be sent through SPI
#define SPI_BAUD 1000000  //speed at which data transmitted through SPI
#define  SPI_BIT_WIDTH 16 //SPI bit width can be 16/8 for 16/8 bit data transfer

#define ULP_PININT_IRQ_HANDLER     IRQ018_Handler                   /* GPIO interrupt IRQ function name            */
#define ULP_PININT_NVIC_NAME       ULP_EGPIO_PIN_IRQn               /* GPIO interrupt NVIC interrupt name          */

#define PORT                       0                                /* GPIO port number                            */
#define GPIO_PIN                   6                                /* GPIO pin number  (0 to 15)                           */
#define ULP_PIN_INT                0                                /* ULP pin interrupt                           */

/* SPI Driver */
extern ARM_DRIVER_SPI Driver_SSI_MASTER;
volatile bool spi_event = false;
volatile uint8_t spi_done = 0;
volatile uint8_t pack_received = 0;

//interrupt handler
/**
 * @brief Interrupt handler
 * @return Nothing
 */
void ULP_PININT_IRQ_HANDLER(void)
{
uint32_t gintStatus;

/*get interrupt status*/
gintStatus=RSI_EGPIO_GetIntStat(EGPIO1,ULP_PIN_INT);

if((gintStatus &EGPIO_PIN_INT_CLR_RISING ) || (gintStatus &EGPIO_PIN_INT_CLR_FALLING ))
{
/*clear interrupt*/
spi_event = true;
RSI_EGPIO_IntClr(EGPIO1, ULP_PIN_INT ,EGPIO_PIN_INT_CLR_RISING);
RSI_EGPIO_IntClr(EGPIO1, ULP_PIN_INT ,EGPIO_PIN_INT_CLR_FALLING);
}
else
{
RSI_EGPIO_IntMask(EGPIO1 , ULP_PIN_INT);
}
return ;
}

static void Set_Up_INT(void){
/*Configures the system default clock and power configurations*/
  SystemCoreClockUpdate();

/*Enable clock for EGPIO module*/
RSI_ULPSS_PeripheralEnable(ULPCLK,ULP_EGPIO_CLK,ENABLE_STATIC_CLK);

/*Ren enable for input pin*/
RSI_EGPIO_UlpPadReceiverEnable(GPIO_PIN);

/*Configure default GPIO mode(0) */
RSI_EGPIO_SetPinMux(EGPIO1,PORT ,GPIO_PIN,EGPIO_PIN_MUX_MODE0);
RSI_EGPIO_SetIntRiseEdgeEnable(EGPIO1, ULP_PIN_INT);
RSI_EGPIO_SetIntFallEdgeDisable(EGPIO1, ULP_PIN_INT);
/*Selects the pin interrupt for the GPIO*/
RSI_EGPIO_PinIntSel(EGPIO1, ULP_PIN_INT , PORT, GPIO_PIN);

/*Configures the edge /level interrupt*/
//RSI_EGPIO_SetIntLowLevelEnable(EGPIO1,ULP_PIN_INT);

/*unmask the  interrupt*/
RSI_EGPIO_IntUnMask(EGPIO1 , ULP_PIN_INT);

/*NVIC enable */
NVIC_EnableIRQ(ULP_EGPIO_PIN_IRQn);
}
void mySPI_callback(uint32_t event)
{
switch (event)
{
case ARM_SPI_EVENT_TRANSFER_COMPLETE:
spi_done=1;
break;
case ARM_SPI_EVENT_DATA_LOST:
/*  Occurs in slave mode when data is requested/sent by master
            but send/receive/transfer operation has not been started
            and indicates that data is lost. Occurs also in master mode
            when driver cannot transfer data fast enough. */
__breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
break;
case ARM_SPI_EVENT_MODE_FAULT:
/*  Occurs in master mode when Slave Select is deactivated and
            indicates Master Mode Fault. */
__breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
break;
}
}


int32_t     client_socket;
struct      rsi_sockaddr_in server_addr;

#define SEND_LENGTH 96
//! Memory to initialize driver
typedef struct {
int bot_id;
uint8_t send_buffer[SEND_LENGTH];
}array_sending;
typedef struct{
int length;
uint8_t* recv_buffer;
}array_receiving;



uint8_t global_buf[GLOBAL_BUFF_LEN];
uint8_t recv_buffer[SEND_LENGTH];
volatile int pack_length;
uint8_t SPI_buff[1024];
/* Test data buffers */
uint16_t testdata_out[BUFFER_SIZE];
uint16_t testdata_in [BUFFER_SIZE];
wifi_struct* wifi_setup;

/*More OTA*/
uint8_t send_dummy_to_slave[BUFFER_SIZE];
uint8_t slave_in_data[BUFFER_SIZE];
uint8_t slave_in_dummy[BUFFER_SIZE];	// Extra byte for ACK or NACK frame
uint8_t master_out_ACK[BUFFER_SIZE];
uint8_t send_to_slave[BUFFER_SIZE];

static void receive_callback(uint32_t sock_no, uint8_t *buffer, uint32_t length){
//receive and spi
pack_length = length;
pack_received = 1;
//DEBUGOUT("Receive callback\n");
//memcpy(SPI_buff, (uint8_t)* &length, 1);
memcpy(SPI_buff, buffer, length);
}

int32_t rsi_udp_client()
{
  int32_t     status       = RSI_SUCCESS;
  int32_t     packet_count = 0;
struct hostent * he;
char* hostname = HOSTNAME;


//char* psk = PSK;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#endif

  //! WC initialization
wireless_again:
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
DEBUGOUT("init wireless failed\n");
goto wireless_again;
    //return status;
  }

  //! Scan for Access points

sanning_again:
  status = rsi_wlan_scan((int8_t *)(wifi_setup->ssid), (uint8_t)CHANNEL_NO, NULL, 0);
  if(status != RSI_SUCCESS)
  {
DEBUGOUT("scanning failed\n");
goto sanning_again;
    //return status;
  }

  //! Connect to an Access point
connect_again:
  status = rsi_wlan_connect((int8_t *)(wifi_setup->ssid), SECURITY_TYPE, (void *)(wifi_setup->psk));
  if(status != RSI_SUCCESS)
  {
DEBUGOUT("connecting failed\n");
goto connect_again;
    //return status;
  }

  //! Configure IP
IP_CONFIG:
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, NULL, 0,0);//RSI_DHCP_HOSTNAME gives hostname feature, not sure what it does, will try out later
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif

  if(status != RSI_SUCCESS)
  {
DEBUGOUT("config ip address failed\n");
goto IP_CONFIG;
    //return status;
  }

if ((he = gethostbyname (wifi_setup->host_name)) == NULL) {
    goto Find_Host;
        //perror ("gethostbyname");
        //exit (1);
    }

  //! Create socket
  client_socket = rsi_socket_async(AF_INET, SOCK_DGRAM, 0, receive_callback);
  if(client_socket < 0)
  {
    status = rsi_wlan_get_status();
    //return status;
  }

  //! Set server structure
  memset(&server_addr, 0, sizeof(server_addr));

  //! Set server address family
  server_addr.sin_family = AF_INET;

  //! Set server port number, using htons function to use proper byte order
  server_addr.sin_port = htons(SERVER_PORT);
Find_Host:

server_addr.sin_addr = * ((struct in_addr *) he-> h_addr_list[0]);
  return 0;
}


int main()
{
int i;
  int32_t status;
array_sending sender;
uint8_t testing [1024];
char string_test [100];
	uint32_t addr_size = sizeof(server_addr);
//----------------------------------SPI init---------------------------------------------------------
ARM_DRIVER_SPI* SPIdrv = &Driver_SSI_MASTER;
  SystemCoreClockUpdate();
//uint8_t send_buffer[SEND_LENGTH];

//initialize spi transfer data
for(i=0;i<BUFFER_SIZE;i++)
  {
     testdata_out[i]=0;
		send_dummy_to_slave[i] = 1;
		master_out_ACK[i] = 0x79;
  }
SPI_MEM_MAP_PLL(INTF_PLL_500_CTRL_REG9) = 0xD900;  
  RSI_CLK_SetIntfPllFreq(M4CLK,180000000,40000000);
 
  /*Configure m4 soc to 180mhz*/
  RSI_CLK_M4SocClkConfig(M4CLK,M4_INTFPLLCLK,0);

  /*configure socpll to 20mhz*/  
  RSI_CLK_SocPllLockConfig(1,1,0xA4);
  RSI_CLK_SetSocPllFreq(M4CLK,20000000,40000000);

  /* Initialize the SPI driver */
SPIdrv->Initialize(mySPI_callback);

  /* Power up the SPI peripheral */
SPIdrv->PowerControl(ARM_POWER_FULL);
 
/* Configure the SPI to Master, 16-bit mode @10000 kBits/sec */
SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_SS_MASTER_HW_OUTPUT | ARM_SPI_DATA_BITS(SPI_BIT_WIDTH), SPI_BAUD);
  Set_Up_INT();
//-------------------------------SPI Init--------------------------------------------------------------

//------------------------------getting initializations------------------------------------------------
SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);

SPIdrv->Transfer(testdata_out, testdata_in, BUFFER_SIZE);

/* Waits until spi_done=0 */
while (!spi_done){
}
spi_done = 0;

/* SS line = ACTIVE = LOW */
SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
memcpy(testing, (uint8_t*) testdata_in, 1024);
spi_decode(testing, wifi_setup);
memcpy(string_test, (uint8_t*)wifi_setup->psk, sizeof(wifi_setup->psk));

//-------------------------------getting initialization-------------------------------------------------

#ifdef RSI_WITH_OS
 
  rsi_task_handle_t wlan_task_handle = NULL;

  rsi_task_handle_t driver_task_handle = NULL;
#endif
#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif
//RSI_Board_Init();
  //! Driver initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if((status < 0) || (status > GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! Redpine module intialisation
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }


#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_udp_client, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call UDP client application
  status = rsi_udp_client();
  //-------------------------------------------------------------
  //temperory buffer content, will be changed after SPI is implemented
sender.bot_id = 1;
for (i = 1; i < 10 ;i++){
sender.send_buffer[i] = 30;
  }
  for (i = 10; i < 46 ;i++){
sender.send_buffer[i] = 5;
  }

  sender.send_buffer[46] = 100;
  sender.send_buffer[47] = 100;
//--------------------------------------------------------------
status = RSI_bsd_sendto(client_socket, &sender, (sizeof(sender)), 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
status = RSI_bsd_sendto(client_socket, &sender, (sizeof(sender)), 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
status = RSI_bsd_sendto(client_socket, &sender, (sizeof(sender)), 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
  while(1)
  {
	// Server sends user code in binary form via Wi-Fi
		Recieve_upgrade:
			receive_callback(client_socket, send_to_slave, BUFFER_SIZE);
		
		// SPI sends recieved binary upgrade to STM32
		Send_pack:
			// Send byte of data
			if(spi_event){
				spi_event = false;
				SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE); 
			
				//SPIdrv->Transfer(testdata_out, data_in, BUFFER_SIZE);			
				SPIdrv->Transfer(send_to_slave, slave_in_dummy, BUFFER_SIZE);		// Send data packs from firmware upgrade bin file through spi while recieving dummy
			
			/* Waits until spi_done=0 */
				while (!spi_done){
				}
				spi_done = 0;
		
		
		//if (pack_received == 1){

			//pack_received = 0;
			
			memcpy(testdata_out, SPI_buff, pack_length);
			if(spi_event){
				spi_event = false;
			//	SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE); 
			
				//SPIdrv->Transfer(testdata_out, data_in, BUFFER_SIZE);			
				SPIdrv->Transfer(testdata_out, slave_in_dummy, BUFFER_SIZE);		// Send 0s to slave and recieve dummy data from slave
			
			/* Waits until spi_done=0 */
				while (!spi_done){
				}
				spi_done = 0;
				
				/*STM FW upgrade code*/
		Send_dummy:		
				//Send dummy to slave and recieve data from slave. Slave sends ack or nack signals back.
				SPIdrv->Transfer(send_dummy_to_slave, slave_in_data, BUFFER_SIZE);
				while (!spi_done){
				}
				spi_done = 0;
				
				for (i = 0; i < BUFFER_SIZE; i++) {
					if (slave_in_data[i] == 0x79) {
						// ACK
						SPIdrv->Transfer(master_out_ACK, slave_in_dummy, BUFFER_SIZE);
						while (!spi_done){
						}
						spi_done = 0;
						goto Send_pack;
					}
					else {
						if (slave_in_data[i] == 0x1F) {
							//NACK
							SPIdrv->Transfer(master_out_ACK, slave_in_dummy, BUFFER_SIZE);
							while (!spi_done){
							}
							spi_done = 0;
							goto Send_pack;
						}
						else
							goto Send_dummy;
					}
				}
				
				
				/* SS line = ACTIVE = LOW; */
				SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
				rsi_delay_ms(10); 

  }
#endif
rsi_shutdown(client_socket, 0);
  return 0;

		}
	}
}