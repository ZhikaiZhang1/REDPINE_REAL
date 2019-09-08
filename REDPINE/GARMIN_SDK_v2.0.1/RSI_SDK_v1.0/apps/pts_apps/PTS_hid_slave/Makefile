#Makefile

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_hid_slave
LINKS= -lpthread -L../../../sapis/build/ -lrps
PATH_CONFIG=$(shell pwd)

RSI_DRIVER_PATH = ../../../sapis/include
RSI_ROM_PATH = ../../../sapis/rom
CFLAGS+=  -I$(RSI_DRIVER_PATH)/
CFLAGS+=  -I$(RSI_ROM_PATH)/
CFLAGS+=  -I$(PATH_CONFIG)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_BT_ENABLE

BT_HID_SLAVE_APP = rsi_hid_slave.o 

all: $(BT_HID_SLAVE_APP) 

	make bt_classic CC=$(CC) -C ../../../sapis/build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(BT_HID_SLAVE_APP) core *.o *~
