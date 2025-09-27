#include "matrix.hpp"
#include <iostream>
#include <cassert>
#include <stdexcept>

void test_basic_construction() {
    std::cout << "Testing basic construction..." << std::endl;

    // Test basic construction
    Matrix<double> m(3, 4);
    assert(m.nrows() == 3);
    assert(m.ncols() == 4);
    assert(m.size() == std::make_pair(3ul, 4ul));

    // Test construction with value
    Matrix<int> m2(2, 3, 42);
    assert(m2.nrows() == 2);
    assert(m2.ncols() == 3);
    assert(m2(0, 0) == 42);
    assert(m2(1, 2) == 42);

    std::cout << "✓ Basic construction tests passed" << std::endl;
}

void test_element_access() {
    std::cout << "Testing element access..." << std::endl;

    Matrix<int> m(3, 3, 1);

    // Test operator() access
    m(0, 0) = 10;
    m(1, 1) = 20;
    m(2, 2) = 30;

    assert(m(0, 0) == 10);
    assert(m(1, 1) == 20);
    assert(m(2, 2) == 30);
    assert(m(0, 1) == 1);  // unchanged

    // Test operator[] with pair access
    m[{0, 1}] = 15;
    assert((m[{0, 1}] == 15));

    // Test safe get method
    auto* ptr = m.get(0, 0);
    assert(ptr != nullptr && *ptr == 10);

    auto* null_ptr = m.get(10, 10);  // out of bounds
    assert(null_ptr == nullptr);

    // Test at method with bounds checking
    try {
        [[maybe_unused]] auto temp = m.at(10, 10);  // should throw
        assert(false && "Should have thrown exception");
    } catch (const std::out_of_range&) {
        // Expected
    }

    std::cout << "✓ Element access tests passed" << std::endl;
}

void test_copy_and_move() {
    std::cout << "Testing copy and move semantics..." << std::endl;

    Matrix<int> original(2, 2, 5);
    original(0, 1) = 10;

    // Test copy construction
    Matrix<int> copy(original);
    assert(copy.size() == original.size());
    assert(copy(0, 0) == 5);
    assert(copy(0, 1) == 10);

    // Modify copy to ensure independence
    copy(0, 0) = 99;
    assert(original(0, 0) == 5);  // original unchanged

    // Test copy assignment
    Matrix<int> assigned(1, 1);
    assigned = original;
    assert(assigned.size() == original.size());
    assert(assigned(0, 1) == 10);

    // Test move construction
    Matrix<int> moved(std::move(copy));
    assert(moved.size() == std::make_pair(2ul, 2ul));
    assert(moved(0, 0) == 99);

    std::cout << "✓ Copy and move tests passed" << std::endl;
}

void test_resize() {
    std::cout << "Testing resize..." << std::endl;

    Matrix<int> m(2, 2, 1);
    m(0, 0) = 10;
    m(1, 1) = 20;

    m.resize(3, 3);
    assert(m.size() == std::make_pair(3ul, 3ul));
    // Original data should be preserved where possible
    assert(m(0, 0) == 10);

    m.resize(1, 1);
    assert(m.size() == std::make_pair(1ul, 1ul));

    std::cout << "✓ Resize tests passed" << std::endl;
}

void test_matrix_view() {
    std::cout << "Testing matrix views..." << std::endl;

    Matrix<double> m(4, 4, 1.0);

    // Set some test values
    m(0, 0) = 0.0;
    m(1, 1) = 2.0;
    m(2, 2) = 3.0;
    m(3, 3) = 4.0;

    // Create a 2x2 view starting at (1, 1)
    auto view = m.view(1, 1, 2, 2);
    assert(view.size() == std::make_pair(2ul, 2ul));
    assert(view(0, 0) == 2.0);  // m(1, 1)
    assert(view(1, 1) == 3.0);  // m(2, 2)

    // Test sub-view
    auto sub_view = view.view(0, 0, 1, 1);
    assert(sub_view.size() == std::make_pair(1ul, 1ul));
    assert(sub_view(0, 0) == 2.0);

    // Test bounds checking
    try {
        [[maybe_unused]] auto temp_view = m.view(3, 3, 2, 2);  // out of bounds
        assert(false && "Should have thrown exception");
    } catch (const std::out_of_range&) {
        // Expected
    }

    std::cout << "✓ Matrix view tests passed" << std::endl;
}

void test_matrix_view_mut() {
    std::cout << "Testing mutable matrix views..." << std::endl;

    Matrix<double> m(3, 3, 0.0);

    {
        auto view_mut = m.view_mut(1, 1, 2, 2);
        view_mut(0, 0) = 5.0;
        view_mut(1, 1) = 6.0;
    }

    // Check that the original matrix was modified
    assert(m(1, 1) == 5.0);
    assert(m(2, 2) == 6.0);
    assert(m(0, 0) == 0.0);  // unchanged

    // Test nested mutable views
    {
        auto view_mut = m.view_mut(0, 0, 3, 3);
        auto sub_view_mut = view_mut.view_mut(0, 0, 2, 2);
        sub_view_mut(0, 0) = 99.0;
    }

    assert(m(0, 0) == 99.0);

    std::cout << "✓ Mutable matrix view tests passed" << std::endl;
}

void test_factory_methods() {
    std::cout << "Testing factory methods..." << std::endl;

    // Test zeros
    auto zeros = Matrix<int>::zeros(2, 3);
    assert(zeros.size() == std::make_pair(2ul, 3ul));
    assert(zeros(0, 0) == 0);
    assert(zeros(1, 2) == 0);

    // Test transpose
    Matrix<int> m(2, 3);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            m(i, j) = i * 3 + j;
        }
    }

    auto transposed = m.transpose<int>();
    assert(transposed.size() == std::make_pair(3ul, 2ul));

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            assert(transposed(j, i) == m(i, j));
        }
    }

    std::cout << "✓ Factory method tests passed" << std::endl;
}

void test_edge_cases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test 0x0 matrix
    Matrix<int> empty(0, 0);
    assert(empty.size() == std::make_pair(0ul, 0ul));

    // Test 1x1 matrix
    Matrix<int> single(1, 1, 42);
    assert(single(0, 0) == 42);

    // Test very wide matrix
    Matrix<int> wide(1, 1000, 1);
    assert(wide.ncols() == 1000);
    assert(wide(0, 999) == 1);

    // Test very tall matrix
    Matrix<int> tall(1000, 1, 2);
    assert(tall.nrows() == 1000);
    assert(tall(999, 0) == 2);

    std::cout << "✓ Edge case tests passed" << std::endl;
}

int main() {
    try {
        test_basic_construction();
        test_element_access();
        test_copy_and_move();
        test_resize();
        test_matrix_view();
        test_matrix_view_mut();
        test_factory_methods();
        test_edge_cases();

        std::cout << "All tests passed successfully!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}
