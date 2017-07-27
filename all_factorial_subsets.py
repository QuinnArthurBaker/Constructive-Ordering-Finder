import itertools #Used for calculating Permutations
import math 
import time #timing commands
import multiprocessing #used for processing subsets faster
import concurrent.futures #used for multiprocessing


'''
Unneccesary; showing you get the entire set back is sufficient; used to see if a set is closed under addition modulo mod
'''
def is_closed(l,mod):
	prod_list = list(itertools.product(l,l))
	for i in prod_list:
		t = (i[0]+i[1])%mod
		if t not in l:
			return False

	return True

#Used to verify good subsets
def check_subsets(subset, n):
	running_sum = 0
	already_seen_nums = []
	if subset[0] == n/2: #if the first element in the ordering is n/2, we know the ordering is invalid, so skip it
		return False
	for i in subset:
		running_sum+=i
		running_sum%=n
		if running_sum in already_seen_nums or running_sum == 0:
			return False
		else:
			already_seen_nums.append(running_sum)
	return True

#used to run through all possible subsets which would constitute the beginning of orderings
def subset_processing(size):
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
	gsf.close()
	return	


#Beginning of Program
method = raw_input("which method (a|b|c): ") #determine the method of calculation; method a calculates all Z/nZ for n up to the provided value. method b only calculates Z/nZ for the provided value
max_n = int(input("Enter the largest value for n: ")) #the value for n in Z/nZ
subset_check = raw_input("use known subsets? (y|n): ") 

use_subsets = True
if subset_check == 'n':
	use_subsets = False

#open the files for writing

sizes_f = None
results_f = None
results_headless_f = None 
iset = [] # the list which holds the values for n to iterate through
if method == 'a':
	sizes_f = open("sizes/sizes" + str(max_n) + "set","w")
	results_f = open("results/results" + str(max_n) + "set","w")
	iset = range(2,max_n+1)
elif method == 'b':
	sizes_f = open("sizes/sizes" + str(max_n) + "Sset","w")
	results_f = open("results/results" + str(max_n) + "Sset","w")
	results_headless_f = open("results/results" + str(max_n) + "Sset_headless", "w")
	iset = range(max_n,max_n+1)
elif method == 'c':
	sizes_f = open("/dev/null","w")
	results_f = open("/dev/null","w")
	results_headless_f = open("/dev/null","w")
	iset = range(max_n, max_n+1)

before = int(time.time()) #get the initial timing
for n in iset:#for each modular group between Z/14Z and Z/16Z

	if n%2==1: #we know that if n is odd, no ordering will reproduce the whole set
		continue

	if not __import__("os").path.exists("good_subsets/"+str(n)+"subsets"):
		#Load the file if it exists, otherwise create it
		print "Creating subset list"
		subset_processing(n)
	#Load the now made file here
	subsets_file = open("good_subsets/" + str(n) + "subsets")
	subset_collection = set()
	for line in subsets_file:
		exec("tup = " + line)
		subset_collection.add(tup)
	results_f.write("##############################\r\nZ/"+str(n)+"Z\r\n")#write the header for each group
	group = range(n)#create the group (0,1,...i-1)

	fact_set_count = 0 #initialize variable to count factorial sets which are subgroups
	iter_obj = itertools.permutations(group)#create the iterable which contains all permutations of the group to create all the orderings for the factorial sets

	results_f.flush()
	
	for perm in iter_obj:#for each ordering
		tot = 0#initialize the factorial variable
		factorial_list = [] #create the list to append each factorial result
		if perm[0] != 0: #We know that if 0 is not the least element in the group, no ordering will reproduce the entire set
			continue

		if use_subsets:
			if perm[1:int(n/2)+1] not in subset_collection: #ensure that the beginning of potential permutations match the beginnings of known valid permutation beginnings
				continue
		for item in perm: #for each number in the permutation
			tot += item
			if tot%len(group) in factorial_list or tot%n==0 or tot%n==n/2: #if the element is already in the list, we know that the full set cannot be reproduced, so stop calcuating this ordering
				break
			else:
				factorial_list.append(tot%len(group))#append each factorial to the factorial_list
		
			



		factorial_set = set(factorial_list) #create a set of the factorial_list, removing duplicates
		#Subgroup stuff
		'''''''''''''''''''''''
		if len(group)%len(factorial_set)==0 and 0 in factorial_set: #if the factorial set (1) satisfies Lagranges Theorem, and (2) contains the identity, it passes the initial test of being a subgroup
			if is_closed(factorial_set,len(group)) or len(factorial_set)==len(group): #if the factorial set is closed, or the length is the same as the original group, then factorial_set is indeed a subgroup
				fact_set_count += 1 #increment the number of factorial_sets which are subgroups
				results_f.write(str(perm)+"\n"+str(factorial_set)+"\r\n\r\n")#write the results to the result file
				if len(factorial_set)!=len(group):#if the length of the subgroup isnot the size of the original group
					results_f.write("!!!!\n") #write a line for searching (subgroups not the size of the original group are interesting)
		'''''''''''''''''''''''''''

		#now that we have the set we want to check to see if the factorial set is equal to the original set
		if factorial_set == set(range(n)):
			fact_set_count += 1
			results_f.write(str(perm) + "\r\n")
			results_headless_f.write(str(perm) + "\r\n")





		results_f.flush()#push the results to the results file
		results_headless_f.flush()
					
	#at the end of each group 
	sizes_f.write(str(n)+" "+str(fact_set_count)+" "+str(math.factorial(n))+"\r\n") #write the group number, the number of factorial sets, and i! to the sizes file
	results_f.write("###############################\r\n") #write the footer for each group

	#push the buffers to each file before checking the next group
	sizes_f.flush()
	results_f.flush()

after = int(time.time())
print after-before
#close each file
	
sizes_f.close()
results_f.close()
results_headless_f.close()
	
