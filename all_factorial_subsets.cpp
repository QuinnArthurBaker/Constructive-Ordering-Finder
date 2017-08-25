
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

int verify_permutation (int**& perm, int&num_arrs, int& n){

	int total_good = 0;
	int val = n+1;
	for(int i=0;i<num_arrs;i++){
		int total=0;//holds the running element total
		//cout << "Thread " << this_thread::get_id() << " - Working on permutation " << i << " of " << num_arrs << endl;

		int* elements_seen = new int[n]();//the array to hold the elements in the current permutation
		int elements_seen_index = 0;//index variable for elements_seen
		bool full_set = true;
		//cout << "Starting permutation " << this_thread::get_id() << " " << i << " calc" << endl;
		if(i==num_arrs-1){
			//cout << "Skipping missing permutation" << endl;
			break;
		}
		for(int element=0;element<n;element++){//for each element in the current permutation
			//mu.lock();
			///cout << "Thread " << this_thread::get_id() << " - getting element number " << element <<  " of " << n << endl;
			total += perm[i][element];
			//mu.unlock();
			total%=val;
			int *find_result = find(elements_seen,elements_seen+n,total);
			if( find_result != elements_seen+n || total==0){//if the element has already been seen, break to the next permutation
				// delete find_result;
				full_set = false;
				break;
			}else{
				elements_seen[elements_seen_index] = total;
				elements_seen_index++;
			}
			
		}

		delete [] elements_seen;
		//delete [] perm;
		if(full_set)
			total_good+=1;

		delete [] perm[i];
	}
	delete [] perm;
	cout << "Thread " << this_thread::get_id() << " Done" << endl;
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

	int batch_size = factorial(n)/thread::hardware_concurrency();//batch size is the number of permutations per batch that each thread handles, set as the total number of permutations divided by the number of threadable cores
	int*** perms_batch = new int**[thread::hardware_concurrency()];//perms_batch is a 3D array, which holds a number of arrays of batches equal to the number of threads supported. each batch holds batch_size permutations
	future<int>* workers = new future<int>[thread::hardware_concurrency()];//workers is the array of future objects, the threads to be implemented. There are futures equal to the number of threads able to be implemented
	int* save_list = new int[n];//a container list used to deep copy baselist
	int workers_index = 0;//used to determine which worker is being used and which batch is being assembled
	for(int i=0;i<thread::hardware_concurrency();i++){//for each worker
	//	cout << "Working on worker" << i << endl;
		perms_batch[workers_index] = new int*[batch_size];//initialize each batch 
	//	cout << "Created current worker batch array" << endl;
		while(next_permutation(baselist, baselist+n)){
			index++;
			if(index == factorial(n)-1) index++;
			perms_batch[workers_index][index%batch_size] = new int[n]();//initialize the container for the current permutation in the batch
			//cout << "Creating individual permutation list" << endl;
			copy(baselist,baselist+n, perms_batch[i][index%batch_size]);//copy the permutation from baselist to perms_batch
			//cout << "Copied list over" << endl;
			if(index%batch_size==0 && index!=0){//if the batch is full
				//cout << "Launching thread " << workers_index << endl;
				workers[workers_index] = async(launch::async, verify_permutation, ref(perms_batch[workers_index]),ref(batch_size),ref(n));//create the future and load its job
				workers_index++;//get ready for the next batch/worker
				//cout << "Thread launched" << endl;
				copy(baselist, baselist+n, save_list);//copy baselist over so we don't lose our spot in the permutations
				break;//break out of the while loop loading the batch
			}
			
			//valid_subsets += verify_permutation(baselist, n);
			/*perms_batch[index%batch_size] = new int[n];

			copy(baselist,baselist+n,perms_batch[index%batch_size]);
			workers[index%batch_size] = async(launch::async, verify_permutation, ref(perms_batch[index%batch_size]), ref(n));
			if(index%batch_size==0 && index!=0){
				for(int i=0;i<batch_size;i++){
					valid_subsets += workers[i].get();
					delete [] perms_batch[i];
				}

				delete [] workers;
				workers = new future<int>[batch_size];
			}
			index++;*/
		}
		//cout << "Copying list back to baselist" << endl;
		copy(save_list, save_list+n, baselist);//copy baselist back over into its regular container
		//cout << "baselist is: ";
		//cout << "List copied" << endl;
	}
	
	//int final_batch_size = index%batch_size;
	//workers[workers_index] = async(launch::async, verify_permutation, ref(perms_batch[workers_index]), ref(final_batch_size) ,ref(n));
	
//	print_arr(baselist, n);
	//cout << "Preparing for thread retrieval" << endl;
	cout << thread::hardware_concurrency() << endl;
	for(int i=0;i<thread::hardware_concurrency();i++){
	//	cout << "Getting thread " << i << endl;
		valid_subsets += workers[i].get();
	//	cout << "finished with thread " << i << endl;
	}
	delete [] workers;
	delete [] perms_batch; 
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

