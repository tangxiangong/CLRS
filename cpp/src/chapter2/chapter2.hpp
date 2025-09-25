// Implementation of Chapter 2 algorithms.

#ifndef CHAPTER2_HPP
#define CHAPTER2_HPP

#include <algorithm>
#include <concepts>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

using std::vector;

template <typename T>
concept LessComparable = requires(const T &a, const T &b) {
    { a < b } -> std::convertible_to<bool>;
};

/*
 * ```
 * BEGIN A[1:n]
 * for i = 2 to n
 *     key = A[i]
 *     j = i - 1
 *     while j > 0 and A[j] > key
 *         A[j + 1] = A[j]
 *         j = j - 1
 *     A[j + 1] = key
 * ```
 *
 * Insertion sort implementation.
 */
template <LessComparable T> void insertion_sort(vector<T> &arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        T key = arr[i];
        size_t j = i - 1;
        bool overflow = false;
        while (arr[j] > key) {
            arr[j + 1] = arr[j];
            if (j == 0) {
                overflow = true;
                break;
            } else {
                j--;
            }
        }
        if (overflow) {
            arr[0] = key;
        } else {
            arr[j + 1] = key;
        }
    }
}

/**
 * ```
 * BEGIN A[1:n]
 * sum = 0
 * for i = 1 to n
 *     sum = sum + A[i]
 * return sum
 * ```
 *
 * Sum of array elements.
 */
template <typename T>
    requires requires(const T &a, const T &b) {
        { a + b } -> std::convertible_to<T>;
        { T{} } -> std::convertible_to<T>;
    }
auto sum_array(const vector<T> &arr, T initial_value = T{}) -> T {
    T sum = initial_value;
    for (const auto &elem : arr) {
        sum += elem;
    }
    return sum;
}

/**
 * Linear search implementation.
 */
template <typename T>
    requires requires(const T &a, const T &b) {
        { a == b } -> std::convertible_to<bool>;
    }
auto linear_search(const vector<T> &arr, const T &target)
    -> std::optional<size_t> {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return std::nullopt;
}

/**
 * Selection sort implementation.
 */
template <LessComparable T> void selection_sort(vector<T> &arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) {
        size_t min_index = i;
        for (size_t j = i; j < arr.size(); j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        std::swap(arr[i], arr[min_index]);
    }
}

/**
 * Merge sort auxiliary function.
 */
template <typename T>
    requires requires(const T &a, const T &b) {
        { a <= b } -> std::convertible_to<bool>;
    }
void merge(vector<T> &arr, size_t p, size_t q, size_t r) {
    // p..<q
    // q..<r
    size_t left_length = q - p;
    size_t right_length = r - q;
    vector<T> left_arr(left_length);
    vector<T> right_arr(right_length);
    // for (size_t i = 0; i < left_length; i++) {
    //     left_arr[i] = arr[p + i];
    // }
    // for (size_t i = 0; i < right_length; i++) {
    //     right_arr[i] = arr[q + i];
    // }
    std::copy(arr.begin() + p, arr.begin() + q, left_arr.begin());
    std::copy(arr.begin() + q, arr.begin() + r, right_arr.begin());
    // i indexes the smallest remaining element in the left array
    // j indexes the smallest remaining element in the right array
    // k indexes the location in A to fill
    size_t i = 0, j = 0, k = p;
    while (i < left_length && j < right_length) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }
    // while (i < left_length) {
    //     arr[k] = left_arr[i];
    //     i++;
    //     k++;
    // }
    // while (j < right_length) {
    //     arr[k] = right_arr[j];
    //     j++;
    //     k++;
    // }
    std::copy(left_arr.begin() + i, left_arr.end(), arr.begin() + k);
    std::copy(right_arr.begin() + j, right_arr.end(),
              arr.begin() + k + (left_length - i));
}

