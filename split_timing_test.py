import time
import itertools

ite = itertools.permutations(range(1,14))
ite = itertools.islice(ite,0,None,720)

a = time.time()
for i in ite:
	print i
b = time.time()

print "With split:", b-a

# ite = itertools.permutations(range(12))
# a = time.time()
# for i in ite:
# 	pass
# b = time.time()
# print "Without split:", b-a