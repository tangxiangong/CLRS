use clrs::{
    chapter2::{insertion_sort, sum_array},
    utils::randn,
};

fn main() {
    println!("Hello, welcome to Chapter 2!");
    let mut arr = randn(0.0, 1.0, 10).unwrap();
    println!("{:?}", arr);
    insertion_sort(&mut arr);
    println!("{:?}", arr);
    println!("Sum of array elements: {}", sum_array(&arr));
}
