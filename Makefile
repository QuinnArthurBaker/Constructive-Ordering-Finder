compile:
	clang++ -pthread -std=c++11 -o fact_sub all_factorial_subsets.cpp
run:	compile
	./fact_sub 10
debug: compile
	valgrind --leak-check=full ./fact_sub 10
clean:
	rm fact_sub
