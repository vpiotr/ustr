#include "../include/ustr/ustr.h"
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <limits>
#include <iomanip>
#include <sstream>

// Demo classes to showcase different conversion scenarios

class Point {
private:
    double x_, y_;
public:
    Point(double x, double y) : x_(x), y_(y) {}
    
    // Custom to_string method - will be detected automatically
    std::string to_string() const {
        return "Point(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
    }
    
    double getX() const { return x_; }
    double getY() const { return y_; }
};

class Rectangle {
private:
    Point topLeft_, bottomRight_;
public:
    Rectangle(const Point& tl, const Point& br) : topLeft_(tl), bottomRight_(br) {}
    
    // This class is streamable but doesn't have to_string
    friend std::ostream& operator<<(std::ostream& os, const Rectangle& rect) {
        return os << "Rectangle[" << rect.topLeft_.to_string() 
                  << " to " << rect.bottomRight_.to_string() << "]";
    }
};

class Color {
private:
    unsigned char r_, g_, b_;
public:
    Color(unsigned char r, unsigned char g, unsigned char b) : r_(r), g_(g), b_(b) {}
    
    // Non-streamable class - will show type info
    unsigned char getRed() const { return r_; }
    unsigned char getGreen() const { return g_; }
    unsigned char getBlue() const { return b_; }
};

class Temperature {
private:
    double celsius_;
public:
    Temperature(double celsius) : celsius_(celsius) {}
    
    // Both to_string and operator<< available
    std::string to_string() const {
        return std::to_string(celsius_) + u8"°C";
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Temperature& temp) {
        return os << temp.celsius_ << " degrees Celsius";
    }
};

void demonstrateBasicTypes() {
    std::cout << "\n=== Basic Type Conversions ===\n";
    
    // Numeric types
    std::cout << "Integer: " << ustr::to_string(42) << "\n";
    std::cout << "Negative integer: " << ustr::to_string(-123) << "\n";
    std::cout << "Float: " << ustr::to_string(3.14159f) << "\n";
    std::cout << "Double: " << ustr::to_string(2.718281828) << "\n";
    std::cout << "Long: " << ustr::to_string(1234567890L) << "\n";
    std::cout << "Unsigned: " << ustr::to_string(4294967295U) << "\n";
    
    // Boolean
    std::cout << "Boolean true: " << ustr::to_string(true) << "\n";
    std::cout << "Boolean false: " << ustr::to_string(false) << "\n";
    
    // Characters
    std::cout << "Character: " << ustr::to_string('A') << "\n";
    std::cout << "Special char (newline): '" << ustr::to_string('\n') << "' (newline)\n";
    std::cout << "Special char (tab): '" << ustr::to_string('\t') << "' (tab)\n";
    
    // Strings
    std::string stdString = "Hello, World!";
    std::cout << "std::string: " << ustr::to_string(stdString) << "\n";
    std::cout << "String literal: " << ustr::to_string("Hello from literal") << "\n";
    
    char charArray[] = "Char array string";
    std::cout << "Char array: " << ustr::to_string(charArray) << "\n";
}

void demonstrateCustomClasses() {
    std::cout << "\n=== Custom Class Conversions ===\n";
    
    // Class with custom to_string method
    Point p1(1.5, 2.7);
    std::cout << "Point with to_string(): " << ustr::to_string(p1) << "\n";
    
    // Streamable class
    Point p2(0.0, 0.0);
    Point p3(10.0, 5.0);
    Rectangle rect(p2, p3);
    std::cout << "Streamable Rectangle: " << ustr::to_string(rect) << "\n";
    
    // Non-streamable class (will show type info)
    Color red(255, 0, 0);
    std::cout << "Non-streamable Color: " << ustr::to_string(red) << "\n";
    
    // Class with both methods (to_string takes precedence)
    Temperature temp(23.5);
    std::cout << "Temperature (to_string precedence): " << ustr::to_string(temp) << "\n";
}

