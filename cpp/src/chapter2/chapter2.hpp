// Implementation of Chapter 2 algorithms.

#ifndef CHAPTER2_HPP
#define CHAPTER2_HPP

#include <concepts>
#include <optional>
#include <utility>
#include <vector>

using std::vector;

template <typename T>
concept LessComparable = requires(const T &a, const T &b) {
    { a < b } -> std::convertible_to<bool>;
};

/*
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

#endif // CHAPTER2_HPP
