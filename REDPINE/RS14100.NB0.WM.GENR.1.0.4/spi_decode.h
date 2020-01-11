#ifndef SPI_DECODE_H
#define SPI_DECODE_H
#include <stdlib.h>
#include <stdio.h>
#include "rsi_data_types.h"

typedef struct{
	char* ssid;
	char* psk;
	char* host_name;
}wifi_struct;

void spi_decode(uint8_t* data_in, wifi_struct* wifi_setup);
#endif