/*

Copyright 2019 Zackary Baker

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include <stdio.h>//used for printf
#include <stdlib.h>//used for atoi
#include <iostream>//used for std::cout, std::ostream, etc.
#include <vector>//used for std::vector (duh)
#include <algorithm>//used for std::find, std::remove
#include <chrono> //used for timekeeping variables

//create a new type name so we don't need to change it everywhere if the size of the "large" type needs to be changed
typedef unsigned long bigValue;

//function prototypes
//this method will calculate the total number of constructive orderings / 2 for a given value of n. the bigValue parameter is incremented to indicate a constructive ordering has been found.
void numOrderings(std::vector<int>, int, bigValue&, std::vector<int>);
//overloaded print operator for std::vector
std::ostream& operator<<(std::ostream&, std::vector<int>);

//global variable to track the value of n, the size of the group
static int n;

int main(int argc, const char* argv[]){
	if(argc<2){
		//error if the user does not provide a value for n
		fprintf(stderr, "[ERROR] Please provide a value for n\n");
		return 1;
	}
	n = atoi(argv[1]);
	if(n%2==1){
		//error if the value provided for n is not even
		fprintf(stderr, "[ERROR] Provided value for n [%d] is not even\n",n);
		return 1;
	}

	printf("n is %d\n",n);
	//create a variable to track the total number of constructive orderings
	bigValue totalOrderings = 0;

	//vectors used in numOrderings
	std::vector<int> remainingVec;// the values not yet used in the current ordering's calculations
	std::vector<int> daggers;//the values of the daggers of the elements of the current ordering

	//create a vector to track which elements of the group have not yet been used in a potential constructive ordering
	for(int i=1;i<n;i++){
		remainingVec.push_back(i);
	}
	//create timekeeping variables and start timing
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	//iterate through [1..n/2), as we know constructive orderings are "mirrored" across (0, n/2, ...). That is, if (0, a_1, a_2, ...) is a constructive ordering, where a_1<n/2, then there exists exactly one constructive ordering (0, b_1, b_2, ...), where b_1 > n/2 which can be mapped to (this doesn't make sense probably) (Prop 2.7)
	for(int i=1;i<n/2;i++){
		//add the current value to the list of used values in the ordering
		
		//since the dagger of the first non-zero element is always itself, push i back to the set of daggers as well
		daggers.push_back(i);
		// remove i from the list of unused values so it does not get reused
		remainingVec.erase(std::remove(remainingVec.begin(),remainingVec.end(),i)); 
		//calculate the number of constructive orderings that begin with i
		numOrderings(remainingVec, i, totalOrderings,daggers);
		//put i back in the rear of the remaining values vector to be reused for potential orderings that do not begin with i
		remainingVec.push_back(i);
		//remove i from the list of used values and the list of daggers 
	
		daggers.erase(daggers.begin());
	}
	//get the time and calculate the difference in time from start to end
	end =  std::chrono::system_clock::now();
	std::chrono::duration<double> timeTaken = end-start;

	//print the total number of constructive orderings, multiplying by 2 to account for the reduced values 
	printf("Total constructive orderings is: %lu - Time taken: %f\n", totalOrderings*2, timeTaken.count());

}

void numOrderings(std::vector<int> remainingVals, int curTotal, bigValue& totalOrderings, std::vector<int> daggers){
	//std::cout << "(" << processedVals << ") Beginning of function. " << std::endl;
	//if there are no more values to add onto the ordering, then all the values have been used and this is a constructive ordering
	if(remainingVals.size()==0){
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
			numOrderings(remainingVals, curTotal, totalOrderings,  daggers);
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


//overloaded vector print operator
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