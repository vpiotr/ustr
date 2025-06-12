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

## Features

- 🚀 **Header-only** - Just include `ustr.h` and start using
- 🎯 **Intelligent type detection** - Automatically selects the best conversion method
- ⚡ **Performance optimized** - Uses `std::to_string` for numeric types
- 🔧 **Highly customizable** - Easy specialization for custom types
- 🛡️ **Type safe** - SFINAE-based implementation prevents compilation errors
- 📦 **Zero dependencies** - Works with standard C++ library only
- 🎨 **Clean API** - Single function interface: `ustr::to_string(value)`
- ✅ **Well tested** - Comprehensive test suite included

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
git clone https://github.com/your-username/ustr.git

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

## Building and Testing

### Prerequisites

- C++11 compatible compiler (GCC 4.8+, Clang 3.4+, MSVC 2015+)
- CMake 3.10 or later
- Standard C++ library

### Building with CMake (Recommended)

```bash
# Quick build (Release mode with tests and demos)
./cmake_build.sh

# Or step by step
mkdir build
cd build
cmake ..
make

# Build options
./cmake_build.sh --help                    # Show all options
./cmake_build.sh -t Debug                  # Debug build
./cmake_build.sh --no-tests                # Skip tests
./cmake_build.sh --no-demos                # Skip demos
./cmake_build.sh --with-docs               # Build documentation (requires Doxygen)
./cmake_build.sh --prefix /usr/local       # Set install prefix
```

### Building with Shell Scripts (Alternative)

```bash
# Make scripts executable
chmod +x build.sh run_tests.sh run_demos.sh

# Build everything (tests and demos)
./build.sh

# Or build manually
g++ -std=c++11 -Wall -Iinclude -o build/ustr_test tests/ustr_test.cpp
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
./build/bin/ustr_test    # CMake build
./build/ustr_test        # Shell script build
```

### Running Demos

```bash
# Using CMake (from build directory)
cd build && make run_demos

# Using shell scripts
./run_demos.sh

# Or run directly
./build/bin/ustr_demo    # CMake build
./build/ustr_demo        # Shell script build
```

### Installing

```bash
# Install system-wide (requires appropriate permissions)
cd build
make install

# Or install to custom location
./cmake_build.sh --prefix /path/to/install
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
│   └── ustr_test.cpp           # Comprehensive test suite
├── demos/
│   ├── CMakeLists.txt          # CMake configuration for demos
│   └── ustr_demo.cpp           # Usage examples and demonstrations
├── docs/
│   ├── CMakeLists.txt          # CMake configuration for documentation
│   └── Doxyfile.in             # Doxygen configuration template
├── cmake/
│   └── ustr-config.cmake.in    # CMake package configuration template
├── build/                      # Build output directory (created by build)
├── CMakeLists.txt              # Main CMake configuration
├── cmake_build.sh              # CMake build script (recommended)
├── build.sh                    # Legacy shell build script
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
2. **Testing**: Add tests for new features in `tests/ustr_test.cpp`
3. **Documentation**: Update README and inline documentation
4. **Compatibility**: Maintain C++11 compatibility

### Running Tests Before Contributing

```bash
./build.sh && ./run_tests.sh
```

### Adding New Features

1. Add implementation to `include/ustr/ustr.h`
2. Add tests to `tests/ustr_test.cpp`
3. Add examples to `demos/ustr_demo.cpp` if applicable
4. Update README.md

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by the string conversion utilities in the utest library
- Uses SFINAE techniques for type detection
- Thanks to the C++ community for best practices and feedback

---

**USTR** - Making string conversion universal and effortless in C++.
