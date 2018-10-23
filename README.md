# Introduction

This program calculates the number of constructive orderings for the group of integers mod n, where n is specified by the user as an argument. This algorithm has an approximate runtime of O(n<sup>2</sup>\*n!), and so the provided value for n should be small (n<20). There are two versions of the algorithm - a singlethreaded and multithreaded version. The multithreaded version detects the number of CPUs present on the current machine to determine how many threads to create. 

# Background
Given the group of integers mod n (Z/nZ), a constructive ordering is a permutation of the group for which the partial sums for each element reproduce the entire group. For example, for the group of integers mod 4, the permutation (0,1,2,3) is a constructive ordering, because the partial sums of 0,1,2 and 3 give 0,1,3 and 2, respectively, thus reproducing the entire group. We seek to find the total number of constructive orderings for the group of integers mod n for a given value of n. These values correspond to the integer sequence [A141599](https://oeis.org/A141599) for even values of n. 

# Installation
Simply run `make` to build both versions. It is currently built using `clang++` and `g++`. Run `make clean` to remove the binaries.
# Usage

### `coSingle`

`./coSingle [n]`
* n is the modulus of the group

### `coMultiC & coMultiG`

`./coMultiC [n] (thread_mult)` or `./coMultiG [n] (thread_mult)`
* n is the modulus of the group
* thread_mult (optional) is used to calculate the total number of threads to use across the program. The total number of threads used is determined by the total number of concurrent threads of the machine times thread_mult. The default value is 1. 
