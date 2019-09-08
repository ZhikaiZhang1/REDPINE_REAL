#Makefile

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -lpthread -L../../../build/ -lrps
PULSE_AUDIO_LINKS= -lpthread -L/usr/lib/ -lpulse-simple
PATH_CONFIG=$(shell pwd)

RSI_DRIVER_PATH = ../../../include
RSI_SBC_PATH = ./SBC/sbc_header
RSI_ROM_PATH = ../../../rom
PULSE_AUDIO_PATH = /work/mallikarjuna/softwares/pulseaudio-11.1/src
CFLAGS+= -I $(RSI_DRIVER_PATH)/
CFLAGS+= -I $(RSI_SBC_PATH)/
CFLAGS+= -I $(RSI_ROM_PATH)/
CFLAGS+= -I $(PULSE_AUDIO_PATH)/
CFLAGS+= -I $(PATH_CONFIG)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_BT_ENABLE

BT_A2DP_SLAVE_APP = rsi_a2dp_sink.o \

all: clean $(BT_A2DP_SLAVE_APP) 

	make bt_classic CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) $(BT_A2DP_SLAVE_APP) $(LINKS) $(PULSE_AUDIO_LINKS) -o $(PROGNAME) 
	rm -f  *.o   


clean:
	$(rm) $(BT_A2DP_SLAVE_APP) core *.o *~
