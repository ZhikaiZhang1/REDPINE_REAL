
#
# @file     convobj_ant_image.sh
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
#

#!/bin/bash
SCRIPTS=./escripts
start_addr=69632
TA_BOOTLOADER=../../TA_Bootloader_0.8.8.6
TOOLCHAIN_BASE_DIR=/work/gnu-ta-2.4.5/bin
OC="/work/gnu-ta-2.4.5/bin/rpine-ta-elf32-objcopy"

if [ -f $1 ] ; then

echo "############################################################################################"
echo "###########PLEASE Update boot descriptor when linker or code is changed in Icache.##########"
echo "############################################################################################"

$OC -O binary -j .qspi_code  $1 p1
dd if=p1 bs=1 seek=92160 count=0 of=p1  2> /dev/null

$OC -O binary -j .bss $1 p2
dd if=p2 bs=1 seek=2048 count=0 of=p2  2> /dev/null

$OC -O binary -j .sc_proto_tcm $1 p3
dd if=p3 bs=1 seek=14336 count=0 of=p3  2> /dev/null
#dd if=p2 bs=1 seek=16384 count=0 of=p2  2> /dev/null

#cat p1 p2 p3 p4 p5 > pmemdata_qspi
#cat p1 p2 p3 p4 p5 p6 p7 p8 > pmemdata_qspi
cat rsi_pmemdata_qspi p1 p2 p3  > pmemdata_qspi

echo "Size of flash image is "
ls -l pmemdata_qspi | awk '{print $5;}'

cat pmemdata_qspi | od -v -w4 -tx4 | awk '{print $2;}' |  sed -e '/^$/d' > pmemdata_qspi_hex


vi -s $SCRIPTS/append_comma_prepend_0x.tcl pmemdata_qspi_hex
#calculate checksum and prepare header
gcc -I ./ -I $SCRIPTS/ $SCRIPTS/prepare_image_header_9116.c $SCRIPTS/crc_calc.c -o prepare_header
#gcc -I ./ -I../../common/lib/include/ $SCRIPTS/prepare_image_header.c ../../common/lib/checksum_onescompliment/checksum_onescompliment.c -o prepare_header
./prepare_header $start_addr 1 0 9 > header.h ##giving firmware start address as argument##
gcc -I ./ -c $SCRIPTS/image_header.c -o header 
objcopy  -O binary header -j .data RS9116_WC_SI.rps

#prepare fmc for flash image
#gcc -I ./  -I $TA_BOOTLOADER/include/  -I $TA_BOOTLOADER/include/rom_security -I $TA_BOOTLOADER/include/usb -I $TA_BOOTLOADER/build/aes_code/ $TA_BOOTLOADER/build/aes_code/prepare_flash_image.c $TA_BOOTLOADER/build/aes_code/crc_calc.c  $TA_BOOTLOADER/build/aes_code/aes.c $TA_BOOTLOADER/build/aes_code/misc.c  -o prepare_image
#./prepare_image $start_addr  > flash_image_hex ##giving firmware start address as argument##

#concatinate remaining binary to header 
#cat pmemdata_qspi >> RS9116_WC_SI.rps
cat RS9116_WC_SI.rps | od -v -w4 -tx4 | awk '{print $2;}' |  sed -e '/^$/d' > RS9116_WC_SI_hex


rm -f p2_1 p2_rom_patch p2_2 p2_3 p1 p2 p3 p4 p5 p6 p7 p8 header.h prepare_header prepare_image header pmemdata_qspi_hex pmemdata_qspi p1_hex p2_ram
fi
