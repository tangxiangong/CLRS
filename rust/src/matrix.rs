//! Matrix data structure
//!

use std::{
    alloc::{self, Layout},
    ops::{Index, IndexMut},
    ptr::NonNull,
};

pub struct Matrix<T> {
    data: NonNull<T>,
    rows: usize,
    cols: usize,
    capacity: usize,
}

impl<T> Matrix<T> {
    pub fn new(rows: usize, cols: usize) -> Self {
        let size = rows * cols;
        let layout = Layout::array::<T>(size).unwrap();
        assert!(
            layout.size() <= isize::MAX as usize,
            "Matrix size exceeds isize::MAX"
        );
        let data = unsafe {
            let ptr = alloc::alloc(layout) as *mut T;
            match NonNull::new(ptr) {
                Some(data) => data,
                None => alloc::handle_alloc_error(layout),
            }
        };
        Self {
            data,
            rows,
            cols,
            capacity: size,
        }
    }

    pub fn nrows(&self) -> usize {
        self.rows
    }

    pub fn ncols(&self) -> usize {
        self.cols
    }

    pub fn size(&self) -> (usize, usize) {
        (self.rows, self.cols)
    }

    pub fn with_value(rows: usize, cols: usize, value: T) -> Self
    where
        T: Clone,
    {
        let matrix = Self::new(rows, cols);
        for i in 0..rows * cols {
            unsafe {
                std::ptr::write(matrix.data.as_ptr().add(i), value.clone());
            }
        }
        matrix
    }

    #[inline]
    pub fn get(&self, row: usize, col: usize) -> Option<&T> {
        if row < self.rows && col < self.cols {
            unsafe { Some(&*self.data.as_ptr().add(row * self.cols + col)) }
        } else {
            None
        }
    }

    #[inline]
    pub fn get_mut(&mut self, row: usize, col: usize) -> Option<&mut T> {
        if row < self.rows && col < self.cols {
            unsafe { Some(&mut *self.data.as_ptr().add(row * self.cols + col)) }
        } else {
            None
        }
    }

    /// # Safety
    /// The caller must ensure that the given row and column indices are within bounds.
    #[inline]
    pub unsafe fn get_unchecked(&self, row: usize, col: usize) -> &T {
        unsafe { &*self.data.as_ptr().add(row * self.cols + col) }
    }

    /// # Safety
    /// The caller must ensure that the given row and column indices are within bounds.
    #[inline]
    pub unsafe fn get_unchecked_mut(&mut self, row: usize, col: usize) -> &mut T {
        unsafe { &mut *self.data.as_ptr().add(row * self.cols + col) }
    }

    pub fn resize(&mut self, new_rows: usize, new_cols: usize) {
        let new_size = new_rows * new_cols;
        if new_size > self.capacity {
            self.reallocate(new_size);
        } else {
            self.rows = new_rows;
            self.cols = new_cols;
        }
    }

    fn reallocate(&mut self, new_size: usize) {
        let old_layout = Layout::array::<T>(self.capacity).unwrap();
        let new_layout = Layout::array::<T>(new_size).unwrap();
        assert!(new_size <= isize::MAX as usize, "Capacity overflow");
        let new_ptr = unsafe {
            let ptr = alloc::realloc(self.data.as_ptr() as *mut _, old_layout, new_layout.size())
                as *mut T;
            match NonNull::new(ptr) {
                Some(ptr) => ptr,
                None => alloc::handle_alloc_error(new_layout),
            }
        };
        self.data = new_ptr;
        self.capacity = new_size;
    }

    /// Create a view of a submatrix without copying data
    pub fn view<'a>(
        &'a self,
        row_start: usize,
        col_start: usize,
        rows: usize,
        cols: usize,
    ) -> MatrixView<'a, T> {
        assert!(row_start + rows <= self.rows, "Row range out of bounds");
        assert!(col_start + cols <= self.cols, "Column range out of bounds");

        MatrixView {
            data: self.data,
            parent_cols: self.cols,
            rows,
            cols,
            row_offset: row_start,
            col_offset: col_start,
            _phantom: std::marker::PhantomData,
        }
    }

    /// Create a mutable view of a submatrix without copying data
    pub fn view_mut<'a>(
        &'a mut self,
        row_start: usize,
        col_start: usize,
        rows: usize,
        cols: usize,
    ) -> MatrixViewMut<'a, T> {
        assert!(row_start + rows <= self.rows, "Row range out of bounds");
        assert!(col_start + cols <= self.cols, "Column range out of bounds");

        MatrixViewMut {
            data: self.data,
            parent_cols: self.cols,
            rows,
            cols,
            row_offset: row_start,
            col_offset: col_start,
            _phantom: std::marker::PhantomData,
        }
    }
}

impl<T> Index<(usize, usize)> for Matrix<T> {
    type Output = T;

