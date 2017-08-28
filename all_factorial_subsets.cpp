
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

mutex mu;

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
int verify_permutations(int*& start_list, int& jobs_to_do, int& n){
	mu.lock();
	cout << "Thread " << this_thread::get_id() << " starting with list ";
	mu.unlock();
	print_arr(start_list, n);
	int total_good = 0;
	int val = n+1;
	int index = 0;

	do{
		int total = 0;

		if(index>=jobs_to_do){
			break;
		}
		bool full_set = true;
		for(int i=0;i<n;i++){
			//verify each list here
			int total = 0;
			int* elements_seen = new int[n]();
			for(int element=0;element<n;element++){
				total += start_list[element];
				total%= val;
				int *find_result = find(elements_seen, elements_seen+n,total);
				if(find_result != elements_seen+n || total==0){
					full_set = false;
					delete [] elements_seen;
					break;
				}
				else{
					elements_seen[element] = total;
				}
			}
		}
		if(full_set){
			total_good++;
		}
		index++;

	}while(next_permutation(start_list, start_list+n));
	
	
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

	int* start_list;
	copy(baselist, baselist+n, start_list);
	int range = factorial(n)/thread::hardware_concurrency();
	int total = 0;
	int batch = 0;
	cout << "Range: " << range << "\tTotal permutations: " << factorial(n) << "\tThreads: " << thread::hardware_concurrency() << endl;
	//future<int>* workers = new future<int>[thread::hardware_concurrency()];
	do{
		//print_arr(baselist, n);
		if(index%range==0){
			cout << "Starting batch #" << batch << endl;
			cout << "Assigning worker" << endl;
			//workers[batch] = async(launch::async, verify_permutations, ref(start_list), ref(range), ref(n));			
			total+=verify_permutations(start_list, range, n);
			if(batch<thread::hardware_concurrency()){
				cout << "Copying" << endl;
				copy(baselist, baselist+n, start_list);
			}
			batch++;
			
		}
		index++;

	}while(next_permutation(baselist, baselist+n));
	// for(int i=0;i<thread::hardware_concurrency();i++){
	// 	cout << "getting results from worker " << i << endl;
	// 	int future_result = workers[i].get();
	// 	cout << "result from worker 1: " << future_result << endl;
	// 	total+= future_result;
	// 	cout << "Results recieved" << endl;
	// }
	cout << "Outside of loop" << endl;
	//delete [] workers;
	delete [] baselist;
	cout << "Total: " << total << endl;
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

