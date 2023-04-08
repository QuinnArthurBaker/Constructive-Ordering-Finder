use cof::{validate_perm, factorial, kth_permutation,next_permutation};
use num::BigUint;
use rand::{SeedableRng, Rng};

#[test]
fn valid_32(){
    let mut p = Vec::new();
    for i in 0..32{
        p.push(i);
    }
    assert_eq!(1, validate_perm(&p));
}
#[test]
fn invalid_32(){
    let mut p = Vec::new();
    for i in 0..32{
        p.push(i);
    }
    p[1] = 2;
    p[2] = 1;
    
    assert_eq!(0, validate_perm(&p));
}

#[test]

fn factorial_10(){
    let ten = BigUint::from(10 as usize);
    let expected = BigUint::from(3628800 as usize);
    assert_eq!(expected, factorial(ten));
}

#[test]
fn next_permutation_0(){
    let k = BigUint::from(0 as usize);
    let mut p = kth_permutation(32, BigUint::from(0 as usize));
    let expected = kth_permutation(32, BigUint::from(1 as usize));
    next_permutation(&mut p);
    assert_eq!(p, expected);
}

#[test]
fn next_permutation_rand(){
    let mut rng = rand::thread_rng();
    let v: usize = rng.gen();
    let B = BigUint::from(v as usize);
    let mut p = kth_permutation(1024, B.clone());
    let mut expected = kth_permutation(1024, B+(1 as usize));
    next_permutation(&mut p);
    assert_eq!(expected, p);
}