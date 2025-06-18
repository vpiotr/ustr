#include "../include/ustr/ustr.h"
#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <memory>
#include <limits>
#include <iomanip>
#include <sstream>
#include <tuple>
#include <set>
#include <deque>
#include <list>

// Include string_view for C++17 and later
#if __cplusplus >= 201703L
#include <string_view>
#endif

// Demo classes to showcase different conversion scenarios

class Vehicle {
private:
    std::string make_;
    std::string model_;
    int year_;
    
public:
    Vehicle(const std::string& make, const std::string& model, int year) 
        : make_(make), model_(model), year_(year) {}
    
    // Custom to_string method - highest priority
    std::string to_string() const {
        return ustr::quoted_str(make_) + " " + ustr::quoted_str(model_) + " (" + std::to_string(year_) + ")";
    }
    
    std::string getMake() const { return make_; }
    std::string getModel() const { return model_; }
    int getYear() const { return year_; }
};

class Engine {
private:
    double displacement_;
    int cylinders_;
    std::string fuel_type_;
    
public:
    Engine(double displacement, int cylinders, const std::string& fuel_type)
        : displacement_(displacement), cylinders_(cylinders), fuel_type_(fuel_type) {}
    
    // Streamable class without to_string - second priority
    friend std::ostream& operator<<(std::ostream& os, const Engine& engine) {
        return os << "Engine(" << engine.displacement_ << "L, " 
                  << engine.cylinders_ << " cylinders, " << ustr::quoted_str(engine.fuel_type_) << ")";
    }
    
    double getDisplacement() const { return displacement_; }
    int getCylinders() const { return cylinders_; }
    std::string getFuelType() const { return fuel_type_; }
};

class GPS {
private:
    double latitude_;
    double longitude_;
    
public:
    GPS(double lat, double lng) : latitude_(lat), longitude_(lng) {}
    
    // Non-streamable class - will show type info
    double getLatitude() const { return latitude_; }
    double getLongitude() const { return longitude_; }
};

class Car {
private:
    Vehicle vehicle_;
    Engine engine_;
    GPS gps_;
    std::vector<std::string> features_;
    
public:
    Car(const Vehicle& vehicle, const Engine& engine, const GPS& gps, 
        const std::vector<std::string>& features)
        : vehicle_(vehicle), engine_(engine), gps_(gps), features_(features) {}
    
    // This class has both to_string and operator<< - to_string takes precedence
    std::string to_string() const {
        return "Car[" + vehicle_.to_string() + " with " + ustr::to_string(engine_) + "]";
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Car& car) {
        return os << "Car::operator<<[" << car.vehicle_.getMake() << " " << car.vehicle_.getModel() << "]";
    }
    
    const Vehicle& getVehicle() const { return vehicle_; }
    const Engine& getEngine() const { return engine_; }
    const GPS& getGPS() const { return gps_; }
    const std::vector<std::string>& getFeatures() const { return features_; }
};

void demonstrateBasicTypes() {
    std::cout << "\n=== Basic Type Conversions ===\n";
    
    // Numeric types
    std::cout << "int: " << ustr::to_string(42) << "\n";
    std::cout << "negative int: " << ustr::to_string(-123) << "\n";
    std::cout << "float: " << ustr::to_string(3.14159f) << "\n";
    std::cout << "double: " << ustr::to_string(2.718281828) << "\n";
    std::cout << "long: " << ustr::to_string(1234567890L) << "\n";
    std::cout << "unsigned: " << ustr::to_string(4294967295U) << "\n";
    
    // Boolean and character types
    std::cout << "bool true: " << ustr::to_string(true) << "\n";
    std::cout << "bool false: " << ustr::to_string(false) << "\n";
    std::cout << "char: " << ustr::to_string('A') << "\n";
    std::cout << "special char: " << ustr::to_string('\n') << "\n";
    
    // String types
    std::string str = "Hello, World!";
    std::cout << "std::string: " << ustr::to_string(str) << "\n";
    std::cout << "string literal: " << ustr::to_string("Hello from literal") << "\n";
    
#if __cplusplus >= 201703L
    // C++17+ string_view support
    std::string_view sv = "std::string_view example";
    std::cout << "std::string_view: " << ustr::to_string(sv) << "\n";
    
    // String view from substring
    std::string original = "This is a long string";
    std::string_view substr(original.c_str() + 10, 4); // "long"
    std::cout << "string_view substring: " << ustr::to_string(substr) << "\n";
#endif
    
    // Pointers
    std::cout << "nullptr: " << ustr::to_string(nullptr) << "\n";
    const char* null_ptr = nullptr;
    std::cout << "null char*: " << ustr::to_string(null_ptr) << "\n";
}

