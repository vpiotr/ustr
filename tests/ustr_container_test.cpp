#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <vector>
#include <string>
#include <map>
#include <array>

// Test iterator-based to_string function
UTEST_FUNC_DEF2(IteratorConversion, VectorOfInts) {
    std::vector<int> values = {1, 2, 3};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "[1, 2, 3]");
}

UTEST_FUNC_DEF2(IteratorConversion, EmptyContainer) {
    std::vector<int> values;
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "[]");
}

UTEST_FUNC_DEF2(IteratorConversion, VectorOfStrings) {
    std::vector<std::string> values = {"hello", "world"};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "[\"hello\", \"world\"]");
}

UTEST_FUNC_DEF2(IteratorConversion, MapContainerStringKeys) {
    std::map<std::string, std::string> values = {{"key1", "value1"}, {"key2", "value2"}};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "{\"key1\": \"value1\", \"key2\": \"value2\"}");
}

UTEST_FUNC_DEF2(IteratorConversion, MapContainerNumericKeys) {
    std::map<int, std::string> values = {{1, "one"}, {2, "two"}, {42, "answer"}};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "{1: \"one\", 2: \"two\", 42: \"answer\"}");
}

UTEST_FUNC_DEF2(IteratorConversion, MapContainerDoubleKeys) {
    std::map<double, int> values = {{1.5, 15}, {2.7, 27}};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    // std::to_string for double typically gives decimal places
    UTEST_ASSERT_TRUE(result.find("{1.5") == 0);
    UTEST_ASSERT_TRUE(result.find(": 15") != std::string::npos);
    UTEST_ASSERT_TRUE(result.find("2.7") != std::string::npos);
    UTEST_ASSERT_TRUE(result.find(": 27}") != std::string::npos);
}

// Test std::array container
UTEST_FUNC_DEF2(IteratorConversion, ArrayContainer) {
    std::array<int, 5> values = {{10, 20, 30, 40, 50}};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "[10, 20, 30, 40, 50]");
}

UTEST_FUNC_DEF2(IteratorConversion, ArrayContainerStrings) {
    std::array<std::string, 3> values = {{"apple", "banana", "cherry"}};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "[\"apple\", \"banana\", \"cherry\"]");
}

// Test the new cbegin/cend specialization
UTEST_FUNC_DEF2(CBeginCEndSpecialization, VectorSpecialization) {
    std::vector<int> values = {1, 2, 3};
    std::string result = ustr::to_string(values);
    // Should use the cbegin/cend specialization, which uses iterator-based conversion
    UTEST_ASSERT_STR_EQUALS(result, "[1, 2, 3]");
}

UTEST_FUNC_DEF2(CBeginCEndSpecialization, StringSpecialization) {
    std::string value = "test";
    std::string result = ustr::to_string(value);
    // std::string should use the special string implementation, not cbegin/cend
    UTEST_ASSERT_STR_EQUALS(result, "test");
}

UTEST_FUNC_DEF2(CBeginCEndSpecialization, MapSpecializationStringKeys) {
    std::map<std::string, int> values = {{"a", 1}, {"b", 2}};
    std::string result = ustr::to_string(values);
    // Should use the cbegin/cend specialization, which uses iterator-based conversion
    UTEST_ASSERT_STR_EQUALS(result, "{\"a\": 1, \"b\": 2}");
}

UTEST_FUNC_DEF2(CBeginCEndSpecialization, MapSpecializationNumericKeys) {
    std::map<int, std::string> values = {{10, "ten"}, {20, "twenty"}};
    std::string result = ustr::to_string(values);
    // Should use the cbegin/cend specialization, numeric keys should not be quoted, but string values should be quoted
    UTEST_ASSERT_STR_EQUALS(result, "{10: \"ten\", 20: \"twenty\"}");
}

UTEST_FUNC_DEF2(CBeginCEndSpecialization, EmptyVectorSpecialization) {
    std::vector<int> values;
    std::string result = ustr::to_string(values);
    // Should use the cbegin/cend specialization for empty containers too
    UTEST_ASSERT_STR_EQUALS(result, "[]");
}

UTEST_FUNC_DEF2(CBeginCEndSpecialization, ArraySpecialization) {
    std::array<int, 4> values = {{100, 200, 300, 400}};
    std::string result = ustr::to_string(values);
    // Should use the cbegin/cend specialization for std::array
    UTEST_ASSERT_STR_EQUALS(result, "[100, 200, 300, 400]");
}

