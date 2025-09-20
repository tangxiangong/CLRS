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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_insertion_sort() {
        let mut arr = vec![5, 2, 4, 6, 1, 3];
        insertion_sort(&mut arr);
        assert!(arr.is_sorted());
    }
}
