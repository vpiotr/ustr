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

// Integration test with STL containers (if they're streamable)
UTEST_FUNC_DEF2(STLContainers, StreamableContainer) {
    // Create a simple pair which is streamable in some implementations
    std::pair<int, std::string> p{42, "hello"};
    std::string result = ustr::to_string(p);
    // Result depends on STL implementation, but should not crash
    UTEST_ASSERT_TRUE(result.length() > 0);
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
    
    // STL container tests
    UTEST_FUNC2(STLContainers, StreamableContainer);
    
    UTEST_EPILOG();
}
