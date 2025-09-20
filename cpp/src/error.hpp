#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <utility>

/**
 * @brief Error structure representing various error conditions in the
 * diffusionx library
 *
 * This structure encapsulates error information with a descriptive message.
 * It provides static factory methods for creating common error types.
 */
struct Error {
    std::string message; ///< The error message describing what went wrong

    /**
     * @brief Constructs an Error with the given message
     * @param msg The error message
     */
    explicit Error(std::string msg) : message(std::move(msg)) {}

    /**
     * @brief Creates an invalid argument error
     * @param msg Additional context about the invalid argument
     * @return Error instance representing an invalid argument error
     */
    static auto InvalidArgument(const std::string &msg) -> Error {
        return Error("Invalid argument: " + msg);
    }
};

#endif // ERROR_HPP