void demonstrateCustomClasses() {
    std::cout << "\n=== Custom Class Conversions ===\n";
    
    // Class with custom to_string method (highest priority)
    Vehicle car_vehicle("Toyota", "Camry", 2023);
    std::cout << "Vehicle (to_string): " << ustr::to_string(car_vehicle) << "\n";
    
    // Streamable class without to_string (second priority)
    Engine car_engine(2.5, 4, "Gasoline");
    std::cout << "Engine (streamable): " << ustr::to_string(car_engine) << "\n";
    
    // Non-streamable class (fallback to type info)
    GPS car_gps(37.7749, -122.4194);
    std::cout << "GPS (non-streamable): " << ustr::to_string(car_gps) << "\n";
    
    // Class with both methods (to_string takes precedence)
    std::vector<std::string> features = {"Navigation", "Bluetooth", "Backup Camera"};
    Car my_car(car_vehicle, car_engine, car_gps, features);
    std::cout << "Car (to_string precedence): " << ustr::to_string(my_car) << "\n";
}

void demonstratePairsAndTuples() {
    std::cout << "\n=== Pairs and Tuples ===\n";
    
    // Basic pairs
    std::pair<int, std::string> basic_pair = {42, "answer"};
    std::cout << "Basic pair: " << ustr::to_string(basic_pair) << "\n";
    
    std::pair<double, bool> numeric_pair = {3.14, true};
    std::cout << "Numeric pair: " << ustr::to_string(numeric_pair) << "\n";
    
    // Nested pairs
    std::pair<std::pair<int, int>, std::string> nested_pair = {{10, 20}, "coordinates"};
    std::cout << "Nested pair: " << ustr::to_string(nested_pair) << "\n";
    
    // Basic tuples
    std::tuple<> empty_tuple;
    std::cout << "Empty tuple: " << ustr::to_string(empty_tuple) << "\n";
    
    std::tuple<int> single_tuple{42};
    std::cout << "Single tuple: " << ustr::to_string(single_tuple) << "\n";
    
    std::tuple<int, std::string, bool> basic_tuple{123, "hello", false};
    std::cout << "Basic tuple: " << ustr::to_string(basic_tuple) << "\n";
    
    // Complex nested structures
    std::tuple<std::pair<int, std::string>, std::vector<int>, double> complex_tuple{
        {99, "key"}, {1, 2, 3, 4, 5}, 2.718
    };
    std::cout << "Complex tuple: " << ustr::to_string(complex_tuple) << "\n";
}

