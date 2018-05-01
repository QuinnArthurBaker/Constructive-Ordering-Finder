#include <stdio.h> //printf, fprintf
#include <time.h>

int main(int argc, char const *argv[])
{
	if(argc<2){
		fprintf(stderr,"USAGE: ./%s [n]\nn - the group to calculate the number of constructive orderings for; Z/nZ\n");
		return 1;
	}
	int n = atoi(argv[1]);//get value for n from passed parameter
	printf("n=%d\n", n);

	//create timing variables
	time_t start_time, end_time;
	start_time = time(NULL);

	//get size of an int for future calculations
	int size = sizeof(int);

	int numBlocks = 1024;
	int numThreads = 512;

	int* baselist = (int*)malloc((n-1)*size);//the list will be n-1 ints long, because we dont need to store the leading 0

	//initialize baselist
	for(int i=0;i<(n-1);i++){
		baselist[i] = i+1;
		printf("Baselist[%d] is %d\n", i, baselist[i]);
	}	
	//make a pointer to hold a certain number of permutations (a batch) for work on the graphics card

	int** lists = malloc(size*(n-1)*1024);

	for(int i=0;i<1024;i++){
		
	}
	return 0;
}