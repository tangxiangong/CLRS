// Implementation of insertion sort algorithm and merge sort algorithm.

#ifndef CHAPTER2_HPP
#define CHAPTER2_HPP

#include <concepts>
#include <optional>
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

#endif // CHAPTER2_HPP
