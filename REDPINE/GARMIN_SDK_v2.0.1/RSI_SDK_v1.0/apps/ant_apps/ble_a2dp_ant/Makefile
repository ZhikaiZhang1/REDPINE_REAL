#Makefile

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -lpthread -L../../../sapis/build/ -lrps
PATH_CONFIG=$(shell pwd)

RSI_DRIVER_PATH = ../../../sapis/include
RSI_ROM_PATH = ../../../sapis/rom
CFLAGS+= -I $(RSI_DRIVER_PATH)/
CFLAGS+= -I $(RSI_ROM_PATH)/
CFLAGS+= -I $(PATH_CONFIG)/

#CFLAGS+= -D RSI_DEBUG_PRINTS
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_BT_ENABLE
CFLAGS+= -D RSI_BLE_ENABLE
CFLAGS+= -D RSI_UART_FLOW_CTRL_ENABLE
#CFLAGS+= -D BLE_ENABLE
#CFLAGS+= -D BLE_CENTRAL

BT_A2DP_SOURCE_AVRCP_APP =	rsi_a2dp_source_avrcp.o \
												 		rsi_ble_app.o \
														rsi_ant.o \
														rsi_ble_central.o \
														main.o \

all: clean $(BT_A2DP_SOURCE_AVRCP_APP) 

	make wlan_dual_mode CC=$(CC) -C ../../../sapis/build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) $(BT_A2DP_SOURCE_AVRCP_APP) $(LINKS) -o $(PROGNAME) 
	rm -f  *.o   

clean:
	$(rm) $(BT_A2DP_SOURCE_AVRCP_APP) core *.o *~ rsi_wc_app
