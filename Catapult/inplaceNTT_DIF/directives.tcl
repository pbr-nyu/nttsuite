set sfd [file dirname [info script]]
solution new -state initial
solution options defaults

option set Input/TargetPlatform x86_64
option set Input/CppStandard c++11

solution file add ${sfd}/src/ntt_tb.cpp -exclude true
solution file add ${sfd}/src/ntt.cpp
go analyze

go compile

solution library add mgc_Xilinx-VIRTEX-uplus-3_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family VIRTEX-uplus -speed -3 -part xcvu13p-flga2577-3-e
solution library add Xilinx_RAMS
go libraries

directive set DSP_EXTRACTION yes
directive set -CLOCKS {clk {-CLOCK_PERIOD 10}}
go assembly

go memories
directive set SCHED_USE_MULTICYCLE true

go extract


