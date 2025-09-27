#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory>
#include <stdexcept>
#include <utility>

// Forward declarations
template <typename T> class Matrix;

template <typename T> class MatrixView;

template <typename T> class MatrixViewMut;

/**
 * @brief A 2D matrix data structure with dynamic memory allocation
 *
 * This class provides a safe, modern C++ implementation of a 2D matrix
 * with efficient memory management and bounds checking.
 */
template <typename T> class Matrix {
  private:
    std::unique_ptr<T[]> m_data;
    size_t m_rows;
    size_t m_cols;
    size_t m_capacity;

    void reallocate(size_t new_capacity) {
        if (new_capacity >
            static_cast<size_t>(std::numeric_limits<std::ptrdiff_t>::max())) {
            throw std::overflow_error(
                "Matrix size exceeds maximum allowed size");
        }

        auto new_data = std::make_unique<T[]>(new_capacity);

        // Copy existing data if any
        if (m_data && m_capacity > 0) {
            std::size_t copy_size = std::min(m_capacity, new_capacity);
            std::move(m_data.get(), m_data.get() + copy_size, new_data.get());
        }

        m_data = std::move(new_data);
        m_capacity = new_capacity;
    }

  public:
    /**
     * @brief Construct a new Matrix with given dimensions
     *
     * @param rows Number of rows
     * @param cols Number of columns
     */
    explicit Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
        size_t size = rows * cols;
        if (size >
            static_cast<size_t>(std::numeric_limits<std::ptrdiff_t>::max())) {
            throw std::overflow_error(
                "Matrix size exceeds maximum allowed size");
        }
        m_capacity = size;
        m_data = std::make_unique<T[]>(m_capacity);
    }

    /**
     * @brief Construct a matrix filled with a specific value
     *
     * @param rows Number of rows
     * @param cols Number of columns
     * @param value Value to fill the matrix with
     */
    Matrix(size_t rows, size_t cols, const T &value) : Matrix(rows, cols) {
        std::fill_n(m_data.get(), m_capacity, value);
    }

    // Rule of 5
    Matrix(const Matrix &other)
        : m_rows(other.m_rows), m_cols(other.m_cols),
          m_capacity(other.m_capacity) {
        m_data = std::make_unique<T[]>(m_capacity);
        std::copy_n(other.m_data.get(), m_capacity, m_data.get());
    }

    Matrix &operator=(const Matrix &other) {
        if (this != &other) {
            Matrix temp(other);
            swap(temp);
        }
        return *this;
    }

    Matrix(Matrix &&) noexcept = default;
    Matrix &operator=(Matrix &&) noexcept = default;
    ~Matrix() = default;

    void swap(Matrix &other) noexcept {
        using std::swap;
        swap(m_data, other.m_data);
        swap(m_rows, other.m_rows);
        swap(m_cols, other.m_cols);
        swap(m_capacity, other.m_capacity);
    }

    // Accessors
    [[nodiscard]] size_t nrows() const noexcept { return m_rows; }
    [[nodiscard]] size_t ncols() const noexcept { return m_cols; }
    [[nodiscard]] std::pair<size_t, size_t> size() const noexcept {
        return {m_rows, m_cols};
    }

    /**
     * @brief Safe element access with bounds checking
     */
    [[nodiscard]] const T *get(size_t row, size_t col) const noexcept {
        if (row >= m_rows || col >= m_cols) {
            return nullptr;
        }
        return &m_data[row * m_cols + col];
    }

    [[nodiscard]] T *get(size_t row, size_t col) noexcept {
        if (row >= m_rows || col >= m_cols) {
            return nullptr;
        }
        return &m_data[row * m_cols + col];
    }

    /**
     * @brief Unchecked element access (unsafe but fast)
     *
     * @warning The caller must ensure indices are within bounds
     */
    [[nodiscard]] const T &get_unchecked(size_t row,
                                         size_t col) const noexcept {
        assert(row < m_rows && col < m_cols);
        return m_data[row * m_cols + col];
    }

    [[nodiscard]] T &get_unchecked(size_t row, size_t col) noexcept {
        assert(row < m_rows && col < m_cols);
        return m_data[row * m_cols + col];
    }

    [[nodiscard]] T &at(size_t row, size_t col) noexcept {
        if (row >= m_rows || col >= m_cols) {
            throw std::out_of_range("Matrix indices out of bounds");
        }
        return m_data[row * m_cols + col];
    }

    [[nodiscard]] const T &at(size_t row, size_t col) const noexcept {
        if (row >= m_rows || col >= m_cols) {
            throw std::out_of_range("Matrix indices out of bounds");
        }
        return m_data[row * m_cols + col];
    }

    [[nodiscard]] const T &operator()(size_t row, size_t col) const noexcept {
        return at(row, col);
    }

    [[nodiscard]] T &operator()(size_t row, size_t col) { return at(row, col); }

    [[nodiscard]] const T &
    operator[](const std::pair<size_t, size_t> &idx) const {
        auto [row, col] = idx;
        return at(row, col);
    }

    [[nodiscard]] T &operator[](const std::pair<size_t, size_t> &idx) {
        auto [row, col] = idx;
        return at(row, col);
    }

    void resize(size_t new_rows, size_t new_cols) {
        size_t new_size = new_rows * new_cols;
        if (new_size > m_capacity) {
            reallocate(new_size);
        }
        m_rows = new_rows;
        m_cols = new_cols;
    }

    [[nodiscard]] MatrixView<T> view(size_t row_start, size_t col_start,
                                     size_t rows, size_t cols) const {
        if (row_start + rows > m_rows || col_start + cols > m_cols) {
            throw std::out_of_range("View bounds exceed matrix dimensions");
        }
        return MatrixView<T>(*this, row_start, col_start, rows, cols);
    }

    [[nodiscard]] MatrixViewMut<T> view_mut(size_t row_start, size_t col_start,
                                            size_t rows, size_t cols) {
        if (row_start + rows > m_rows || col_start + cols > m_cols) {
            throw std::out_of_range("View bounds exceed matrix dimensions");
        }
        return MatrixViewMut<T>(*this, row_start, col_start, rows, cols);
    }

    template <typename U = T>
        requires std::default_initializable<U>
    [[nodiscard]] static Matrix<U> zeros(size_t rows, size_t cols) {
        return Matrix<U>(rows, cols, U{});
    }

    template <typename U = T>
        requires std::copy_constructible<U>
    [[nodiscard]] Matrix<U> transpose() const {
        Matrix<U> result(m_cols, m_rows);
        for (size_t i = 0; i < m_rows; ++i) {
            for (size_t j = 0; j < m_cols; ++j) {
                result.get_unchecked(j, i) = get_unchecked(i, j);
            }
        }
        return result;
    }

    [[nodiscard]] const T *data() const noexcept { return m_data.get(); }
    [[nodiscard]] T *data() noexcept { return m_data.get(); }
};

