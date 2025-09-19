#include "sort.hpp"
#include "utils.hpp"
#include <print>

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
  return 0;
}
