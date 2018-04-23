#include <stdio.h> //printf, fprintf


int main(int argc, char const *argv[])
{
	if(argc<2){
		fprinf("USAGE: ./%s [n]\nn - the group to calculate the number of constructive orderings for; Z/nZ\n");
		return 1;
	}
	int n = atoi(argv[1]);
	printf("n=%d\n", n);


	return 0;
}