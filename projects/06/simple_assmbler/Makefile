all: simple_assembler.cpp 
	g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror simple_assembler.cpp -o simple_assembler

clean:
	rm -f *.a *.o simple_assembler

test:
	./simple_assembler ../add/Add.asm ../add/Add.out
