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
    return 0;
}
