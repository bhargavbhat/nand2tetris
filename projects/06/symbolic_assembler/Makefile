all: *.cpp *.h 
	g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror *.cpp -o symbolic_assembler

clean:
	rm -f *.a *.o symbolic_assembler

test:
	./symbolic_assembler
