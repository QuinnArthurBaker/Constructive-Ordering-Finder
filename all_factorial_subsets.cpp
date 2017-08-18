#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <pthread.h>

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
int verify_permutation (int*& perm, int& n){

	int total=0;//holds the running element total
	int val = n+1;
	int* elements_seen = new int[n]();//the array to hold the elements in the current permutation
	int elements_seen_index = 0;//index variable for elements_seen
	bool full_set = true;

	for(int element=0;element<n;element++){//for each element in the current permutation
		total += perm[element];
		total%=val;
		int *find_result = find(elements_seen,elements_seen+n,total);
		if( find_result != elements_seen+n || total==0){//if the element has already been seen, break to the next permutation
			// delete find_result;
			delete [] elements_seen;
			return 0;
		}else{
			elements_seen[elements_seen_index] = total;
			elements_seen_index++;
		}
		
	}

	delete [] elements_seen;
	//delete [] perm;
	return 1;

}
void subset_identifier(int v){
	int n,val;
	val = v;
	n = val-1;
	cout << "n is: " << n << endl;
	int* baselist = new int[n];
	for(int i=0;i<n;i++){
		baselist[i] = i+1;
		cout << baselist[i]  << endl;
	}
	int index = 0;
	int valid_subsets = 0;
	do{//main loop
		valid_subsets += verify_permutation(baselist, n);
	}while(next_permutation(baselist, baselist+n));
	cout << "Total: " << valid_subsets << endl;

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

