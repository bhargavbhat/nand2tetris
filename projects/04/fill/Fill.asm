// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

(LOOP)			// main loop
    @KBD
    D=M         // D == 0 if no key pressed
    @DRAW
    D;JEQ       // Jump to DRAW routine, Since D == 0, it'll draw white
    D=-1        // D != 0 -> Key pressed
				// Fallthru to DRAW routine, since D != 0, it'll draw black
(DRAW)
	@colour
	M=D
    
    @SCREEN
    D=A			// Base address of SCREEN

    @8192		// Size of SCREEN
    D=D+A       // D = Base address of SCREEN + End Of SCREEN

    @i
    M=D         // i = End address of SCREEN
    
(FILL) 
    @i
    D=M-1
    M=D         // decrement i
    @LOOP
    D;JLT       // repeat till all pixels drawn
    
	@colour
    D=M			// read colour value into D register

	@i
	A=M			// access address (Base address of SCREEN + i)
    M=D         // M[i] = D register i.e value of colour variable

    @FILL		// repeat till all pixels drawn
    0;JMP
