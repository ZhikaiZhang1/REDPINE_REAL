#Makefile

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../sapis/build/ -lrps
PATH_CONFIG=$(shell pwd)

RSI_DRIVER_PATH = ../../../sapis/include
RSI_ROM_PATH = ../../../sapis/rom
RSI_SBC_PATH = ../../../sapis/bt_ble/
PATH_LIB= -L$(RSI_SBC_PATH) -lRSI_SBC_LNX
CFLAGS+= -I $(RSI_DRIVER_PATH)/
CFLAGS+= -I $(RSI_ROM_PATH)/
CFLAGS+= -I $(PATH_CONFIG)/
CFLAGS+= -I $(RSI_SBC_PATH)/
#CFLAGS+= -D RSI_UART_FLOW_CTRL_ENABLE
#CFLAGS+= -D RSI_DEBUG_PRINTS
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_BT_ENABLE
#CFLAGS+= -g

BT_A2DP_SOURCE_AVRCP_APP = rsi_a2dp_source_avrcp_pts.o

all: clean $(BT_A2DP_SOURCE_AVRCP_APP) 

	make bt_classic CC=$(CC) -C ../../../sapis/build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) $(BT_A2DP_SOURCE_AVRCP_APP) $(LINKS) $(PATH_LIB) -o $(PROGNAME) 
	rm -f  *.o   

clean:
	$(rm) $(BT_A2DP_SOURCE_AVRCP_APP) core *.o *~
