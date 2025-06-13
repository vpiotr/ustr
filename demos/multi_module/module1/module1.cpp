#include "ustr/ustr.h"
#include <string>
#include <iostream>
#include <cassert>

// Define a test function using standard assertions
bool test_module1_nullptr_handling() {
    // Test that nullptr is correctly converted to "null"
    std::nullptr_t null_value = nullptr;
    std::string result = ustr::to_string(null_value);
    assert(result == "null");
    
    // Test that null char pointer is also converted to "null"
    const char* null_ptr = nullptr;
    result = ustr::to_string(null_ptr);
    assert(result == "null");
    
    return true;
}

// Function that uses ustr to convert various values to strings
std::string module1_convert_values() {
    std::string result;
    
    // Convert a nullptr
    result += "nullptr: " + ustr::to_string(nullptr) + "\n";
    
    // Convert a null char pointer
    const char* null_ptr = nullptr;
    result += "null char*: " + ustr::to_string(null_ptr) + "\n";
    
    // Convert other basic types
    result += "int: " + ustr::to_string(42) + "\n";
    result += "bool: " + ustr::to_string(true) + "\n";
    result += "double: " + ustr::to_string(3.14159) + "\n";
    
    return result;
}

// Function to run the test
bool module1_run_test() {
    std::cout << "Running Module1 nullptr handling test..." << std::endl;
    bool success = test_module1_nullptr_handling();
    if (success) {
        std::cout << "Module1 test passed!" << std::endl;
    } else {
        std::cout << "Module1 test failed!" << std::endl;
    }
    return success;
}
