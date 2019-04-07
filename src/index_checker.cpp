/*
This program calulates the index and factorial coefficient representation for each constructive ordering for a given value for n
*/
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
//typedef name largeNum for large values, to be changed easily at will
typedef unsigned long largeNum;

//function prototypes
largeNum factorial(int);
int* permutationLookup(int,largeNum);
largeNum factorial(int);
void print_arr(int*,int);
int ordering_checker(int*, int);
void print_info(std::ofstream*, int, largeNum);

int main(int argc, char const *argv[])
{
	//standard parameter checking
	if(argc<2 || atoi(argv[1])<4){
		printf("[ERROR] Please provide a positive command-line argument for n greater than 3\n");
		return 1;
	}
	// store the command line parameter as n
	int n = atoi(argv[1]);
	// open a file to store the results
	std::ofstream file;
	std::string name = "";
	name += "/home/zack/research/constructive-orderings-program/results/indexResults/" + std::to_string(n) + "results.csv";
	file.open(name);
	//for each value less than n!
	for(int i=0;i<factorial(n);i++){
		//create the ordering from the index value
		int* ordering = permutationLookup(n,i);
		//if the ordering is a constructive ordering, print the index of the ordering to the file opened above
		if(ordering_checker(ordering,n)){
			//printf("Constructive ordering @ index %d\n", i);
			//print_arr(ordering,n-1);
			print_info(&file,n,i);
		}
	}
	//close the file
	file.close();
	return 0;
}


void print_info(std::ofstream* f, int n, largeNum index){
	int* factorialRep = new int[n-1];
	(*f) << index << ": (";
	for(int i=0;i<n-2;i++){
		(*f) << (index/factorial(n-i-1));
		if(i<n-3){
			(*f) << ",";
		}
		index -= factorial(n-i-1)*(index/factorial(n-i-1));
	}
	(*f) << ")\n";




}
int* permutationLookup(int n, largeNum index){
	int* factorialRep = new int[n-1];

	for(int i=0;i<n-1;i++){
		factorialRep[i] = index/factorial(n-i-1);
		index -= factorial(n-i-1)*factorialRep[i];
	}
	std::vector<int> orderedList;
	for(int i=1;i<n;i++){
		orderedList.push_back(i);
	}

	int* ordering = new int[n-1];

	for(int i=0;i<n-1;i++){
		ordering[i] = orderedList.at(factorialRep[i]);
		orderedList.erase(orderedList.begin() + factorialRep[i]);
	}

	return ordering;
}

largeNum factorial(int n){
	largeNum prod = 1;
	for(int i=1;i<n;i++){
		prod*=i;
	}
	return prod;
}

void print_arr(int* arr, int n){
	for(int i=0;i<n;i++){
		printf("%d,",arr[i]);
	}
	printf("\n");
}

int ordering_checker(int* ordering, int n){
	bool* seenVals = new bool[n]();
	seenVals[0] = true;
	int sum = 0;
	for(int i=0;i<n-1;i++){
		sum+=ordering[i];
		sum%=n;
		if(seenVals[sum]==true){
			return 0;
		}else{
			seenVals[sum] = true;
		}
	}
	return 1;
}