void demonstrateContainers() {
    std::cout << "\n=== Container Conversions ===\n";
    
    // Vectors
    std::vector<int> int_vector = {1, 2, 3, 4, 5};
    std::cout << "Vector<int>: " << ustr::to_string(int_vector) << "\n";
    
    std::vector<std::string> string_vector = {"apple", "banana", "cherry", "date"};
    std::cout << "Vector<string>: " << ustr::to_string(string_vector) << "\n";
    
    std::vector<Vehicle> vehicle_vector = {
        Vehicle("Honda", "Civic", 2022),
        Vehicle("Ford", "F-150", 2023),
        Vehicle("Tesla", "Model 3", 2024)
    };
    std::cout << "Vector<Vehicle>: " << ustr::to_string(vehicle_vector) << "\n";
    
    // Arrays
    std::array<int, 6> int_array = {{10, 20, 30, 40, 50, 60}};
    std::cout << "Array<int>: " << ustr::to_string(int_array) << "\n";
    
    std::array<std::string, 3> color_array = {{"red", "green", "blue"}};
    std::cout << "Array<string>: " << ustr::to_string(color_array) << "\n";
    
    // Other container types
    std::deque<double> double_deque = {1.1, 2.2, 3.3, 4.4};
    std::cout << "Deque<double>: " << ustr::to_string(double_deque) << "\n";
    
    std::list<char> char_list = {'a', 'b', 'c', 'd', 'e'};
    std::cout << "List<char>: " << ustr::to_string(char_list) << "\n";
    
    std::set<int> int_set = {5, 3, 8, 1, 9, 2};
    std::cout << "Set<int>: " << ustr::to_string(int_set) << "\n";
    
#if __cplusplus >= 201703L
    // C++17+ string_view containers
    std::string str1 = "first", str2 = "second", str3 = "third";
    std::vector<std::string_view> sv_vector = {str1, str2, str3};
    std::cout << "Vector<string_view>: " << ustr::to_string(sv_vector) << "\n";
#endif
    
    // Empty containers
    std::vector<int> empty_vector;
    std::cout << "Empty vector: " << ustr::to_string(empty_vector) << "\n";
}

void demonstrateMapSpecializations() {
    std::cout << "\n=== Map Specializations (Key Quoting) ===\n";
    
    // String keys (should be quoted)
    std::map<std::string, int> string_key_map = {
        {"first", 1}, {"second", 2}, {"third", 3}
    };
    std::cout << "Map<string, int>: " << ustr::to_string(string_key_map) << "\n";
    
    // Numeric keys (should NOT be quoted)
    std::map<int, std::string> int_key_map = {
        {1, "one"}, {2, "two"}, {42, "answer"}, {100, "hundred"}
    };
    std::cout << "Map<int, string>: " << ustr::to_string(int_key_map) << "\n";
    
    std::map<double, std::string> double_key_map = {
        {1.5, "one and half"}, {2.718, "e"}, {3.14159, "pi"}
    };
    std::cout << "Map<double, string>: " << ustr::to_string(double_key_map) << "\n";
    
#if __cplusplus >= 201703L
    // C++17+ string_view keys and values (should be quoted like regular strings)
    std::string k1 = "view_key1", k2 = "view_key2";
    std::string v1 = "view_value1", v2 = "view_value2";
    std::map<std::string_view, std::string_view> sv_map = {{k1, v1}, {k2, v2}};
    std::cout << "Map<string_view, string_view>: " << ustr::to_string(sv_map) << "\n";
#endif
    
    // char keys (should NOT be quoted, as they're not string types)
    std::map<char, int> char_key_map = {
        {'a', 1}, {'b', 2}, {'z', 26}
    };
    std::cout << "Map<char, int>: " << ustr::to_string(char_key_map) << "\n";
    
    // Complex value types
    std::map<int, Vehicle> vehicle_map = {
        {1, Vehicle("BMW", "X5", 2023)},
        {2, Vehicle("Audi", "A4", 2022)},
        {3, Vehicle("Mercedes", "C-Class", 2024)}
    };
    std::cout << "Map<int, Vehicle>: " << ustr::to_string(vehicle_map) << "\n";
    
    // Nested containers as values
    std::map<int, std::vector<std::string>> nested_map = {
        {1, {"red", "green"}},
        {2, {"blue", "yellow", "purple"}},
        {3, {"orange"}}
    };
    std::cout << "Map<int, vector<string>>: " << ustr::to_string(nested_map) << "\n";
}

