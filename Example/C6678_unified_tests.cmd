/****************************************************************************/
/*  C6678_unified.cmd                                                       */
/*  Copyright (c) 2011 Texas Instruments Incorporated                       */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an C6678              */
/*                 device.  Use it as a guideline.  You will want to        */
/*                 change the memory layout to match your specific C6xxx    */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

-stack 0xFA00
-heap  0xFA00

SECTIONS
{
    .text          >  MSMCSRAM
    .stack         >  MSMCSRAM
    .bss           >  MSMCSRAM
    .cio           >  MSMCSRAM
    .const         >  MSMCSRAM
    .data          >  MSMCSRAM
    .switch        >  MSMCSRAM
    .sysmem        >  MSMCSRAM
    .far           >  MSMCSRAM
    .args          >  MSMCSRAM
    .ppinfo        >  MSMCSRAM
    .ppdata        >  MSMCSRAM
  
    /* COFF sections */
    .pinit         >  MSMCSRAM
    .cinit         >  MSMCSRAM
  
    /* EABI sections */
    .binit         >  MSMCSRAM
    .init_array    >  MSMCSRAM
    .neardata      >  MSMCSRAM
    .fardata       >  MSMCSRAM
    .rodata        >  MSMCSRAM
    .c6xabi.exidx  >  MSMCSRAM
    .c6xabi.extab  >  MSMCSRAM


    .shared_section           > MSMCSRAM
    .shared_section_1 	      > MSMCSRAM
    .shared_section_2         > MSMCSRAM
    .shared_section_3         > MSMCSRAM
    .shared_section_4         > MSMCSRAM
    .shared_section_5         > MSMCSRAM
    .shared_section_6         > MSMCSRAM
    .codesection           	  > MSMCSRAM
    .shared_code_section      > MSMCSRAM
	.local_data               > L2SRAM
	.local_data: load >> L2SRAM

    /* Custom Sections */
	.ipc_nocache_section     > MSMCSRAM_NC_VIRT
	.ipc_local_data          > L2SRAM
    .ipc_shared_section   	 > MSMCSRAM
    //.ipc_shared_section_1 	 > MSMCSRAM
    //.ipc_shared_section_2    > MSMCSRAM
    //.ipc_shared_section_3    > MSMCSRAM
    //.ipc_shared_section_4    > MSMCSRAM
    //.ipc_shared_section_5    > MSMCSRAM
    //.ipc_shared_section_6    > MSMCSRAM
}
