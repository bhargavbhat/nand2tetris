// Compute RAM[1] = 1 + 2 + ... to RAM[0]

@R0
D=M
@n
M=D			// Set var n to the limit from RAM[0]

@i
M=1			// Set var i to 1 (initial value)

@sum	
M=0			// Set var sum to 0 (initial value)

(LOOP)
	@i
	D=M
	@n
	D=D-M
	@STOP
	D;JGT	// JMP to stop when i > n

	@sum
	D=M
	@i
	D=D+M
	@sum	// sum = sum + i
	M=D

	@i
	M=M+1	// Increment i

	@LOOP
	0;JMP	// Repeat loop body

(STOP)
	@sum
	D=M
	@R1
	M=D		// Write var sum (Result) to RAM[1]

(END)
	@END
	0;JMP	// Terminate Program
