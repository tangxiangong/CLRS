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
