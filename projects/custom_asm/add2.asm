// Compute RAM[2] = RAM[1] + RAM[0]

@0
D=M		// D = RAM[0]

@1
D=D+M	// D = D + RAM[1]

@2
M=D		// RAM[2] = D

@6
0;JMP	// Terminate
