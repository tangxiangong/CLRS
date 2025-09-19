#ifndef UTILS_HPP
#define UTILS_HPP

#include "error.hpp"
#include <expected>
#include <format>
#include <random>
#include <type_traits>
#include <vector>

using std::format;
using std::vector;

/**
 * @brief Generates a vector of normally distributed random values
 * @tparam T The floating-point type for the generated values
 * @param n The number of values to generate
 * @param mean The mean (μ) of the normal distribution
 * @param stddev The standard deviation (σ) of the normal distribution (must be
 * positive)
 */
template <typename T = double>
  requires std::is_floating_point_v<T>
auto randn(size_t n, T mean = 0, T stddev = 1)
    -> std::expected<vector<T>, Error> {
  if (stddev <= 0) {
    return std::unexpected<Error>(Error::InvalidArgument(
        format("The standard deviation `stddev` must be positive, but got {}",
               stddev)));
  }
  auto sampler = [mean, stddev]() mutable -> T {
    std::random_device rd;
    thread_local static std::mt19937 gen = std::mt19937(rd());
    std::normal_distribution<T> dist(mean, stddev);
    return dist(gen);
  };
  vector<T> result(n);
  if (n == 0) {
    return std::expected<vector<T>, Error>(result);
  }

  for (T &value : result) {
    value = sampler();
  }
  return std::expected<vector<T>, Error>(result);
}
#endif // UTILS_HPP