void demonstrateIteratorConversion() {
    std::cout << "\n=== Iterator-Based Conversion ===\n";
    
    std::vector<int> numbers = {10, 20, 30, 40, 50, 60, 70};
    
    // Full range
    std::cout << "Full range: " << ustr::to_string(numbers.cbegin(), numbers.cend()) << "\n";
    
    // Subset ranges
    std::cout << "First 3 elements: " << ustr::to_string(numbers.cbegin(), numbers.cbegin() + 3) << "\n";
    std::cout << "Last 3 elements: " << ustr::to_string(numbers.cend() - 3, numbers.cend()) << "\n";
    std::cout << "Middle elements: " << ustr::to_string(numbers.cbegin() + 2, numbers.cend() - 2) << "\n";
    
    // Mixed types with pairs
    std::vector<std::pair<int, std::string>> id_name_pairs = {
        {101, "Alice"}, {102, "Bob"}, {103, "Charlie"}
    };
    std::cout << "Pairs with numeric keys: " << ustr::to_string(id_name_pairs.cbegin(), id_name_pairs.cend()) << "\n";
    
    std::vector<std::pair<std::string, double>> name_score_pairs = {
        {"Alice", 95.5}, {"Bob", 87.2}, {"Charlie", 92.8}
    };
    std::cout << "Pairs with string keys: " << ustr::to_string(name_score_pairs.cbegin(), name_score_pairs.cend()) << "\n";
}

void demonstrateComplexNestedStructures() {
    std::cout << "\n=== Complex Nested Structures ===\n";
    
    // Company structure with departments and employees
    struct Employee {
        std::string name;
        int id;
        std::string department;
        
        std::string to_string() const {
            return ustr::quoted_str(name) + " (ID: " + std::to_string(id) + ", Dept: " + ustr::quoted_str(department) + ")";
        }
    };
    
    std::map<std::string, std::vector<Employee>> company = {
        {"Engineering", {
            {"Alice Johnson", 101, "Engineering"},
            {"Bob Smith", 102, "Engineering"},
            {"Carol Davis", 103, "Engineering"}
        }},
        {"Marketing", {
            {"David Wilson", 201, "Marketing"},
            {"Eva Brown", 202, "Marketing"}
        }},
        {"Sales", {
            {"Frank Miller", 301, "Sales"},
            {"Grace Lee", 302, "Sales"},
            {"Henry Chen", 303, "Sales"}
        }}
    };
    
    std::cout << "Company structure:\\n" << ustr::to_string(company) << "\n";
    
    // Multi-level nested containers
    std::map<int, std::map<std::string, std::vector<std::tuple<std::string, int, double>>>> mega_structure = {
        {2023, {
            {"Q1", {
                {"Product A", 100, 1500.50},
                {"Product B", 150, 2250.75}
            }},
            {"Q2", {
                {"Product A", 120, 1800.60},
                {"Product B", 180, 2700.90},
                {"Product C", 50, 750.25}
            }}
        }},
        {2024, {
            {"Q1", {
                {"Product A", 140, 2100.70},
                {"Product B", 200, 3000.00},
                {"Product C", 75, 1125.37}
            }}
        }}
    };
    
    std::cout << "\\nMega nested structure (Year -> Quarter -> Sales Data):\\n" 
              << ustr::to_string(mega_structure) << "\n";
}

void demonstrateScopedFormatting() {
    std::cout << "\n=== Scoped Custom Formatting ===\n";
    
    ustr::format_context ctx;
    
    // Custom boolean formatter
    ctx.set_formatter<bool>([](bool b) { return b ? "YES" : "NO"; });
    
    // Custom double formatter with precision
    ctx.set_formatter<double>([](double d) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << d;
        return ss.str();
    });
    
    std::cout << "Custom bool formatting: " << ctx.to_string(true) << ", " << ctx.to_string(false) << "\n";
    std::cout << "Custom double formatting: " << ctx.to_string(3.14159265359) << "\n";
    
    // Note: Custom formatters work only on direct calls to ctx.to_string()
    // When using containers, the elements are converted using the default ustr::to_string()
    // This is by design to maintain performance and simplicity
    
    std::cout << "Default formatting comparison:\n";
    std::cout << "  Default bool: " << ustr::to_string(true) << ", " << ustr::to_string(false) << "\n";
    std::cout << "  Default double: " << ustr::to_string(3.14159265359) << "\n";
    
    // Individual element formatting using custom context
    std::vector<bool> bool_vector = {true, false, true, true, false};
    std::cout << "\nManual element formatting with custom context:\n";
    std::cout << "  Bool vector elements: ";
    for (size_t i = 0; i < bool_vector.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << ctx.to_string(bool_vector[i]);
    }
    std::cout << "\n";
    
    std::cout << "  Default vector formatting: " << ustr::to_string(bool_vector) << "\n";
}

