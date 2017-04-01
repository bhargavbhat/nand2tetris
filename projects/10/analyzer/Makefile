all: *.h *.cpp
	g++ -ggdb3 -std=c++11 -Wall -Wextra -Wpedantic -Werror *.cpp -o JackAnalyzer

clean:
	rm -f *.a *.o JackAnalyzer

check: all
	./JackAnalyzer ./test.jack
	cat ./test.xml
	rm test.xml

test: all
	./JackAnalyzer ../ArrayTest/Main.jack
	diff -u --ignore-all-space ../ArrayTest/Main.xml ../ArrayTest/MainT.xml
