import multiprocessing
import itertools
import time
import concurrent.futures
import math



def main(size):
	gsf = open("good_subsets/"+str(size)+"subsets","w")
	subsets = set(map(lambda x: x[:int(size/2)],itertools.islice(itertools.permutations(range(1,size)),0,None,math.factorial(int(size/2)-1)))) #get the half-sized subsets for verification
	#print subsets
	
	with concurrent.futures.ThreadPoolExecutor(max_workers=multiprocessing.cpu_count()) as executor:
		good_subsets = []
		future_results = {executor.submit(check_subsets,subset,size): subset for subset in subsets}
		for future in concurrent.futures.as_completed(future_results):
				subset = future_results[future]
				try:
					if future.result():
						good_subsets.append(subset)
						gsf.write(str(subset)+"\n")
						gsf.flush()
				except Exception as e:
					print e

		# for subset in subsets:
		# 	#print "Checking ", subset
		# 	if check_subsets(subset, size):
		# 		good_subsets.append(subset)
		# 		gsf.write(str(subset) + "\n")
		# 	#	print "Pass"
		# for s in good_subsets:
		# 	gsf.write(str(s)+"\n")

	print "Original list: ", len(subsets), "good list: ", len(good_subsets)
	return	

size = int(raw_input("Enter a value for n: ")) # get the value for the subsets to be generated

before = int(time.time()) 

main(size)

after = int(time.time())

print after-before
