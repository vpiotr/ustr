#ifndef __USTR_H__
#define __USTR_H__

/**
 * @file ustr.h
 * @brief Universal string conversion library for C++
 * @author Extracted from utest library by Piotr Likus
 * @date 13/06/2025
 * 
 * @mainpage ustr - Universal String Conversion Library
 * 
 * @section introduction Introduction
 * 
 * ustr is a simple, lightweight, header-only C++ library that provides
 * universal string conversion functionality. It supports intelligent conversion
 * of various C++ types to string representation with proper type detection.
 * 
 * Features:
 * - Automatic type detection and optimal conversion strategy
 * - Support for numeric types using std::to_string
 * - Support for streamable types using std::ostringstream
 * - Support for custom to_string() methods
 * - Specializations for common types (bool, char, string literals)
 * - Fallback for non-streamable types with type information
 * - SFINAE-based implementation for type safety
 * 
 * @section usage_sec Basic Usage
 * 
 * @code{.cpp}
 * #include "ustr.h"
 * 
 * int main() {
 *     std::string s1 = ustr::to_string(42);           // "42"
 *     std::string s2 = ustr::to_string(3.14);         // "3.140000"
 *     std::string s3 = ustr::to_string(true);         // "true"
 *     std::string s4 = ustr::to_string('A');          // "A"
 *     std::string s5 = ustr::to_string("hello");      // "hello"
 *     
 *     // Custom class with to_string method
 *     MyClass obj;
 *     std::string s6 = ustr::to_string(obj);          // Uses obj.to_string()
 *     
 *     // Container with cbegin/cend
 *     std::vector<int> vec{1, 2, 3};
 *     std::string s7 = ustr::to_string(vec);          // "[1, 2, 3]"
 *     
 *     // Pairs and tuples
 *     std::pair<int, std::string> p{42, "pair"};
 *     std::string s8 = ustr::to_string(p);            // "(42, pair)"
 *     
 *     std::tuple<int, double, bool> t{1, 2.5, true};
 *     std::string s9 = ustr::to_string(t);            // "(1, 2.500000, true)"
 *     
 *     return 0;
 * }
 * @endcode
 * 
 * @section customization_sec Customization
 * 
 * You can customize the behavior for your types:
 * 
 * @code{.cpp}
 * // Enable has_to_string for your class
 * class MyClass {
 * public:
 *     std::string to_string() const { return "MyClass instance"; }
 * };
 * 
 * // Specialize has_to_string if auto-detection doesn't work
 * namespace ustr {
 *     template<>
 *     struct has_to_string<MyClass> : std::true_type {};
 * }
 * @endcode
 */

#include <string>
#include <sstream>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <map>
#include <memory>
#include <tuple>

namespace ustr {

/**
 * @defgroup type_traits Type Detection Traits
 * @brief SFINAE-based type detection utilities
 * @{
 */

/**
 * @brief Detects if a type has a to_string() method
 * 
 * This trait checks if a type T has a public to_string() method that returns
 * something convertible to std::string. Can be specialized for custom types.
 * 
 * @tparam T Type to check
 * 
 * @code{.cpp}
 * class MyClass {
 * public:
 *     std::string to_string() const { return "MyClass"; }
 * };
 * 
 * static_assert(ustr::has_to_string<MyClass>::value, "Should have to_string");
 * @endcode
 */
template<typename T>
struct has_to_string {
private:
    template<typename U>
    static auto test(int) -> decltype(
        std::declval<U>().to_string(),
        std::true_type{}
    );
    
    template<typename>
    static std::false_type test(...);
    
public:
    static const bool value = decltype(test<T>(0))::value;
};

/**
 * @brief Detects if a type can be streamed with operator<<
 * 
 * This trait checks if a type T can be output to an std::ostream using
 * the insertion operator (operator<<).
 * 
 * @tparam T Type to check
 * 
 * @code{.cpp}
 * static_assert(ustr::is_streamable<int>::value, "int is streamable");
 * static_assert(ustr::is_streamable<std::string>::value, "string is streamable");
 * @endcode
 */
template<typename T>
struct is_streamable {
private:
    template<typename U>
    static auto test(int) -> decltype(
        std::declval<std::ostringstream&>() << std::declval<U>(),
        std::true_type{}
    );
    
