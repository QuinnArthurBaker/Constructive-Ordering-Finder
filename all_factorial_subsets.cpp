#include <stdio.h> //used for input stuff
#include <pthread.h> //used for thread stuff
#include <unistd.h> //used for sysconf, _SC_NPROCESSORS_ONLN, 
#include <mutex> //used for mutex
#include <chrono> //used for timing functions
#include <algorithm> //used for std::copy
#include <cstring>

std::mutex mu;
//function prototypes
void print_arr(int*,int);
int* get_starting_baselist(int,int,int);
void* thread_subsets_creator(void*);
long factorial(int);
int verify_ordering(int*,int);

//Struct to hold the parameters to provide to the function each thread is processing
struct Thread_Param{
	int id;//the id of the thread
	int* baselist;// a pointer to the permutation to start calculation from
	int n;//the group to calculate the permutations for; Z/nZ
	long partition_size;//the number of permutations to calculate
	Thread_Param(){

	}
	Thread_Param(int i, int* b, int n_val, long ps){
	 	id = i;
	 	//baselist = b;//shallow copy; bad?
		n = n_val;
	 	baselist = new int[n-1];
	 	for(int i=0;i<n-1;i++){
	 		baselist[i] = b[i];
	 	}
	 	partition_size = ps;
	}
	 Thread_Param(Thread_Param& t){//Copy Constructor for Thread_Param
	 	id = t.id;
	 	n = t.n;
	 	partition_size = t.partition_size;
	 	baselist = t.baselist;//shallow copy; bad?
	 }
};

//this function confirms or denies if ord is a valid ordering
int verify_ordering(int* ord, int size){
	int sum = 0;
	int* seen_vals = new int[size]();//this is an array to hold the already seen values from processing the factorials of the ordering
	for(int i=0;i<size;i++){
		sum+= ord[i];//add the current value to the sum
		sum %= size+1;//mod by the original n
		int* find_results = std::find(seen_vals, seen_vals+size, sum);//calculate if sum has already been seen
		if(sum==0 || find_results!=seen_vals+size){//if sum is 0 or we've already seen the current value
	//		delete find_results;
	//		delete [] seen_vals;
			return 0;//it is not a valid subset
		}
	//	delete find_results;
	}
	//delete [] seen_vals;
	return 1;//if we make it to the end, then the ordering is valid
}
//This function calculates the first permutation each thread will start processing
int* get_starting_baselist(int tid, int n, int partition_size){
	int v = n-1;
	int target_list_count = tid*partition_size;//calculate the target list's position in the permutation order
	int list_count = 0;
	printf("[LIST %d] Target List Count: %d, \n", tid, target_list_count);
	int* baselist = new int[v];//create the list to permute through
	for(int i=0;i<v;i++){
		baselist[i]=i+1;
	}
	do{
		list_count++;
		if(list_count>=target_list_count){
			break;
		}
	}
	while(std::next_permutation(baselist,baselist+v));
	
	return baselist;

}
//this function prints the first size elements from arr
void print_arr(int* arr, int size){
	for(int i=0;i<size;i++){
		printf("%d,",arr[i]);
	}
	printf("\n");
	return;
}

//this function iterates through the permutations each thread is responsible for checking
void* thread_ordering_creator(void* args){
	// Get the Thread_Param struct out of args
	Thread_Param* tp = (Thread_Param*)args;
	Thread_Param params = *(new Thread_Param(*tp));
	
	mu.lock();
	printf("[THREAD %d] My start list is:\n", params.id);
	int v = params.n-1;
	print_arr(params.baselist, params.n-1);
	int* iterate_list = new int[v];//list used to iterate through
	for(int i=0;i<v;i++){
		iterate_list[i] = params.baselist[i];//deepcopy into iterate_list
	}
	mu.unlock();
	long int good_orderings_calculated = 0;
	long int total_orderings_seen = 0;
	mu.lock();
	printf("[THREAD %d] Entering loop; \n", params.id);
	mu.unlock();
	do{	
		good_orderings_calculated += verify_ordering(iterate_list, v);
		total_orderings_seen++;
		if(total_orderings_seen>=params.partition_size){
			break;
		}
	}while(std::next_permutation(iterate_list, iterate_list+v));


	


	return (void*)good_orderings_calculated;
}



inline long factorial(long n){
	if(n<0)
		return -1;
	if(n==0 || n==1)
		return 1;
	else{
		return n*factorial(n-1);
	}
}

int main(int argc, char const *argv[])
{
	if(argc<2){//if usage is incorrect, exit
		fprintf(stderr, "[ERROR] Expected value for n as CLI. Exiting with code 1\n");
		_exit(1);
	}
	std::chrono::time_point<std::chrono::system_clock> start, end;//create the timekeeping variables
	start = std::chrono::system_clock::now();//initialize the first time variable
	int n = atoi(argv[1]);//convert the CLI to an int to process
	int max_threads = sysconf(_SC_NPROCESSORS_ONLN);//get the number of online concurrent threads
	long int total_perms = factorial((long)(n-1));
	long int partition_size = total_perms/max_threads;//calculate the number of lists each thread should process
	printf("MAX THREADS: %d\n", max_threads);
	pthread_t threads[max_threads];//array of threads
	for(int i=0;i<max_threads;i++){//for each thread
		Thread_Param tp = *(new Thread_Param(i, get_starting_baselist(i,n,partition_size), n, partition_size));//create the Thread_Param object to supply to the function provided to each thread
		int t_status = pthread_create(&threads[i], NULL, thread_ordering_creator,(void*)&tp);//create the thread
		if(t_status!=0){//if there is an error creating the thread, exit
			fprintf(stderr, "[ERROR] Error creating thread %d; status is %d; exiting\n", i, t_status);
			_exit(2);
		}

	}




	int total_good_perms = 0;
	void** results_ptr = new void*[max_threads];
	for(int i=0;i<max_threads;i++){
		results_ptr[i] = new void*;
		pthread_join(threads[i], &results_ptr[i]);
		total_good_perms+= (int)(long)(results_ptr[i]);
		//printf("results from thread %d: total good perms is now : %d\n", i, total_good_perms);
		//delete  (int*)(results_ptr[i]);
		//delete threads_args_ptr[i];
	}
	delete [] results_ptr;
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> time_taken = end-start;
	printf("FINISHED - Total good orderings: %d - Time taken: %f\n", total_good_perms, time_taken.count());


	return 0;
}