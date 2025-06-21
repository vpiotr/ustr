# USTR - Universal String Conversion Library

[![CI](https://github.com/vpiotr/ustr/actions/workflows/ci.yml/badge.svg)](https://github.com/vpiotr/ustr/actions/workflows/ci.yml)
[![C++](https://img.shields.io/badge/C%2B%2B-11%2F14%2F17%2F20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Header Only](https://img.shields.io/badge/header--only-yes-brightgreen.svg)]()

A simple, lightweight, header-only C++ library for universal string conversion with intelligent type detection and optimal conversion strategy selection.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Usage Examples](#usage-examples)
- [Type Detection System](#type-detection-system)
- [API Reference](#api-reference)
- [Building and Testing](#building-and-testing)
  - [Running Demos](#running-demos)
- [Performance Considerations](#performance-considerations)
- [Contributing](#contributing)
- [License](#license)

## Overview

USTR provides a single, universal `to_string()` function that intelligently converts any C++ type to its string representation using the most appropriate method available. The library uses SFINAE-based type detection to automatically choose the optimal conversion strategy for each type.

### Conversion Priority Order

1. **Custom `to_string()` method** - If the type has a public `to_string()` method
2. **Numeric types** - Uses `std::to_string()` for optimal performance
3. **Streamable types** - Uses `operator<<` with `std::ostringstream`
4. **Fallback** - Returns type information with memory address for non-convertible types

## Key Features at a Glance

```cpp
#include "ustr/ustr.h"
#include <vector>
#include <map>

// Works with ANY type - one universal function
class Point {
    double x, y;
public:
    Point(double x, double y) : x(x), y(y) {}
    std::string to_string() const { return "(" + std::to_string(x) + "," + std::to_string(y) + ")"; }
};

int main() {
    // Basic types
    auto s1 = ustr::to_string(42);              // "42"
    auto s2 = ustr::to_string(3.14159);         // "3.141590"
    auto s3 = ustr::to_string(true);            // "true"
    auto s4 = ustr::to_string('A');             // "A"
    
    // Custom classes (automatic detection)
    Point p(1.5, 2.7);
    auto s5 = ustr::to_string(p);               // "(1.500000,2.700000)"
    
    // Containers and pairs (with specializations)
    std::vector<int> vec = {1, 2, 3};
    auto s6 = ustr::to_string(vec);             // "[1, 2, 3]"
    
    std::pair<int, std::string> pair = {42, "hello"};
    auto s7 = ustr::to_string(pair);            // "(42, hello)"
    
    std::map<std::string, int> map = {{"key", 123}};
    auto s8 = ustr::to_string(map);             // {"key": 123}
    
    // Enum support (integer or symbolic)
    enum Status { PENDING, APPROVED, REJECTED };
    auto s9 = ustr::to_string(APPROVED);        // "1" (or "APPROVED" with specialization)
    
    // Custom specializations possible
    long long x = 123456789012345LL;
    auto s10 = ustr::to_string(x);              // Can be customized to "123456789012345LL"
    
    return 0;
}
```

**What makes USTR special:**
- **One function for everything** - `ustr::to_string()` works with any type
- **Intelligent detection** - Automatically finds the best conversion method
- **Zero overhead** - Uses optimal conversion for each type
- **Fully customizable** - Easy specialization and custom implementations
- **Header-only** - Just include and use, no dependencies

## Features

- **Header-only** - Just include `ustr.h` and start using
- **Intelligent type detection** - Automatically selects the best conversion method
- **Performance optimized** - Uses `std::to_string` for numeric types
- **Highly customizable** - Easy specialization for custom types
- **Type safe** - SFINAE-based implementation prevents compilation errors
- **Zero dependencies** - Works with standard C++ library only
- **Clean API** - Single function interface: `ustr::to_string(value)`
- **Container support** - Works with vectors, maps, pairs, tuples, arrays
- **Enum conversion** - Multiple approaches for enum to string conversion
- **Well tested** - Comprehensive test suite included

## Quick Start

```cpp
#include "ustr/ustr.h"
#include <iostream>

int main() {
    // Basic types
    std::cout << ustr::to_string(42) << "\n";           // "42"
    std::cout << ustr::to_string(3.14) << "\n";         // "3.140000"
    std::cout << ustr::to_string(true) << "\n";         // "true"
    std::cout << ustr::to_string('A') << "\n";          // "A"
    std::cout << ustr::to_string("hello") << "\n";      // "hello"
    
    return 0;
}
```

## Installation

USTR is a header-only library. Simply:

1. Copy `include/ustr/ustr.h` to your project
2. Include the header: `#include "ustr/ustr.h"`
3. Use the library: `ustr::to_string(value)`

### Requirements

- C++11 or later
- Standard library support for `<string>`, `<sstream>`, `<type_traits>`

### Using Git

```bash
# Clone the repository
git clone https://github.com/vpiotr/ustr.git

# Include in your project
#include "ustr/include/ustr/ustr.h"
```

## Usage Examples

### Basic Types

```cpp
#include "ustr/ustr.h"

// Numeric types (uses std::to_string)
auto s1 = ustr::to_string(42);              // "42"
auto s2 = ustr::to_string(-123);            // "-123"
auto s3 = ustr::to_string(3.14159f);        // "3.141590"
auto s4 = ustr::to_string(2.718281828);     // "2.718282"

// Boolean (special handling)
auto s5 = ustr::to_string(true);            // "true"
auto s6 = ustr::to_string(false);           // "false"

// Characters (special handling)
auto s7 = ustr::to_string('A');             // "A"
auto s8 = ustr::to_string('\n');            // "\n"

// Strings (passthrough)
std::string str = "Hello";
auto s9 = ustr::to_string(str);             // "Hello"
auto s10 = ustr::to_string("World");        // "World"
```

### Custom Classes

```cpp
// Class with custom to_string method
class Point {
private:
    double x_, y_;
public:
    Point(double x, double y) : x_(x), y_(y) {}
    
    std::string to_string() const {
        return "(" + std::to_string(x_) + "," + std::to_string(y_) + ")";
    }
};

Point p(1.5, 2.7);
auto result = ustr::to_string(p);           // "(1.500000,2.700000)"
```

### Streamable Classes

```cpp
// Class with operator<< (no to_string method)
class Rectangle {
private:
    Point topLeft_, bottomRight_;
public:
    Rectangle(Point tl, Point br) : topLeft_(tl), bottomRight_(br) {}
    
    friend std::ostream& operator<<(std::ostream& os, const Rectangle& rect) {
        return os << "Rectangle[" << rect.topLeft_.to_string() 
                  << " to " << rect.bottomRight_.to_string() << "]";
    }
};

Rectangle rect(Point(0,0), Point(10,5));
auto result = ustr::to_string(rect);        // "Rectangle[(0.000000,0.000000) to (10.000000,5.000000)]"
```

### Priority Demonstration

```cpp
// Class with both to_string() and operator<< 
// (to_string takes precedence)
class Temperature {
private:
    double celsius_;
public:
    Temperature(double c) : celsius_(c) {}
    
    std::string to_string() const {
        return std::to_string(celsius_) + "°C";
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Temperature& temp) {
        return os << temp.celsius_ << " degrees Celsius";
    }
};

Temperature temp(23.5);
auto result = ustr::to_string(temp);        // "23.500000°C" (uses to_string, not operator<<)
```

### Debug and Logging

```cpp
// Universal debug logging function
template<typename T>
void debugLog(const std::string& name, const T& value) {
    std::cout << "[DEBUG] " << name << " = " << ustr::to_string(value) << std::endl;
}

debugLog("user_id", 12345);
debugLog("balance", 1234.56);
debugLog("is_premium", true);
debugLog("location", Point(40.7128, -74.0060));
```

### Enum Conversions

USTR supports multiple approaches for enum to string conversion:

```cpp
// Approach 1: Default integer conversion (automatic)
enum Color { RED, GREEN, BLUE };
auto s1 = ustr::to_string(GREEN);           // "1" (shows underlying value)

// Approach 2: Symbolic conversion with custom specialization
enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

// Enable custom specialization
namespace ustr {
    template<> struct has_custom_specialization<LogLevel> : std::true_type {};
    namespace details {
        template<>
        inline std::string to_string_impl<LogLevel>(const LogLevel& value) {
            switch(value) {
                case LogLevel::DEBUG: return "DEBUG";
                case LogLevel::INFO: return "INFO"; 
                case LogLevel::WARNING: return "WARNING";
                case LogLevel::ERROR: return "ERROR";
                default: return "UNKNOWN";
            }
        }
    }
}

auto s2 = ustr::to_string(LogLevel::WARNING); // "WARNING" (symbolic name)

// Enums work in containers too
std::vector<Color> colors = {RED, GREEN, BLUE};
auto s3 = ustr::to_string(colors);           // "[0, 1, 2]" or symbolic if specialized

std::map<LogLevel, std::string> messages = {
    {LogLevel::INFO, "All good"}, 
    {LogLevel::ERROR, "Failed"}
};
auto s4 = ustr::to_string(messages);         // {INFO: "All good", ERROR: "Failed"}
```

## Type Detection System

USTR uses three main type traits for intelligent conversion:

### `ustr::has_to_string<T>`

Detects if type `T` has a public `to_string()` method:

```cpp
class MyClass {
public:
    std::string to_string() const { return "MyClass"; }
};

static_assert(ustr::has_to_string<MyClass>::value, "Should detect to_string method");
static_assert(!ustr::has_to_string<int>::value, "int doesn't have to_string method");
```

### `ustr::is_streamable<T>`

Detects if type `T` can be used with `operator<<`:

```cpp
static_assert(ustr::is_streamable<int>::value, "int is streamable");
static_assert(ustr::is_streamable<std::string>::value, "string is streamable");

class NonStreamable {
    int value;
    // No operator<< defined
};
static_assert(!ustr::is_streamable<NonStreamable>::value, "Not streamable");
```

### `ustr::is_numeric<T>`

Detects numeric types that should use `std::to_string()`:

```cpp
static_assert(ustr::is_numeric<int>::value, "int is numeric");
static_assert(ustr::is_numeric<double>::value, "double is numeric");
static_assert(!ustr::is_numeric<bool>::value, "bool gets special handling");
static_assert(!ustr::is_numeric<char>::value, "char gets special handling");
```

### Custom Specializations

You can specialize the type traits for your own types:

```cpp
// For types where auto-detection doesn't work
namespace ustr {
    template<>
    struct has_to_string<MySpecialClass> : std::true_type {};
}
```

### Custom `to_string_impl` Specializations

For complete control over how specific types are converted, you can provide explicit specializations of the `to_string_impl` function:

```cpp
// Step 1: Mark the type as having a custom specialization
namespace ustr {
    template<>
    struct has_custom_specialization<MyType> : std::true_type {};
}

// Step 2: Provide the specialization
namespace ustr {
    namespace details {
        template<>
        inline std::string to_string_impl<MyType>(const MyType& value) {
            return "custom:" + std::to_string(value.getValue());
        }
    }
}
```

This approach gives you the highest priority in the conversion hierarchy and allows you to completely override the default behavior for any type.

**Example with long long:**
```cpp
namespace ustr {
    template<>
    struct has_custom_specialization<long long> : std::true_type {};
}

namespace ustr {
    namespace details {
        template<>
        inline std::string to_string_impl<long long>(const long long& value) {
            return std::to_string(value) + "LL";  // Add suffix
        }
    }
}

long long x = 123456789012345LL;
auto result = ustr::to_string(x);  // "123456789012345LL"
```

## API Reference

### Main Function

#### `ustr::to_string(const T& value)`

Converts any value to its string representation using the optimal method.

**Parameters:**
- `value` - The value to convert (any type)

**Returns:**
- `std::string` - String representation of the value

**Example:**
```cpp
std::string result = ustr::to_string(42);
```

### Type Traits

#### `ustr::has_to_string<T>::value`

Boolean constant indicating if type `T` has a `to_string()` method.

#### `ustr::is_streamable<T>::value`

Boolean constant indicating if type `T` supports `operator<<`.

#### `ustr::is_numeric<T>::value`

Boolean constant indicating if type `T` should use `std::to_string()`.

#### `ustr::has_custom_specialization<T>::value`

Boolean constant indicating if type `T` has a custom `to_string_impl` specialization. This trait can be specialized to mark types that have explicit specializations defined, giving them the highest priority in the conversion hierarchy.

## Building and Testing

### Prerequisites

- C++11 compatible compiler (GCC 4.8+, Clang 3.4+, MSVC 2015+)
- CMake 3.10 or later
- Standard C++ library

### Building with CMake (Recommended)

```bash
# Quick build (Release mode with tests and demos)
./rebuild.sh

# Or step by step
mkdir build
cd build
cmake ..
make

# Build options
./rebuild.sh --help                    # Show all options
./rebuild.sh -t Debug                  # Debug build
./rebuild.sh --no-tests                # Skip tests
./rebuild.sh --no-demos                # Skip demos
./rebuild.sh --with-docs               # Build documentation (requires Doxygen)
./rebuild.sh --prefix /usr/local       # Set install prefix
```

### Building with Shell Scripts (Alternative)

```bash
# Make scripts executable
chmod +x rebuild.sh run_tests.sh run_demos.sh

# Build everything (tests and demos)
./rebuild.sh

# Or build manually
g++ -std=c++11 -Wall -Iinclude -o build/ustr_core_features_test tests/ustr_core_features_test.cpp
g++ -std=c++11 -Wall -Iinclude -o build/ustr_container_test tests/ustr_container_test.cpp
g++ -std=c++11 -Wall -Iinclude -o build/ustr_demo demos/ustr_demo.cpp
```

### Running Tests

```bash
# Using CMake (from build directory)
cd build && make run_tests
# Or
cd build && ctest

# Using shell scripts
./run_tests.sh

# Or run directly
./build/bin/ustr_core_features_test  # Core features tests (CMake build)
./build/bin/ustr_container_test      # Container tests (CMake build)
```

### Running Demos

USTR includes several comprehensive demos that showcase different aspects of the library:

#### Basic Demo (`ustr_demo`)
```bash
./build/bin/ustr_demo
```
Demonstrates fundamental USTR functionality including:
- Basic type conversions (numeric, boolean, character, string)
- Custom classes with `to_string()` methods vs streamable classes
- Type detection and conversion priority
- Container support (vectors, maps, pairs, tuples)
- Non-streamable types and fallback behavior

#### Comprehensive Demo (`comprehensive_demo`)
```bash
./build/bin/comprehensive_demo
```
The most complete demonstration featuring:
- **All basic types**: integers, floats, booleans, characters, strings
- **Custom class hierarchies**: Vehicle, Engine, GPS, Car classes showing different conversion approaches
- **Complex containers**: Nested vectors, maps with mixed types, arrays, deques, lists, sets
- **Real-world scenarios**: Vehicle fleet management system with service records
- **Advanced features**: Custom formatting contexts, iterator-based conversions
- **Scoped formatting**: Temporary custom formatters for specific use cases

#### Enum Conversion Demo (`enum_conversion_demo`)
```bash
./build/bin/enum_conversion_demo
```
**Comprehensive guide to enum string conversion with three approaches:**

**1. Default Integer Conversion (Automatic)**
- Works immediately with any enum, no setup required
- Shows underlying integer values
- Perfect for performance-critical code or internal IDs
```cpp
enum BasicColor { RED, GREEN, BLUE };
// ustr::to_string(GREEN) returns "1"
```

**2. Manual Custom Specialization (Full Control)**
- Complete control over conversion logic
- More verbose but maximum flexibility
- Ideal for complex formatting requirements
```cpp
enum class LogLevel : int { DEBUG = 1, INFO = 2, WARNING = 3, ERROR = 4 };
// Custom specialization returns "WARNING" instead of "3"
```

**3. Macro-Assisted Symbolic Conversion (Recommended)**
- Easy to implement with minimal boilerplate
- Human-readable symbolic names
- Best balance of simplicity and readability
```cpp
enum Direction { NORTH, SOUTH, EAST, WEST };
// Macro setup returns "NORTH" instead of "0"
```

The demo shows:
- All three approaches with practical examples
- Usage in containers (vectors, maps, pairs, tuples)
- Mixed usage scenarios (combining different approaches)
- Performance and readability trade-offs
- Usage guidelines and recommendations

#### Multi-Module Demo (`multi_module_demo`)
```bash
./build/bin/multi_module_demo
```
Demonstrates USTR usage across separate compilation units:
- **Module separation**: Shows how USTR works across different modules
- **Header-only verification**: Confirms no linking issues
- **Cross-module compatibility**: Custom types defined in one module work in another
- **Build system integration**: Example of multi-module CMake setup

#### Running All Demos
```bash
# Using CMake (from build directory)
cd build && make run_demos

# Using shell scripts
./run_demos.sh

# Or run individual demos directly
./build/bin/ustr_demo              # Basic functionality
./build/bin/comprehensive_demo     # Complete feature showcase  
./build/bin/enum_conversion_demo   # Enum conversion approaches
./build/bin/multi_module_demo      # Multi-module usage
```

### Installing

```bash
# Install system-wide (requires appropriate permissions)
cd build
make install

# Or install to custom location
./rebuild.sh --prefix /path/to/install
cd build
make install
```

### Using USTR in Your CMake Project

After installation, you can use USTR in your CMake projects:

```cmake
find_package(ustr REQUIRED)
target_link_libraries(your_target PRIVATE ustr::ustr)
```

Or include it directly:

```cmake
add_subdirectory(path/to/ustr)
target_link_libraries(your_target PRIVATE ustr::ustr)
```

### Project Structure

```
ustr/
├── include/
│   ├── ustr/
│   │   └── ustr.h              # Main header file
│   └── utest/
│       └── utest.h             # Testing framework (included)
├── tests/
│   ├── CMakeLists.txt          # CMake configuration for tests
│   ├── ustr_core_features_test.cpp    # Core features test suite
│   ├── ustr_container_test.cpp        # Container-specific test suite
│   ├── ustr_custom_classes_test.cpp   # Custom classes test suite
│   ├── ustr_enum_test.cpp             # Enum conversion test suite
│   ├── ustr_format_context_test.cpp   # Format context test suite
│   ├── ustr_pair_test.cpp             # Pair conversion test suite
│   ├── ustr_tuple_test.cpp            # Tuple conversion test suite
│   ├── ustr_custom_specialization_test.cpp  # Custom specialization tests
│   └── ustr_quoted_str_test.cpp       # Quoted string test suite
├── demos/
│   ├── CMakeLists.txt          # CMake configuration for demos
│   ├── ustr_demo.cpp           # Basic usage examples and demonstrations
│   ├── comprehensive_demo.cpp  # Complete feature showcase with real-world scenarios
│   ├── enum_conversion_demo.cpp # Enum conversion approaches (integer, symbolic, custom)
│   └── multi_module/           # Multi-module demo project
│       ├── main.cpp            # Main demo coordinator
│       ├── module1/            # First module with basic types
│       └── module2/            # Second module with custom classes
├── docs/
│   ├── CMakeLists.txt          # CMake configuration for documentation
│   └── Doxyfile.in             # Doxygen configuration template
├── cmake/
│   └── ustr-config.cmake.in    # CMake package configuration template
├── build/                      # Build output directory (created by build)
├── CMakeLists.txt              # Main CMake configuration
├── rebuild.sh                  # CMake build script (recommended)
├── build_docs.sh               # Documentation build script
├── run_tests.sh               # Test runner script
├── run_demos.sh               # Demo runner script
└── README.md                  # This file
```

## Performance Considerations

USTR is designed for performance:

1. **Zero overhead for basic types** - Direct forwarding to `std::to_string()`
2. **Compile-time type detection** - No runtime type checking
3. **Minimal template instantiation** - Efficient SFINAE implementation
4. **String specializations** - Direct return for string types

### Benchmarks

Basic performance characteristics (relative to manual conversion):

- **Numeric types**: ~100% (identical to `std::to_string`)
- **String types**: ~100% (direct passthrough)
- **Custom `to_string()`**: ~100% (direct method call)
- **Streamable types**: ~95-98% (small `ostringstream` overhead)

## Contributing

Contributions are welcome! Please follow these guidelines:

1. **Code Style**: Follow existing style (spaces, naming conventions)
2. **Testing**: Add tests for new features in appropriate test file:
   - Core functionality: `tests/ustr_core_features_test.cpp`
   - Container functionality: `tests/ustr_container_test.cpp`
   - Custom classes: `tests/ustr_custom_classes_test.cpp`
   - Enum support: `tests/ustr_enum_test.cpp`
   - Format context: `tests/ustr_format_context_test.cpp`
   - Pair support: `tests/ustr_pair_test.cpp`
   - Tuple support: `tests/ustr_tuple_test.cpp`
   - Custom specializations: `tests/ustr_custom_specialization_test.cpp`
   - Quoted strings: `tests/ustr_quoted_str_test.cpp`
3. **Documentation**: Update README and inline documentation
4. **Compatibility**: Maintain C++11 compatibility

### Running Tests Before Contributing

```bash
./rebuild.sh && ./run_tests.sh
```

### Adding New Features

1. Add implementation to `include/ustr/ustr.h`
2. Add tests to appropriate test file:
   - Core functionality: `tests/ustr_core_features_test.cpp`
   - Container functionality: `tests/ustr_container_test.cpp`
   - Custom classes: `tests/ustr_custom_classes_test.cpp`
   - Enum support: `tests/ustr_enum_test.cpp`
   - Format context: `tests/ustr_format_context_test.cpp`
   - Pair support: `tests/ustr_pair_test.cpp`
   - Tuple support: `tests/ustr_tuple_test.cpp`
   - Custom specializations: `tests/ustr_custom_specialization_test.cpp`
   - Quoted strings: `tests/ustr_quoted_str_test.cpp`
3. **Examples**: Add examples to appropriate demo files if applicable:
   - Basic examples: `demos/ustr_demo.cpp`
   - Complex scenarios: `demos/comprehensive_demo.cpp` 
   - Enum features: `demos/enum_conversion_demo.cpp`
   - Multi-module usage: `demos/multi_module/`
4. Update README.md

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by the string conversion utilities in the utest library
- Uses SFINAE techniques for type detection
- Thanks to the C++ community for best practices and feedback

---

**USTR** - Making string conversion universal and effortless in C++.
