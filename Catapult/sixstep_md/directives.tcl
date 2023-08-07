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
# Error: ntt.cpp(52):   MEMORYWRITE "butterFly#1:write_mem(xx:rsc(0)(0).@)#1" ntt.cpp(52,4,9) (BASIC-25)
# Error: ntt.cpp(48):   MEMORYREAD "butterFly#1:f1:read_mem(xx:rsc(0)(0).@)" ntt.cpp(48,19,9) (BASIC-25)
ignore_memory_precedences -from butterFly*:write_mem(xx:rsc(*)(0).@)* -to butterFly*:f1:read_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from butterFly*:write_mem(xx:rsc(*)(0).@)* -to butterFly*:f2:read_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from butterFly*:write_mem(xx:rsc(*)(0).@)* -to butterFly*:write_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from /DIT_FLAT/core/COMP_LOOP:write_mem* -to /DIT_FLAT/core/COMP_LOOP:write_mem*
ignore_memory_precedences -from /DIT_FLAT/core/COMP_LOOP:write_mem(result:rsc.@)#1 -to /DIT_FLAT/core/COMP_LOOP:write_mem(result:rsc.@)


ignore_memory_precedences -from butterFly#1:write_mem(xx:rsc(*)(0).@)* -to butterFly#1:f1:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#2:write_mem(xx:rsc(*)(0).@)* -to butterFly#2:f1:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#3:write_mem(xx:rsc(*)(0).@)* -to butterFly#3:f1:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#4:write_mem(xx:rsc(*)(0).@)* -to butterFly#4:f1:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#5:write_mem(xx:rsc(*)(0).@)* -to butterFly#5:f1:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#6:write_mem(xx:rsc(*)(0).@)* -to butterFly#6:f1:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#7:write_mem(xx:rsc(*)(0).@)* -to butterFly#7:f1:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#8:write_mem(xx:rsc(*)(0).@)* -to butterFly#8:f1:read_mem(xx:rsc(*)(0).@)

ignore_memory_precedences -from butterFly#1:write_mem(xx:rsc(*)(0).@)* -to butterFly#1:f2:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#2:write_mem(xx:rsc(*)(0).@)* -to butterFly#2:f2:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#3:write_mem(xx:rsc(*)(0).@)* -to butterFly#3:f2:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#4:write_mem(xx:rsc(*)(0).@)* -to butterFly#4:f2:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#5:write_mem(xx:rsc(*)(0).@)* -to butterFly#5:f2:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#6:write_mem(xx:rsc(*)(0).@)* -to butterFly#6:f2:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#7:write_mem(xx:rsc(*)(0).@)* -to butterFly#7:f2:read_mem(xx:rsc(*)(0).@)
ignore_memory_precedences -from butterFly#8:write_mem(xx:rsc(*)(0).@)* -to butterFly#8:f2:read_mem(xx:rsc(*)(0).@)

go extract

