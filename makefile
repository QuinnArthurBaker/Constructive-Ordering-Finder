compile:
	clang++ -pthread -std=c++11 -o fact_sub all_factorial_subsets.cpp
run:
	clang++ -pthread -std=c++11 -o fact_sub all_factorial_subsets.cpp && ./fact_sub
debug:
	clang++ -g -pthread -std=c++11 -o fact_sub all_factorial_subsets.cpp
