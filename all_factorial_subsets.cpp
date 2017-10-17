#include <stdio.h> //used for input stuff
#include <pthread.h> //used for thread stuff
#include <unistd.h> //used for sysconf, _SC_NPROCESSORS_ONLN, 
#include <mutex> //used for mutex
#include <chrono> //used for timing functions
#include <algorithm> //used for std::copy


std::mutex mu;
//function prototypes
void print_arr(int*,int, FILE*);
int* get_starting_baselist(int,int,unsigned long long int);
void* thread_subsets_creator(void*);
long factorial(int);
int verify_ordering(int*,int);

//Struct to hold the parameters to provide to the function each thread is processing
struct Thread_Param{
	int id;//the id of the thread
	int* baselist;// a pointer to the permutation to start calculation from
	int n;//the group to calculate the permutations for; Z/nZ
	unsigned long long int partition_size;//the number of permutations to calculate
	Thread_Param(){

	}
	Thread_Param(int i, int* b, int n_val, unsigned long long int ps){
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
	~Thread_Param(){
	 }
};

//this function confirms or denies if ord is a valid ordering
int verify_ordering(int* ord, int size, int tid){
	int total = 0;
	int n = size+1;
	
	int* elements_seen = new int[size]();
	for(int i=0;i<size;i++){
		total += ord[i];
		int temp_total = total;
		total %= n;
		int* find_result = std::find(elements_seen, elements_seen+size, total);
		if(find_result != elements_seen+size || total==0){
			//delete find_result;
			delete [] elements_seen;
			return 0;
		}else{
			elements_seen[i] = total;
		}
		//delete find_result;
	}
	delete [] elements_seen;
	return 1;
}
int* get_starting_baselist2(int* old_baselist, int n, unsigned long long int partition_size){
	int v = n-1;
	unsigned long long int list_count = 0;
	int* perm_list = new int[v];
	if(old_baselist==NULL){
		for(int i=0;i<v;i++){
			perm_list[i] = i+1;
		}
		return perm_list;
	}
	for(int i=0;i<v;i++){
		perm_list[i] = old_baselist[i];
	}
	do{
		list_count++;
		if(list_count>=partition_size){
			break;
		}
	}while(std::next_permutation(perm_list, perm_list+v));
	return perm_list;
}
//This function calculates the first permutation each thread will start processing
int* get_starting_baselist(int tid, int n, unsigned long long int  partition_size){
	int v = n-1;
	unsigned long long int target_list_count = tid*partition_size;//calculate the target list's position in the permutation order
	unsigned long long int list_count = 0;
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
void print_arr(int* arr, int size,FILE* f=stdout){
	for(int i=0;i<size;i++){
		fprintf(f, "%d,",arr[i]);
	}
	fprintf(f, "\n");
	return;
}

//this function iterates through the permutations each thread is responsible for checking
void* thread_ordering_creator(void* args){
	// Get the Thread_Param struct out of args
	Thread_Param params = *((Thread_Param*)(args));	
	int v = params.n-1;
	int* iterate_list = new int[v];//list used to iterate through
	for(int i=0;i<v;i++){
		iterate_list[i] = params.baselist[i];//deepcopy into iterate_list
	}
	long int good_orderings_calculated = 0;
	unsigned long long int total_orderings_seen = 0;
	do{	
		int is_good_ordering = verify_ordering(iterate_list, v, params.id);
		good_orderings_calculated += is_good_ordering;
		total_orderings_seen++;
		if(total_orderings_seen>params.partition_size-1){
			break;
		}
	}while(std::next_permutation(iterate_list, iterate_list+v));

	delete [] iterate_list;
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
	int thread_mult = 1;
	if(argc>2){
		thread_mult = atoi(argv[2]);
	}
	int max_threads = thread_mult*sysconf(_SC_NPROCESSORS_ONLN);//get the number of online concurrent threads
	int n = atoi(argv[1]);//convert the CLI to an int to process
	//int max_threads = n;
	if(n<(max_threads)/thread_mult){
		fprintf(stderr, "[ERROR] CLI Argument too small; must be greater than the number of concurrent threads, %d\n", max_threads);
		_exit(3);
	}
	std::chrono::time_point<std::chrono::system_clock> start, end;//create the timekeeping variables
	start = std::chrono::system_clock::now();//initialize the first time variable
	
	unsigned long long int total_perms = factorial((long)(n-1));
	unsigned long long int partition_size = (total_perms/max_threads)/2;//calculate the number of lists each thread should process
	printf("MAX THREADS: %d\n", max_threads);
	pthread_t threads[max_threads];//array of threads
	int* baselist = NULL;
	for(int i=0;i<max_threads;i++){//for each thread
		std::chrono::time_point<std::chrono::system_clock> list_s, list_e;
		list_s = std::chrono::system_clock::now();
		baselist = get_starting_baselist2(baselist, n, partition_size);
		list_e= std::chrono::system_clock::now();
		std::chrono::duration<double> list_t = list_e-list_s;
		printf("[LIST %d] Time : %f\n", i, list_t.count());
		Thread_Param tp = {i,baselist, n, partition_size};//create the Thread_Param object to supply to the function provided to each thread
		int t_status = pthread_create(&threads[i], NULL, thread_ordering_creator,(void*)&tp);//create the thread
		usleep(1000);
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
		//printf("value from thread %d: %d\n",i, (int)(long)(results_ptr[i]) );
		mu.lock();
		total_good_perms+= (int)(long)(results_ptr[i]);
		mu.unlock();

		//printf("results from thread %d: total good perms is now : %d\n", i, total_good_perms);
		//delete  (int*)(results_ptr[i]);
		//delete threads_args_ptr[i];
	}
	delete [] results_ptr;
	printf("Pre-mult: %d\n", total_good_perms);
	total_good_perms*=2;
	printf("Final val: %d\n", total_good_perms);
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> time_taken = end-start;
	printf("FINISHED - Total good orderings: %d - Time taken: %f\n", total_good_perms, time_taken.count());
	
	FILE* f = popen("echo -ne '\007' > /dev/tty1","r");//this should beep
	pclose(f);
	return 0;


}