    fn index(&self, (row, col): (usize, usize)) -> &Self::Output {
        self.get(row, col).expect("Index out of bounds")
    }
}

impl<T> IndexMut<(usize, usize)> for Matrix<T> {
    fn index_mut(&mut self, (row, col): (usize, usize)) -> &mut Self::Output {
        self.get_mut(row, col).expect("Index out of bounds")
    }
}

impl<T> Drop for Matrix<T> {
    fn drop(&mut self) {
        if self.capacity != 0 {
            unsafe {
                let layout = Layout::array::<T>(self.capacity).unwrap();
                alloc::dealloc(self.data.as_ptr() as *mut _, layout);
            }
        }
    }
}

unsafe impl<T: Send> Send for Matrix<T> {}
unsafe impl<T: Send> Sync for Matrix<T> {}

impl<T: Default + Clone> Matrix<T> {
    pub fn zeros(rows: usize, cols: usize) -> Self {
        Self::with_value(rows, cols, T::default())
    }

    pub fn transpose(&self) -> Self {
        let result = Self::new(self.cols, self.rows);
        for i in 0..self.rows {
            for j in 0..self.cols {
                unsafe {
                    std::ptr::write(
                        result.data.as_ptr().add(j * self.rows + i),
                        self.get_unchecked(i, j).clone(),
                    );
                }
            }
        }
        result
    }
}

pub struct MatrixView<'a, T> {
    data: NonNull<T>,
    parent_cols: usize, // Number of columns in the parent matrix
    rows: usize,        // Number of rows in this view
    cols: usize,        // Number of columns in this view
    row_offset: usize,  // Starting row in parent matrix
    col_offset: usize,  // Starting column in parent matrix
    _phantom: std::marker::PhantomData<&'a T>,
}

impl<'a, T> MatrixView<'a, T> {
    pub fn nrows(&self) -> usize {
        self.rows
    }

    pub fn ncols(&self) -> usize {
        self.cols
    }

    pub fn size(&self) -> (usize, usize) {
        (self.rows, self.cols)
    }

    #[inline]
    pub fn get(&self, row: usize, col: usize) -> Option<&T> {
        if row < self.rows && col < self.cols {
            unsafe {
                let actual_row = self.row_offset + row;
                let actual_col = self.col_offset + col;
                Some(
                    &*self
                        .data
                        .as_ptr()
                        .add(actual_row * self.parent_cols + actual_col),
                )
            }
        } else {
            None
        }
    }

    /// # Safety
    /// The caller must ensure that the given row and column indices are within bounds.
    #[inline]
    pub unsafe fn get_unchecked(&self, row: usize, col: usize) -> &T {
        let actual_row = self.row_offset + row;
        let actual_col = self.col_offset + col;
        unsafe {
            &*self
                .data
                .as_ptr()
                .add(actual_row * self.parent_cols + actual_col)
        }
    }

    /// Create a sub-view of this view
    pub fn view(
        &self,
        row_start: usize,
        col_start: usize,
        rows: usize,
        cols: usize,
    ) -> MatrixView<'_, T> {
        assert!(row_start + rows <= self.rows, "Row range out of bounds");
        assert!(col_start + cols <= self.cols, "Column range out of bounds");

        MatrixView {
            data: self.data,
            parent_cols: self.parent_cols,
            rows,
            cols,
            row_offset: self.row_offset + row_start,
            col_offset: self.col_offset + col_start,
            _phantom: std::marker::PhantomData,
        }
    }
}

impl<'a, T> Index<(usize, usize)> for MatrixView<'a, T> {
    type Output = T;

    fn index(&self, (row, col): (usize, usize)) -> &Self::Output {
        self.get(row, col).expect("Index out of bounds")
    }
}

pub struct MatrixViewMut<'a, T> {
    data: NonNull<T>,
    parent_cols: usize, // Number of columns in the parent matrix
    rows: usize,        // Number of rows in this view
    cols: usize,        // Number of columns in this view
    row_offset: usize,  // Starting row in parent matrix
    col_offset: usize,  // Starting column in parent matrix
    _phantom: std::marker::PhantomData<&'a mut T>,
}

impl<'a, T> MatrixViewMut<'a, T> {
    pub fn nrows(&self) -> usize {
        self.rows
    }

    pub fn ncols(&self) -> usize {
        self.cols
    }

    pub fn size(&self) -> (usize, usize) {
        (self.rows, self.cols)
    }

    #[inline]
    pub fn get(&self, row: usize, col: usize) -> Option<&T> {
        if row < self.rows && col < self.cols {
            unsafe {
                let actual_row = self.row_offset + row;
                let actual_col = self.col_offset + col;
                Some(
                    &*self
                        .data
                        .as_ptr()
                        .add(actual_row * self.parent_cols + actual_col),
                )
            }
        } else {
            None
        }
    }

