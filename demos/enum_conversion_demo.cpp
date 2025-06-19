#include "../include/ustr/ustr.h"
#include <iostream>
#include <vector>
#include <map>

/**
 * @file enum_conversion_demo.cpp
 * @brief Comprehensive demonstration of enum conversion approaches in ustr library
 * 
 * This demo shows three approaches for enum to string conversion:
 * 1. Default integer conversion (automatic, no setup required)
 * 2. Manual custom specialization (full control, more verbose)
 * 3. Macro-assisted symbolic conversion (recommended for readable output)
 */

//=============================================================================
// APPROACH 1: Default Integer Conversion (No additional code needed)
//=============================================================================

enum BasicColor { RED, GREEN, BLUE };
enum Priority { LOW = 10, MEDIUM = 20, HIGH = 30 };

//=============================================================================
// APPROACH 2: Manual Custom Specialization for Complete Control
//=============================================================================

enum class LogLevel : int { DEBUG = 1, INFO = 2, WARNING = 3, ERROR = 4 };

// Step 1: Enable custom specialization
namespace ustr {
    template<>
    struct has_custom_specialization<LogLevel> : std::true_type {};
}

// Step 2: Provide the custom implementation
namespace ustr {
    namespace details {
        template<>
        inline std::string to_string_impl<LogLevel>(const LogLevel& value) {
            switch(value) {
                case LogLevel::DEBUG: return "DEBUG";
                case LogLevel::INFO: return "INFO"; 
                case LogLevel::WARNING: return "WARNING";
                case LogLevel::ERROR: return "ERROR";
                default: return "UNKNOWN_LOGLEVEL";
            }
        }
    }
}

//=============================================================================
// APPROACH 3: Macro-Assisted Symbolic Conversion (Recommended)
//=============================================================================

// Helper macro for easier symbolic enum conversion
#define USTR_ENUM_SYMBOLIC(EnumType, ...) \
namespace ustr { \
    template<> \
    struct has_custom_specialization<EnumType> : std::true_type {}; \
} \
namespace ustr { \
    namespace details { \
        template<> \
        inline std::string to_string_impl<EnumType>(const EnumType& value) { \
            switch(value) { \
                __VA_ARGS__ \
                default: return "UNKNOWN_" #EnumType; \
            } \
        } \
    } \
}

// Convenience macro for simple case statements
#define ENUM_CASE(name) case name: return #name;

// Define enums for macro demonstration
enum Direction { NORTH, SOUTH, EAST, WEST };
enum class Status : int { PENDING = 100, APPROVED = 200, REJECTED = 300 };

// Enable symbolic conversion using the macro
USTR_ENUM_SYMBOLIC(Direction,
    ENUM_CASE(NORTH)
    ENUM_CASE(SOUTH)
    ENUM_CASE(EAST)
    ENUM_CASE(WEST)
)

USTR_ENUM_SYMBOLIC(Status,
    case Status::PENDING: return "PENDING";
    case Status::APPROVED: return "APPROVED";
    case Status::REJECTED: return "REJECTED";
)

//=============================================================================
// DEMONSTRATION FUNCTIONS
//=============================================================================

void demonstrateBasicEnumConversion() {
    std::cout << "\n=== 1. Default Integer Conversion (Automatic) ===\n";
    std::cout << "No setup required - works out of the box for all enums\n";
    
    BasicColor color = GREEN;
    Priority priority = HIGH;
    
    std::cout << "BasicColor::GREEN = " << ustr::to_string(color) << " (shows underlying value)\n";
    std::cout << "Priority::HIGH = " << ustr::to_string(priority) << " (shows custom value)\n";
    
    // In containers
    std::vector<BasicColor> colors = {RED, GREEN, BLUE};
    std::cout << "Vector<BasicColor>: " << ustr::to_string(colors) << "\n";
}

void demonstrateManualSpecialization() {
    std::cout << "\n=== 2. Manual Custom Specialization (Full Control) ===\n";
    std::cout << "Provides complete control over conversion logic\n";
    
    LogLevel level = LogLevel::WARNING;
    std::cout << "LogLevel::WARNING = " << ustr::to_string(level) << " (symbolic name)\n";
    
    // In containers and maps
    std::vector<LogLevel> levels = {LogLevel::DEBUG, LogLevel::INFO, LogLevel::ERROR};
    std::cout << "Vector<LogLevel>: " << ustr::to_string(levels) << "\n";
    
    std::map<LogLevel, std::string> messages = {
        {LogLevel::INFO, "All good"},
        {LogLevel::ERROR, "Something failed"}
    };
    std::cout << "Map<LogLevel, string>: " << ustr::to_string(messages) << "\n";
}

