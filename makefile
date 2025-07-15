core.o: core.cpp
	clang++ -c core.cpp -o core.o

parse.o: parse.cpp core.o
	clang++ -c parse.cpp -o parse.o

parse: parse.o
	clang++ core.o parse.o -o parse