    #[inline]
    pub fn get_mut(&mut self, row: usize, col: usize) -> Option<&mut T> {
        if row < self.rows && col < self.cols {
            unsafe {
                let actual_row = self.row_offset + row;
                let actual_col = self.col_offset + col;
                Some(
                    &mut *self
                        .data
                        .as_ptr()
                        .add(actual_row * self.parent_cols + actual_col),
                )
            }
        } else {
            None
        }
    }

    /// # Safety
    /// The caller must ensure that the given row and column indices are within bounds.
    #[inline]
    pub unsafe fn get_unchecked(&self, row: usize, col: usize) -> &T {
        let actual_row = self.row_offset + row;
        let actual_col = self.col_offset + col;
        unsafe {
            &*self
                .data
                .as_ptr()
                .add(actual_row * self.parent_cols + actual_col)
        }
    }

    /// # Safety
    /// The caller must ensure that the given row and column indices are within bounds.
    #[inline]
    pub unsafe fn get_unchecked_mut(&mut self, row: usize, col: usize) -> &mut T {
        let actual_row = self.row_offset + row;
        let actual_col = self.col_offset + col;
        unsafe {
            &mut *self
                .data
                .as_ptr()
                .add(actual_row * self.parent_cols + actual_col)
        }
    }

    /// Create a sub-view of this mutable view (returns immutable view)
    pub fn view(
        &self,
        row_start: usize,
        col_start: usize,
        rows: usize,
        cols: usize,
    ) -> MatrixView<'_, T> {
        assert!(row_start + rows <= self.rows, "Row range out of bounds");
        assert!(col_start + cols <= self.cols, "Column range out of bounds");

        MatrixView {
            data: self.data,
            parent_cols: self.parent_cols,
            rows,
            cols,
            row_offset: self.row_offset + row_start,
            col_offset: self.col_offset + col_start,
            _phantom: std::marker::PhantomData,
        }
    }

    /// Create a mutable sub-view of this mutable view
    pub fn view_mut(
        &mut self,
        row_start: usize,
        col_start: usize,
        rows: usize,
        cols: usize,
    ) -> MatrixViewMut<'_, T> {
        assert!(row_start + rows <= self.rows, "Row range out of bounds");
        assert!(col_start + cols <= self.cols, "Column range out of bounds");

        MatrixViewMut {
            data: self.data,
            parent_cols: self.parent_cols,
            rows,
            cols,
            row_offset: self.row_offset + row_start,
            col_offset: self.col_offset + col_start,
            _phantom: std::marker::PhantomData,
        }
    }
}

impl<'a, T> Index<(usize, usize)> for MatrixViewMut<'a, T> {
    type Output = T;

    fn index(&self, (row, col): (usize, usize)) -> &Self::Output {
        self.get(row, col).expect("Index out of bounds")
    }
}

impl<'a, T> IndexMut<(usize, usize)> for MatrixViewMut<'a, T> {
    fn index_mut(&mut self, (row, col): (usize, usize)) -> &mut Self::Output {
        self.get_mut(row, col).expect("Index out of bounds")
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_with_value() {
        let matrix = Matrix::with_value(3, 3, 1.0);
        assert_eq!(matrix.rows, 3);
        assert_eq!(matrix.cols, 3);
        assert_eq!(matrix.get(0, 0), Some(&1.0));
        assert_eq!(matrix[(0, 1)], 1.0);
    }

    #[test]
    fn test_matrix_view() {
        let mut matrix = Matrix::with_value(4, 4, 1.0);

        // Set some test values
        matrix[(0, 0)] = 0.0;
        matrix[(1, 1)] = 2.0;
        matrix[(2, 2)] = 3.0;
        matrix[(3, 3)] = 4.0;

        // Create a 2x2 view starting at (1, 1)
        let view = matrix.view(1, 1, 2, 2);
        assert_eq!(view.size(), (2, 2));
        assert_eq!(view[(0, 0)], 2.0); // matrix[(1, 1)]
        assert_eq!(view[(1, 1)], 3.0); // matrix[(2, 2)]

        // Test sub-view
        let sub_view = view.view(0, 0, 1, 1);
        assert_eq!(sub_view.size(), (1, 1));
        assert_eq!(sub_view[(0, 0)], 2.0);
    }

    #[test]
    fn test_matrix_view_mut() {
        let mut matrix = Matrix::with_value(3, 3, 0.0);

        {
            let mut view = matrix.view_mut(1, 1, 2, 2);
            view[(0, 0)] = 5.0;
            view[(1, 1)] = 6.0;
        }

        // Check that the original matrix was modified
        assert_eq!(matrix[(1, 1)], 5.0);
        assert_eq!(matrix[(2, 2)], 6.0);
        assert_eq!(matrix[(0, 0)], 0.0); // Should be unchanged
    }
}
