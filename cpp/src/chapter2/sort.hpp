// Implementation of insertion sort algorithm and merge sort algorithm.

#ifndef SORT_HPP
#define SORT_HPP

#include <concepts>
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

#endif // SORT_HPP
