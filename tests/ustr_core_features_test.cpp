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
        return os << "StreamableClass[" << ustr::quoted_str(obj.name_) << "]";
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

// Custom container class with cbegin/cend methods for testing
class CustomContainer {
private:
    std::vector<int> data_;
public:
    CustomContainer(std::initializer_list<int> init) : data_(init) {}
    
    std::vector<int>::const_iterator cbegin() const { return data_.cbegin(); }
    std::vector<int>::const_iterator cend() const { return data_.cend(); }
    
    // Also provide regular begin/end for completeness
    std::vector<int>::iterator begin() { return data_.begin(); }
    std::vector<int>::iterator end() { return data_.end(); }
    std::vector<int>::const_iterator begin() const { return data_.begin(); }
    std::vector<int>::const_iterator end() const { return data_.end(); }
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

UTEST_FUNC_DEF2(TypeTraits, IsQuotableString) {
    // String types should be quotable
    UTEST_ASSERT_TRUE(ustr::is_quotable_string<std::string>::value);
    UTEST_ASSERT_TRUE(ustr::is_quotable_string<const char*>::value);
    UTEST_ASSERT_TRUE(ustr::is_quotable_string<char*>::value);
    
    // Numeric types should not be quotable
    UTEST_ASSERT_FALSE(ustr::is_quotable_string<int>::value);
    UTEST_ASSERT_FALSE(ustr::is_quotable_string<double>::value);
    UTEST_ASSERT_FALSE(ustr::is_quotable_string<float>::value);
    UTEST_ASSERT_FALSE(ustr::is_quotable_string<long>::value);
    
    // Other types should not be quotable
    UTEST_ASSERT_FALSE(ustr::is_quotable_string<bool>::value);
    UTEST_ASSERT_FALSE(ustr::is_quotable_string<char>::value);
    
    // Use type aliases to avoid macro parsing issues
    typedef std::vector<int> VectorInt;
    UTEST_ASSERT_FALSE(ustr::is_quotable_string<VectorInt>::value);
}

UTEST_FUNC_DEF2(TypeTraits, HasCBeginCEnd) {
    // Use type aliases to avoid macro parsing issues with nested templates
    typedef std::vector<int> VectorInt;
    typedef std::map<int, int> MapIntInt;
    typedef std::vector<std::string> VectorString;
    
    // Standard containers should have cbegin/cend
    UTEST_ASSERT_TRUE(ustr::has_cbegin_cend<VectorInt>::value);
    UTEST_ASSERT_TRUE(ustr::has_cbegin_cend<std::string>::value);
    UTEST_ASSERT_TRUE(ustr::has_cbegin_cend<MapIntInt>::value);
    UTEST_ASSERT_TRUE(ustr::has_cbegin_cend<VectorString>::value);
    
    // Custom container with cbegin/cend should have it
    UTEST_ASSERT_TRUE(ustr::has_cbegin_cend<CustomContainer>::value);
    
    // Primitive types should not have cbegin/cend
    UTEST_ASSERT_FALSE(ustr::has_cbegin_cend<int>::value);
    UTEST_ASSERT_FALSE(ustr::has_cbegin_cend<float>::value);
    UTEST_ASSERT_FALSE(ustr::has_cbegin_cend<bool>::value);
    UTEST_ASSERT_FALSE(ustr::has_cbegin_cend<char>::value);
    
    // Custom classes without cbegin/cend should not have it
    UTEST_ASSERT_FALSE(ustr::has_cbegin_cend<CustomToString>::value);
    UTEST_ASSERT_FALSE(ustr::has_cbegin_cend<StreamableClass>::value);
    UTEST_ASSERT_FALSE(ustr::has_cbegin_cend<NonStreamableClass>::value);
    UTEST_ASSERT_FALSE(ustr::has_cbegin_cend<BothMethods>::value);
}

UTEST_FUNC_DEF2(TypeTraits, IsCArray) {
    // C-style arrays (non-char) should be detected
    UTEST_ASSERT_TRUE(ustr::is_c_array<int[5]>::value);
    UTEST_ASSERT_TRUE(ustr::is_c_array<double[3]>::value);
    UTEST_ASSERT_TRUE(ustr::is_c_array<const char*[2]>::value);
    UTEST_ASSERT_TRUE(ustr::is_c_array<std::string[4]>::value);
    
    // char arrays should NOT be detected (treated as C-strings)
    UTEST_ASSERT_FALSE(ustr::is_c_array<char[10]>::value);
    UTEST_ASSERT_FALSE(ustr::is_c_array<signed char[5]>::value);
    UTEST_ASSERT_FALSE(ustr::is_c_array<unsigned char[5]>::value);
    
    // Non-array types should not be detected
    UTEST_ASSERT_FALSE(ustr::is_c_array<int>::value);
    UTEST_ASSERT_FALSE(ustr::is_c_array<std::vector<int>>::value);
    UTEST_ASSERT_FALSE(ustr::is_c_array<std::string>::value);
    UTEST_ASSERT_FALSE(ustr::is_c_array<char*>::value);  // pointer, not array
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

// Test C-style arrays support
UTEST_FUNC_DEF2(CStyleArrays, IntArray) {
    int arr[5] = {1, 2, 3, 4, 5};
    std::string result = ustr::to_string(arr);
    UTEST_ASSERT_STR_EQUALS(result, "[1, 2, 3, 4, 5]");
}

UTEST_FUNC_DEF2(CStyleArrays, DoubleArray) {
    double arr[3] = {1.5, 2.7, 3.14};
    std::string result = ustr::to_string(arr);
    // Note: exact string comparison for floating point may be fragile
    // We test that it's formatted as an array with brackets
    UTEST_ASSERT_TRUE(result.front() == '[');
    UTEST_ASSERT_TRUE(result.back() == ']');
    UTEST_ASSERT_TRUE(result.find("1.5") != std::string::npos);
    UTEST_ASSERT_TRUE(result.find("2.7") != std::string::npos);
    UTEST_ASSERT_TRUE(result.find("3.14") != std::string::npos);
}

UTEST_FUNC_DEF2(CStyleArrays, StringArray) {
    const char* arr[3] = {"hello", "world", "test"};
    std::string result = ustr::to_string(arr);
    UTEST_ASSERT_STR_EQUALS(result, "[\"hello\", \"world\", \"test\"]");
}

UTEST_FUNC_DEF2(CStyleArrays, BoolArray) {
    bool arr[4] = {true, false, true, false};
    std::string result = ustr::to_string(arr);
    UTEST_ASSERT_STR_EQUALS(result, "[true, false, true, false]");
}

UTEST_FUNC_DEF2(CStyleArrays, CharArrayAsCString) {
    char arr[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    std::string result = ustr::to_string(arr);
    // char arrays should be treated as C-strings, not arrays
    UTEST_ASSERT_STR_EQUALS(result, "hello");
}

UTEST_FUNC_DEF2(CStyleArrays, SingleElementArray) {
    int arr[1] = {42};
    std::string result = ustr::to_string(arr);
    UTEST_ASSERT_STR_EQUALS(result, "[42]");
}

UTEST_FUNC_DEF2(CStyleArrays, MixedTypeArrayWithString) {
    std::string arr[2] = {"first", "second"};
    std::string result = ustr::to_string(arr);
    UTEST_ASSERT_STR_EQUALS(result, "[\"first\", \"second\"]");
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
    UTEST_FUNC2(TypeTraits, IsQuotableString);
    UTEST_FUNC2(TypeTraits, HasCBeginCEnd);
    UTEST_FUNC2(TypeTraits, IsCArray);
    
    // Edge case tests
    UTEST_FUNC2(EdgeCases, EmptyString);
    UTEST_FUNC2(EdgeCases, ZeroValues);
    UTEST_FUNC2(EdgeCases, NullCharPointer);
    UTEST_FUNC2(EdgeCases, NullptrType);
    
    // C-style array tests
    UTEST_FUNC2(CStyleArrays, IntArray);
    UTEST_FUNC2(CStyleArrays, DoubleArray);
    UTEST_FUNC2(CStyleArrays, StringArray);
    UTEST_FUNC2(CStyleArrays, BoolArray);
    UTEST_FUNC2(CStyleArrays, CharArrayAsCString);
    UTEST_FUNC2(CStyleArrays, SingleElementArray);
    UTEST_FUNC2(CStyleArrays, MixedTypeArrayWithString);
    
    UTEST_EPILOG();
}
