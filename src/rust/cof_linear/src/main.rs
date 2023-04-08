use cof::{factorial, kth_permutation, validate_perm, next_permutation};
use num::BigUint;
use std::sync::mpsc;
use std::thread;
use std::env;
use std::process;

#[allow(dead_code)]
struct ThreadResult {
    id: usize,
    sum: BigUint,
}

pub fn main() {
    //program constants
    let args: Vec<String> = env::args().collect();
    if args.len() < 2{
        eprintln!("Missing required command line argument [n]");
        process::exit(1);
    }
    let n: usize = args[1].parse::<usize>().expect("n should be a positive integer");
    let num_threads: usize = num_cpus::get();
    let two = BigUint::from(2 as usize);

    let partition_size: BigUint = BigUint::from(n - 1);
    
    let partition_size = (factorial(partition_size)) / num_threads;
    let partition_size = partition_size / &two;
    // Create shared mutable state for the results of each thread
    //let thread_result1 = Arc::new(Mutex::new(ThreadResult { sum: 0 }));
    //let thread_result2 = Arc::new(Mutex::new(ThreadResult { sum: 0 }));
    let mut threads = Vec::new();
    let (tx, rx) = mpsc::channel();
    for threadid in 0..num_threads {
        let this_tx = tx.clone();
        let index: BigUint = &partition_size * threadid;
        let end_index = &partition_size * (threadid + 1);  
        let thandle = thread::spawn(move || {
            let mut p = kth_permutation(n, index.clone());
            let mut tracker = index.clone();
            let mut ord_count: BigUint = BigUint::from(0 as usize);
            while tracker < end_index {
                next_permutation(&mut p);
                ord_count += validate_perm(&p);
                tracker = tracker + (1 as usize);
            }
            let thread_result = ThreadResult {
                id: threadid,
                sum: ord_count,
            };
            this_tx.send(thread_result).unwrap();
        });
        threads.push(thandle);
    }

    for handle in threads {
        handle.join().unwrap();
    }

    let mut recv = rx.try_recv();
    let mut result_sum = BigUint::from(0 as usize);
    while recv.is_ok() {
        let tr = recv.unwrap();
        result_sum = result_sum + tr.sum;
        recv = rx.try_recv();
    }
    result_sum = result_sum * two;
    println!("Total constructive orderings: {}", result_sum);
}

/*
fn get_starting_perms(n: usize, num_perms: usize) -> Vec<Vec<usize>>{
    let mut perms: Vec<Vec<usize>> = Vec::new();
    let offset: u128 = ((n-1).factorial()/num_perms) as u128;

    for i in 0..num_perms{
        perms.push(kth_permutation(n, (offset*i as u128)));
    }
    perms

}
*/
