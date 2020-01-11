#include <stdlib.h>
#include <stdio.h>
#include "spi_decode.h"
#include "rsi_data_types.h"
#include <string.h>

#define DATASIZE 1024


void spi_decode(uint8_t* data_in, wifi_struct* wifi_setup){
	int i = 0;
	int j;
	uint16_t command, length;
	
	
	while(i < DATASIZE){
		if (data_in[i] == 'c'){
			if (data_in[i+1] == '1'){
				wifi_setup->ssid = (char*)malloc(data_in[i+2]);
				memcpy(wifi_setup->ssid, data_in + i + 3, data_in[i+2]);
				i = i + data_in[i+2] + 3;
			}
			else if (data_in[i+1] == '2'){
				wifi_setup->psk = (char*)malloc(data_in[i+2]);
				memcpy(wifi_setup->psk, data_in + i + 3, data_in[i+2]);
				i = i + data_in[i+2] + 3;
			}
			else if (data_in[i+1] == '3'){
				wifi_setup->host_name = (char*)malloc(data_in[i+2]);
				memcpy(wifi_setup->host_name, data_in + i + 3, data_in[i+2]);
				i = i + data_in[i+2] + 3;
			}
			
		}
		else{
				break;
			}
		
	}
	//for (i = 0; i < DATASIZE; i++){
		
	//}

}
