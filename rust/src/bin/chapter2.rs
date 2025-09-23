use clrs::{
    chapter2::{insertion_sort, linear_search, selection_sort, sum_array},
    utils::randn,
};

fn main() {
    println!("Hello, welcome to Chapter 2!");
    let mut arr = randn(0.0, 1.0, 10).unwrap();
    let mut arr_clone = arr.clone();
    println!("{:?}", arr);
    insertion_sort(&mut arr);
    println!("{:?}", arr);
    selection_sort(&mut arr_clone);
    println!("{:?}", arr_clone);
    println!("Sum of array elements: {}", sum_array(&arr));
    let target = arr[4];
    let index = linear_search(&arr, &target);
    if let Some(i) = index {
        println!("Found target at index {}", i);
    } else {
        println!("Target not found");
    }
    let index = linear_search(&arr, &10.0);
    if let Some(i) = index {
        println!("Found target at index {}", i);
    } else {
        println!("Target not found");
    }
}
