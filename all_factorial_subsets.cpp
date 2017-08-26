
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <thread>
#include <future>
#include <mutex>
using namespace std;
 
/*


*/
inline int factorial(int n){
	if(n<=1){
		return n;
	}
	else{
		return n*factorial(n-1);
	}
}

void print_arr(int* arr, int size){
	for(int i=0;i<size;i++){
		cout << arr[i] << ",";
	}
	cout << endl;

}

mutex mu;

int verify_permutations(int*& start_list, int*& end_list, int& n){

	int total_good = 0;
	int val = n+1;
	do{
		int total = 0;
		if(equal_arrays(start_list, end_list)){//if the current array is equal to what we define as the end of this batch, break out of the permutation loop to end the function
			break;
		}
		for(int i=0;i<n;i++){
			//verify each list here
		}

	}while(next_permutation(start_list, start_list+n))
	
	
	return total_good;

}
void subset_identifier(int v){
	int n,val;
	val = v;
	n = val-1;
	int* baselist = new int[n];//create the container array that holds the current permutation
	for(int i=0;i<n;i++){
		baselist[i] = i+1;//initialize its values
	}
	int index = 0;
	int valid_subsets = 0;

	if((v&(v-1)) == 0)//if v is a power of two, we know its first/"natural" ordering is valid. Since we skip the first ordering in the main loop, increment its value here
		valid_subsets++;

	int* start_list, end_list;
	int range = factorial(n)/thread::hardware_concurrency();
	start_list = baselist;

	do{
		if(index%range==0 && index!=0){
			end_list = baselist;
			verify_permutations(start_list, end_list, n);
			start_list = baselist;
		}
		index++;

	}while(next_permutation(baselist, baselist+n))


	delete [] baselist;
}

int main(){

	cout << "Enter a value for n: ";
	int v;
	cin >> v; 
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	subset_identifier(v);

	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end-start;
	cout << "Time: " << elapsed_seconds.count() << endl;
	return 0;
}

