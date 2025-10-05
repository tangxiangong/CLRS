// Chapter 4 Divide and Conquer

#include "matrix.hpp"
#include <cassert>

/**
 * MATRIX-MULTIPLY A, B, C, n
 * ```
 * for i = 1 to n
 *     for j = 1 to n
 *         for k = 1 to n
 *             C[i, j] = C[i, j] + A[i, k] * B[k, j]
 * ```
 */
template <typename T>
    requires requires(T &&a, T &&b) {
        { a + b } -> std::convertible_to<T>;
        { a * b } -> std::convertible_to<T>;
        { a += b } -> std::same_as<T&>;
    }
void matrix_multiply(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C) {
    assert(A.ncols() == B.nrows() && A.nrows() == C.nrows() &&
           B.ncols() == C.ncols());
    for (size_t i = 0; i < A.nrows(); ++i) {
        for (size_t j = 0; j < B.ncols(); ++j) {
            for (size_t k = 0; k < A.ncols(); ++k) {
                C[i, j] += (A[i, k] * B[k, j]);
            }
        }
    }
}