template <typename T> class MatrixView {
  private:
    const Matrix<T> *m_parent;
    size_t m_parent_cols;
    size_t m_rows;
    size_t m_cols;
    size_t m_row_offset;
    size_t m_col_offset;

  public:
    MatrixView(const Matrix<T> &parent, size_t row_offset, size_t col_offset,
               size_t rows, size_t cols)
        : m_parent(&parent), m_parent_cols(parent.ncols()), m_rows(rows),
          m_cols(cols), m_row_offset(row_offset), m_col_offset(col_offset) {}

    [[nodiscard]] size_t nrows() const noexcept { return m_rows; }
    [[nodiscard]] size_t ncols() const noexcept { return m_cols; }
    [[nodiscard]] std::pair<size_t, size_t> size() const noexcept {
        return {m_rows, m_cols};
    }

    [[nodiscard]] const T *get(size_t row, size_t col) const noexcept {
        if (row >= m_rows || col >= m_cols) {
            return nullptr;
        }
        size_t actual_row = m_row_offset + row;
        size_t actual_col = m_col_offset + col;
        return m_parent->get(actual_row, actual_col);
    }

    [[nodiscard]] const T &get_unchecked(size_t row,
                                         size_t col) const noexcept {
        assert(row < m_rows && col < m_cols);
        return m_parent->get_unchecked(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] T &at(size_t row, size_t col) {
        if (row >= m_rows || col >= m_cols) {
            throw std::out_of_range("View indices out of bounds");
        }
        return m_parent->at(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] const T &at(size_t row, size_t col) const {
        if (row >= m_rows || col >= m_cols) {
            throw std::out_of_range("View indices out of bounds");
        }
        return m_parent->at(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] const T &operator()(size_t row, size_t col) const {
        return at(row, col);
    }

    [[nodiscard]] const T &
    operator[](const std::pair<size_t, size_t> &idx) const {
        return at(idx.first, idx.second);
    }

    [[nodiscard]] MatrixView<T> view(size_t row_start, size_t col_start,
                                     size_t rows, size_t cols) const {
        if (row_start + rows > m_rows || col_start + cols > m_cols) {
            throw std::out_of_range("Sub-view bounds exceed view dimensions");
        }
        return MatrixView<T>(*m_parent, m_row_offset + row_start,
                             m_col_offset + col_start, rows, cols);
    }
};

template <typename T> class MatrixViewMut {
  private:
    Matrix<T> *m_parent;
    size_t m_parent_cols;
    size_t m_rows;
    size_t m_cols;
    size_t m_row_offset;
    size_t m_col_offset;

  public:
    MatrixViewMut(Matrix<T> &parent, size_t row_offset, size_t col_offset,
                  size_t rows, size_t cols)
        : m_parent(&parent), m_parent_cols(parent.ncols()), m_rows(rows),
          m_cols(cols), m_row_offset(row_offset), m_col_offset(col_offset) {}

    [[nodiscard]] size_t nrows() const noexcept { return m_rows; }
    [[nodiscard]] size_t ncols() const noexcept { return m_cols; }
    [[nodiscard]] std::pair<size_t, size_t> size() const noexcept {
        return {m_rows, m_cols};
    }

    [[nodiscard]] const T *get(size_t row, size_t col) const noexcept {
        if (row >= m_rows || col >= m_cols) {
            return nullptr;
        }
        return m_parent->get(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] T *get(size_t row, size_t col) noexcept {
        if (row >= m_rows || col >= m_cols) {
            return nullptr;
        }
        return m_parent->get(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] const T &get_unchecked(size_t row,
                                         size_t col) const noexcept {
        assert(row < m_rows && col < m_cols);
        return m_parent->get_unchecked(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] T &get_unchecked(size_t row, size_t col) noexcept {
        assert(row < m_rows && col < m_cols);
        return m_parent->get_unchecked(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] const T &at(size_t row, size_t col) const {
        if (row >= m_rows || col >= m_cols) {
            throw std::out_of_range("View indices out of bounds");
        }
        return m_parent->at(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] T &at(size_t row, size_t col) {
        if (row >= m_rows || col >= m_cols) {
            throw std::out_of_range("View indices out of bounds");
        }
        return m_parent->at(m_row_offset + row, m_col_offset + col);
    }

    [[nodiscard]] const T &operator()(size_t row, size_t col) const {
        return at(row, col);
    }

    [[nodiscard]] T &operator()(size_t row, size_t col) { return at(row, col); }

    [[nodiscard]] const T &
    operator[](const std::pair<size_t, size_t> &idx) const {
        return at(idx.first, idx.second);
    }

    [[nodiscard]] T &operator[](const std::pair<size_t, size_t> &idx) {
        return at(idx.first, idx.second);
    }

    [[nodiscard]] MatrixView<T> view(size_t row_start, size_t col_start,
                                     size_t rows, size_t cols) const {
        if (row_start + rows > m_rows || col_start + cols > m_cols) {
            throw std::out_of_range("Sub-view bounds exceed view dimensions");
        }
        return MatrixView<T>(*m_parent, m_row_offset + row_start,
                             m_col_offset + col_start, rows, cols);
    }

    [[nodiscard]] MatrixViewMut<T> view_mut(size_t row_start, size_t col_start,
                                            size_t rows, size_t cols) {
        if (row_start + rows > m_rows || col_start + cols > m_cols) {
            throw std::out_of_range("Sub-view bounds exceed view dimensions");
        }
        return MatrixViewMut<T>(*m_parent, m_row_offset + row_start,
                                m_col_offset + col_start, rows, cols);
    }
};

template <typename T> void swap(Matrix<T> &lhs, Matrix<T> &rhs) noexcept {
    lhs.swap(rhs);
}

#endif // MATRIX_HPP
