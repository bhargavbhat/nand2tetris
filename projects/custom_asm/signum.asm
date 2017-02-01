// Set RAM[1] = 1 if RAM[0] > 0

@R0
D=M			// Load RAM[0] into D

@POS		
D;JGT		// JMP to Label POS if D > 0

@R1	
M=0			// Set RAM[1] = 0 since D i.e. RAM[0] <= 0

@END
0;JMP		// Terminate Program

(POS)
	@R1
	M=1		// Set RAM[1] = 1 since D i.e RAM[0] > 0

(END)
	@END
	0;JMP	// Terminate Program


