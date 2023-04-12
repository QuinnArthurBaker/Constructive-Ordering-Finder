//use cof::tree_search;
use num::{BigUint, FromPrimitive};
use std::thread;
use std::sync::mpsc;
use cof_tree::N;

pub fn tree_search(rem: &mut Vec<usize>, daggers: &mut Vec<bool>, current_sum: &mut usize, total: &mut BigUint){
    if rem.is_empty() {
        *total += 1_usize;
        return;
        //ordering passed, add one to the total number of orderings. 
    }
    if *current_sum == 0 || (rem.len()>1 && *current_sum == N/2) {
        //ordering failed
        return;
    }

    for _i in 0..rem.len() {
        let popped = rem[0];
        //drain (remove) the first element of rem. 
         rem.drain(0..1);
        //rem.retain(|&x|x != popped);
        let save_sum = *current_sum;
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
    //let mut nat = kth_permutation(N, zero.clone());
    let mut rem: Vec<usize> = Vec::new();
    let mut daggers: Vec<bool> = vec![false; N];
	daggers[0] = true;
    for i in 1..N{
        rem.push(i);
    }


    let mut threads = Vec::new();
    let (tx,rx) = mpsc::channel();

    for i in 1..N/2{
		let this_tx = tx.clone();
		let mut this_daggers = daggers.clone();
		let mut this_rem = rem.clone();
		let thandle = thread::spawn(move || {
			this_daggers[i] = true;
			this_rem.retain(|&x| x != i);
			let mut s = i;
			let mut total = BigUint::from_usize(0).unwrap();
			tree_search(&mut this_rem, &mut this_daggers, &mut s, &mut total);
			this_tx.send(total).unwrap();
		});
		threads.push(thandle);
        //rem.push(i);
		//daggers[i] = false;
    }
	
	for handle in threads {
		handle.join().unwrap();
	}	

	let mut total = BigUint::from_usize(0).unwrap();

	let mut recv = rx.try_recv();
	while recv.is_ok() {
		let result = recv.unwrap();
		total += result;
		recv = rx.try_recv();	
	}

	total *= 2_usize;

    println!("Total constructive orderings for N={} - {}", N, total);

}

