#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <vector>
#include <sstream>
#include <map>
#include <iomanip>  // for std::setprecision

// Test classes for different scenarios
// Note: These are duplicated in ustr_custom_classes_test.cpp
// TODO: Consider refactoring to avoid duplication

// Class with to_string method
class CustomToString {
private:
    int value_;
public:
    CustomToString(int value) : value_(value) {}
    std::string to_string() const {
        return "CustomToString(" + std::to_string(value_) + ")";
    }
};

// Streamable class without to_string
class StreamableClass {
private:
    std::string name_;
public:
    StreamableClass(const std::string& name) : name_(name) {}
    friend std::ostream& operator<<(std::ostream& os, const StreamableClass& obj) {
        return os << "StreamableClass[" << obj.name_ << "]";
    }
};

// Non-streamable class
class NonStreamableClass {
private:
    int value_;
public:
    NonStreamableClass(int value) : value_(value) {}
    int getValue() const { return value_; }
};

// Class that has both to_string and operator<< (to_string should take precedence)
class BothMethods {
private:
    int value_;
public:
    BothMethods(int value) : value_(value) {}
    std::string to_string() const {
        return "to_string_method:" + std::to_string(value_);
    }
    friend std::ostream& operator<<(std::ostream& os, const BothMethods& obj) {
        return os << "operator<<:" << obj.value_;
    }
};

// Test std::string input
UTEST_FUNC_DEF2(StringConversion, StdString) {
    std::string input = "hello world";
    std::string result = ustr::to_string(input);
    UTEST_ASSERT_STR_EQUALS(result, "hello world");
}

// Test string literals
UTEST_FUNC_DEF2(StringConversion, StringLiteral) {
    const char* literal = "hello literal";
    std::string result = ustr::to_string(literal);
    UTEST_ASSERT_STR_EQUALS(result, "hello literal");
}

UTEST_FUNC_DEF2(StringConversion, CharArray) {
    char arr[] = "char array";
    std::string result = ustr::to_string(arr);
    UTEST_ASSERT_STR_EQUALS(result, "char array");
}

// These tests have been moved to ustr_custom_classes_test.cpp
// but we keep the declarations to avoid compilation errors
UTEST_FUNC_DEF2(CustomToStringTest, BasicUsage) {
    CustomToString obj(42);
    std::string result = ustr::to_string(obj);
    UTEST_ASSERT_STR_EQUALS(result, "CustomToString(42)");
}

UTEST_FUNC_DEF2(CustomToStringTest, PrecedenceOverStreamable) {
    BothMethods obj(123);
    std::string result = ustr::to_string(obj);
    UTEST_ASSERT_STR_EQUALS(result, "to_string_method:123");
}

// Test streamable classes
UTEST_FUNC_DEF2(StreamableTest, BasicStreamable) {
    StreamableClass obj("test");
    std::string result = ustr::to_string(obj);
    UTEST_ASSERT_STR_EQUALS(result, "StreamableClass[test]");
}

// Test numeric types
UTEST_FUNC_DEF2(NumericTypes, Integer) {
    int value = 42;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "42");
}

UTEST_FUNC_DEF2(NumericTypes, NegativeInteger) {
    int value = -123;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "-123");
}

UTEST_FUNC_DEF2(NumericTypes, Float) {
    float value = 3.14f;
    std::string result = ustr::to_string(value);
    // std::to_string for float typically gives 6 decimal places
    UTEST_ASSERT_TRUE(result.find("3.14") == 0);
}

UTEST_FUNC_DEF2(NumericTypes, Double) {
    double value = 2.718281828;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_TRUE(result.find("2.718") == 0);
}

UTEST_FUNC_DEF2(NumericTypes, Long) {
    long value = 1234567890L;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "1234567890");
}

UTEST_FUNC_DEF2(NumericTypes, UnsignedInt) {
    unsigned int value = 4294967295U;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "4294967295");
}

// Test character types
UTEST_FUNC_DEF2(CharacterTypes, RegularChar) {
    char c = 'A';
    std::string result = ustr::to_string(c);
    UTEST_ASSERT_STR_EQUALS(result, "A");
}

UTEST_FUNC_DEF2(CharacterTypes, SignedChar) {
    signed char c = 'B';
    std::string result = ustr::to_string(c);
    UTEST_ASSERT_STR_EQUALS(result, "B");
}

UTEST_FUNC_DEF2(CharacterTypes, UnsignedChar) {
    unsigned char c = 'C';
    std::string result = ustr::to_string(c);
    UTEST_ASSERT_STR_EQUALS(result, "C");
}

UTEST_FUNC_DEF2(CharacterTypes, SpecialChars) {
    char space = ' ';
    char newline = '\n';
    char tab = '\t';
    
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(space), " ");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(newline), "\n");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(tab), "\t");
}

// Test boolean type
UTEST_FUNC_DEF2(BooleanType, True) {
    bool value = true;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "true");
}

UTEST_FUNC_DEF2(BooleanType, False) {
    bool value = false;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "false");
}

