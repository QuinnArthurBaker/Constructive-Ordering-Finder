#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <pthread.h>
#include <mutex>

typedef unsigned long bigValue;

struct ThreadParam{
	int id;
	std::vector<int> remainingVals;
	std::vector<int> daggers;
	bigValue constructiveOrderings;

	ThreadParam( std::vector<int> rv, std::vector<int> dagg, bigValue& cos){
		this->remainingVals = rv;
		this->id = dagg.front();
		this->daggers = dagg;
		this->constructiveOrderings = cos;
	}
};


static int n;
std::mutex mu;
void* calcFunc(void*);
void numOrderings(int, std::vector<int>, int, bigValue&, std::vector<int>);

int main(int argc, char const *argv[])
{
	if(argc<2){
		fprintf(stderr,"[ERROR] Please provide a value for n\n");
		return 1;
	}

	n = atoi(argv[1]);

	if(n%2==1){
		fprintf(stderr,"[ERROR] n is not even\n");
		return 2;
	}

	printf("n is %d\n",n);
	

	std::vector<int> remainingVec;
	std::vector<int> daggers;

	//initialize a vector with the natural ordering
	for(int i=1;i<n;i++){
		remainingVec.push_back(i);
	}

	//determine the number of threads to make based on the number of possible ordering starting values
	int numThreads = n/2-1;
	//create an array of totalOrdering counters, one per thread.
	bigValue* totalOrderings = new bigValue[numThreads]();
	//create a thread for each possible starting value for an ordering.

	pthread_t threads[numThreads];



	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	for(int i=0;i<numThreads;i++){
		//add the first value to the vector of daggers 
		daggers.push_back(i+1);
		//remainingVec.erase(std::remove(remainingVec.begin(),remainingVec.end(), i+1));
		std::erase(remainingVec, i+1);
		ThreadParam* tp = new ThreadParam(remainingVec, daggers, totalOrderings[i]);
		int threadStatus = pthread_create(&threads[i], NULL, calcFunc, (void*)tp);
		if(threadStatus!=0){
			fprintf(stderr, "[ERROR] Error creating thread %d\n",i);
			return 3;
		}
		remainingVec.push_back(i+1);
		daggers.erase(daggers.begin());
	}

	bigValue total = 0;

	for(int i=0;i<numThreads;i++){
		//join all the threads
		void* results = NULL;
		pthread_join(threads[i], &results);
		total += (bigValue)(results);	
	}

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> timeTaken = end-start;

	total *= 2;
	printf("Total number of constructive orderings is %lu - Time is %f seconds\n", total, timeTaken.count());


	return 0;
}

/* 
 *	This function parses args as a ThreadParam struct
 *  and calculates the total number of constructive orderings
 *  for this struct 
 */
void* calcFunc(void* args){
	ThreadParam* params = (ThreadParam*)(args);

	int id = params->id;
	std::vector<int> remainingVals = params->remainingVals;
	std::vector<int> daggers = params->daggers; 
	int curTotal = daggers.front();
	bigValue ords = 0;
	numOrderings(id, remainingVals, curTotal, ords, daggers);

	return (void*)(ords);

}

void numOrderings(int id,std::vector<int> remainingVals, int curTotal, bigValue& totalOrderings, std::vector<int> daggers){
	
	//if there are no more values to add onto the ordering, then all the values have been used and this is a constructive ordering
	if(remainingVals.empty()){
		//std::cout << "Valid constructive ordering: " << processedVals << std::endl;
		totalOrderings++;
		return;
	}
	//if the total is 0 (prop 1.4(c), since 0 dagger is 0), or the total is n/2 when it shouldn't be (prop 2.3 ish?)
	if(curTotal==0 || (remainingVals.size()>1 && curTotal==n/2)){
		//std::cout << "(" << processedVals << ") - invalid branch (curTotal is [" << curTotal << "]" << std::endl;
		return;
	}
	//for all the remaining values, we want to create new partial orderings and recursively descend down another level of the tree
	for(int i=0;i<remainingVals.size();i++){
		//take the front value everytime; this is because we must push the popped value on the back of the vector when we restore it for the next loop iteration, pushing the next value of interest to the front of the vector
		int poppedVal = remainingVals.front();
		
		//erase the poppedVal
		remainingVals.erase(std::remove(remainingVals.begin(), remainingVals.end(),poppedVal));
		//std::cout << "(" << processedVals << "): Popped Val is: [" << poppedVal << "] curTotal is: [" << curTotal << "]" << std::endl;
		//perform the summation calc
		int saveTotal = curTotal;
		curTotal += poppedVal;
		curTotal %= n;
		//std::cout << "(" << processedVals << ") curTotal calculated: [" << curTotal << "] Daggers: " << daggers << std::endl;
		//put the removed value into the list of used values 
		
		//if the new total does not already exists in the set of daggers, then this is a possible constructive ordering (Prop 1.4c)
		if(!(std::find(daggers.begin(),daggers.end(), curTotal)!=daggers.end())){ 
			//std::cout << "(" << processedVals << ") - invalid branch; curTotal [" << curTotal << "] seen in daggers [" << processedVals << "]" << std::endl;
			//add the current total to the list of daggers for the next level of the tree
			daggers.push_back(curTotal);
			//recursively descend down the next level of the tree
			numOrderings(id, remainingVals, curTotal, totalOrderings,  daggers);
			//remove that total for other "branches" of this "node"
			daggers.erase(std::remove(daggers.begin(), daggers.end(), curTotal));
		}

		//restore the current total to its value previously 
		curTotal = saveTotal;
		// curTotal-=poppedVal;
		// curTotal %= n;
		// if(curTotal<0){
		// 	curTotal+=n;
		// }
		//std::cout << "(" << processedVals << ") Reverted curTotal: [" << curTotal << "]" << std::endl;

		//remove the current newest value in ordering for the next pass to create the next possible family of orderings. i.e. if the ordering is currently (1,4,2), remove the 2 so that the ordering becomes (1,4) so in the next loop pass it can become (1,4,3) as opposed to (1,4,2,3)
		
		//similarily, re add this value back into the list of remaining values for future use; if the ordering is (1,4,3), then we want to be able to place a 2 somewhere in the future of this ordering.
		remainingVals.push_back(poppedVal);
		
	}
}
