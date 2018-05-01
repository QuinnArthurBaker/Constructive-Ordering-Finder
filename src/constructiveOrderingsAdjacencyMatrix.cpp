#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
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

void print_arr(int* arr, int size, FILE* out=stdout){
	for(int i=0;i<size;i++){
		fprintf(out, "%d, ", arr[i]);
	}
	fprintf(out, "\n");
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

/**
*	This function takes an integer (representing our value for n) and calculates the number of constructive orderings for that group
*	INPUT:
*		v - the modulus of the group to use, i.e. Z/vZ
*	OUTPUT: 
*		The number of constructive orderings for Z/vZ is printed to stdout, as well as the right-adjacency matrix
*/
void subset_identifier(int v){
	int n = v-1; 
	cout << "n is: " << n << endl;

	int* baselist = new int[n];
	for(int i=0;i<n;i++){
		baselist[i] = i+1;
	}

	int index = 0;
	int valid_subsets = 0;
	//This 2d matrix holds the adjacency values for each element of Z/vZ 
	int** adj_matrix = new int*[v];

	//initialize the adjacency matrix
	for(int i=0;i<v;i++){
		adj_matrix[i] = new int[v];
		for(int j=0;j<v;j++){
			adj_matrix[i][j] = 0;  
		}
	}
	do{//main loop
		int res = verify_permutation(baselist, n);//check to see if this ordering is a constructive ordering
		if(res==1){//if baselist is a constructive ordering
			for(int i=0;i<n;i++){
				if(i==0){//if i is 0, increment a certain position, because 0 isn't included in the baselist. That is , the permutations look like (1,2,...) instead of (0,1,2,...)
					adj_matrix[0][baselist[i]]++;
				}else{
					adj_matrix[baselist[i-1]][baselist[i]]++;//the value at baselist[i] is directly preceded by the value at baselist[i-1]
				}

			}
			valid_subsets++;
		}

	}while(next_permutation(baselist, baselist+n));//iterate through each possible permutation of baselist
	//print the adjacency matrix
	for(int i=0;i<v;i++){
		printf("%d| ", i);
		for(int j=0;j<v;j++){
			printf("%d ", adj_matrix[i][j]);
		}
		delete [] adj_matrix[i];
		printf("\n");
	}
	delete [] adj_matrix;
	cout << "Total: " << valid_subsets << endl;

	delete [] baselist;
}

int main(int argc, char const *argv[])
{
	if(argc<2){//if the user provides too few arguments, warn, then exit
		printf("[ERROR] too few arguments; value needed for n\n");
		exit(1);
	}
	int v = atoi(argv[1]);
	//create timing metrics
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	//identify all the orderings
	subset_identifier(v);

	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end-start;
	cout << "Time: " << elapsed_seconds.count() << endl;
	FILE* f = popen("echo -ne '\007' > /dev/tty1", "r");
	pclose(f);
	return 0;
}

