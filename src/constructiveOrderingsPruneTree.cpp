#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono> //used for timekeeping variables

typedef unsigned long bigValue;

void numOrderings(std::vector<int>, int, bigValue&, std::vector<int>, std::vector<int>);
std::ostream& operator<<(std::ostream&, std::vector<int>);

static int n;

int main(int argc, const char* argv[]){
	if(argc<2){
		fprintf(stderr, "[ERROR] Please provide a value for n\n");
		return 1;
	}
	n = atoi(argv[1]);
	if(n%2==1){
		fprintf(stderr, "[ERROR] Provided value for n [%d] is not even\n",n);
		return 1;
	}

	printf("n is %d\n",n);

	bigValue totalOrderings = 0;
	std::vector<int> processedVals;
	std::vector<int> remainingVec;
	std::vector<int> daggers;
	for(int i=1;i<n;i++){
		remainingVec.push_back(i);
	}
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	for(int i=1;i<n/2;i++){
		processedVals.push_back(i);
		daggers.push_back(i);
		remainingVec.erase(std::remove(remainingVec.begin(),remainingVec.end(),i));
		numOrderings(remainingVec, i, totalOrderings,processedVals,daggers);
		remainingVec.push_back(i);
		processedVals.erase(processedVals.begin());
		daggers.erase(daggers.begin());
	}
	end =  std::chrono::system_clock::now();
	std::chrono::duration<double> timeTaken = end-start;


	printf("Total constructive orderings is: %lu - Time taken: %f\n", totalOrderings*2, timeTaken.count());

}

void numOrderings(std::vector<int> remainingVals, int curTotal, bigValue& totalOrderings, std::vector<int> processedVals, std::vector<int> daggers){
	//std::cout << "(" << processedVals << ") Beginning of function. " << std::endl;
	if(remainingVals.size()==0){
		//std::cout << "Valid constructive ordering: " << processedVals << std::endl;
		totalOrderings++;
		return;
	}
	if(curTotal==0 || (remainingVals.size()>1 && curTotal==n/2)){
		//std::cout << "(" << processedVals << ") - invalid branch (curTotal is [" << curTotal << "]" << std::endl;
		return;
	}
	for(int i=0;i<remainingVals.size();i++){
		//take the front value everytime; this is because we must push the popped value on the back of the vector when we restore it for the next loop iteration, pushing the next value of interest to the front of the vector
		int poppedVal = remainingVals.front();
		
		//erase the poppedVal
		remainingVals.erase(std::remove(remainingVals.begin(), remainingVals.end(),poppedVal));
		//std::cout << "(" << processedVals << "): Popped Val is: [" << poppedVal << "] curTotal is: [" << curTotal << "]" << std::endl;
		//perform the summation calc
		curTotal += poppedVal;
		curTotal %= n;
		//std::cout << "(" << processedVals << ") curTotal calculated: [" << curTotal << "] Daggers: " << daggers << std::endl;
		processedVals.push_back(poppedVal);

		if(std::find(daggers.begin(),daggers.end(), curTotal)!=daggers.end()){
			//std::cout << "(" << processedVals << ") - invalid branch; curTotal [" << curTotal << "] seen in daggers [" << processedVals << "]" << std::endl;
		}
		else{
			//process the subtree
			daggers.push_back(curTotal);
			numOrderings(remainingVals, curTotal, totalOrderings, processedVals, daggers);
			daggers.erase(std::remove(daggers.begin(), daggers.end(), curTotal));
		}
		
		curTotal-=poppedVal;
		curTotal %= n;
		if(curTotal<0){
			curTotal+=n;
		}
		//std::cout << "(" << processedVals << ") Reverted curTotal: [" << curTotal << "]" << std::endl;
		processedVals.erase(std::remove(processedVals.begin(), processedVals.end(), poppedVal));
		
		remainingVals.push_back(poppedVal);
		//restore the vector and total for the next pass
		
	}
}



std::ostream& operator<<(std::ostream& os, std::vector<int> vec){
	if(vec.size()==0){
		os << "Vector is empty";
		return os;
	}
	for(int i=0;i<vec.size();i++){
		os << vec.at(i) << " ";
	}
	return os;
}