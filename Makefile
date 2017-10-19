compile:
	clang++ -std=c++11 -o fact_sub_single all_factorial_subsets_singlethread.cpp
	clang++ -std=c++11 -lpthread -o fact_sub_multi all_factorial_subsets_multithread.cpp
clean:
	rm fact_sub_multi fact_sub_single
