//use cof::tree_search;
use num::{BigUint, FromPrimitive};

const N: usize = 20;

pub fn tree_search(rem: &mut Vec<usize>, daggers: &mut Vec<usize>, current_sum: &mut usize, total: &mut BigUint) -> (){
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

        if !daggers.contains(current_sum){
            daggers.push(*current_sum);
            tree_search(rem, daggers, current_sum, total);
            daggers.retain(|&x| x != *current_sum);
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
    let mut daggers: Vec<usize> = Vec::new();
    for i in 1..N{
        rem.push(i);
    }

    let mut total = zero.clone();
    for i in 1..N/2{
        daggers.push(i);
        rem.retain(|&x| x != i);
        let mut s = i;
        tree_search(&mut rem, &mut daggers, &mut s, &mut total);
        rem.push(i);
        let front = daggers[0];
        daggers.retain(|&x| x!=front);
    }

    println!("Total constructive orderings for N={} - {}", N, total*2 as usize);




}

