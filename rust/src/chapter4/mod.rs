// Chapter 4 Divide and Conquer

use crate::matrix::Matrix;
use std::ops::{Add, AddAssign, Mul};

/// MATRIX-MULTIPLY A, B, C, n
/// ```
/// for i = 1 to n
///     for j = 1 to n
///         for k = 1 to n
///             c_{i,j} = c_{i,j} + a_{i,k} * b_{k,j}
/// ```
///
/// C = C + A * B
#[allow(non_snake_case)]
pub fn matrix_multiply<T>(A: &Matrix<T>, B: &Matrix<T>, C: &mut Matrix<T>)
where
    T: Copy + Add<Output = T> + Mul<Output = T> + AddAssign,
{
    assert_eq!(A.ncols(), B.nrows());
    assert_eq!(A.nrows(), C.nrows());
    assert_eq!(B.ncols(), C.ncols());
    for i in 0..A.nrows() {
        for j in 0..B.ncols() {
            for k in 0..A.ncols() {
                C[(i, j)] += A[(i, k)] * B[(k, j)];
            }
        }
    }
}
