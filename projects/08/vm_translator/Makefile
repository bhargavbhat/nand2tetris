all: *.h *.cpp
	g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror *.cpp -o vm_translator

clean:
	rm -f *.a *.o vm_translator

test:
	./vm_translator ../MemoryAccess/BasicTest/BasicTest.vm
