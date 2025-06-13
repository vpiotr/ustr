#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <tuple>
#include <vector>
#include <string>

// Test std::tuple specialization
UTEST_FUNC_DEF2(TupleSpecialization, EmptyTuple) {
    std::tuple<> empty_tuple;
    std::string result = ustr::to_string(empty_tuple);
    UTEST_ASSERT_STR_EQUALS(result, "()");
}

UTEST_FUNC_DEF2(TupleSpecialization, SingleElementTuple) {
    std::tuple<int> single_tuple{42};
    std::string result = ustr::to_string(single_tuple);
    UTEST_ASSERT_STR_EQUALS(result, "(42)");
}

UTEST_FUNC_DEF2(TupleSpecialization, TwoElementTuple) {
    std::tuple<int, std::string> two_tuple{42, "hello"};
    std::string result = ustr::to_string(two_tuple);
    UTEST_ASSERT_STR_EQUALS(result, "(42, hello)");
}

UTEST_FUNC_DEF2(TupleSpecialization, ThreeElementTuple) {
    std::tuple<int, double, std::string> three_tuple{123, 3.14, "world"};
    std::string result = ustr::to_string(three_tuple);
    UTEST_ASSERT_TRUE(result.find("(123, 3.14") == 0);
    UTEST_ASSERT_TRUE(result.find(", world)") != std::string::npos);
}

UTEST_FUNC_DEF2(TupleSpecialization, BoolCharIntTuple) {
    std::tuple<bool, char, int> bool_tuple{true, 'A', 999};
    std::string result = ustr::to_string(bool_tuple);
    UTEST_ASSERT_STR_EQUALS(result, "(true, A, 999)");
}

UTEST_FUNC_DEF2(TupleSpecialization, NestedTuple) {
    std::tuple<std::tuple<int, int>, std::string> nested_tuple{{10, 20}, "nested"};
    std::string result = ustr::to_string(nested_tuple);
    UTEST_ASSERT_STR_EQUALS(result, "((10, 20), nested)");
}

UTEST_FUNC_DEF2(TupleSpecialization, TupleWithVector) {
    std::tuple<std::vector<int>, std::string> vector_tuple{{1, 2, 3}, "numbers"};
    std::string result = ustr::to_string(vector_tuple);
    UTEST_ASSERT_STR_EQUALS(result, "([1, 2, 3], numbers)");
}

UTEST_FUNC_DEF2(TupleSpecialization, TupleWithPair) {
    std::tuple<std::pair<int, std::string>, double> pair_tuple{{42, "key"}, 2.718};
    std::string result = ustr::to_string(pair_tuple);
    UTEST_ASSERT_TRUE(result.find("((42, key), 2.718") == 0);
}

UTEST_FUNC_DEF2(TupleSpecialization, LargeTuple) {
    std::tuple<int, double, std::string, bool, char> large_tuple{1, 2.5, "test", false, 'X'};
    std::string result = ustr::to_string(large_tuple);
    UTEST_ASSERT_TRUE(result.find("(1, 2.5") == 0);
    UTEST_ASSERT_TRUE(result.find(", test, false, X)") != std::string::npos);
}

// Test is_tuple trait
UTEST_FUNC_DEF2(TypeTraits, IsTuple) {
    // Use type aliases to avoid macro parsing issues with nested templates
    typedef std::tuple<int> TupleInt;
    typedef std::tuple<> EmptyTuple;
    typedef std::tuple<int, std::string> TupleIntString;
    typedef std::tuple<int, double, std::string, bool> LargeTuple;
    typedef std::pair<int, int> PairIntInt;
    typedef std::vector<int> VectorInt;
    
    // Should be tuples
    UTEST_ASSERT_TRUE(ustr::is_tuple<TupleInt>::value);
    UTEST_ASSERT_TRUE(ustr::is_tuple<EmptyTuple>::value);
    UTEST_ASSERT_TRUE(ustr::is_tuple<TupleIntString>::value);
    UTEST_ASSERT_TRUE(ustr::is_tuple<LargeTuple>::value);
    
    // Should not be tuples
    UTEST_ASSERT_FALSE(ustr::is_tuple<int>::value);
    UTEST_ASSERT_FALSE(ustr::is_tuple<std::string>::value);
    UTEST_ASSERT_FALSE(ustr::is_tuple<PairIntInt>::value);
    UTEST_ASSERT_FALSE(ustr::is_tuple<VectorInt>::value);
    UTEST_ASSERT_FALSE(ustr::is_tuple<bool>::value);
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Tuple specialization tests
    UTEST_FUNC2(TupleSpecialization, EmptyTuple);
    UTEST_FUNC2(TupleSpecialization, SingleElementTuple);
    UTEST_FUNC2(TupleSpecialization, TwoElementTuple);
    UTEST_FUNC2(TupleSpecialization, ThreeElementTuple);
    UTEST_FUNC2(TupleSpecialization, BoolCharIntTuple);
    UTEST_FUNC2(TupleSpecialization, NestedTuple);
    UTEST_FUNC2(TupleSpecialization, TupleWithVector);
    UTEST_FUNC2(TupleSpecialization, TupleWithPair);
    UTEST_FUNC2(TupleSpecialization, LargeTuple);
    
    // Type trait tests
    UTEST_FUNC2(TypeTraits, IsTuple);
    
    UTEST_EPILOG();
}
