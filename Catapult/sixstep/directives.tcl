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
directive set SCHED_USE_MULTICYCLE true
go assembly
go memories
go architect
# Error: ntt.cpp(53):   MEMORYWRITE "butterFly:write_mem(xx:rsc(0)(0).@)#1" ntt.cpp(53,4,9) (BASIC-25)
# Error: ntt.cpp(82):   MEMORYWRITE "S2_OUTER_LOOP:for:for:write_mem(xx:rsc(0)(0).@)#1" ntt.cpp(82,20,11) (BASIC-25)
# Error: ntt.cpp(81):   MEMORYWRITE "S2_OUTER_LOOP:for:for:write_mem(xx:rsc(0)(0).@)" ntt.cpp(81,20,11) (BASIC-25)
# Error: ac_int.h(2815):   MEMORYREAD "operator*<64,false>:read_mem(yy:rsc(0)(0).@)" ac_int.h(2815,0,1) (BASIC-25)
# Error: ntt.cpp(91):   MEMORYWRITE "S2_COPY_LOOP:for:write_mem(yy:rsc(0)(0).@)#1" ntt.cpp(91,12,16) (BASIC-25)
# Error: ntt.cpp(61):   MEMORYWRITE "S1_OUTER_LOOP:for:write_mem(xx:rsc(0)(0).@)#1" ntt.cpp(61,12,8) (BASIC-25)
ignore_memory_precedences -from butterFly*:write_mem(xx:rsc(*)(0).@)* -to butterFly*:write_mem(xx:rsc(*)(0).@)*



ignore_memory_precedences -from S1_OUTER_LOOP:for:write_mem(xx:rsc(*)(0).@)* -to S1_OUTER_LOOP:for:write_mem(xx:rsc(*)(0).@)* 
ignore_memory_precedences -from S2_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S2_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from S5_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S5_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from S34_OUTER_LOOP:for:write_mem(xx:rsc(*)(0).@)* -to operator*<64,false>*:read_mem(yy:rsc(*)(0).@)*
ignore_memory_precedences -from S2_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S2_OUTER_LOOP:for:for:f1:read_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from S2_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S2_OUTER_LOOP:for:for:f2:read_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from S5_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S5_OUTER_LOOP:for:for:f1:read_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from S5_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S5_OUTER_LOOP:for:for:f2:read_mem(xx:rsc(*)(0).@)*
go allocate
go extract

go memories
directive set SCHED_USE_MULTICYCLE true

ignore_memory_precedences -from S5_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S5_OUTER_LOOP:for:for:f1:read_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from S5_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S5_OUTER_LOOP:for:for:f2:read_mem(xx:rsc(*)(0).@)*
# Error: ntt.cpp(82):   MEMORYWRITE "S2_OUTER_LOOP:for:for:write_mem(xx:rsc(0)(0).@)#1" ntt.cpp(82,16,11) (BASIC-25)
ignore_memory_precedences -from S2_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S2_OUTER_LOOP:for:for:f1:read_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from S2_OUTER_LOOP:for:for:write_mem(xx:rsc(*)(0).@)* -to S2_OUTER_LOOP:for:for:f2:read_mem(xx:rsc(*)(0).@)*
ignore_memory_precedences -from S5_OUTER_LOOP:for:for:write_mem(yy:rsc(*)(0).@)* -to S5_OUTER_LOOP:for:for:f1:read_mem(yy:rsc(*)(0).@)*
ignore_memory_precedences -from S5_OUTER_LOOP:for:for:write_mem(yy:rsc(*)(0).@)* -to S5_OUTER_LOOP:for:for:f2:read_mem(yy:rsc(*)(0).@)*
