/// ```
/// BEGIN A[1:n]
/// for i = 2 to n
///     key = A[i]
///     j = i - 1
///     while j > 0 and A[j] > key
///         A[j + 1] = A[j]
///         j = j - 1
///     A[j + 1] = key
/// ```
///
/// Insertion sort implementation.
pub fn insertion_sort<T>(arr: &mut [T])
where
    T: PartialOrd + Clone,
{
    for i in 1..arr.len() {
        let mut j = i - 1;
        let mut overflow = false;
        let key = arr[i].clone();
        while arr[j] > key {
            arr[j + 1] = arr[j].clone();
            if j == 0 {
                overflow = true;
                break;
            } else {
                j -= 1;
            }
        }
        if overflow {
            arr[0] = key;
        } else {
            arr[j + 1] = key;
        }
    }
}

/// Recursive insertion sort implementation.
pub fn recursive_insertion_sort<T>(arr: &mut [T])
where
    T: PartialOrd + Clone,
{
    recursive_insertion_sort_helper(arr, arr.len());
}

/// Helper function for recursive insertion sort.
fn recursive_insertion_sort_helper<T>(arr: &mut [T], n: usize)
where
    T: PartialOrd + Clone,
{
    // Base case: if array has 1 or 0 elements, it's already sorted
    if n <= 1 {
        return;
    }

    // Recursively sort the first n-1 elements
    recursive_insertion_sort_helper(arr, n - 1);

    // Insert the nth element into its correct position
    let key = arr[n - 1].clone();
    let mut j = n - 1;

    // Move elements that are greater than key one position ahead
    while j > 0 && arr[j - 1] > key {
        arr[j] = arr[j - 1].clone();
        j -= 1;
    }

    // Place key at its correct position
    arr[j] = key;
}

/// ```
/// BEGIN A[1:n]
/// sum = 0
/// for i = 1 to n
///     sum = sum + A[i]
/// return sum
/// ```
///
/// Sum of array elements.
pub fn sum_array<'a, T>(arr: &'a [T]) -> T
where
    T: std::ops::Add<&'a T, Output = T>,
    T: Default,
{
    arr.iter().fold(T::default(), |acc, x| acc + x)
}

/// Sum of array elements with initial value.
pub fn sum_array_with_initial_value<'a, T>(arr: &'a [T], initial_value: T) -> T
where
    T: std::ops::Add<&'a T, Output = T>,
{
    arr.iter().fold(initial_value, |acc, x| acc + x)
}

/// Linear search implementation.
pub fn linear_search<T>(arr: &[T], target: &T) -> Option<usize>
where
    T: PartialEq,
{
    for (i, item) in arr.iter().enumerate() {
        if item == target {
            return Some(i);
        }
    }
    None
}

/// Selection sort implementation.
pub fn selection_sort<T>(arr: &mut [T])
where
    T: PartialOrd,
{
    for i in 0..arr.len() - 1 {
        let mut min_index = i;
        for j in i..arr.len() {
            if arr[j] < arr[min_index] {
                min_index = j;
            }
        }
        arr.swap(i, min_index);
    }
}

fn merge<T>(arr: &mut [T], p: usize, q: usize, r: usize)
where
    T: PartialOrd + Clone,
{
    let left_length = q - p;
    let right_length = r - q;
    let left_arr = arr[p..q].to_vec();
    let right_arr = arr[q..r].to_vec();
    let mut i = 0;
    let mut j = 0;
    let mut k = p;
    while i < left_length && j < right_length {
        if left_arr[i] <= right_arr[j] {
            arr[k] = left_arr[i].clone();
            i += 1;
        } else {
            arr[k] = right_arr[j].clone();
            j += 1;
        }
        k += 1;
    }
    // while i < left_length {
    //     arr[k] = left_arr[i].clone();
    //     i += 1;
    //     k += 1;
    // }
    // while j < right_length {
    //     arr[k] = right_arr[j].clone();
    //     j += 1;
    //     k += 1;
    // }
    if i < left_length {
        arr[k..k + (left_length - i)].clone_from_slice(&left_arr[i..]);
    } else if j < right_length {
        arr[k..k + (right_length - j)].clone_from_slice(&right_arr[j..]);
    }
}

