#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

int verify_permutation(int* baselist, int n){
	bool* tracker = new bool[n]();
	int total = 0;
	for(int i=0;i<n;i++){
		total+=baselist[i];
		total%=n;
		if(tracker[total]){
			delete [] tracker;
			return i;
		}else{
			tracker[total] = true;
		}
	}

	delete [] tracker;
	return -1;
}

int main(int argc, char const *argv[])
{
	int n = atoi(argv[1]);
	printf("n is: %d\n", n);

	int* baselist = new int[n];
	for(int i=0;i<n;i++){
		baselist[i] = i;
	}

	int* tracker = new int[n]();
	int good = 0;
	do{
		if(baselist[0]!=0){
			break;
		}
		int repeat = verify_permutation(baselist,n);
		if(repeat==-1){
			good++;
		}else{
			tracker[repeat]++;
		}
	}while(std::next_permutation(baselist, baselist+n));

	delete [] baselist;

	for(int i=0;i<n;i++){
		printf("There were %d cases of violations at index %d\n",tracker[i],i);
		fprintf(stderr,"%d,%d\n",i,tracker[i]);
	}
	printf("There were %d good orderings\n", good );

	delete [] tracker;
	return 0;
}