void demonstrateTypeTraits() {
    std::cout << "\n=== Type Trait Demonstrations ===\n";
    
    std::cout << "Point has_to_string: " << (ustr::has_to_string<Point>::value ? "true" : "false") << "\n";
    std::cout << "Rectangle has_to_string: " << (ustr::has_to_string<Rectangle>::value ? "true" : "false") << "\n";
    std::cout << "Color has_to_string: " << (ustr::has_to_string<Color>::value ? "true" : "false") << "\n";
    std::cout << "int has_to_string: " << (ustr::has_to_string<int>::value ? "true" : "false") << "\n";
    
    std::cout << "\nRectangle is_streamable: " << (ustr::is_streamable<Rectangle>::value ? "true" : "false") << "\n";
    std::cout << "Color is_streamable: " << (ustr::is_streamable<Color>::value ? "true" : "false") << "\n";
    std::cout << "int is_streamable: " << (ustr::is_streamable<int>::value ? "true" : "false") << "\n";
    std::cout << "std::string is_streamable: " << (ustr::is_streamable<std::string>::value ? "true" : "false") << "\n";
    
    std::cout << "\nint is_numeric: " << (ustr::is_numeric<int>::value ? "true" : "false") << "\n";
    std::cout << "double is_numeric: " << (ustr::is_numeric<double>::value ? "true" : "false") << "\n";
    std::cout << "bool is_numeric: " << (ustr::is_numeric<bool>::value ? "true" : "false") << "\n";
    std::cout << "char is_numeric: " << (ustr::is_numeric<char>::value ? "true" : "false") << "\n";
}

void demonstrateEdgeCases() {
    std::cout << "\n=== Edge Cases ===\n";
    
    // Zero values
    std::cout << "Zero int: " << ustr::to_string(0) << "\n";
    std::cout << "Zero double: " << ustr::to_string(0.0) << "\n";
    
    // Empty string
    std::string empty = "";
    std::cout << "Empty string: '" << ustr::to_string(empty) << "'\n";
    
    // Very large numbers
    std::cout << "Large number: " << ustr::to_string(1234567890123456789LL) << "\n";
    
    // Scientific notation (double)
    std::cout << "Scientific notation: " << ustr::to_string(1.23e-10) << "\n";
    
    // Special float values
    std::cout << "Infinity: " << ustr::to_string(std::numeric_limits<double>::infinity()) << "\n";
    std::cout << "NaN: " << ustr::to_string(std::numeric_limits<double>::quiet_NaN()) << "\n";
    
    // nullptr conversion
    std::cout << "\n--- nullptr Conversion ---\n";
    std::cout << "nullptr literal: " << ustr::to_string(nullptr) << "\n";
    std::nullptr_t null_value = nullptr;
    std::cout << "std::nullptr_t variable: " << ustr::to_string(null_value) << "\n";
    
    // Compare with null pointer string
    const char* null_char_ptr = nullptr;
    std::cout << "null char pointer: " << ustr::to_string(null_char_ptr) << "\n";
}

// Helper struct for debug logging (outside function for C++11 compatibility)
struct DebugLogger {
    template<typename T>
    void operator()(const std::string& message, const T& value) const {
        std::cout << "[DEBUG] " << message << ": " << ustr::to_string(value) << "\n";
    }
};

// Helper struct for error formatting (outside function for C++11 compatibility)
struct ErrorFormatter {
    template<typename ErrorCode, typename Timestamp>
    std::string operator()(const std::string& operation, const ErrorCode& errorCode, const Timestamp& timestamp) const {
        return "Error in " + operation + 
               " (code: " + ustr::to_string(errorCode) + 
               ", time: " + ustr::to_string(timestamp) + ")";
    }
};

void demonstrateRealWorldUsage() {
    std::cout << "\n=== Real-World Usage Examples ===\n";
    
    // Debug logging function - C++11 compatible version
    DebugLogger debugLog;
    
    debugLog("User ID", 12345);
    debugLog("Account balance", 1234.56);
    debugLog("Is premium user", true);
    debugLog("Username", std::string("john_doe"));
    
    Point userLocation(40.7128, -74.0060);  // NYC coordinates
    debugLog("User location", userLocation);
    
    // Configuration values
    std::cout << "\n--- Configuration Display ---\n";
    std::map<std::string, std::string> config;
    
    config["max_connections"] = ustr::to_string(100);
    config["timeout_seconds"] = ustr::to_string(30.5);
    config["debug_mode"] = ustr::to_string(true);
    config["server_name"] = ustr::to_string("web-server-01");
    
    // C++11 compatible iteration
    for (std::map<std::string, std::string>::const_iterator it = config.begin(); it != config.end(); ++it) {
        std::cout << it->first << " = " << it->second << "\n";
    }
    
    // Error message formatting - C++11 compatible version
    std::cout << "\n--- Error Message Formatting ---\n";
    ErrorFormatter formatError;
    
    std::cout << formatError("file_read", 404, 1703701234.567) << "\n";
    std::cout << formatError("network_connect", -1, 1703701235.123) << "\n";
}

