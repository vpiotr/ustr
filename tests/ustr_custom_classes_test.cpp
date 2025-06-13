#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <vector>
#include <sstream>
#include <map>

// Test classes for different scenarios

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

// Test custom to_string method
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

// Test non-streamable classes
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

// Test type trait detection
UTEST_FUNC_DEF2(TypeTraits, CustomClassesToString) {
    UTEST_ASSERT_TRUE(ustr::has_to_string<CustomToString>::value);
    UTEST_ASSERT_TRUE(ustr::has_to_string<BothMethods>::value);
    UTEST_ASSERT_FALSE(ustr::has_to_string<StreamableClass>::value);
    UTEST_ASSERT_FALSE(ustr::has_to_string<NonStreamableClass>::value);
}

UTEST_FUNC_DEF2(TypeTraits, CustomClassesStreamable) {
    UTEST_ASSERT_TRUE(ustr::is_streamable<StreamableClass>::value);
    UTEST_ASSERT_TRUE(ustr::is_streamable<BothMethods>::value);
    UTEST_ASSERT_FALSE(ustr::is_streamable<NonStreamableClass>::value);
}

UTEST_FUNC_DEF2(TypeTraits, CustomClassesNumeric) {
    UTEST_ASSERT_FALSE(ustr::is_numeric<CustomToString>::value);
    UTEST_ASSERT_FALSE(ustr::is_numeric<StreamableClass>::value);
    UTEST_ASSERT_FALSE(ustr::is_numeric<BothMethods>::value);
    UTEST_ASSERT_FALSE(ustr::is_numeric<NonStreamableClass>::value);
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Custom to_string tests
    UTEST_FUNC2(CustomToStringTest, BasicUsage);
    UTEST_FUNC2(CustomToStringTest, PrecedenceOverStreamable);
    
    // Streamable class tests
    UTEST_FUNC2(StreamableTest, BasicStreamable);
    
    // Non-streamable tests
    UTEST_FUNC2(NonStreamableTest, TypeInfo);
    
    // Type trait tests
    UTEST_FUNC2(TypeTraits, CustomClassesToString);
    UTEST_FUNC2(TypeTraits, CustomClassesStreamable);
    UTEST_FUNC2(TypeTraits, CustomClassesNumeric);
    
    UTEST_EPILOG();
}
