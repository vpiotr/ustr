#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <vector>
#include <string>
#include <map>

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
    UTEST_ASSERT_STR_EQUALS(result, "[hello, world]");
}

UTEST_FUNC_DEF2(IteratorConversion, PairContainer) {
    std::vector<std::pair<std::string, int>> values = {{"a", 1}, {"b", 2}};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "{\"a\": 1, \"b\": 2}");
}

UTEST_FUNC_DEF2(IteratorConversion, MapContainer) {
    std::map<std::string, std::string> values = {{"key1", "value1"}, {"key2", "value2"}};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "{\"key1\": value1, \"key2\": value2}");
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

UTEST_FUNC_DEF2(CBeginCEndSpecialization, MapSpecialization) {
    std::map<std::string, int> values = {{"a", 1}, {"b", 2}};
    std::string result = ustr::to_string(values);
    // Should use the cbegin/cend specialization, which uses iterator-based conversion
    UTEST_ASSERT_STR_EQUALS(result, "{\"a\": 1, \"b\": 2}");
}

UTEST_FUNC_DEF2(CBeginCEndSpecialization, EmptyVectorSpecialization) {
    std::vector<int> values;
    std::string result = ustr::to_string(values);
    // Should use the cbegin/cend specialization for empty containers too
    UTEST_ASSERT_STR_EQUALS(result, "[]");
}

// Test the new std::pair specialization
UTEST_FUNC_DEF2(PairSpecialization, IntStringPair) {
    std::pair<int, std::string> p = {42, "hello"};
    std::string result = ustr::to_string(p);
    // Should use the pair specialization, format as (first, second)
    UTEST_ASSERT_STR_EQUALS(result, "(42, hello)");
}

UTEST_FUNC_DEF2(PairSpecialization, DoubleBoolPair) {
    std::pair<double, bool> p = {3.14, true};
    std::string result = ustr::to_string(p);
    // Should use the pair specialization
    UTEST_ASSERT_STR_EQUALS(result, "(3.140000, true)");
}

UTEST_FUNC_DEF2(PairSpecialization, NestedPair) {
    std::pair<std::pair<int, int>, std::string> nested = {{1, 2}, "nested"};
    std::string result = ustr::to_string(nested);
    // Should handle nested pairs correctly
    UTEST_ASSERT_STR_EQUALS(result, "((1, 2), nested)");
}

UTEST_FUNC_DEF2(PairSpecialization, StringStringPair) {
    std::pair<std::string, std::string> p = {"key", "value"};
    std::string result = ustr::to_string(p);
    // Should use the pair specialization
    UTEST_ASSERT_STR_EQUALS(result, "(key, value)");
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Iterator conversion tests
    UTEST_FUNC2(IteratorConversion, VectorOfInts);
    UTEST_FUNC2(IteratorConversion, EmptyContainer);
    UTEST_FUNC2(IteratorConversion, VectorOfStrings);
    UTEST_FUNC2(IteratorConversion, PairContainer);
    UTEST_FUNC2(IteratorConversion, MapContainer);
    
    // cbegin/cend specialization tests
    UTEST_FUNC2(CBeginCEndSpecialization, VectorSpecialization);
    UTEST_FUNC2(CBeginCEndSpecialization, StringSpecialization);
    UTEST_FUNC2(CBeginCEndSpecialization, MapSpecialization);
    UTEST_FUNC2(CBeginCEndSpecialization, EmptyVectorSpecialization);
    
    // std::pair specialization tests
    UTEST_FUNC2(PairSpecialization, IntStringPair);
    UTEST_FUNC2(PairSpecialization, DoubleBoolPair);
    UTEST_FUNC2(PairSpecialization, NestedPair);
    UTEST_FUNC2(PairSpecialization, StringStringPair);
    
    UTEST_EPILOG();
}
