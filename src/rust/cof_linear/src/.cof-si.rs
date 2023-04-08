use std::env;
use std::time::Instant;

fn main() {

	let args: Vec<_> = env::args().collect();
	if args.len() < 2 {
		println!("Must provide a value for n, exiting");
		std::process::exit(1);
	}
	let n: u32 = args[1].parse::<u32>().unwrap();

	println!("n/2 is: {}", n/2);
	let mut perm_index = 0;
	let mut num_orderings = 0;
	let duration = Instant::now();
	loop {
		
		let mut cur_index =   erm_index;
		let cur_perm: Vec<u32> = get_perm_by_index(n, &mut cur_index);
		if cur_perm[0] == n/2 {
			break;
		}
		if verify_perm(cur_perm, n) {
			num_orderings += 1;
		}
		perm_index += 1;
	}
	num_orderings *= 2;
	println!("run complete. Time is: {}ms (~{}s)", duration.elapsed().as_millis(), duration.elapsed().as_secs());
	println!("num_orderings is: {}", num_orderings);

}

fn get_perm_by_index(n: u32, index: &mut u128) -> Vec<u32>{
	let mut factorial_vec = Vec::new();
	let mut l_index = 0;
	while l_index < n-1 {
		let coef: u128 = (*index as u128) / factorial(n-l_index-1);
		factorial_vec.push(coef);
		*index = *index - factorial(n-l_index-1) * coef;
		l_index += 1;
	}
	
	

	let mut ordered_vec = Vec::new();
	l_index = 0;
	while l_index < n {
		ordered_vec.push(l_index);
		l_index += 1;
	}
	
	let mut ordering = Vec::new();

	l_index = 0;
	 while l_index < n-1 {
	 	//calculate the index of the value we want to pull from the ordered vector	
	 	let ordered_vec_index: usize = factorial_vec[l_index as usize] as usize;
	 	//add the appropriate value to the ordering vector
	 	ordering.push(ordered_vec[ordered_vec_index]);
	 	//remove the pushed value from the ordered vector
	 	ordered_vec.remove(ordered_vec_index);

	 	l_index+=1;
	}
	//add the final element from the ordered vector to the ordering vector
	ordering.push(ordered_vec[0]);
	//remove the leading element (to remove zeros in desired orderings for better calculation)
	ordering.remove(0);
	//println!("ordering: {:?}", ordering);
	ordering
	//println!("{:?}: size {}", ordering, ordering.len());

}
/** Simple recursive implementation. Uses u128 because factorials grow quickly */
fn factorial (n: u32) -> u128 {
	if n < 2 {
		return 1 as u128;
	}
	n as u128 * factorial(n-1)
}

/** */
fn verify_perm(perm: Vec<u32>, n: u32) -> bool {
	let mut l_index: usize = 0;
	let mut running_total = 0;
	let mut seen_bools: Vec<bool> = vec![false; n as usize];
	while l_index < perm.len() {
		running_total += perm[l_index];
		running_total = running_total % n;
		if running_total == 0 || seen_bools[running_total as usize] || (running_total==n/2 && l_index as u32 != n-2){
			return false;
		}
		seen_bools[running_total as usize] = true;
		l_index += 1;
	}
	return true;
}