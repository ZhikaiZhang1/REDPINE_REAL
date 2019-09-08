# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -lpthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)

RSI_DRIVER_PATH = ../../../include

CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+=  -I$(INCLUDE_PATH)/
CFLAGS+=  -I$(PATH_CONFIG)/
CFLAGS+= -DRSI_SAMPLE_HAL
CFLAGS+= -DRSI_BT_ENABLE
CFLAGS+= -DRSI_BLE_ENABLE\
         -I ../../../rom/

WLAN_AP_BLE_BRIDGE_APP = main.o \
                      rsi_wlan_ap_app.o \
                      rsi_ble_app.o  \
											rsi_bt_app.o 

all: $(WLAN_AP_BLE_BRIDGE_APP) 

	make wlan_dual_mode CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(WLAN_AP_BLE_BRIDGE_APP) core *.o *~ rsi_wc_app