    template<typename>
    static std::false_type test(...);
    
public:
    static const bool value = decltype(test<T>(0))::value;
};

/**
 * @brief Detects if a type should use std::to_string
 * 
 * This trait identifies numeric types that should use std::to_string for
 * optimal conversion. Excludes bool and character types which have better
 * specialized representations.
 * 
 * @tparam T Type to check
 * 
 * @code{.cpp}
 * static_assert(ustr::is_numeric<int>::value, "int is numeric");
 * static_assert(ustr::is_numeric<double>::value, "double is numeric");
 * static_assert(!ustr::is_numeric<bool>::value, "bool is not considered numeric for string conversion");
 * @endcode
 */
template<typename T>
struct is_numeric : std::integral_constant<bool, 
    std::is_arithmetic<T>::value && 
    !std::is_same<T, bool>::value && 
    !std::is_same<T, char>::value &&
    !std::is_same<T, signed char>::value &&
    !std::is_same<T, unsigned char>::value &&
    !std::is_same<T, wchar_t>::value &&
    !std::is_same<T, char16_t>::value &&
    !std::is_same<T, char32_t>::value> {};

/**
 * @brief Detects if a type has cbegin() and cend() methods
 * 
 * This trait checks if a type T has public cbegin() and cend() methods
 * that return const iterators. This is useful for detecting standard
 * containers and other types that support const iteration.
 * 
 * @tparam T Type to check
 * 
 * @code{.cpp}
 * static_assert(ustr::has_cbegin_cend<std::vector<int>>::value, "vector has cbegin/cend");
 * static_assert(ustr::has_cbegin_cend<std::string>::value, "string has cbegin/cend");
 * static_assert(ustr::has_cbegin_cend<std::map<int, int>>::value, "map has cbegin/cend");
 * static_assert(!ustr::has_cbegin_cend<int>::value, "int does not have cbegin/cend");
 * @endcode
 */
template<typename T>
struct has_cbegin_cend {
private:
    template<typename U>
    static auto test(int) -> decltype(
        std::declval<U>().cbegin(),
        std::declval<U>().cend(),
        std::true_type{}
    );
    
