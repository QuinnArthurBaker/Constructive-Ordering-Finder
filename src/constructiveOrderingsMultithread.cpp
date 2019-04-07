/*
Copyright 2019 Zackary Baker

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdlib.h> //used for atoi
#include <stdio.h> //used for printf
#include <unistd.h> //used for sysconf
#include <chrono> //used for timekeeping variables
#include <vector> //used for the vector datatype 
#include <pthread.h> // used for pthread_create, pthread_join
#include <algorithm> //used for std::next_permutation


//typedef used to make definition of large variables more readable
typedef  unsigned long largeNum;

//function prototypes
largeNum factorial(int);
int* lookupOrdering(int, largeNum);
void* threadProcessorFunc(void*);
int verifyOrdering(int*, int);


//definition of the struct used to pass information to each thread
struct Thread_Param{
	int id; // the id of the thread, beginning at 0
	int n; // the size of the group Z/nZ
	largeNum partitionSize;// the number of orderings for the thread to process
	int* firstOrdering;//a pointer to an array corresponding to the first ordering the thread will process
	//default constructor; not used
	Thread_Param(){}
	//main constructor used; takes individual values and sets the corresponding members of the struct
	Thread_Param(int id, int n, largeNum partitionSize){
		//set the member variables passed in
		this->id = id;
		this->n = n;
		this->partitionSize = partitionSize;
		//calculate the ordering this thread will begin calculation with
		this->firstOrdering = lookupOrdering(n, id*partitionSize);
	}
	//deconstructor; frees firstOrdering
	~Thread_Param(){
		delete [] firstOrdering;
	}

};


/**
*	The main method of the program. This program calculates the number of constructive orderings for the integers mod n
*/
int main(int argc, char const *argv[])
{
	//exit and print usage information if the program is run with 0 args
	if(argc==1){
		fprintf(stderr, "USAGE: %s n threadMult\n", argv[0]);
		return 1;
	}
	//get program parameters from the command line as the program is executed
	int n = atoi(argv[1]);
	int threadMult = 1;
	//set threadMult if provided by the user
	if(argc==3){
		threadMult = atoi(argv[2]);
	}

	//calculate the total number of threads to run based on the multiplier provided by the user and the total number of online processors at the time of program execution
	int maxThreads = threadMult*sysconf(_SC_NPROCESSORS_ONLN);

	printf("n is %d, threadMult is %d, total threads to create is %d\n", n, threadMult, maxThreads);
	//the total number of orderings to calculate; Since we only care about orderings that start with zero, we only have (n-1)! orderings to check, and since each constructive ordering of the form (0, k, ...), k<n/2 has exactly one corresponding constructive ordering (0,n-k,...), we only need to check the first half of the orderings, hence the division by 2
	largeNum orderingCount = factorial(n-1)/2;

	//if there arent an evenly divisible number of orderings per thread, we can't continue
	if(orderingCount%maxThreads!=0){
		fprintf(stderr,"[ERROR] The number of orderings (%ld) is not evenly divisible across the number of threads (%d)\n", orderingCount, maxThreads);
		return 2;
	}	
	//each thread will process exactly partitionSize orderings
	largeNum partitionSize = orderingCount/maxThreads;

	//create an array of thread objects
	pthread_t threads[maxThreads]; 


	//create the variables used for timekeeping
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	//launch each thread
	for(int i=0;i<maxThreads;i++){
		Thread_Param* tp = new Thread_Param(i, n, partitionSize);
		int threadStatus = pthread_create(&threads[i], NULL, threadProcessorFunc,(void*)tp);
		if(threadStatus!=0){// if there is an error creating the thread, exit
			fprintf(stderr, "[ERROR] Error creating thread %d\n", i);
			return 3;
		}
	}

	largeNum constructiveOrderings = 0;

	for(int i=0;i<maxThreads;i++){
		void* results = NULL;
		pthread_join(threads[i], &results);
		constructiveOrderings+=(largeNum)(results);
	}

	//multiply the final result by 2, to compensate for the fact that only the first half of orderings are checked.
	constructiveOrderings*=2;

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> timeTaken = end-start;
	printf("FINISHED - Total constructive orderings: %ld - Time taken: %f\n", constructiveOrderings, timeTaken.count());


	return 0;
}

/**
*	A simple recursive implementation of the factorial function
*	INPUT:
		- n: the value to calculate the factorial of
	OUTPUT:
		returns n!
*/
largeNum factorial(int n){
	if(n>1){
		return factorial(n-1)*n;
	}
	else{
		return 1;
	}
}

