set sfd [file dirname [info script]]

solution new -state initial
solution options defaults

option set Input/TargetPlatform x86_64
option set Input/CppStandard c++98

solution file add ${sfd}/src/main.cpp -exclude true
solution file add ${sfd}/src/ntt.cpp
solution file add ${sfd}/src/utils.cpp -exclude true
go analyze

go compile
directive set DSP_EXTRACTION yes
solution library add mgc_Xilinx-VIRTEX-7-2_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family VIRTEX-7 -speed -2 -part xc7vx690tffg1761-2
solution library add Xilinx_RAMS
go libraries


directive set -CLOCKS {clk {-CLOCK_PERIOD 10}}
go memories
directive set SCHED_USE_MULTICYCLE true
go assembly
go architect

directive set /inPlaceNTT_DIF_precomp/vec:rsc -MAP_TO_MODULE Xilinx_RAMS.BLOCK_DPRAM_RBW_DUAL
directive set /inPlaceNTT_DIF_precomp/twiddle:rsc -MAP_TO_MODULE Xilinx_RAMS.BLOCK_DPRAM_RBW_DUAL
directive set /inPlaceNTT_DIF_precomp/twiddle_h:rsc -MAP_TO_MODULE Xilinx_RAMS.BLOCK_DPRAM_RBW_DUAL
go extract

