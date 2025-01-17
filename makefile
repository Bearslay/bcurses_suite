test:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/test.cpp -std=c++17 -m64 -g -Wall -I bengine
	@g++ test.o -o bin/debug/test -lncursesw
	@./bin/debug/test
