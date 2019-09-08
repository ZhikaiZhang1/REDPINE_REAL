
#
# @file     start_wol.sh
# @version  1.0
# @date     03 Sep 2015
#
#  Copyright(C) Redpine Signals 2015
#  All rights reserved by Redpine Signals.
#
#  @section License
#  This program should be used on your own responsibility.
#  Redpine Signals assumes no responsibility for any losses
#  incurred by customers or third parties arising from the use of this file.
#
#  @brief : This file contains BLE GAP API's which needs to be called from application
#
#  @section Description  This file contains BLE GAP API's called from application
#

#export LD_LIBRARY_PATH=/tools/eda/ToolChain/gnu-ta-2.4.5/lib/gdb
#/tools/eda/ToolChain/gnu-ta-2.4.5/bin/rpine-ta-elf32-gdb -x ta_wol ./onebox_fw*

#export LD_LIBRARY_PATH=/work/satish/gnu-ta-2.4.6/lib/gdb
#/work/satish/gnu-ta-2.4.6/bin/rpine-ta-elf32-gdb -x ta_wol ./onebox_fw_qflash_noload

#export LD_LIBRARY_PATH=/work/siva/gnu-ta-2.4.5/lib/gdb
#/work/siva/gnu-ta-2.4.5/bin/rpine-ta-elf32-gdb -x ta_wol ./onebox*

export LD_LIBRARY_PATH=/backup/gnu-ta-2.4.5/lib/gdb
/backup/gnu-ta-2.4.5/bin/rpine-ta-elf32-gdb -x ta_wol ./ant*
