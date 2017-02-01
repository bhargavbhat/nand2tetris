// Draw a solid rectangle at top-left corner of screen
// Width: 16px x Height: RAM[0] px

@SCREEN
D=A
@addr
M=D

@R0
D=M
@n
M=D			// init n with rect height

@i
M=0			// init i

(LOOP)
	@i
	D=M
	@n
	D=D-M
	@END
	D;JGT	// loop termination

	@addr
	A=M
	M=-1	// draw pixels, -1 = 0xffff

	@i
	M=M+1	// incr i

	@32
	D=A

	@addr
	M=D+M	// jump to next row addr += 32

	@LOOP
	0;JMP	// repeat

(END)
	@END
	0;JMP	// Program terminate