/*
*	This method calculates the lexicographical ordering based on a given index for the integers mod n, minus the first element.
	Example:
	All permutations of the integers mod 3 are listed as follows, in lexicographical order:
	(0,1,2)
	(0,2,1)
	(1,0,2)
	(1,2,0)
	(2,0,1)
	(2,1,0)

	Indexing these in this order gives the following relation:
	0 -> (0,1,2)
	1 -> (0,2,1)
	2 -> (1,0,2)
	3 -> (1,2,0)
	4 -> (2,0,1)
	5 -> (2,1,0)

	Thus, lookupOrdering(3,4), for example, would return [0,1], which is [2,0,1] without the first element.

	INPUT:
		- n: the size of the group
		- orderingIndex: the index of the ordering we are interested in
	OUTPUT:
		- the ordering with index orderingIndex, in lexicographical order, minus the first element
*/
int* lookupOrdering(int n, largeNum orderingIndex){
	//tuple is an array which holds the factorial representation of orderingIndex
	int* tuple = new int[n-1];
	for(int i=0;i<n-1;i++){
		//calculate the ith coefficient of orderingIndex
		int coefficient = orderingIndex/factorial(n-1-i);
		tuple[i] = coefficient;
		//reduce orderingIndex to calculate the next coefficient
		orderingIndex-=factorial(n-1-i)*coefficient;
	}

	//create a standard vector of size n with values 0 through n-1
	std::vector<int> orderedList;
	for(int i=0;i<n;i++){
		orderedList.push_back(i);
	}
	int* ordering = new int[n];
	//for each element in the sequence
	for(int i=0;i<n-1;i++){
		//set each position to the value of the ordered list, indexed by the values in tuple created above, then remove that value to prevent duplicates
		ordering[i] = orderedList.at(tuple[i]);
		orderedList.erase(orderedList.begin()+tuple[i]);
	}
	//manually add the final value to the ordering.
	ordering[n-1] = orderedList.front();

	//simply create a new array from the old array of one size smaller to remove the first element
	int* nl = new int[n-1];
	for(int i=0;i<n-1;i++){
		nl[i] = ordering[i+1];
	}

	delete [] ordering;
	delete [] tuple;

	return nl;


}


/**
*	This method is used by each thread to begin calculation.
*	INPUT:
*		- args: a pointer which is cast into a Thread_Param pointer, used to access parameters intended for the method.
	OUTPUT:
		- the number of constructive orderings in the range for the thread, cast into a void pointer
*/
void* threadProcessorFunc(void* args){
	//cast args into a Thread_Param struct
	Thread_Param* params = (Thread_Param*)(args);

	//extract the members of the params struct
	int n = params->n;
	largeNum partitionSize = params->partitionSize;
	int* currentOrdering = params->firstOrdering;
	//constructiveOrderings holds the count of how many constructive orderings are in the range of the thread
	largeNum constructiveOrderings = 0;

	do{//iterate through each permutation in range and verify each of them.
		constructiveOrderings+=verifyOrdering(currentOrdering, n);
		partitionSize--;
		if(partitionSize==0){
			break;
		}
	}while(std::next_permutation(currentOrdering, currentOrdering+(n-1)));

	//clean up allocated memory and return constructiveOrderings
	delete params;
	return (void*)(constructiveOrderings);


}

/**
	This method determines if a given ordering is a constructive ordering.
	INPUT:
		- ordering: an integer array of size n-1 representing a potential constructive ordering.
		- n: the size of the group
	OUTPUT:
		returns 1 if ordering is a constructive ordering, and 0 otherwise.
*/
int verifyOrdering(int* ordering, int n){

	int total = 0;// the running total sum
	bool* elementsSeen = new bool[n](); // an array to keep track of whether each index has been seen previously 
	for(int i=0;i<n-1;i++){
		total += ordering[i];
		total %= n; 
		if(total==0 || elementsSeen[total]==true || (total==n/2 && i!=n-2)){//if the running total is 0 or this total has been seen before or the total is n/2 and is not the final total, return 0 (false)
			delete [] elementsSeen; 
			return 0;
		}
		else{//otherwise indicate that we have seen this total for future passes
			elementsSeen[total] = true;
		}
	}

	//if no total is seen twice, and the other conditions are met, this is a constructive ordering
	delete [] elementsSeen;
	return 1;
}