// Test non-streamable classes - moved to ustr_custom_classes_test.cpp
UTEST_FUNC_DEF2(NonStreamableTest, TypeInfo) {
    NonStreamableClass obj(42);
    std::string result = ustr::to_string(obj);
    
    // Should contain type name and address
    UTEST_ASSERT_TRUE(result.find("[") == 0);
    UTEST_ASSERT_TRUE(result.find("NonStreamableClass") != std::string::npos || 
                     result.find("16NonStreamableClass") != std::string::npos ||  // mangled name variant
                     result.find("N") != std::string::npos);  // some part of mangled name
    UTEST_ASSERT_TRUE(result.find(" at ") != std::string::npos);
    UTEST_ASSERT_TRUE(result.find("]") == result.length() - 1);
}

// Test type trait detection - custom class tests moved to ustr_custom_classes_test.cpp
UTEST_FUNC_DEF2(TypeTraits, HasToString) {
    UTEST_ASSERT_FALSE(ustr::has_to_string<int>::value);
}

UTEST_FUNC_DEF2(TypeTraits, IsStreamable) {
    UTEST_ASSERT_TRUE(ustr::is_streamable<int>::value);
    UTEST_ASSERT_TRUE(ustr::is_streamable<std::string>::value);
}

UTEST_FUNC_DEF2(TypeTraits, IsNumeric) {
    UTEST_ASSERT_TRUE(ustr::is_numeric<int>::value);
    UTEST_ASSERT_TRUE(ustr::is_numeric<float>::value);
    UTEST_ASSERT_TRUE(ustr::is_numeric<double>::value);
    UTEST_ASSERT_TRUE(ustr::is_numeric<long>::value);
    UTEST_ASSERT_TRUE(ustr::is_numeric<unsigned int>::value);
    
    // These should NOT be considered numeric for string conversion
    UTEST_ASSERT_FALSE(ustr::is_numeric<bool>::value);
    UTEST_ASSERT_FALSE(ustr::is_numeric<char>::value);
    UTEST_ASSERT_FALSE(ustr::is_numeric<signed char>::value);
    UTEST_ASSERT_FALSE(ustr::is_numeric<unsigned char>::value);
    UTEST_ASSERT_FALSE(ustr::is_numeric<std::string>::value);
}

// Test edge cases
UTEST_FUNC_DEF2(EdgeCases, EmptyString) {
    std::string empty = "";
    std::string result = ustr::to_string(empty);
    UTEST_ASSERT_STR_EQUALS(result, "");
}

UTEST_FUNC_DEF2(EdgeCases, ZeroValues) {
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(0), "0");             // int
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(0.0), "0.000000");    // double
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(0.0f), "0.000000");   // float
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(0L), "0");            // long
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(0LL), "0");           // long long
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(0U), "0");            // unsigned int
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(0UL), "0");           // unsigned long
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(0ULL), "0");          // unsigned long long
    UTEST_ASSERT_STR_EQUALS(ustr::to_string((short)0), "0");      // short
    UTEST_ASSERT_STR_EQUALS(ustr::to_string((unsigned short)0), "0"); // unsigned short
}

UTEST_FUNC_DEF2(EdgeCases, NullCharPointer) {
    const char* null_ptr = nullptr;
    std::string result = ustr::to_string(null_ptr);
    UTEST_ASSERT_STR_EQUALS(result, "null");
}

UTEST_FUNC_DEF2(EdgeCases, NullptrType) {
    std::nullptr_t null_value = nullptr;
    std::string result = ustr::to_string(null_value);
    UTEST_ASSERT_STR_EQUALS(result, "null");
    
    // Also test with literal nullptr
    result = ustr::to_string(nullptr);
    UTEST_ASSERT_STR_EQUALS(result, "null");
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // String conversion tests
    UTEST_FUNC2(StringConversion, StdString);
    UTEST_FUNC2(StringConversion, StringLiteral);
    UTEST_FUNC2(StringConversion, CharArray);
    
    // Numeric type tests
    UTEST_FUNC2(NumericTypes, Integer);
    UTEST_FUNC2(NumericTypes, NegativeInteger);
    UTEST_FUNC2(NumericTypes, Float);
    UTEST_FUNC2(NumericTypes, Double);
    UTEST_FUNC2(NumericTypes, Long);
    UTEST_FUNC2(NumericTypes, UnsignedInt);
    
    // Character type tests
    UTEST_FUNC2(CharacterTypes, RegularChar);
    UTEST_FUNC2(CharacterTypes, SignedChar);
    UTEST_FUNC2(CharacterTypes, UnsignedChar);
    UTEST_FUNC2(CharacterTypes, SpecialChars);
    
    // Boolean tests
    UTEST_FUNC2(BooleanType, True);
    UTEST_FUNC2(BooleanType, False);
    
    // Type trait tests
    UTEST_FUNC2(TypeTraits, HasToString);
    UTEST_FUNC2(TypeTraits, IsStreamable);
    UTEST_FUNC2(TypeTraits, IsNumeric);
    
    // Edge case tests
    UTEST_FUNC2(EdgeCases, EmptyString);
    UTEST_FUNC2(EdgeCases, ZeroValues);
    UTEST_FUNC2(EdgeCases, NullCharPointer);
    UTEST_FUNC2(EdgeCases, NullptrType);
    
    UTEST_EPILOG();
}