void demonstrateMacroApproach() {
    std::cout << "\n=== 3. Macro-Assisted Symbolic Conversion (Recommended) ===\n";
    std::cout << "Easy to use, minimal boilerplate, readable output\n";
    
    Direction dir = WEST;
    Status status = Status::APPROVED;
    
    std::cout << "Direction::WEST = " << ustr::to_string(dir) << " (symbolic)\n";
    std::cout << "Status::APPROVED = " << ustr::to_string(status) << " (symbolic)\n";
    
    // Complex nested usage
    std::vector<Direction> route = {NORTH, EAST, SOUTH, WEST};
    std::cout << "Route: " << ustr::to_string(route) << "\n";
    
    std::pair<Direction, Status> response = {NORTH, Status::PENDING};
    std::cout << "Pair<Direction, Status>: " << ustr::to_string(response) << "\n";
    
    std::map<Status, Direction> flow = {
        {Status::PENDING, NORTH},
        {Status::APPROVED, SOUTH}
    };
    std::cout << "Map<Status, Direction>: " << ustr::to_string(flow) << "\n";
}

void demonstrateMixedUsage() {
    std::cout << "\n=== 4. Mixed Usage: Integer + Symbolic ===\n";
    std::cout << "Different enums can use different conversion approaches\n";
    
    // Mix integer and symbolic enums in the same container
    std::tuple<BasicColor, LogLevel, Direction> mixed = {
        GREEN,                // Integer conversion
        LogLevel::WARNING,    // Symbolic via manual specialization  
        EAST                  // Symbolic via macro
    };
    std::cout << "Tuple<BasicColor, LogLevel, Direction>: " << ustr::to_string(mixed) << "\n";
    
    // Nested containers with mixed approaches
    std::vector<std::pair<Priority, Direction>> priorities = {
        {HIGH, NORTH},     // Integer + Symbolic
        {LOW, SOUTH}       // Integer + Symbolic
    };
    std::cout << "Vector<Pair<Priority, Direction>>: " << ustr::to_string(priorities) << "\n";
}

void showUsageGuidelines() {
    std::cout << "\n=== Usage Guidelines ===\n";
    std::cout << "\n1. DEFAULT INTEGER CONVERSION:\n";
    std::cout << "   - Use when: Performance critical, calculations, internal IDs\n";
    std::cout << "   - Pros: Zero overhead, automatic, compact\n";
    std::cout << "   - Cons: Not human-readable\n";
    
    std::cout << "\n2. MANUAL CUSTOM SPECIALIZATION:\n";
    std::cout << "   - Use when: Need special formatting, complex logic\n";
    std::cout << "   - Pros: Maximum control and flexibility\n";
    std::cout << "   - Cons: More verbose, boilerplate code\n";
    
    std::cout << "\n3. MACRO-ASSISTED SYMBOLIC:\n";
    std::cout << "   - Use when: Human-readable output needed\n";
    std::cout << "   - Pros: Easy to use, readable, minimal code\n";
    std::cout << "   - Cons: Slight compile-time overhead\n";
    
    std::cout << "\nRECOMMENDATION: Use symbolic conversion for user-facing enums,\n";
    std::cout << "integer conversion for internal/performance-critical enums.\n";
}

//=============================================================================
// MAIN DEMONSTRATION
//=============================================================================

int main() {
    std::cout << "USTR Library - Comprehensive Enum Conversion Demo\n";
    std::cout << "================================================\n";
    std::cout << "This demo showcases three approaches for enum to string conversion.\n";
    
    demonstrateBasicEnumConversion();
    demonstrateManualSpecialization(); 
    demonstrateMacroApproach();
    demonstrateMixedUsage();
    showUsageGuidelines();
    
    std::cout << "\n=== Summary ===\n";
    std::cout << u8"✓ Integer conversion: Works automatically for all enums\n";
    std::cout << u8"✓ Symbolic conversion: Available via custom specialization\n";
    std::cout << u8"✓ Macro helpers: Simplify symbolic conversion setup\n";
    std::cout << u8"✓ Container support: All approaches work in vectors, maps, pairs, tuples\n";
    std::cout << u8"✓ Mixed usage: Different enums can use different approaches\n";
    std::cout << "\nChoose the approach that best fits your use case!\n";
    
    return 0;
}
