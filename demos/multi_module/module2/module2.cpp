#include "ustr/ustr.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cassert>

// Define a test function using standard assertions
bool test_module2_complex_types_handling() {
    // Test that containers are properly formatted
    std::vector<int> vec = {1, 2, 3};
    std::string result = ustr::to_string(vec.begin(), vec.end());
    assert(result == "[1, 2, 3]");
    
    // Test that nullptr is correctly converted to "null" in this module too
    std::nullptr_t null_value = nullptr;
    result = ustr::to_string(null_value);
    assert(result == "null");
    
    return true;
}

// Custom class with to_string method
class Module2Point {
private:
    int x_, y_;
public:
    Module2Point(int x, int y) : x_(x), y_(y) {}
    
    std::string to_string() const {
        return "Point(" + ustr::to_string(x_) + "," + ustr::to_string(y_) + ")";
    }
};

// Function that uses ustr to convert complex types to strings
std::string module2_convert_complex_values() {
    std::string result;
    
    // Convert a nullptr (same as in module1)
    result += "nullptr: " + ustr::to_string(nullptr) + "\n";
    
    // Convert a custom class with to_string method
    Module2Point point(10, 20);
    result += "Point: " + ustr::to_string(point) + "\n";
    
    // Convert containers
    std::vector<std::string> strings = {"hello", "world"};
    result += "Vector: " + ustr::to_string(strings.begin(), strings.end()) + "\n";
    
    std::map<std::string, int> map = {{"one", 1}, {"two", 2}};
    result += "Map: " + ustr::to_string(map.begin(), map.end()) + "\n";
    
    return result;
}

// Function to run the test
bool module2_run_test() {
    std::cout << "Running Module2 complex types handling test..." << std::endl;
    bool success = test_module2_complex_types_handling();
    if (success) {
        std::cout << "Module2 test passed!" << std::endl;
    } else {
        std::cout << "Module2 test failed!" << std::endl;
    }
    return success;
}
