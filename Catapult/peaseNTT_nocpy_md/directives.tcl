set sfd [file dirname [info script]]
solution new -state initial
solution options defaults

option set Input/TargetPlatform x86_64
option set Input/CppStandard c++98

solution file add ${sfd}/src/main.cpp -exclude true
solution file add ${sfd}/src/ntt.cpp
solution file add ${sfd}/src/utils.cpp
go analyze

go compile

solution library add mgc_Xilinx-VIRTEX-7-2_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family VIRTEX-7 -speed -2 -part xc7vx690tffq1761-2
solution library add Xilinx_RAMS
go libraries

directive set DSP_EXTRACTION yes
directive set -CLOCKS {clk {-CLOCK_PERIOD 15}}
go memories
directive set SCHED_USE_MULTICYCLE true
go assembly
ignore_memory_precedences -from /DIT_FLAT/core/COMP_LOOP:write_mem* -to /DIT_FLAT/core/COMP_LOOP:f1:read_mem*
ignore_memory_precedences -from /DIT_FLAT/core/COMP_LOOP:write_mem* -to /DIT_FLAT/core/COMP_LOOP:f2:read_mem*
ignore_memory_precedences -from /DIT_FLAT/core/COMP_LOOP:write_mem* -to /DIT_FLAT/core/COMP_LOOP:write_mem*
ignore_memory_precedences -from /DIT_FLAT/core/COMP_LOOP:write_mem(result:rsc.@)#1 -to /DIT_FLAT/core/COMP_LOOP:write_mem(result:rsc.@)

go extract

