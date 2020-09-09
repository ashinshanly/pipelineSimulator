# pipelineSimulator
To compile the program, execute the command "g++ -std=c++11 -o pipeline p1.cpp execution.cpp parser.cpp hazard_parser.cpp printer.cpp" 
To run, execute "./pipeline F input.txt" for extracting the output with forwarding and "./pipeline N input.txt" for the output without forwarding.

Note: 
The program computes till the 16th cycle, and outputs the contents of the registers as well as which stage the instruction is in each cycle for no-forwarding condition, and outputs the instruction set and contents of registers for the forwarding case.

The format for LOAD and STORE used in the program is "LOAD/STORE $t1,$t2,imm" which is equivalent to the instruction "LOAD/STORE $t1,imm($t2)".

For forwarding, the latch after EXECUTE stage i.e L3 is where the value is forwarded from. Here, the instruction displayed simply contains at the end, from which latch the value has been forwarded from, and does not display which source's value has been forwarded.
