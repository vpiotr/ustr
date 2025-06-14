#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <vector>
#include <string>

// Test iterator-based conversion with pairs (containers of pairs use JSON-like format)
UTEST_FUNC_DEF2(IteratorConversion, PairContainer) {
    std::vector<std::pair<std::string, int>> values = {{"a", 1}, {"b", 2}};
    std::string result = ustr::to_string(values.cbegin(), values.cend());
    UTEST_ASSERT_STR_EQUALS(result, "{\"a\": 1, \"b\": 2}");
}

// Test the std::pair specialization (direct pair conversion uses parentheses format)
UTEST_FUNC_DEF2(PairSpecialization, IntStringPair) {
    std::pair<int, std::string> p = {42, "hello"};
    std::string result = ustr::to_string(p);
    // Should use the pair specialization, string values are now quoted
    UTEST_ASSERT_STR_EQUALS(result, "(42, \"hello\")");
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
    // Should handle nested pairs correctly, string values are now quoted
    UTEST_ASSERT_STR_EQUALS(result, "((1, 2), \"nested\")");
}

UTEST_FUNC_DEF2(PairSpecialization, StringStringPair) {
    std::pair<std::string, std::string> p = {"key", "value"};
    std::string result = ustr::to_string(p);
    // Should use the pair specialization, both strings are now quoted
    UTEST_ASSERT_STR_EQUALS(result, "(\"key\", \"value\")");
}

UTEST_FUNC_DEF2(PairSpecialization, PairWithContainer) {
    std::pair<std::vector<int>, std::string> p = {{1, 2, 3}, "numbers"};
    std::string result = ustr::to_string(p);
    // Should handle pairs containing containers, string values are now quoted
    UTEST_ASSERT_STR_EQUALS(result, "([1, 2, 3], \"numbers\")");
}

UTEST_FUNC_DEF2(PairSpecialization, MixedTypePair) {
    std::pair<bool, char> p = {true, 'X'};
    std::string result = ustr::to_string(p);
    // Should handle different primitive types
    UTEST_ASSERT_STR_EQUALS(result, "(true, X)");
}

// Test comprehensive quotation behavior in pairs and tuples
UTEST_FUNC_DEF2(PairSpecialization, QuotationComprehensiveTest) {
    // Test various pair types to demonstrate quotation functionality
    
    // 1. Pair with string values - both should be quoted
    std::pair<std::string, std::string> stringPair = {"key", "value"};
    std::string stringPairResult = ustr::to_string(stringPair);
    UTEST_ASSERT_STR_EQUALS(stringPairResult, "(\"key\", \"value\")");
    
    // 2. Pair with numeric and string - only string should be quoted
    std::pair<int, std::string> intStringPair = {42, "answer"};
    std::string intStringPairResult = ustr::to_string(intStringPair);
    UTEST_ASSERT_STR_EQUALS(intStringPairResult, "(42, \"answer\")");
    
    // 3. Pair with string and numeric - only string should be quoted
    std::pair<std::string, int> stringIntPair = {"count", 5};
    std::string stringIntPairResult = ustr::to_string(stringIntPair);
    UTEST_ASSERT_STR_EQUALS(stringIntPairResult, "(\"count\", 5)");
    
    // 4. Pair with numeric values - no quotation
    std::pair<int, double> numericPair = {10, 3.14};
    std::string numericPairResult = ustr::to_string(numericPair);
    UTEST_ASSERT_STR_EQUALS(numericPairResult, "(10, 3.140000)");
}

// Test comparison between direct pair conversion and iterator-based conversion
UTEST_FUNC_DEF2(PairFormatComparison, DirectVsIterator) {
    std::pair<std::string, int> p = {"key", 42};
    
    // Direct pair conversion uses (first, second) format, string values are now quoted
    std::string direct_result = ustr::to_string(p);
    UTEST_ASSERT_STR_EQUALS(direct_result, "(\"key\", 42)");
    
    // Container of one pair uses {"key": value} format
    std::vector<std::pair<std::string, int>> container = {p};
    std::string iterator_result = ustr::to_string(container.cbegin(), container.cend());
    UTEST_ASSERT_STR_EQUALS(iterator_result, "{\"key\": 42}");
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Iterator conversion tests with pairs
    UTEST_FUNC2(IteratorConversion, PairContainer);
    
    // std::pair specialization tests
    UTEST_FUNC2(PairSpecialization, IntStringPair);
    UTEST_FUNC2(PairSpecialization, DoubleBoolPair);
    UTEST_FUNC2(PairSpecialization, NestedPair);
    UTEST_FUNC2(PairSpecialization, StringStringPair);
    UTEST_FUNC2(PairSpecialization, PairWithContainer);
    UTEST_FUNC2(PairSpecialization, MixedTypePair);
    UTEST_FUNC2(PairSpecialization, QuotationComprehensiveTest);
    
    // Format comparison tests
    UTEST_FUNC2(PairFormatComparison, DirectVsIterator);
    
    UTEST_EPILOG();
}
