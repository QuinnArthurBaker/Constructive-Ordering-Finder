use criterion::{black_box, criterion_group, criterion_main, Criterion, BenchmarkId};
use cof::{factorial, validate_perm, kth_permutation, next_permutation};
use num::{BigUint, pow};

pub fn criterion_benchmark_valid_ordering(c: &mut Criterion){
    let mut p = Vec::new();
    for i in 0..32{
        p.push(i);
    }
    c.bench_function("valid 32", |b| b.iter(||{
        validate_perm(black_box(&p));
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
        validate_perm(black_box(&p));
    }));
}

pub fn criterion_benchmark_valid_ordering_large(c: &mut Criterion){
    let mut p = Vec::new();
    for i in 0..1024{
        p.push(i);
    }
    c.bench_function("valid 1024", |b| b.iter(||{
        validate_perm(black_box(&p));
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
}

pub fn criterion_benchmark_next_perm(c: &mut Criterion){
    let k = BigUint::from(0 as usize);
    let mut p = kth_permutation(1024, k);

    c.bench_function("Next perm 1024", |b| b.iter( || {
        next_permutation(black_box(&mut p));
    }));
      
}

pub fn criterion_benchmark_kth_perm(c: &mut Criterion){
    let one = BigUint::from(1 as usize);
    c.bench_function("kth_perm 1024", |b| b.iter(
        || {kth_permutation(1024, one.clone())}));
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
criterion_group!(unit_benches, criterion_benchmark_fact, criterion_benchmark_valid_ordering_large, criterion_benchmark_valid_ordering, criterion_benchmark_invalid_ordering, criterion_benchmark_next_perm, criterion_benchmark_kth_perm);
criterion_main!(unit_benches);
