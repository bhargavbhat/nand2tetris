// Swap contents of RAM[0] and RAM[1]

@R1
D=M			// Load D with RAM[1]

@temp
M=D			// Load temp with D i.e. RAM[1]

@R0
D=M			// Load D with RAM[0]

@R1
M=D			// Load M i.e RAM[1] with D i.e RAM[0]

@temp
D=M			// Load D with temp i.e. RAM[1]

@R0
M=D			// Load M i.e. RAM[0] with D

(END)
	@END
	0;JMP	// Terminate Program