UTEST_FUNC_DEF2(IteratorConversion, MapWithMixedValueTypes) {
    // Test a map where values have mixed quotable and non-quotable types
    std::map<std::string, int> intValues = {{"count", 42}, {"age", 25}};
    std::map<int, std::string> stringValues = {{1, "one"}, {2, "two"}};
    
    std::string intResult = ustr::to_string(intValues.cbegin(), intValues.cend());
    std::string stringResult = ustr::to_string(stringValues.cbegin(), stringValues.cend());
    
    // String keys should be quoted, int values should not be quoted
    UTEST_ASSERT_STR_EQUALS(intResult, "{\"age\": 25, \"count\": 42}");
    
    // Int keys should not be quoted, string values should be quoted
    UTEST_ASSERT_STR_EQUALS(stringResult, "{1: \"one\", 2: \"two\"}");
}

UTEST_FUNC_DEF2(IteratorConversion, QuotationComprehensiveTest) {
    // Test various container types to demonstrate complete quotation functionality
    
    // 1. Vector of strings - values should be quoted
    std::vector<std::string> stringVec = {"hello", "world"};
    std::string stringVecResult = ustr::to_string(stringVec.cbegin(), stringVec.cend());
    UTEST_ASSERT_STR_EQUALS(stringVecResult, "[\"hello\", \"world\"]");
    
    // 2. Vector of integers - values should not be quoted
    std::vector<int> intVec = {1, 2, 3};
    std::string intVecResult = ustr::to_string(intVec.cbegin(), intVec.cend());
    UTEST_ASSERT_STR_EQUALS(intVecResult, "[1, 2, 3]");
    
    // 3. Map with string keys and string values - both should be quoted
    std::map<std::string, std::string> stringMap = {{"key", "value"}};
    std::string stringMapResult = ustr::to_string(stringMap.cbegin(), stringMap.cend());
    UTEST_ASSERT_STR_EQUALS(stringMapResult, "{\"key\": \"value\"}");
    
    // 4. Map with numeric keys and string values - only values should be quoted
    std::map<int, std::string> intStringMap = {{42, "answer"}};
    std::string intStringMapResult = ustr::to_string(intStringMap.cbegin(), intStringMap.cend());
    UTEST_ASSERT_STR_EQUALS(intStringMapResult, "{42: \"answer\"}");
    
    // 5. Map with string keys and numeric values - only keys should be quoted
    std::map<std::string, int> stringIntMap = {{"count", 5}};
    std::string stringIntMapResult = ustr::to_string(stringIntMap.cbegin(), stringIntMap.cend());
    UTEST_ASSERT_STR_EQUALS(stringIntMapResult, "{\"count\": 5}");
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Iterator conversion tests
    UTEST_FUNC2(IteratorConversion, VectorOfInts);
    UTEST_FUNC2(IteratorConversion, EmptyContainer);
    UTEST_FUNC2(IteratorConversion, VectorOfStrings);
    UTEST_FUNC2(IteratorConversion, MapContainerStringKeys);
    UTEST_FUNC2(IteratorConversion, MapContainerNumericKeys);
    UTEST_FUNC2(IteratorConversion, MapContainerDoubleKeys);
    UTEST_FUNC2(IteratorConversion, ArrayContainer);
    UTEST_FUNC2(IteratorConversion, ArrayContainerStrings);
    UTEST_FUNC2(IteratorConversion, MapWithMixedValueTypes);
    UTEST_FUNC2(IteratorConversion, QuotationComprehensiveTest);
    
    // cbegin/cend specialization tests
    UTEST_FUNC2(CBeginCEndSpecialization, VectorSpecialization);
    UTEST_FUNC2(CBeginCEndSpecialization, StringSpecialization);
    UTEST_FUNC2(CBeginCEndSpecialization, MapSpecializationStringKeys);
    UTEST_FUNC2(CBeginCEndSpecialization, MapSpecializationNumericKeys);
    UTEST_FUNC2(CBeginCEndSpecialization, EmptyVectorSpecialization);
    UTEST_FUNC2(CBeginCEndSpecialization, ArraySpecialization);
    
    UTEST_EPILOG();
}
