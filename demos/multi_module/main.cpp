#include "module1/module1.h"
#include "module2/module2.h"
#include <iostream>

int main() {
    std::cout << "USTR Multi-Module Demo\n";
    std::cout << "=====================\n\n";
    
    // Run tests from both modules
    std::cout << "Running tests from Module 1...\n";
    bool module1_success = module1_run_test();
    
    std::cout << "\nRunning tests from Module 2...\n";
    bool module2_success = module2_run_test();
    
    // Display conversion results from both modules
    std::cout << "\nModule 1 Conversions:\n";
    std::cout << "-------------------\n";
    std::cout << module1_convert_values();
    
    std::cout << "\nModule 2 Conversions:\n";
    std::cout << "-------------------\n";
    std::cout << module2_convert_complex_values();
    
    // Overall success
    if (module1_success && module2_success) {
        std::cout << "\nAll tests passed successfully!\n";
        return 0;
    } else {
        std::cout << "\nSome tests failed!\n";
        return 1;
    }
}
