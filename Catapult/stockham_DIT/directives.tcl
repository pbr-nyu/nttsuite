set sfd [file dirname [info script]]
solution new -state initial
solution options defaults

option set Input/TargetPlatform x86_64
option set Input/CppStandard c++11

solution file add ${sfd}/src/main.cpp -exclude true
solution file add ${sfd}/src/ntt.cpp
solution file add ${sfd}/src/utils.cpp
go analyze

go compile

solution library add mgc_Xilinx-VIRTEX-7-2_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family VIRTEX-7 -speed -2 -part xc7vx690tffq1761-2
solution library add Xilinx_RAMS
go libraries

directive set DSP_EXTRACTION yes
directive set -CLOCKS {clk {-CLOCK_PERIOD 10}}
go assembly

go extract

go memories
directive set SCHED_USE_MULTICYCLE true
# Error: ntt.cpp(52):   MEMORYWRITE "for:for:write_mem(xt:rsc(0)(0).@)#1" ntt.cpp(52,20,35) (BASIC-25)
# Error: ntt.cpp(49):   MEMORYREAD "for:for:a:read_mem(xt:rsc(0)(0).@)" ntt.cpp(49,40,19) (BASIC-25)
ignore_memory_precedences -from for:for:write_mem(xt:rsc(0)(*).@)* -to for:for:a:read_mem(xt:rsc(0)(*).@)*
ignore_memory_precedences -from for:for:write_mem(xt:rsc(0)(*).@)* -to for:for:b:read_mem(xt:rsc(0)(*).@)*
ignore_memory_precedences -from for:for:write_mem(xt:rsc(0)(*).@)* -to for:for:write_mem(xt:rsc(0)(*).@)* 
ignore_memory_precedences -from for:for:write_mem(yt:rsc(0)(*).@)* -to for:for:a:read_mem(yt:rsc(0)(*).@)*
ignore_memory_precedences -from for:for:write_mem(yt:rsc(0)(*).@)* -to for:for:b:read_mem(yt:rsc(0)(*).@)*
ignore_memory_precedences -from for:for:write_mem(yt:rsc(0)(*).@)* -to for:for:write_mem(yt:rsc(0)(*).@)* 