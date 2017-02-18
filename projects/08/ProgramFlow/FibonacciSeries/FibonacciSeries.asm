//BEG : FibonacciSeries
// push argument 1
@ARG
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop pointer 1           
@SP
AM=M-1
D=M
@R4
M=D
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop that 0              
@THAT
D=M
@0
D=D+A
@3500
M=D
@SP
AM=M-1
D=M
@3500
A=M
M=D
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop that 1              
@THAT
D=M
@1
D=D+A
@3500
M=D
@SP
AM=M-1
D=M
@3500
A=M
M=D
// push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
AM=M-1
D=M
M=0
A=A-1
M=M-D
// pop argument 0          
@ARG
D=M
@0
D=D+A
@3500
M=D
@SP
AM=M-1
D=M
@3500
A=M
M=D
// label MAIN_LOOP_START
(FibonacciSeries$MAIN_LOOP_START)
// push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@JG.T0
D;JGT
D=0
@JG.F0
0;JMP
(JG.T0)
D=-1
(JG.F0)
@SP
A=M
M=D
@SP
M=M+1
// if-goto COMPUTE_ELEMENT 
@SP
AM=M-1
D=M
@FibonacciSeries$COMPUTE_ELEMENT
D;JNE
// goto END_PROGRAM        
@FibonacciSeries$END_PROGRAM
0;JMP
// label COMPUTE_ELEMENT
(FibonacciSeries$COMPUTE_ELEMENT)
// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
// push that 1
@THAT
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
AM=M-1
D=M
M=0
A=A-1
M=M+D
// pop that 2              
@THAT
D=M
@2
D=D+A
@3500
M=D
@SP
AM=M-1
D=M
@3500
A=M
M=D
// push pointer 1
@R4
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
AM=M-1
D=M
M=0
A=A-1
M=M+D
// pop pointer 1           
@SP
AM=M-1
D=M
@R4
M=D
// push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
AM=M-1
D=M
M=0
A=A-1
M=M-D
// pop argument 0          
@ARG
D=M
@0
D=D+A
@3500
M=D
@SP
AM=M-1
D=M
@3500
A=M
M=D
// goto MAIN_LOOP_START
@FibonacciSeries$MAIN_LOOP_START
0;JMP
// label END_PROGRAM
(FibonacciSeries$END_PROGRAM)
