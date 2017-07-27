import itertools

res_file = None
n = input("Enter a value for n: ")
res_name = "results/results" + str(n) + "Sset_headless"
try:
	res_file = open(res_name,'r')
except:
	print ("err: file not found")

#Check for numbers in positions
'''tup_list = []
for line in res_file:
	exec("tup = " + line)
	tup_list.append(tup)

full_set = set(list(range(n)))
for i in range(1,n):
	existing_nums = set()
	for tup in tup_list:
		existing_nums.add(tup[i])
	print "Numbers not found in position ", i, ": ", full_set - existing_nums


'''

#Frequency Analysis
tup_list = []
for line in res_file:
	exec("tup = " + line)
	tup_list.append(tup)

for i in range(1,n):
	num_counts = [0]*n
	for tup in tup_list:
		num_counts[tup[i]] += 1
	print "Position",i,": ", num_counts1

#Check size 2 tuples #all size 2 tuples not found in the valid permutations add to either n/2 or 0 mod n.
'''tup_list = []
for tup in itertools.product(range(1,n),range(1,n)):
	if tup[0] != tup[1]:#hardcoded for size 2 tuples
		tup_list.append(tup)

all_tuples = set(tup_list)
found_tuples = set()
for line in res_file:
	exec("tup = " + line)
	found_tuples.add(tup[1:3])

missing_tuples = sorted(all_tuples - found_tuples)
print "Total missing: ", len(missing_tuples)
for tup in missing_tuples:
	if tup[0]==n/2:
		pass
	else:
		print tup

subsets_file = open("good_subsets/" + str(n) + "subsets","r")
mod_set = set()
for line in subsets_file:
	exec("tup ="  + line)
	mod_set.add(sum(tup)%n)

print mod_set
'''