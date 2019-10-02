#
_XDCBUILDCOUNT = 1
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/idris/ti/libarch_1_0_0_0/packages;/home/idris/ti/mathlib_c66x_3_1_2_1/packages;/home/idris/ti/linalg_1_2_0_0/packages;/home/idris/ti/dsplib_c66x_3_4_0_0/packages;/home/idris/ti/xdais_7_24_00_04/packages;/home/idris/ti/xdais_7_24_00_04/examples;/home/idris/ti/ctoolslib_2_2_0_0/packages;/home/idris/ti/ipc_1_24_03_32/packages;/home/idris/ti/bios_6_73_00_12/packages;/home/idris/ti/ndk_3_40_01_01/packages;/home/idris/ti/edma3_lld_2_12_05_30C/packages;/home/idris/ti/openmp_dsp_c667x_2_06_02_01/packages;/home/idris/ti/pdk_c667x_2_0_11/packages;/home/idris/ti/imglib_c66x_3_1_1_0/packages;/home/idris/ti/framework_components_3_40_02_07/packages;/home/idris/ti/framework_components_3_40_02_07/examples;/home/idris/ti/ipc_3_50_02_02/packages;/home/idris/ti/uia_2_30_01_02/packages;/home/idris/ti/xdctools_3_50_08_24_core/packages
override XDCROOT = /home/idris/ti/xdctools_3_50_08_24_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/idris/ti/libarch_1_0_0_0/packages;/home/idris/ti/mathlib_c66x_3_1_2_1/packages;/home/idris/ti/linalg_1_2_0_0/packages;/home/idris/ti/dsplib_c66x_3_4_0_0/packages;/home/idris/ti/xdais_7_24_00_04/packages;/home/idris/ti/xdais_7_24_00_04/examples;/home/idris/ti/ctoolslib_2_2_0_0/packages;/home/idris/ti/ipc_1_24_03_32/packages;/home/idris/ti/bios_6_73_00_12/packages;/home/idris/ti/ndk_3_40_01_01/packages;/home/idris/ti/edma3_lld_2_12_05_30C/packages;/home/idris/ti/openmp_dsp_c667x_2_06_02_01/packages;/home/idris/ti/pdk_c667x_2_0_11/packages;/home/idris/ti/imglib_c66x_3_1_1_0/packages;/home/idris/ti/framework_components_3_40_02_07/packages;/home/idris/ti/framework_components_3_40_02_07/examples;/home/idris/ti/ipc_3_50_02_02/packages;/home/idris/ti/uia_2_30_01_02/packages;/home/idris/ti/xdctools_3_50_08_24_core/packages;/home/idris/ti/xdctools_3_50_08_24_core/packages;..
HOSTOS = Linux
endif
