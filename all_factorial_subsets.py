import itertools #Used for calculating Permutations
import math 
import time #timing commands


'''
Currently unused; used to see if a set is closed under addition modulo mod
'''
def is_closed(l,mod):
	prod_list = list(itertools.product(l,l))
	for i in prod_list:
		t = (i[0]+i[1])%mod
		if t not in l:
			return False

	return True

method = raw_input("which method (a|b): ") #determine the method of calculation; method a calculates all Z/nZ for n up to the provided value. method b only calculates Z/nZ for the provided value
max_n = int(input("Enter the largest value for n: ")) #the value for n in Z/nZ

#open the files for writing

sizes_f = None
results_f = None

iset = [] # the list which holds the values for n to iterate through
if method == 'a':
	sizes_f = open("sizes/sizes" + str(max_n) + "set","a")
	results_f = open("results/results" + str(max_n) + "set","a")
	iset = range(2,max_n+1)
else:
	sizes_f = open("sizes/sizes" + str(max_n) + "Sset","a")
	results_f = open("results/results" + str(max_n) + "Sset","a")
	iset = range(max_n,max_n+1)

before = int(time.time()) #get the initial timing
for n in iset:#for each modular group between Z/14Z and Z/16Z

	if n%2==1: #we know that if n is odd, no ordering will reproduce the whole set
		continue

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


		for item in perm: #for each number in the permutation
			tot += item
			if tot%len(group) in factorial_list: #if the element is already in the list, we know that the full set cannot be reproduced, so stop calcuating this ordering
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





		results_f.flush()#push the results to the results file
					
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
	