/// MERGE
/// ```
/// BEGIN A[1:n] p q r
/// n_L = q - p + 1  // length of A[p:q]
/// n_R = r - q  // length of A[q+1:r]
/// let L[0:n_L - 1] and R[0:n_R - 1] be new arrays
/// for i = 0 to n_L - 1  // copy A[p:q] into L[0:n_L - 1]
///     L[i] = A[p + i]
/// for j = 0 to n_R - 1  // copy A[q+1:r] into R[0:n_R - 1]
///     R[j] = A[q + 1 + j]
/// i = 0  // i indexes the smallest remaining element in L
/// j = 0  // j indexes the smallest remaining element in R
/// k = p  // k indexes the next position in A to fill
/// // As long as each of the arrays L and R contains an unmerged element,
/// // copy the smallest unmerged element back into A[p:r].
/// while i < n_L and j < n_R
///     if L[i] <= R[j]
///         A[k] = L[i]
///         i = i + 1
///     else
///         A[k] = R[j]
///         j = j + 1
///     k = k + 1
/// // Having gone through one of L and R entirely, copy the
/// // remaining elements of the other array into A[p:r].
/// while i < n_L
///     A[k] = L[i]
///     i = i + 1
///     k = k + 1
/// while j < n_R
///     A[k] = R[j]
///     j = j + 1
///     k = k + 1
/// END
/// ```
///
/// MERGE SORT
/// ```
/// BEGIN A[1:n] p r
/// if p >= r  // zero or one element?
///     return
/// q = ceil((p + r) / 2)
/// MERGE_SORT(A, p, q)
/// MERGE_SORT(A, q, r)
/// MERGE(A, p, q + 1, r)
/// END
/// ```
///
/// Merge sort implementation.
pub fn merge_sort<T>(arr: &mut [T])
where
    T: PartialOrd + Clone,
{
    merge_sort_helper(arr, 0, arr.len());
}

fn merge_sort_helper<T>(arr: &mut [T], p: usize, r: usize)
where
    T: PartialOrd + Clone,
{
    if r - p <= 1 {
        return;
    }
    let q = (p + r) / 2;
    merge_sort_helper(arr, p, q);
    merge_sort_helper(arr, q, r);
    merge(arr, p, q, r);
}

/// ```
/// BEGIN A[1:n]
/// for i = 1 to n - 1
///     for j = n downto i + 1
///         if A[j] < A[j - 1]
///             exchange A[j] with A[j - 1]
/// END
/// ```
///
/// Bubble sort implementation.
pub fn bubble_sort<T>(arr: &mut [T])
where
    T: PartialOrd,
{
    for i in 0..arr.len() - 2 {
        for j in (i + 1..arr.len()).rev() {
            if arr[j] < arr[j - 1] {
                arr.swap(j, j - 1);
            }
        }
    }
}

/// ```
/// BEGIN A[0:n], x
/// p = 0
/// for i = n downto  0
///     p = A[i] + x * p
/// return p
/// END
/// ```
///
/// Horner's method for polynomial evaluation.
///
/// p(x) = a_0 + a_1 x + a_2 x^2 + ... + a_n x^n
///
/// a = [a_0, a_1, a_2, ..., a_n]
pub fn horner(arr: &[f64], x: f64) -> f64 {
    // let mut result = 0.0;
    // for &a in arr.iter().rev() {
    //     result = result * x + a;
    // }
    // result
    arr.iter().rev().fold(0.0, |acc, &a| acc * x + a)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_insertion_sort() {
        let mut arr = vec![5, 2, 4, 6, 1, 3];
        insertion_sort(&mut arr);
        assert!(arr.is_sorted());
    }

    #[test]
    fn test_selection_sort() {
        let mut arr = vec![5, 2, 4, 6, 1, 3];
        selection_sort(&mut arr);
        assert!(arr.is_sorted());
    }

    #[test]
    fn test_recursive_insertion_sort() {
        let mut arr = vec![5, 2, 4, 6, 1, 3];
        recursive_insertion_sort(&mut arr);
        assert!(arr.is_sorted());
    }

    #[test]
    fn test_bubble_sort() {
        let mut arr = vec![5, 2, 4, 6, 1, 3];
        bubble_sort(&mut arr);
        assert!(arr.is_sorted());
    }

    #[test]
    fn test_merge_sort() {
        let mut arr = vec![5, 2, 4, 6, 1, 3];
        merge_sort(&mut arr);
        assert!(arr.is_sorted());
    }

    #[test]
    fn test_sum_array() {
        let arr = vec![1, 2, 3, 4, 5];
        let initial_value = 10;
        let sum = sum_array_with_initial_value(&arr, initial_value);
        assert_eq!(sum, initial_value + arr.iter().sum::<i32>());
    }

    #[test]
    fn test_linear_search() {
        let arr = vec![1, 2, 3, 4, 5];
        let target = 3;
        let index = linear_search(&arr, &target);
        assert_eq!(index, Some(2));
        let index = linear_search(&arr, &10);
        assert_eq!(index, None);
    }
}
