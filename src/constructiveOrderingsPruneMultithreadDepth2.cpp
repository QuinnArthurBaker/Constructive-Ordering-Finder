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
	std::vector<int> prefix;
	bigValue constructiveOrderings;
	ThreadParam(int id, std::vector<int> rv, std::vector<int> dagg, bigValue& cos, std::vector<int> prefix){
		this->remainingVals = rv;
		//this->id = dagg.front();
		this->id = id;
		this->daggers = dagg;
		this->constructiveOrderings = cos;
		this->prefix = prefix;

	}
};


static int n;

std::mutex mu;
void* calcFunc(void*);
void numOrderings(int, std::vector<int>, int, bigValue&, std::vector<int>);
std::vector<int> getStartingDaggers(int,int&);
std::vector<std::vector<int>> getVectorOfStartingDaggers();


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
	
	//tuple size specifies the number of values in the second non-zero position of the ordering. Naievely, g_2 cannot be 0 or g_1, so there are n-2 potential values for the second position.
	// tuple_size = n-2;

	std::vector<int> _remainingVec;
	std::vector<int> remainingVec;
	std::vector<int> daggers;

	//initialize a vector with the natural ordering
	for(int i=1;i<n;i++){
		_remainingVec.push_back(i);
	}
	remainingVec = _remainingVec;
	//determine the number of threads to make based on the number of possible ordering starting values. There are ((n/2)-1)(n-2) possible tuples which start a potiential constructive ordering

	std::vector<std::vector<int>> vosd = getVectorOfStartingDaggers();
	int numThreads = vosd.size();
	printf("Number of threads: %d\n", numThreads);
	//create an array of totalOrdering counters, one per thread.
	bigValue* totalOrderings = new bigValue[numThreads]();
	//create a thread for each possible starting value for an ordering.

	pthread_t threads[numThreads];

	//this variable is used to control the tuples generated for each thread. 
	
	// for(int i=0;i<vosd.size();i++){
	// 	printf("Dagger %d: ", i);
	// 	for(int j=0;j<vosd.at(i).size();j++){
	// 		printf("%d,", vosd.at(i).at(j));
	// 	}
	// 	printf("\n");
	// }

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	
	
	for(int i=0;i<numThreads;i++){
		
		//reset preinc if the first positions value has changed.
		//create the vector of the first 2 values of the ordering
		std::vector<int> prefix = vosd.at(i);
		
		//remove those values from the list of those which will potentially add to the ordering
		std::erase(remainingVec, prefix.at(0));
		std::erase(remainingVec, prefix.at(1));

		//create the vector of the first two daggers from the prefix vector

		std::vector<int> daggers = {prefix.at(0), (prefix.at(0)+prefix.at(1))%n};

		//create the ThreadPararm struct with the preset values
		ThreadParam* tp = new ThreadParam(i, remainingVec, daggers, totalOrderings[i], prefix);
		
		//create the thread
		int threadStatus = pthread_create(&threads[i], NULL, calcFunc, (void*)tp);
		if(threadStatus!=0){
			fprintf(stderr, "[ERROR] Error creating thread %d\n",i);
			return 3;
		}
		//re-add the removed values to reuse 
		remainingVec.push_back(prefix.at(1));
		remainingVec.push_back(prefix.at(0));
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

/** Function definitions **/

std::vector<std::vector<int>> getVectorOfStartingDaggers(){
	int div = 1;
	int mod = 1;
	std::vector<std::vector<int>> daggers;
	while(div < n/2){
		while((mod==div) || ((mod+div)%n == 0) || (mod+div)%n == n/2){
			mod++;
		}
		if(mod>=n){
			mod = 1;
			div++;
			continue;
		}
		std::vector<int> tVec = {div,mod};
		daggers.push_back(tVec);
		mod++;
	}
	return daggers;
}

/*std::vector<int> getStartingDaggers(int index, int& preinc){
	std::vector<int> v;
	int div = index / tuple_size + 1;
	v.push_back(div);
	int mod = (index % tuple_size) + 1 + preinc;
	while(mod==div){
	 	preinc++;
	 	mod++;
	}
	v.push_back(mod);
	return v;
}*/
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
	std::vector<int> prefix = params->prefix;

	int curTotal = daggers.back();
	bigValue ords = 0;

	numOrderings(id, remainingVals, curTotal, ords, daggers);
	
	
	
	if(ords==0){
		mu.lock();
		printf("Thread %d has %lu constructive orderings\n",id, ords);
		printf("Thread %d prefix: %d, %d\n", id, prefix.at(0), prefix.at(1));
		mu.unlock();
	}
	
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
		//remainingVals.erase(std::remove(remainingVals.begin(), remainingVals.end(),poppedVal));
		std::erase(remainingVals, poppedVal);
		//std::cout << "(" << processedVals << "): Popped Val is: [" << poppedVal << "] curTotal is: [" << curTotal << "]" << std::endl;
		//perform the summation calc
		int saveTotal = curTotal;
		curTotal = (poppedVal + curTotal) % n;
		//curTotal %= n;
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
			//daggers.erase(std::remove(daggers.begin(), daggers.end(), curTotal));
			std::erase(daggers,curTotal);
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
