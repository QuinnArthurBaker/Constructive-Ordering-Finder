import itertools
import time
def check_subsets(subset, n):
	running_sum = 0
	already_seen_nums = []
	for i in subset:
		running_sum+=i
		running_sum%=n
		if running_sum in already_seen_nums or running_sum == 0:
		#	print "Fail: list seen: ", already_seen_nums
			return False
		else:
			already_seen_nums.append(running_sum)
	return True


def main(size):
	subsets = map(lambda x: x[:int(len(x)/2)],itertools.permutations(range(1,size))) #get the half-sized subsets for verification
	#print subsets
	good_subsets = []
	for subset in subsets:
		#print "Checking ", subset
		if check_subsets(subset, size):
			good_subsets.append(subset)
		#	print "Pass"


	print "Original list: ", len(subsets), "good list: ", len(good_subsets)
	return	

size = int(raw_input("Enter a value for n: ")) # get the value for the subsets to be generated

before = int(time.time()) 

main(size)

after = int(time.time())

print after-before
