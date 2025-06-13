#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <vector>
#include <sstream>
#include <map>
#include <iomanip>  // for std::setprecision

// Test format context functionality
UTEST_FUNC_DEF2(FormatContext, BasicUsage) {
    ustr::format_context ctx;
    
    // Test default behavior (should fall back to standard formatting)
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "true");
    UTEST_ASSERT_EQUALS(ctx.to_string(false), "false");
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "42");
}

UTEST_FUNC_DEF2(FormatContext, CustomBoolFormatter) {
    ustr::format_context ctx;
    
    // Set custom bool formatter
    ctx.set_formatter<bool>([](bool b) { return b ? "YES" : "NO"; });
    
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "YES");
    UTEST_ASSERT_EQUALS(ctx.to_string(false), "NO");
    UTEST_ASSERT_TRUE(ctx.has_formatter<bool>());
    UTEST_ASSERT_FALSE(ctx.has_formatter<int>());
}

UTEST_FUNC_DEF2(FormatContext, CustomFloatFormatter) {
    ustr::format_context ctx;
    
    // Set custom float formatter with fixed precision
    ctx.set_formatter<float>([](float f) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << f;
        return ss.str();
    });
    
    // Test with positive value
    std::string result = ctx.to_string(3.14159f);
    UTEST_ASSERT_EQUALS(result, "3.14");
    UTEST_ASSERT_TRUE(ctx.has_formatter<float>());
    
    // Test with negative value
    result = ctx.to_string(-1.234f);
    UTEST_ASSERT_EQUALS(result, "-1.23");
    
    // Test with zero value
    result = ctx.to_string(0.0f);
    UTEST_ASSERT_EQUALS(result, "0.00");
}

UTEST_FUNC_DEF2(FormatContext, MultipleFormatters) {
    ustr::format_context ctx;
    
    // Set multiple custom formatters
    ctx.set_formatter<bool>([](bool b) { return b ? "TRUE" : "FALSE"; });
    ctx.set_formatter<int>([](int i) { return "INT:" + std::to_string(i); });
    ctx.set_formatter<float>([](float f) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << f;
        return "FLOAT:" + ss.str();
    });
    
    // Test each formatter type
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "TRUE");
    UTEST_ASSERT_EQUALS(ctx.to_string(false), "FALSE");
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "INT:42");
    UTEST_ASSERT_EQUALS(ctx.to_string(0), "INT:0");
    UTEST_ASSERT_EQUALS(ctx.to_string(3.14f), "FLOAT:3.1");
    
    // Test default formatting for types without custom formatters
    UTEST_ASSERT_EQUALS(ctx.to_string(3.14), "3.140000"); // double uses default formatting
    
    // Test that we have the formatters we expect
    UTEST_ASSERT_TRUE(ctx.has_formatter<bool>());
    UTEST_ASSERT_TRUE(ctx.has_formatter<int>());
    UTEST_ASSERT_TRUE(ctx.has_formatter<float>());
    UTEST_ASSERT_FALSE(ctx.has_formatter<double>());
    UTEST_ASSERT_FALSE(ctx.has_formatter<std::string>());
}

UTEST_FUNC_DEF2(FormatContext, RemoveFormatter) {
    ustr::format_context ctx;
    
    ctx.set_formatter<bool>([](bool b) { return b ? "YES" : "NO"; });
    ctx.set_formatter<int>([](int i) { return "INT:" + std::to_string(i); });
    
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "YES");
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "INT:42");
    UTEST_ASSERT_TRUE(ctx.has_formatter<bool>());
    UTEST_ASSERT_TRUE(ctx.has_formatter<int>());
    
    ctx.remove_formatter<bool>();
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "true"); // Back to default
    UTEST_ASSERT_FALSE(ctx.has_formatter<bool>());
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "INT:42"); // Int formatter should still work
    UTEST_ASSERT_TRUE(ctx.has_formatter<int>());
    
    ctx.remove_formatter<int>();
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "42"); // Back to default
    UTEST_ASSERT_FALSE(ctx.has_formatter<int>());
}

UTEST_FUNC_DEF2(FormatContext, ClearFormatters) {
    ustr::format_context ctx;
    
    ctx.set_formatter<bool>([](bool b) { return b ? "YES" : "NO"; });
    ctx.set_formatter<int>([](int i) { return "NUM:" + std::to_string(i); });
    
    UTEST_ASSERT_TRUE(ctx.has_formatter<bool>());
    UTEST_ASSERT_TRUE(ctx.has_formatter<int>());
    
    // Test formatted values before clear
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "YES");
    UTEST_ASSERT_EQUALS(ctx.to_string(false), "NO");
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "NUM:42");
    UTEST_ASSERT_EQUALS(ctx.to_string(0), "NUM:0");
    
    ctx.clear();
    
    UTEST_ASSERT_FALSE(ctx.has_formatter<bool>());
    UTEST_ASSERT_FALSE(ctx.has_formatter<int>());
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "true");
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "42");
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Format context tests
    UTEST_FUNC2(FormatContext, BasicUsage);
    UTEST_FUNC2(FormatContext, CustomBoolFormatter);
    UTEST_FUNC2(FormatContext, CustomFloatFormatter);
    UTEST_FUNC2(FormatContext, MultipleFormatters);
    UTEST_FUNC2(FormatContext, RemoveFormatter);
    UTEST_FUNC2(FormatContext, ClearFormatters);
    
    UTEST_EPILOG();
}
