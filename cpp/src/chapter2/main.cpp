#include <print>

#include "chapter2.hpp"
#include "utils.hpp"

using std::println;

int main() {
    auto result = randn(10);
    if (!result.has_value()) {
        println("Error: Failed to generate random number");
        return 1;
    }
    auto vec = result.value();
    println("{}", vec);
    insertion_sort(vec);
    println("{}", vec);
    println("Sum: {}", sum_array(vec));

    auto index = linear_search(vec, 5.0);
    if (index.has_value()) {
        println("Found at index {}", index.value());
    } else {
        println("Not found");
    }

    index = linear_search(vec, vec[4]);
    if (index.has_value()) {
        println("Found at index {}", index.value());
    } else {
        println("Not found");
    }

    return 0;
}
