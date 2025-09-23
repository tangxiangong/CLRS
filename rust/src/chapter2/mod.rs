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
