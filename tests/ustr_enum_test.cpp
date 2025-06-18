#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <vector>
#include <map>
#include <tuple>

namespace {

// Test enums
enum BasicColor { 
    BASIC_RED = 0, 
    BASIC_GREEN = 1, 
    BASIC_BLUE = 2 
};

enum class ScopedStatus : int { 
    SCOPED_PENDING = 10, 
    SCOPED_APPROVED = 20, 
    SCOPED_REJECTED = 30 
};

enum Priority { 
    PRIORITY_LOW = 100, 
    PRIORITY_MEDIUM = 200, 
    PRIORITY_HIGH = 300 
};

enum class Size : unsigned char { 
    SIZE_SMALL = 1, 
    SIZE_MEDIUM = 5, 
    SIZE_LARGE = 10 
};

// Enum with negative values
enum class Temperature : int { 
    TEMP_COLD = -10, 
    TEMP_NORMAL = 0, 
    TEMP_HOT = 50 
};

} // anonymous namespace

// Test enum type traits
UTEST_FUNC_DEF2(TypeTraits, IsEnum) {
    UTEST_ASSERT_TRUE(ustr::is_enum<BasicColor>::value);
    UTEST_ASSERT_TRUE(ustr::is_enum<ScopedStatus>::value);
    UTEST_ASSERT_TRUE(ustr::is_enum<Priority>::value);
    UTEST_ASSERT_TRUE(ustr::is_enum<Size>::value);
    UTEST_ASSERT_TRUE(ustr::is_enum<Temperature>::value);
    
    // Non-enum types should return false
    UTEST_ASSERT_FALSE(ustr::is_enum<int>::value);
    UTEST_ASSERT_FALSE(ustr::is_enum<std::string>::value);
    UTEST_ASSERT_FALSE(ustr::is_enum<bool>::value);
    UTEST_ASSERT_FALSE(ustr::is_enum<char>::value);
    UTEST_ASSERT_FALSE(ustr::is_enum<double>::value);
}

// Test basic enum conversions
UTEST_FUNC_DEF2(EnumConversion, BasicEnum) {
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(BASIC_RED), "0");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(BASIC_GREEN), "1");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(BASIC_BLUE), "2");
}

UTEST_FUNC_DEF2(EnumConversion, ScopedEnum) {
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(ScopedStatus::SCOPED_PENDING), "10");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(ScopedStatus::SCOPED_APPROVED), "20");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(ScopedStatus::SCOPED_REJECTED), "30");
}

UTEST_FUNC_DEF2(EnumConversion, CustomValues) {
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(PRIORITY_LOW), "100");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(PRIORITY_MEDIUM), "200");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(PRIORITY_HIGH), "300");
}

UTEST_FUNC_DEF2(EnumConversion, TypedEnum) {
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(Size::SIZE_SMALL), "1");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(Size::SIZE_MEDIUM), "5");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(Size::SIZE_LARGE), "10");
}

UTEST_FUNC_DEF2(EnumConversion, NegativeValues) {
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(Temperature::TEMP_COLD), "-10");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(Temperature::TEMP_NORMAL), "0");
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(Temperature::TEMP_HOT), "50");
}

// Test enums in containers
UTEST_FUNC_DEF2(EnumConversion, InVector) {
    std::vector<BasicColor> colors = {BASIC_RED, BASIC_GREEN, BASIC_BLUE};
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(colors), "[0, 1, 2]");
}

UTEST_FUNC_DEF2(EnumConversion, InVectorScoped) {
    std::vector<ScopedStatus> statuses = {ScopedStatus::SCOPED_PENDING, ScopedStatus::SCOPED_APPROVED};
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(statuses), "[10, 20]");
}

// Test enums in maps
UTEST_FUNC_DEF2(EnumConversion, MapWithEnumKeys) {
    std::map<Priority, std::string> priority_map = {
        {PRIORITY_LOW, "low"},
        {PRIORITY_HIGH, "high"}
    };
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(priority_map), "{100: \"low\", 300: \"high\"}");
}

UTEST_FUNC_DEF2(EnumConversion, MapWithScopedEnumKeys) {
    std::map<ScopedStatus, int> status_map = {
        {ScopedStatus::SCOPED_PENDING, 1},
        {ScopedStatus::SCOPED_APPROVED, 2}
    };
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(status_map), "{10: 1, 20: 2}");
}

// Test enums in pairs and tuples
UTEST_FUNC_DEF2(EnumConversion, InPair) {
    std::pair<BasicColor, Size> item = {BASIC_BLUE, Size::SIZE_MEDIUM};
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(item), "(2, 5)");
}

UTEST_FUNC_DEF2(EnumConversion, InTuple) {
    std::tuple<Priority, ScopedStatus, BasicColor> info = {
        PRIORITY_HIGH, 
        ScopedStatus::SCOPED_APPROVED, 
        BASIC_RED
    };
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(info), "(300, 20, 0)");
}

// Test mixed enum and non-enum containers
UTEST_FUNC_DEF2(EnumConversion, MixedContainers) {
    std::vector<std::pair<int, BasicColor>> mixed = {
        {1, BASIC_RED},
        {2, BASIC_GREEN}
    };
    UTEST_ASSERT_STR_EQUALS(ustr::to_string(mixed), "{1: 0, 2: 1}");
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Type trait tests
    UTEST_FUNC2(TypeTraits, IsEnum);
    
    // Basic enum conversion tests
    UTEST_FUNC2(EnumConversion, BasicEnum);
    UTEST_FUNC2(EnumConversion, ScopedEnum);
    UTEST_FUNC2(EnumConversion, CustomValues);
    UTEST_FUNC2(EnumConversion, TypedEnum);
    UTEST_FUNC2(EnumConversion, NegativeValues);
    
    // Container tests
    UTEST_FUNC2(EnumConversion, InVector);
    UTEST_FUNC2(EnumConversion, InVectorScoped);
    
    // Map tests
    UTEST_FUNC2(EnumConversion, MapWithEnumKeys);
    UTEST_FUNC2(EnumConversion, MapWithScopedEnumKeys);
    
    // Pair and tuple tests
    UTEST_FUNC2(EnumConversion, InPair);
    UTEST_FUNC2(EnumConversion, InTuple);
    
    // Mixed tests
    UTEST_FUNC2(EnumConversion, MixedContainers);
    
    UTEST_EPILOG();
}
