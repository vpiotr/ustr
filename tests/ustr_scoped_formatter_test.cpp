#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"
#include <vector>
#include <sstream>
#include <map>
#include <iomanip>  // for std::setprecision

// Test scoped formatter functionality
UTEST_FUNC_DEF2(ScopedFormatter, BasicUsage) {
    ustr::scoped_formatter ctx;
    
    // Test default behavior (should fall back to standard formatting)
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "true");
    UTEST_ASSERT_EQUALS(ctx.to_string(false), "false");
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "42");
}

UTEST_FUNC_DEF2(ScopedFormatter, CustomBoolFormatter) {
    ustr::scoped_formatter ctx;
    
    // Set custom bool formatter
    ctx.set_formatter<bool>([](bool b) { return b ? "YES" : "NO"; });
    
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "YES");
    UTEST_ASSERT_EQUALS(ctx.to_string(false), "NO");
    UTEST_ASSERT_TRUE(ctx.has_formatter<bool>());
    UTEST_ASSERT_FALSE(ctx.has_formatter<int>());
}

UTEST_FUNC_DEF2(ScopedFormatter, CustomFloatFormatter) {
    ustr::scoped_formatter ctx;
    
    // Set custom float formatter with fixed precision
    ctx.set_formatter<float>([](float f) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << f;
        return ss.str();
    });
    
    std::string result = ctx.to_string(3.14159f);
    UTEST_ASSERT_EQUALS(result, "3.14");
    UTEST_ASSERT_TRUE(ctx.has_formatter<float>());
}

UTEST_FUNC_DEF2(ScopedFormatter, MultipleFormatters) {
    ustr::scoped_formatter ctx;
    
    // Set multiple custom formatters
    ctx.set_formatter<bool>([](bool b) { return b ? "TRUE" : "FALSE"; });
    ctx.set_formatter<int>([](int i) { return "INT:" + std::to_string(i); });
    
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "TRUE");
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "INT:42");
    UTEST_ASSERT_EQUALS(ctx.to_string(3.14f), "3.140000"); // Uses default formatting
}

UTEST_FUNC_DEF2(ScopedFormatter, RemoveFormatter) {
    ustr::scoped_formatter ctx;
    
    ctx.set_formatter<bool>([](bool b) { return b ? "YES" : "NO"; });
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "YES");
    UTEST_ASSERT_TRUE(ctx.has_formatter<bool>());
    
    ctx.remove_formatter<bool>();
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "true"); // Back to default
    UTEST_ASSERT_FALSE(ctx.has_formatter<bool>());
}

UTEST_FUNC_DEF2(ScopedFormatter, ClearFormatters) {
    ustr::scoped_formatter ctx;
    
    ctx.set_formatter<bool>([](bool b) { return b ? "YES" : "NO"; });
    ctx.set_formatter<int>([](int i) { return "NUM:" + std::to_string(i); });
    
    UTEST_ASSERT_TRUE(ctx.has_formatter<bool>());
    UTEST_ASSERT_TRUE(ctx.has_formatter<int>());
    
    ctx.clear();
    
    UTEST_ASSERT_FALSE(ctx.has_formatter<bool>());
    UTEST_ASSERT_FALSE(ctx.has_formatter<int>());
    UTEST_ASSERT_EQUALS(ctx.to_string(true), "true");
    UTEST_ASSERT_EQUALS(ctx.to_string(42), "42");
}

int main() {
    UTEST_PROLOG();
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Scoped formatter tests
    UTEST_FUNC2(ScopedFormatter, BasicUsage);
    UTEST_FUNC2(ScopedFormatter, CustomBoolFormatter);
    UTEST_FUNC2(ScopedFormatter, CustomFloatFormatter);
    UTEST_FUNC2(ScopedFormatter, MultipleFormatters);
    UTEST_FUNC2(ScopedFormatter, RemoveFormatter);
    UTEST_FUNC2(ScopedFormatter, ClearFormatters);
    
    UTEST_EPILOG();
}