/**
 *MERGE
 * ```
 * BEGIN A[1:n] p q r
 * n_L = q - p + 1  // length of A[p:q]
 * n_R = r - q  // length of A[q+1:r]
 * let L[0:n_L - 1] and R[0:n_R - 1] be new arrays
 * for i = 0 to n_L - 1  // copy A[p:q] into L[0:n_L - 1]
 *     L[i] = A[p + i]
 * for j = 0 to n_R - 1  // copy A[q+1:r] into R[0:n_R - 1]
 *     R[j] = A[q + 1 + j]
 * i = 0  // i indexes the smallest remaining element in L
 * j = 0  // j indexes the smallest remaining element in R
 * k = p  // k indexes the next position in A to fill
 * // As long as each of the arrays L and R contains an unmerged element,
 * // copy the smallest unmerged element back into A[p:r].
 * while i < n_L and j < n_R
 *     if L[i] <= R[j]
 *         A[k] = L[i]
 *         i = i + 1
 *     else
 *         A[k] = R[j]
 *         j = j + 1
 *     k = k + 1
 * // Having gone through one of L and R entirely, copy the
 * // remaining elements of the other array into A[p:r].
 * while i < n_L
 *     A[k] = L[i]
 *     i = i + 1
 *     k = k + 1
 * while j < n_R
 *     A[k] = R[j]
 *     j = j + 1
 *     k = k + 1
 * END
 * ```
 *
 * MERGE SORT
 * ```
 * BEGIN A[1:n] p r
 * if p >= r  // zero or one element?
 *     return
 * q = ceil((p + r) / 2)
 * MERGE_SORT(A, p, q)
 * MERGE_SORT(A, q, r)
 * MERGE(A, p, q + 1, r)
 * END
 * ```
 *
 * Merge sort implementation
 */
template <typename T>
    requires requires(const T &a, const T &b) {
        { a <= b } -> std::convertible_to<bool>;
    }
void merge_sort(vector<T> &arr) {
    merge_sort_helper(arr, 0, arr.size());
}

/**
 * Merge sort helper function
 */
template <typename T>
    requires requires(const T &a, const T &b) {
        { a <= b } -> std::convertible_to<bool>;
    }
void merge_sort_helper(vector<T> &arr, size_t p, size_t r) {
    if (r - p <= 1) {
        return;
    }
    size_t q = (p + r) / 2;
    merge_sort_helper(arr, p, q);
    merge_sort_helper(arr, q, r);
    merge(arr, p, q, r);
}

/**
 * Recursive insertion sort implementation.
 */
template <typename T>
    requires requires(T a, T b) {
        { a > b } -> std::convertible_to<bool>;
    }
void recursive_insertion_sort(vector<T> &arr) {
    recursive_insertion_sort_helper(arr, arr.size());
}

/**
 * Helper function for recursive insertion sort.
 */
template <typename T>
    requires requires(T a, T b) {
        { a > b } -> std::convertible_to<bool>;
    }
void recursive_insertion_sort_helper(vector<T> &arr, size_t n) {
    // Base case: if array has 1 or 0 elements, it's already sorted
    if (n <= 1) {
        return;
    }

    // Recursively sort the first n-1 elements
    recursive_insertion_sort_helper(arr, n - 1);

    // Insert the nth element into its correct position
    T key = arr[n - 1];
    size_t j = n - 1;

    // Move elements that are greater than key one position ahead
    while (j > 0 && arr[j - 1] > key) {
        arr[j] = arr[j - 1];
        j--;
    }

    // Place key at its correct position
    arr[j] = key;
}

/**
 * ```
 * BEGIN A[1:n]
 * for i = 1 to n - 1
 *     for j = n downto i + 1
 *         if A[j] < A[j - 1]
 *             exchange A[j] with A[j - 1]
 * END
 * ```
 *
 * Bubble sort implementation.
 */
template <typename T>
    requires requires(const T &a, const T &b) {
        { a < b } -> std::convertible_to<bool>;
    }
void bubble_sort(vector<T> &arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) {
        for (size_t j = arr.size() - 1; j > i; j--) {
            if (arr[j] < arr[j - 1]) {
                std::swap(arr[j], arr[j - 1]);
            }
        }
    }
}

/**
 * ```
 * BEGIN A[0:n], x
 * p = 0
 * for i = n downto  0
 *     p = A[i] + x * p
 * return p
 * END
 * ```
 *
 * Horner's method for polynomial evaluation.
 *
 * p(x) = a_0 + a_1 x + a_2 x^2 + ... + a_n x^n
 *
 * a = [a_0, a_1, a_2, ..., a_n]
 */
inline auto horner(const vector<double> &coeff, double x) -> double {
    // double result = 0;
    // for (auto iter = coeff.rbegin(); iter != coeff.rend(); ++iter) {
    //     result = result * x + *iter;
    // }
    // return result;
    return std::ranges::fold_left(
        coeff | std::views::reverse, 0.0,
        [x](double acc, double c) { return acc * x + c; });
}
#endif // CHAPTER2_HPP
