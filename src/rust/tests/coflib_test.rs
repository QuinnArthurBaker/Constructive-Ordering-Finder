use cof::{validate_perm, factorial};
use num::BigUint;

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
