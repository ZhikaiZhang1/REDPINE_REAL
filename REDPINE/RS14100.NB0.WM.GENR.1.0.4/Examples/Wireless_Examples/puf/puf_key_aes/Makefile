# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -lpthread -L../../../build/ -lrps
RSI_DRIVER_PATH = ../../../include
RSI_APP_PATH = ../../../puf/
PATH_CONFIG=$(shell pwd)

CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+= -I$(RSI_APP_PATH)/
CFLAGS+=  -I$(INCLUDE_PATH)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_ENABLE_DEBUG_PRINT #Enable to get prints in LINUX
PUF_APP = rsi_puf_key_aes_app.o 

all: $(PUF_APP) 

	make puf CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  

clean:
	$(rm) $(PUF_APP) core *.o *~ rsi_wc_app 
