//use cof::tree_search;
use num::{BigUint, FromPrimitive};

const N: usize = 18;

pub fn tree_search(rem: &mut Vec<usize>, daggers: &mut Vec<bool>, current_sum: &mut usize, total: &mut BigUint) -> (){
    if rem.len() == 0 {
        *total += 1 as usize;
        return;
        //ordering passed, add one to the total number of orderings. 
    }
    if *current_sum == 0 || (rem.len()>1 && *current_sum == N/2) {
        //ordering failed
        return;
    }

    for _i in 0..rem.len(){
        let popped = rem[0];
        rem.retain(|&x|x != popped);
        let save_sum = current_sum.clone();
        *current_sum += popped;
        *current_sum %= N; 

        if !daggers[*current_sum]{
            daggers[*current_sum] = true;
            tree_search(rem, daggers, current_sum, total);
			daggers[*current_sum] = false;
        }      
        *current_sum = save_sum;
        rem.push(popped); 
    }
}


fn main() {
    println!("Hello, world!");
    let zero = BigUint::from_usize(0).unwrap();
    //let mut nat = kth_permutation(N, zero.clone());
    let mut rem: Vec<usize> = Vec::new();
    let mut daggers: Vec<bool> = vec![false; N];
	daggers[0] = true;
    for i in 1..N{
        rem.push(i);
    }

    let mut total = zero.clone();
    for i in 1..N/2{
        daggers[i] = true;
        rem.retain(|&x| x != i);
        let mut s = i;
        tree_search(&mut rem, &mut daggers, &mut s, &mut total);
        rem.push(i);
		daggers[i] = false;
    }

    println!("Total constructive orderings for N={} - {}", N, total*2 as usize);




}

