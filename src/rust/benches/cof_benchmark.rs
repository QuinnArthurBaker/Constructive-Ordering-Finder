use std::clone;

use criterion::{black_box, criterion_group, criterion_main, Criterion, BenchmarkId};
use cof::{factorial, validate_perm};
use num::{BigUint, ToPrimitive, pow};

pub fn criterion_benchmark_valid_ordering(c: &mut Criterion){
    let mut p = Vec::new();
    for i in 0..32{
        p.push(i);
    }
    c.bench_function("valid 32", |b| b.iter(||{
        validate_perm(black_box(p.clone()));
    }));
}

pub fn criterion_benchmark_invalid_ordering(c: &mut Criterion){
    let mut p = Vec::new();
    for i in 0..32{
        p.push(i);
    }
    p[1] = 2;
    p[2] = 1;
    c.bench_function("invalid 32", |b| b.iter(||{
        validate_perm(black_box(p.clone()));
    }));
}

pub fn criterion_benchmark_valid_ordering_large(c: &mut Criterion){
    let mut p = Vec::new();
    for i in 0..1024{
        p.push(i);
    }
    c.bench_function("valid 1024", |b| b.iter(||{
        validate_perm(black_box(p.clone()));
    }));

}
pub fn criterion_benchmark_fact(c: &mut Criterion){
    let input = BigUint::from(100 as usize);
    let mut vals = Vec::new();
    vals.push(BigUint::from(0 as usize));
    for i in 0..3 {
        vals.push(BigUint::from(pow(10, i) as usize));
    }

    let mut group = c.benchmark_group("biguint factorial");

    for val in vals {
        group.bench_with_input(BenchmarkId::from_parameter(val.clone()), &val, |b, i| {
            b.iter(|| factorial(i.clone()))
        });
    }
    group.finish();

    /*c.bench_with_input(BenchmarkId::new("BigUInt Factorial", &input), &input, |b, i| b.iter( || {
        factorial(i.clone());
    }));
    */
}

/*
pub fn criterion_benchmark_kth_perms(c: &mut Criterion){
    let div = BigUint::from(64 as usize);
    let step: BigUint = factorial(BigUint::from(31 as usize))/div.clone();

    let mut steps = Vec::new();
    for i in 0..div.clone().to_usize().unwrap() {
        steps.push(&step*(i as usize));
    } 

    let mut group = c.benchmark_group("kth_perm n=32 split=64");
    for index in steps.into_iter() {
        group.bench_with_input(BenchmarkId::from_parameter(index.clone()), &index, |b, i| {
            b.iter(|| cof::kth_permutation(32, i.clone()))
        });
    }
}
*/
//criterion_group!(benches, criterion_benchmark_invalid_ordering);
criterion_group!(unit_benches, criterion_benchmark_fact, criterion_benchmark_valid_ordering_large, criterion_benchmark_valid_ordering, criterion_benchmark_invalid_ordering);
criterion_main!(unit_benches);