    template<typename>
    static std::false_type test(...);
    
public:
    static const bool value = decltype(test<T>(0))::value;
};

/**
 * @brief Detects if a type is std::tuple
 * 
 * This trait checks if a type T is a std::tuple specialization.
 * This is useful for detecting tuples and providing specialized
 * string conversion that formats all tuple elements.
 * 
 * @tparam T Type to check
 * 
 * @code{.cpp}
 * static_assert(ustr::is_tuple<std::tuple<int, std::string>>::value, "tuple<int, string> is a tuple");
 * static_assert(ustr::is_tuple<std::tuple<>>::value, "empty tuple is a tuple");
 * static_assert(!ustr::is_tuple<std::pair<int, int>>::value, "pair is not a tuple");
 * static_assert(!ustr::is_tuple<int>::value, "int is not a tuple");
 * @endcode
 */
template<typename T>
struct is_tuple : std::false_type {};

template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

/**
 * @brief Detects if a type has a custom specialization
 * 
 * This trait can be specialized to indicate that a type has its own
 * custom to_string_impl specialization and should be handled before
 * the standard type detection logic. This is useful for providing
 * completely custom implementations for specific types.
 * 
 * @tparam T Type to check
 * 
 * @code{.cpp}
 * // Specialize for your custom type
 * namespace ustr {
 *     template<>
 *     struct has_custom_specialization<MyType> : std::true_type {};
 * }
 * 
 * // Then provide the specialization
 * namespace ustr {
 *     namespace details {
 *         template<>
 *         inline std::string to_string_impl<MyType>(const MyType& value) {
 *             return "custom:" + std::to_string(value.getValue());
 *         }
 *     }
 * }
 * @endcode
 */
template<typename T>
struct has_custom_specialization : std::false_type {};

/**
 * @brief Detects if a type should be quoted when converted to string
 * 
 * This trait identifies string-like types that should be wrapped in quotes
 * when used as keys in maps or values in containers. By default, this applies
 * to std::string, const char*, and char*.
 * 
 * @tparam T Type to check
 * 
 * @code{.cpp}
 * static_assert(ustr::is_quotable_string<std::string>::value);
 * static_assert(ustr::is_quotable_string<const char*>::value);
 * static_assert(!ustr::is_quotable_string<int>::value);
 * @endcode
 */
template<typename T>
struct is_quotable_string : std::integral_constant<bool,
    std::is_same<typename std::decay<T>::type, std::string>::value ||
    std::is_same<typename std::decay<T>::type, const char*>::value ||
    std::is_same<typename std::decay<T>::type, char*>::value
> {};

/**
 * @brief Detects if a type is a C-style array
 * 
 * This trait checks if a type T is a C-style array (e.g., int[5], char[10]).
 * It excludes char arrays that should be treated as C-strings.
 * 
 * @tparam T Type to check
 * 
 * @code{.cpp}
 * static_assert(ustr::is_c_array<int[5]>::value, "int[5] is a C-style array");
 * static_assert(!ustr::is_c_array<char[10]>::value, "char[10] is treated as C-string");
 * @endcode
 */
template<typename T>
struct is_c_array : std::integral_constant<bool, 
    std::is_array<T>::value && 
    !std::is_same<typename std::remove_extent<T>::type, char>::value &&
    !std::is_same<typename std::remove_extent<T>::type, signed char>::value &&
    !std::is_same<typename std::remove_extent<T>::type, unsigned char>::value
> {};

/** @} */ // end of type_traits group

// Forward declarations for iterator-based to_string
template<typename IterT>
std::string to_string(IterT begin, IterT end);

namespace details {

/**
 * @defgroup implementation Implementation Details
 * @brief Internal implementation functions - do not use directly
 * @{
 */

// Forward declaration for recursive calls
template<typename T>
std::string to_string_forward(const T& value);

// Optimized shared function to apply quotation if needed for any type
// Uses template specialization to avoid runtime conditionals

// Helper implementation for non-quotable types (no quotes needed)
template<typename T>
inline std::string apply_quotation_impl(const T& value, std::false_type) {
    return to_string_forward(value);
}

// Helper implementation for quotable types (quotes needed)
template<typename T>
inline std::string apply_quotation_impl(const T& value, std::true_type) {
    // Optimized string concatenation using reserve for better performance
    std::string str_value = to_string_forward(value);
    std::string result;
    result.reserve(str_value.length() + 2); // Reserve space for quotes + content
    result += '"';
    result += str_value;
    result += '"';
    return result;
}

// Main function that dispatches to appropriate implementation
template<typename T>
inline std::string apply_quotation_if_needed(const T& value) {
    using value_type = typename std::decay<T>::type;
    return apply_quotation_impl(value, typename is_quotable_string<value_type>::type{});
}

// Constants for common string representations
inline const char* get_null_string() {
    return "null";
}

// Helper to detect if a type has first and second members (like std::pair)
template<typename T, typename = void>
struct has_first_second : std::false_type {};

template<typename T>
struct has_first_second<T, 
    typename std::enable_if<
        !std::is_void<decltype(std::declval<T>().first)>::value && 
        !std::is_void<decltype(std::declval<T>().second)>::value
    >::type
> : std::true_type {};

// Helper to detect if a type is std::pair
template<typename T>
struct is_pair : std::false_type {};

template<typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {};

// Helper to detect if a type is std::tuple
template<typename T>
struct is_tuple : std::false_type {};

template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

// C++11 compatible index sequence implementation
template<std::size_t... Indices>
struct index_sequence {};

template<std::size_t N, std::size_t... Indices>
struct make_index_sequence_impl : make_index_sequence_impl<N-1, N-1, Indices...> {};

template<std::size_t... Indices>
struct make_index_sequence_impl<0, Indices...> {
    typedef index_sequence<Indices...> type;
};

template<std::size_t N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;

// Helper functions for tuple conversion (C++11 compatible)
template<typename Tuple, std::size_t... Indices>
inline std::string tuple_to_string_impl(const Tuple& tuple, index_sequence<Indices...>) {
    std::ostringstream ss;
    ss << "(";
    bool first = true;
    // C++11 compatible element processing using recursive template expansion
    auto process_elements = [&ss, &first](const std::string& element) {
        if (!first) ss << ", ";
        first = false;
        ss << element;
    };
    // Use initializer list expansion for C++11 compatibility with quotation support
    (void)std::initializer_list<int>{(process_elements(apply_quotation_if_needed(std::get<Indices>(tuple))), 0)...};
    ss << ")";
    return ss.str();
}

template<typename Tuple>
inline std::string tuple_to_string(const Tuple& tuple) {
    constexpr std::size_t size = std::tuple_size<Tuple>::value;
    return tuple_to_string_impl(tuple, make_index_sequence<size>{});
}

// Helper to detect if a type is one of the special types that need explicit handling
template<typename T>
struct is_special_type : std::integral_constant<bool,
    std::is_same<T, bool>::value ||
    std::is_same<T, char>::value ||
    std::is_same<T, signed char>::value ||
    std::is_same<T, unsigned char>::value ||
    std::is_same<T, std::string>::value ||
    std::is_same<T, const char*>::value ||
    std::is_same<typename std::decay<T>::type, char*>::value ||
    std::is_same<T, std::nullptr_t>::value
> {};

// Implementation for types with custom to_string method (highest priority)
template<typename T>
inline auto to_string_impl(const T& value) 
    -> typename std::enable_if<
        has_to_string<T>::value && 
        !is_special_type<T>::value, 
        std::string
    >::type {
    return value.to_string();
}

// Implementation for std::string
inline std::string to_string_impl(const std::string& value) {
    return value;
}

// Implementation for C-style strings
inline std::string to_string_impl(const char* value) {
    return value ? std::string(value) : std::string(get_null_string());
}

// Implementation for bool
inline std::string to_string_impl(bool value) {
    return value ? "true" : "false";
}

// Implementation for char
inline std::string to_string_impl(char value) {
    return std::string(1, value);
}

// Implementation for signed char
inline std::string to_string_impl(signed char value) {
    return std::string(1, static_cast<char>(value));
}

// Implementation for unsigned char
inline std::string to_string_impl(unsigned char value) {
    return std::string(1, static_cast<char>(value));
}

// Implementation for nullptr_t
inline std::string to_string_impl(std::nullptr_t) {
    return get_null_string();
}

// Implementation for numeric types (excluding special types)
template<typename T>
inline auto to_string_impl(const T& value)
    -> typename std::enable_if<
        is_numeric<T>::value && 
        !has_to_string<T>::value && 
        !is_special_type<T>::value, 
        std::string
    >::type {
    return std::to_string(value);
}

// Implementation for std::pair types
template<typename T>
inline auto to_string_impl(const T& value)
    -> typename std::enable_if<
        !has_to_string<T>::value && 
        !is_numeric<T>::value && 
        !is_special_type<T>::value &&
        is_pair<T>::value,
        std::string
    >::type {
    std::ostringstream ss;
    ss << "(" << apply_quotation_if_needed(value.first) << ", " << apply_quotation_if_needed(value.second) << ")";
    return ss.str();
}

// Implementation for std::tuple types
template<typename T>
inline auto to_string_impl(const T& value)
    -> typename std::enable_if<
        !has_to_string<T>::value && 
        !is_numeric<T>::value && 
        !is_special_type<T>::value &&
        !is_pair<T>::value &&
        is_tuple<T>::value,
        std::string
    >::type {
    return tuple_to_string(value);
}

// Implementation for types with cbegin/cend (containers) - uses iterator-based conversion
template<typename T>
inline auto to_string_impl(const T& value)
    -> typename std::enable_if<
        !has_to_string<T>::value && 
        !is_numeric<T>::value && 
        !is_special_type<T>::value &&
        !is_pair<T>::value &&
        !is_tuple<T>::value &&
        !is_c_array<T>::value &&
        has_cbegin_cend<T>::value,
        std::string
    >::type {
    return to_string(value.cbegin(), value.cend());
}

// Implementation for streamable types (excluding numeric, special types, pairs, tuples, c-arrays, and containers with cbegin/cend)
template<typename T>
inline auto to_string_impl(const T& value)
    -> typename std::enable_if<
        !has_to_string<T>::value && 
        !is_numeric<T>::value && 
        !is_special_type<T>::value &&
        !is_pair<T>::value &&
        !is_tuple<T>::value &&
        !is_c_array<T>::value &&
        !has_cbegin_cend<T>::value &&
        is_streamable<T>::value, 
        std::string
    >::type {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

// Implementation for non-streamable types (fallback)
template<typename T>
inline auto to_string_impl(const T& value)
    -> typename std::enable_if<
        !has_to_string<T>::value && 
        !is_numeric<T>::value && 
        !is_special_type<T>::value &&
        !is_pair<T>::value &&
        !is_tuple<T>::value &&
        !is_c_array<T>::value &&
        !has_cbegin_cend<T>::value &&
        !is_streamable<T>::value, 
        std::string
    >::type {
    std::ostringstream ss;
    ss << "[" << typeid(T).name() << " at " << &value << "]";
    return ss.str();
}

// Implementation for C-style arrays (excluding char arrays which are handled as C-strings)
template<typename T>
inline auto to_string_impl(const T& value)
    -> typename std::enable_if<
        !has_to_string<T>::value && 
        !is_numeric<T>::value && 
        !is_special_type<T>::value &&
        !is_pair<T>::value &&
        !is_tuple<T>::value &&
        is_c_array<T>::value,
        std::string
    >::type {
    constexpr std::size_t array_size = std::extent<T>::value;
    std::ostringstream ss;
    ss << '[';
    
    for (std::size_t i = 0; i < array_size; ++i) {
        if (i > 0) ss << ", ";
        ss << apply_quotation_if_needed(value[i]);
    }
    
    ss << ']';
    return ss.str();
}

/** @} */ // end of implementation group

} // namespace details

/**
 * @defgroup api Public API
 * @brief Main library functions
 * @{
 */

/**
 * @brief Convert any value to string representation
 * 
 * This function provides universal string conversion with intelligent
 * type detection and optimal conversion strategy selection:
 * 
 * 1. If the type has a to_string() method, use it
 * 2. If the type is numeric, use std::to_string()
 * 3. If the type is std::pair, format as (first, second)
 * 4. If the type is std::tuple, format as (elem1, elem2, ...)
 * 5. If the type has cbegin/cend methods (containers), use iterator-based conversion
 * 6. If the type is streamable, use operator<<
 * 7. Otherwise, return type information with memory address
 * 
 * Special handling for common types:
 * - std::string: returned as-is
 * - const char*: converted to std::string
 * - bool: "true" or "false"
 * - char types: single character string
 * 
 * @tparam T Type of the value to convert
 * @param value Value to convert to string
 * @return String representation of the value
 * 
 * @code{.cpp}
 * // Basic types
 * auto s1 = ustr::to_string(42);           // "42"
 * auto s2 = ustr::to_string(3.14);         // "3.140000"
 * auto s3 = ustr::to_string(true);         // "true"
 * auto s4 = ustr::to_string('A');          // "A"
 * auto s5 = ustr::to_string("hello");      // "hello"
 * 
 * // Custom class with to_string
 * class Point {
 *     int x, y;
 * public:
 *     Point(int x, int y) : x(x), y(y) {}
 *     std::string to_string() const {
 *         return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
 *     }
 * };
 * Point p(1, 2);
 * auto s6 = ustr::to_string(p);            // "(1,2)"
 * 
 * // Pairs and tuples
 * std::pair<int, std::string> pair{42, "hello"};
 * auto s7 = ustr::to_string(pair);         // "(42, hello)"
 * 
 * std::tuple<int, double, std::string> tuple{1, 2.5, "world"};
 * auto s8 = ustr::to_string(tuple);        // "(1, 2.500000, world)"
 * @endcode
 */
template<typename T>
inline std::string to_string(const T& value) {
    return details::to_string_impl(value);
}

// Implementation of forward declaration for recursive calls within details namespace
namespace details {
    template<typename T>
    inline std::string to_string_forward(const T& value) {
        return to_string(value);
    }
}

// Forward declarations for helper functions
template<typename T>
inline void add_iterator_value(std::ostringstream& ss, const T& value, std::false_type);

template<typename T>
inline void add_iterator_value(std::ostringstream& ss, const T& value, std::true_type);

/**
 * @brief Convert a range defined by iterators to a string representation
 * 
 * This function converts a range of elements defined by iterators into
 * a comma-separated list surrounded by square brackets, e.g., [1, 2, 3].
 * For iterators pointing to key-value pairs (like those from std::map),
 * it uses a JSON-like format, e.g., {"key1": "value1", "key2": "value2"}.
 * 
 * @tparam IterT Type of the iterator
 * @param begin Begin iterator of the container
 * @param end End iterator of the container
 * @return String representation of the container contents
 * 
 * @code{.cpp}
 * // Vector example
 * std::vector<int> vec = {1, 2, 3};
 * auto s1 = ustr::to_string(vec.cbegin(), vec.cend());  // "[1, 2, 3]"
 * 
 * // Map example
 * std::map<std::string, int> map = {{"a", 1}, {"b", 2}};
 * auto s2 = ustr::to_string(map.cbegin(), map.cend());  // "{"a": 1, "b": 2}"
 * @endcode
 */ 
template<typename IterT>
inline std::string to_string(IterT begin, IterT end) {
    // Check if we're dealing with a key-value pair container (like std::map)
    using value_type = typename std::iterator_traits<IterT>::value_type;
    
    // Check for pair-like types (having first and second members)
    using pair_check = typename details::has_first_second<value_type>;
    const bool is_pair = pair_check::value;
    
    std::ostringstream ss;
    if (is_pair) {
        // Use JSON-like format for key-value pairs
        ss << '{';
    } else {
        // Use array format for regular containers
        ss << '[';
    }
    
    bool first = true;
    for (IterT it = begin; it != end; ++it) {
        if (!first) {
            ss << ", ";
        } else {
            first = false;
        }
        
        // Use template specialization to handle the different types
        add_iterator_value(ss, *it, typename details::has_first_second<value_type>::type());
    }
    
    if (is_pair) {
        ss << '}';
    } else {
        ss << ']';
    }
    
    return ss.str();
}

// Helper function to add iterator value when it's not a pair
template<typename T>
inline void add_iterator_value(std::ostringstream& ss, const T& value, std::false_type) {
    // Use shared quotation function
    ss << details::apply_quotation_if_needed(value);
}

// Helper function to add iterator value when it's a pair
template<typename T>
inline void add_iterator_value(std::ostringstream& ss, const T& value, std::true_type) {
    // Use shared quotation function for both key and value
    ss << details::apply_quotation_if_needed(value.first) << ": " << details::apply_quotation_if_needed(value.second);
}

/** @} */ // end of api group

/**
 * @defgroup formatters Local Scope Formatters
 * @brief Classes for customizing formatting within specific scopes
 * @{
 */

/**
 * @brief Base class for custom formatters
 * 
 * Allows customization of how specific types are converted to strings
 * within a local scope, enabling different formatting rules for the
 * same types in different contexts.
 */
template<typename T>
class formatter_base {
public:
    virtual ~formatter_base() = default;
    virtual std::string format(const T& value) const = 0;
};

/**
 * @brief Lambda-based formatter for easy inline customization
 * 
 * @tparam T Type to format
 * @tparam Func Function type (usually auto-deduced)
 */
template<typename T, typename Func>
class lambda_formatter : public formatter_base<T> {
private:
    Func func_;
public:
    explicit lambda_formatter(Func func) : func_(std::move(func)) {}
    std::string format(const T& value) const override {
        return func_(value);
    }
};

/**
 * @brief Format context for local customization
 * 
 * This class allows you to specify custom formatting rules for specific
 * types within a limited scope. When the context goes out of scope,
 * the default formatting is restored.
 * 
 * @code{.cpp}
 * format_context ctx;
 * 
 * // Custom bool formatter
 * ctx.set_formatter<bool>([](bool b) { return b ? "YES" : "NO"; });
 * 
 * // Custom float formatter with precision
 * ctx.set_formatter<float>([](float f) { 
 *     std::ostringstream ss;
 *     ss << std::fixed << std::setprecision(2) << f;
 *     return ss.str();
 * });
 * 
 * // Use custom formatting
 * std::string result1 = ctx.to_string(true);    // "YES"
 * std::string result2 = ctx.to_string(3.14159f); // "3.14"
 * @endcode
 */
class format_context {
private:
    std::map<std::type_index, std::shared_ptr<void>> formatters_;

public:
    /**
     * @brief Set a custom formatter for type T
     * @tparam T Type to format
     * @param formatter Formatter object
     */
    template<typename T>
    void set_formatter(std::shared_ptr<formatter_base<T>> formatter) {
        formatters_[std::type_index(typeid(T))] = formatter;
    }

