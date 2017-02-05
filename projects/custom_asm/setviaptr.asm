// Set RAM[100] size 10 to -1 using pointers

@100
D=A
@arr
M=D         // initialize base addr of array

@10
D=A
@n
M=D         // initialize n with size of array

@i
M=0         // initialize loop iterator with 0

(LOOP)
    @i
    D=M
    @n
    D=D-M
    @END
    D;JEQ   // loop termination condition

    @arr
    D=M
    @i
    A=D+M   // load base(arr) + 1 into M
    M=-1    // set base(arr) + 1 to -1

    @i
    M=M+1   // increment loop counter

    @LOOP
    0;JMP   // repeat

(END)
    @END
    0;JMP   // Program termination


