# Introduction

This program calculates the number of constructive orderings for the group of integers mod n, where n is specified by the user as an argument. This algorithm has an approximate runtime of O(n<sup>2</sup>\*n!), and so the provided value for n should be small (n<20). There are two versions of the algorithm - a singlethreaded and multithreaded version. The multithreaded version detects the number of CPUs present on the current machine to determine how many threads to create. 
# Installation

Simply run `make` to build both versions. It is currently built using `clang++`. Run `make clean` to remove the binaries.
# Usage

### `fact_sub_single`

`./fact_sub_single [n]`
* n is the modulus of the group

### `fact_sub_multi`

`./fact_sub_multi [n] (thread_mult)`
* n is the modulus of the group
* thread_mult (optional) is used to calculate the total number of threads to use across the program. The total number of threads used is determined by the total number of concurrent threads of the machine times thread_mult. The default value is 1. 