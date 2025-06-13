#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <list>

// First, we need to specialize the has_custom_specialization trait for our types
namespace ustr {
    // Specialize for long long to indicate it has a custom implementation
    template<>
    struct has_custom_specialization<long long> : std::true_type {};
    
    // Specialize for std::list to indicate it has a custom implementation
    template<typename T>
    struct has_custom_specialization<std::list<T>> : std::true_type {};
}

// Now provide the custom implementations in the details namespace
namespace ustr {
    namespace details {
        // Custom specialization for long long - add "LL" suffix
        template<>
        inline std::string to_string_impl<long long>(const long long& value) {
            return std::to_string(value) + "LL";
        }
        
        // Custom specialization for std::list<int> - use pipe-separated format
        template<>
        inline std::string to_string_impl<std::list<int>>(const std::list<int>& value) {
            std::ostringstream ss;
            ss << "|";
            bool first = true;
            for (const auto& item : value) {
                if (!first) {
                    ss << "|";
                } else {
                    first = false;
                }
                ss << to_string_forward(item);
            }
            ss << "|";
            return ss.str();
        }
        
        // Custom specialization for std::list<std::string> - use pipe-separated format
        template<>
        inline std::string to_string_impl<std::list<std::string>>(const std::list<std::string>& value) {
            std::ostringstream ss;
            ss << "|";
            bool first = true;
            for (const auto& item : value) {
                if (!first) {
                    ss << "|";
                } else {
                    first = false;
                }
                ss << to_string_forward(item);
            }
            ss << "|";
            return ss.str();
        }
        
        // Custom specialization for std::list<char> - use pipe-separated format
        template<>
        inline std::string to_string_impl<std::list<char>>(const std::list<char>& value) {
            std::ostringstream ss;
            ss << "|";
            bool first = true;
            for (const auto& item : value) {
                if (!first) {
                    ss << "|";
                } else {
                    first = false;
                }
                ss << to_string_forward(item);
            }
            ss << "|";
            return ss.str();
        }
    }
}

// Test the custom specialization for long long
UTEST_FUNC_DEF2(CustomSpecialization, LongLongSpecialization) {
    long long value = 1234567890123456789LL;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "1234567890123456789LL");
}

UTEST_FUNC_DEF2(CustomSpecialization, LongLongZero) {
    long long value = 0LL;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "0LL");
}

UTEST_FUNC_DEF2(CustomSpecialization, LongLongNegative) {
    long long value = -123456789012345LL;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "-123456789012345LL");
}

// Test the custom specialization for std::list
UTEST_FUNC_DEF2(CustomSpecialization, ListSpecializationInts) {
    std::list<int> value = {1, 2, 3, 4, 5};
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "|1|2|3|4|5|");
}

UTEST_FUNC_DEF2(CustomSpecialization, ListSpecializationStrings) {
    std::list<std::string> value = {"hello", "world", "from", "list"};
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "|hello|world|from|list|");
}

UTEST_FUNC_DEF2(CustomSpecialization, ListSpecializationEmpty) {
    std::list<int> value;
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "||");
}

UTEST_FUNC_DEF2(CustomSpecialization, ListSpecializationSingleElement) {
    std::list<char> value = {'X'};
    std::string result = ustr::to_string(value);
    UTEST_ASSERT_STR_EQUALS(result, "|X|");
}

// Test that the trait detection works correctly
UTEST_FUNC_DEF2(TypeTraits, HasCustomSpecialization) {
    UTEST_ASSERT_TRUE(ustr::has_custom_specialization<long long>::value);
    UTEST_ASSERT_TRUE(ustr::has_custom_specialization<std::list<int>>::value);
    UTEST_ASSERT_TRUE(ustr::has_custom_specialization<std::list<std::string>>::value);
    
    // These should NOT have custom specializations
    UTEST_ASSERT_FALSE(ustr::has_custom_specialization<int>::value);
    UTEST_ASSERT_FALSE(ustr::has_custom_specialization<long>::value);
    UTEST_ASSERT_FALSE(ustr::has_custom_specialization<std::vector<int>>::value);
    UTEST_ASSERT_FALSE(ustr::has_custom_specialization<std::string>::value);
    UTEST_ASSERT_FALSE(ustr::has_custom_specialization<bool>::value);
}

// Test that regular long long without specialization would behave differently
// (This test demonstrates the difference between regular numeric handling and custom handling)
UTEST_FUNC_DEF2(CustomSpecialization, CompareWithRegularLong) {
    long regular_long = 123456789012345L;
    std::string regular_result = ustr::to_string(regular_long);
    
    long long custom_long_long = 123456789012345LL;
    std::string custom_result = ustr::to_string(custom_long_long);
    
    // Regular long should not have the "LL" suffix
    UTEST_ASSERT_STR_EQUALS(regular_result, "123456789012345");
    
    // Custom long long should have the "LL" suffix
    UTEST_ASSERT_STR_EQUALS(custom_result, "123456789012345LL");
    
    // They should be different
    UTEST_ASSERT_TRUE(regular_result != custom_result);
}

// Test that regular containers without custom specialization behave differently
UTEST_FUNC_DEF2(CustomSpecialization, CompareWithRegularVector) {
    std::vector<int> regular_vector = {1, 2, 3, 4, 5};
    std::string regular_result = ustr::to_string(regular_vector);
    
    std::list<int> custom_list = {1, 2, 3, 4, 5};
    std::string custom_result = ustr::to_string(custom_list);
    
    // Regular vector should use bracket format
    UTEST_ASSERT_STR_EQUALS(regular_result, "[1, 2, 3, 4, 5]");
    
    // Custom list should use pipe format
    UTEST_ASSERT_STR_EQUALS(custom_result, "|1|2|3|4|5|");
    
    // They should be different
    UTEST_ASSERT_TRUE(regular_result != custom_result);
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Custom specialization tests
    UTEST_FUNC2(CustomSpecialization, LongLongSpecialization);
    UTEST_FUNC2(CustomSpecialization, LongLongZero);
    UTEST_FUNC2(CustomSpecialization, LongLongNegative);
    UTEST_FUNC2(CustomSpecialization, ListSpecializationInts);
    UTEST_FUNC2(CustomSpecialization, ListSpecializationStrings);
    UTEST_FUNC2(CustomSpecialization, ListSpecializationEmpty);
    UTEST_FUNC2(CustomSpecialization, ListSpecializationSingleElement);
    
    // Type trait tests
    UTEST_FUNC2(TypeTraits, HasCustomSpecialization);
    
    // Comparison tests
    UTEST_FUNC2(CustomSpecialization, CompareWithRegularLong);
    UTEST_FUNC2(CustomSpecialization, CompareWithRegularVector);
    
    UTEST_EPILOG();
}
