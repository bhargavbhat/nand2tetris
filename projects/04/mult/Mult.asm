// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

@result
M=0         //result=0
@i
M=1         //i=1

(LOOP)
    @i    
    D=M     //D=i
    @R0
    D=D-M   //D=i-R0
    @END
    D;JGT   // if(i-R0) > 0, we're done
    @R1
    D=M     //D=R1
    @result
    M=D+M   //result+=R1
    @i
    M=M+1   //i=i+1
    @LOOP
    0;JMP   //repeat

(END)
    @result
    D=M
    @R2
    M=D     //put result into RAM[2]

(TERM)
    @TERM
    0;JMP
