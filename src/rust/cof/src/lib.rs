use num::{BigUint, ToPrimitive};

#[allow(non_snake_case)]
pub fn factorial(input: BigUint) -> BigUint {
    let ONE = BigUint::from(1 as usize);
    let ZERO = BigUint::from(0 as usize);
    if input != ONE && input != ZERO {
        let mut prod = ONE.clone();
        for m in ONE.to_usize().unwrap()..=input.to_usize().unwrap() {
            prod = &prod * m;
        }
        return prod;
    } 
    ONE
    
}

pub fn validate_perm(perm: &[usize]) -> usize{
    //         This is where you would apply your function to the permutation
    // println!("Permutation: {:?}", perm);
    let n = perm.len();
    let mut seen = vec![false;n];
    let mut sum: usize = 0;
    for e in perm {
        sum += e;
        sum = sum % n;

        if seen[sum] {
            return 0;
        }

        seen[sum] = true;
    }
    1
    
}

pub fn kth_permutation(n: usize, mut k: BigUint) -> Vec<usize> {
    //let mut result: Vec<usize> = (0..n).collect();
    let mut tuple: Vec<usize> = Vec::new();
    

    for i in 0..n-1{
        let coef = &k;
        let denom = BigUint::from(n-i-1);
        let factdenom = factorial(denom);

        let coef = coef/factdenom.clone();
        let small_coef = coef.to_usize();
        tuple.push(small_coef.unwrap());
        let round = factdenom*coef;
        k = k - round;
    }
    //println!("tuple: {:?}", tuple);
    let mut ordered_list: Vec<usize> = Vec::new();
    for i in 0..n {
        ordered_list.push(i);
    }
    //println!("orderedList: {:?}", orderedList);

    let mut result: Vec<usize> = Vec::new();
    for i in 0..n-1{
        let e = ordered_list[tuple[i]];
        result.push(e);
        ordered_list.remove(tuple[i]);
    }
    //println!("result: {:?}", result); 
    result.push(ordered_list.pop().unwrap());

    result 
  
}


/*
next_permutation courtesy of Project Nayuki
https://www.nayuki.io/page/next-lexicographical-permutation-algorithm 
 */
pub fn next_permutation<T: Ord>(array: &mut [T]) -> bool {
	// Find non-increasing suffix
	if array.is_empty() {
		return false;
	}
	let mut i: usize = array.len() - 1;
	while i > 0 && array[i - 1] >= array[i] {
		i -= 1;
	}
	if i == 0 {
		return false;
	}
	
	// Find successor to pivot
	let mut j: usize = array.len() - 1;
	while array[j] <= array[i - 1] {
		j -= 1;
	}
	array.swap(i - 1, j);
	
	// Reverse suffix
	array[i .. ].reverse();
	true
}