void demonstrateIteratorConversion() {
    std::cout << "\n=== Iterator-Based Conversion Demo ===\n";
    
    // Vector of integers
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "Vector of ints: " << ustr::to_string(numbers.cbegin(), numbers.cend()) << "\n";
    
    // Vector of strings
    std::vector<std::string> words = {"hello", "world", "iterator", "conversion"};
    std::cout << "Vector of strings: " << ustr::to_string(words.cbegin(), words.cend()) << "\n";
    
    // Empty container
    std::vector<double> empty;
    std::cout << "Empty vector: " << ustr::to_string(empty.cbegin(), empty.cend()) << "\n";
    
    // Pair-like types (std::pair)
    std::vector<std::pair<std::string, int>> pairs = {{"one", 1}, {"two", 2}, {"three", 3}};
    std::cout << "Vector of pairs: " << ustr::to_string(pairs.cbegin(), pairs.cend()) << "\n";
    
    // Map container (key-value pairs)
    std::map<std::string, std::string> colors = {
        {"red", "#FF0000"}, 
        {"green", "#00FF00"}, 
        {"blue", "#0000FF"}
    };
    std::cout << "Map container: " << ustr::to_string(colors.cbegin(), colors.cend()) << "\n";
    
    // Using with a custom container or subset of elements
    std::cout << "Subset of vector: " << ustr::to_string(numbers.cbegin() + 1, numbers.cend() - 1) << "\n";
}

void demonstrateScopedFormatting() {
    std::cout << "\n=== Format Context Demo ===\n";
    
    // Create a format context
    ustr::format_context ctx;
    
    // Set custom formatters for different types
    ctx.set_formatter<bool>([](bool b) { 
        return b ? u8"✅ YES" : u8"❌ NO"; 
    });
    
    ctx.set_formatter<float>([](float f) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << f << "f";
        return ss.str();
    });
    
    ctx.set_formatter<double>([](double d) {
        std::ostringstream ss;
        ss << std::scientific << std::setprecision(3) << d;
        return ss.str();
    });
    
    // Demonstrate different formatting for same types
    std::cout << "Using custom formatters:\n";
    std::cout << "  bool true:  " << ctx.to_string(true) << "\n";
    std::cout << "  bool false: " << ctx.to_string(false) << "\n";
    std::cout << "  float pi:   " << ctx.to_string(3.14159f) << "\n";
    std::cout << "  double e:   " << ctx.to_string(2.71828) << "\n";
    std::cout << "  int (default): " << ctx.to_string(42) << "\n";
    
    std::cout << "\nUsing default formatting:\n";
    std::cout << "  bool true:  " << ustr::to_string(true) << "\n";
    std::cout << "  bool false: " << ustr::to_string(false) << "\n";
    std::cout << "  float pi:   " << ustr::to_string(3.14159f) << "\n";
    std::cout << "  double e:   " << ustr::to_string(2.71828) << "\n";
    
    // Show practical use case: configuration display
    std::cout << "\nPractical example - Configuration Display:\n";
    
    struct Config {
        bool debug_mode;
        bool verbose_logging;
        float timeout_seconds;
        double precision_threshold;
        int max_connections;
    } config = {true, false, 30.5f, 0.001, 100};
    
    std::cout << "Debug Mode: " << ctx.to_string(config.debug_mode) << "\n";
    std::cout << "Verbose Logging: " << ctx.to_string(config.verbose_logging) << "\n";
    std::cout << "Timeout: " << ctx.to_string(config.timeout_seconds) << "\n";
    std::cout << "Precision: " << ctx.to_string(config.precision_threshold) << "\n";
    std::cout << "Max Connections: " << ctx.to_string(config.max_connections) << "\n";
}

int main() {
    std::cout << "USTR Library Demo - Universal String Conversion\n";
    std::cout << "===============================================\n";
    
    try {
        demonstrateBasicTypes();
        demonstrateCustomClasses();
        demonstrateTypeTraits();
        demonstrateEdgeCases();
        demonstrateRealWorldUsage();
        demonstrateIteratorConversion();
        
        demonstrateScopedFormatting();
        
        std::cout << "\n=== Demo completed successfully! ===\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error during demo: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