void demonstrateRealWorldScenario() {
    std::cout << "\n=== Real-World Scenario: Vehicle Fleet Management ===\n";
    
    // Create a fleet of vehicles with detailed information
    std::map<int, std::tuple<Vehicle, Engine, std::vector<std::string>, std::pair<double, double>>> fleet = {
        {1001, {
            Vehicle("Toyota", "Prius", 2023),
            Engine(1.8, 4, "Hybrid"),
            {"Navigation", "Backup Camera", "Lane Assist"},
            {37.7749, -122.4194}  // San Francisco coordinates
        }},
        {1002, {
            Vehicle("Ford", "F-150", 2023),
            Engine(3.5, 6, "Gasoline"),
            {"4WD", "Towing Package", "Bed Liner"},
            {40.7128, -74.0060}  // New York coordinates
        }},
        {1003, {
            Vehicle("Tesla", "Model S", 2024),
            Engine(0.0, 0, "Electric"),
            {"Autopilot", "Premium Sound", "Glass Roof"},
            {34.0522, -118.2437}  // Los Angeles coordinates
        }}
    };
    
    std::cout << "Vehicle Fleet Database:\\n" << ustr::to_string(fleet) << "\n";
    
    // Service records with nested data
    std::map<int, std::vector<std::tuple<std::string, std::string, double>>> service_records = {
        {1001, {
            {"2023-01-15", "Oil Change", 45.99},
            {"2023-03-20", "Tire Rotation", 25.00},
            {"2023-06-10", "Brake Inspection", 0.00}
        }},
        {1002, {
            {"2023-02-01", "Oil Change", 65.99},
            {"2023-04-15", "Transmission Service", 150.00},
            {"2023-07-22", "Air Filter Replacement", 35.50}
        }},
        {1003, {
            {"2023-03-10", "Software Update", 0.00},
            {"2023-05-18", "Tire Rotation", 80.00},
            {"2023-08-05", "Annual Inspection", 120.00}
        }}
    };
    
    std::cout << "\\nService Records (Vehicle ID -> Services):\\n" << ustr::to_string(service_records) << "\n";
}

int main() {
    std::cout << "USTR Universal String Conversion Library - Comprehensive Demo\n";
    std::cout << "=============================================================\n";
    
    try {
        demonstrateBasicTypes();
        demonstrateCustomClasses();
        demonstratePairsAndTuples();
        demonstrateContainers();
        demonstrateMapSpecializations();
        demonstrateIteratorConversion();
        demonstrateComplexNestedStructures();
        demonstrateScopedFormatting();
        demonstrateRealWorldScenario();
        
        std::cout << "\n=== Demo Complete ===\n";
        std::cout << "This demo showcased:\n";
        std::cout << "• Basic type conversions (numeric, boolean, character, string)\n";
        std::cout << "• Custom class conversions with priority ordering\n";
        std::cout << "• Pair and tuple specializations with nesting\n";
        std::cout << "• Container support (vector, array, deque, list, set)\n";
        std::cout << "• Map key quoting logic (strings quoted, numerics not)\n";
        std::cout << "• Iterator-based conversions with range subsets\n";
        std::cout << "• Complex nested data structures\n";
        std::cout << "• Scoped custom formatters\n";
        std::cout << "• Real-world scenario with vehicle fleet management\n";
        std::cout << "\nAll conversions used a single universal function: ustr::to_string()\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error during demo: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
