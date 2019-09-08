#
# @file     Makefile
# @version  1.0
# @date     Nov 2018
#
#  Copyright(C) Redpine Signals 2015
#  All rights reserved by Redpine Signals.
#
#  @section License
#  This program should be used on your own responsibility.
#  Redpine Signals assumes no responsibility for any losses
#  incurred by customers or third parties arising from the use of this file.
#

#USE tool chain versions after 2.4.5

TOOLCHAIN_BASE_DIR= /work/gnu-ta-2.4.5/bin
OC="/work/gnu-ta-2.4.5/bin/rpine-ta-elf32-objcopy"
TOOLCHAIN_PREFIX = rpine-ta-elf32-
LINKER_SCRIPT_BT_ROM_IMAGE := ./linker_script_icache_qspi_all_coex_9118_wc.x


CC=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)gcc
CCDEP=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)gcc
LD=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)ld
NM=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)nm
OD=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)objdump
OC=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)objcopy
AS=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)as
AR=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)ar
RANLIB=$(TOOLCHAIN_BASE_DIR)/$(TOOLCHAIN_PREFIX)ranlib
ARFLAGS=rcs

ANT_SRC_PATH = ./src
ANT_H_PATH	 := -I ./include/

CFLAGS = -Wall -g -c -fno-common -fno-inline



OBJS_ANT:= $(ANT_SRC_PATH)/vnd_rx_test_setup.c 

OBJS_ANT_H:= $(ANT_H_PATH)

#LINKER_SCRIPT_WLAN				 	= $(COMMON_SRC_PATH)/chip_dep/RS9113/cpu/linker_script_wlan.x





ant_stack: clean
	$(CC)  $(CFLAGS) $(ANT_H_PATH) $(OBJS_ANT)
	$(CC)  -T $(LINKER_SCRIPT_BT_ROM_IMAGE) $(ANT_H_PATH) *.o -o ant_fw_qflash
	$(NM) -n ant_fw_qflash > ant_fw_qflash.map
	$(OD) -S ant_fw_qflash > ant_fw_qflash.asm
	./convobj_ant_image.sh ant_fw_qflash
	rm -f dopcode.asm
	#rm *.o

clean:
	rm -f *.o ant_fw_qflash*
	rm -rf ant/*.o