    /**
     * @brief Set a custom formatter using a lambda function
     * @tparam T Type to format
     * @tparam Func Function type
     * @param func Lambda or function for formatting
     */
    template<typename T, typename Func>
    void set_formatter(Func func) {
        auto formatter = std::make_shared<lambda_formatter<T, Func>>(std::move(func));
        formatters_[std::type_index(typeid(T))] = formatter;
    }

    /**
     * @brief Convert value to string using custom formatters if available
     * @tparam T Type of value
     * @param value Value to convert
     * @return Formatted string
     */
    template<typename T>
    std::string to_string(const T& value) const {
        auto it = formatters_.find(std::type_index(typeid(T)));
        if (it != formatters_.end()) {
            auto formatter = std::static_pointer_cast<formatter_base<T>>(it->second);
            return formatter->format(value);
        }
        // Fall back to default formatting
        return ustr::to_string(value);
    }

    /**
     * @brief Check if a custom formatter is set for type T
     * @tparam T Type to check
     * @return true if custom formatter exists
     */
    template<typename T>
    bool has_formatter() const {
        return formatters_.find(std::type_index(typeid(T))) != formatters_.end();
    }

    /**
     * @brief Remove custom formatter for type T
     * @tparam T Type to remove formatter for
     */
    template<typename T>
    void remove_formatter() {
        formatters_.erase(std::type_index(typeid(T)));
    }

    /**
     * @brief Clear all custom formatters
     */
    void clear() {
        formatters_.clear();
    }
};

/** @} */ // end of formatters group

} // namespace ustr

#endif // __USTR_H__
