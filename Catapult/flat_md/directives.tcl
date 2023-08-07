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
directive set -CLOCKS {clk {-CLOCK_PERIOD 15}}
go memories
directive set SCHED_USE_MULTICYCLE true
go assembly
# Error: ntt.cpp(46):   MEMORYWRITE "butterFly:write_mem(vec:rsc.@)" ntt.cpp(46,4,9) (BASIC-25)
# Error: ntt.cpp(47):   MEMORYWRITE "butterFly:write_mem(vec:rsc(0)(0).@)#1" ntt.cpp(47,4,9) (BASIC-25)
# Error: ntt.cpp(44):   MEMORYREAD "butterFly:f1:read_mem(vec:rsc(0)(0).@)" ntt.cpp(44,19,9) (BASIC-25)
# Error: ntt.cpp(44):   MEMORYREAD "butterFly:f1:read_mem(vec:rsc.@)" ntt.cpp(44,19,9) (BASIC-25)
# Error: ntt.cpp(47):   MEMORYWRITE "butterFly:write_mem(result:rsc.@)#1" ntt.cpp(47,4,14) (BASIC-25)
# Error: ntt.cpp(44):   MEMORYREAD "butterFly:f1:read_mem(result:rsc.@)" ntt.cpp(44,19,12) (BASIC-25)


ignore_memory_precedences -from  butterFly:write_mem(result:rsc.@)* -to butterFly:f1:read_mem(result:rsc.@)*
ignore_memory_precedences -from  butterFly:write_mem(result:rsc.@)* -to butterFly:f2:read_mem(result:rsc.@)*

go extract

