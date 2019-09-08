
/**
* @file     linker_script_icache_qspi_all_coex_9118_wc.x 
* @version  1.0
* @date     03 Sep 2015
*
*  Copyright(C) Redpine Signals 2015
*  All rights reserved by Redpine Signals.
*
*  @section License
*  This program should be used on your own responsibility.
*  Redpine Signals assumes no responsibility for any losses
*  incurred by customers or third parties arising from the use of this file.
*
*/

/*Linker script to be used with toolchain version 3.1.1 [byte mode toolchain] for normal executables */
OUTPUT_ARCH(rpine)
OUTPUT_FORMAT("elf32-rpine")

MEMORY
{ 
   /* RAM */
   /* NOTE : Memory used by Bootloader for USB Stack and DQH 111k to 128k    */
   /* vector table from 0x0 to 0x8c */
   ant_bss         : o = 240k,  l = 2k
   ant_data        : o = 242k,  l = 14k
   qspi_vma        : o = 0x00400000 + 68k + 64 + 54k + 1123k + 218k + 100k,   l = 90k
}

SECTIONS
{

    __common_tcm_start = .;
 
  .bss :
  {
      __bss_start = .;
      *(.bss)
      __heap_start = .;
      __heap_end = .;
      _end = .;
  } > ant_bss

  .qspi_code :AT (0x04000000 + 68k + 64 + 54k + 1123k + 218k + 100k)
  {
      *(.text)
  } > qspi_vma  
  

  .sc_proto_tcm :AT (0x04000000 + 68k + 64 + 54k + 1123k + 218k + 190k)
  {
      *(.ant_data)
      *(.data)
      *(.rodata)
  } > ant_data  

}
