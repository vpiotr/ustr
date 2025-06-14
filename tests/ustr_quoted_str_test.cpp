#include "../include/ustr/ustr.h"
#include "../include/utest/utest.h"

// Test basic quoting functionality
UTEST_FUNC_DEF2(QuotedStr, EmptyString) {
    std::string result = ustr::quoted_str("");
    UTEST_ASSERT_STR_EQUALS(result, "\"\"");
}

UTEST_FUNC_DEF2(QuotedStr, SimpleString) {
    std::string result = ustr::quoted_str("hello");
    UTEST_ASSERT_STR_EQUALS(result, "\"hello\"");
}

UTEST_FUNC_DEF2(QuotedStr, StringWithoutSpecialChars) {
    std::string result = ustr::quoted_str("hello world 123");
    UTEST_ASSERT_STR_EQUALS(result, "\"hello world 123\"");
}

// Test delimiter escaping
UTEST_FUNC_DEF2(QuotedStr, StringWithDelimiter) {
    std::string result = ustr::quoted_str("say \"hello\"");
    UTEST_ASSERT_STR_EQUALS(result, "\"say \\\"hello\\\"\"");
}

UTEST_FUNC_DEF2(QuotedStr, StringWithMultipleDelimiters) {
    std::string result = ustr::quoted_str("\"quoted\" and \"more quotes\"");
    UTEST_ASSERT_STR_EQUALS(result, "\"\\\"quoted\\\" and \\\"more quotes\\\"\"");
}

// Test escape character escaping
UTEST_FUNC_DEF2(QuotedStr, StringWithEscapeChar) {
    std::string result = ustr::quoted_str("path\\file");
    UTEST_ASSERT_STR_EQUALS(result, "\"path\\\\file\"");
}

UTEST_FUNC_DEF2(QuotedStr, StringWithMultipleEscapeChars) {
    std::string result = ustr::quoted_str("\\\\server\\\\share\\\\file");
    UTEST_ASSERT_STR_EQUALS(result, "\"\\\\\\\\server\\\\\\\\share\\\\\\\\file\"");
}

// Test mixed delimiter and escape characters
UTEST_FUNC_DEF2(QuotedStr, StringWithBothDelimAndEscape) {
    std::string result = ustr::quoted_str("say \"hello\\world\"");
    UTEST_ASSERT_STR_EQUALS(result, "\"say \\\"hello\\\\world\\\"\"");
}

// Test custom delimiter
UTEST_FUNC_DEF2(QuotedStr, CustomDelimiter) {
    std::string result = ustr::quoted_str("hello", '\'');
    UTEST_ASSERT_STR_EQUALS(result, "'hello'");
}

UTEST_FUNC_DEF2(QuotedStr, CustomDelimiterWithEscaping) {
    std::string result = ustr::quoted_str("don't worry", '\'');
    UTEST_ASSERT_STR_EQUALS(result, "'don\\'t worry'");
}

// Test custom escape character
UTEST_FUNC_DEF2(QuotedStr, CustomEscapeChar) {
    std::string result = ustr::quoted_str("say \"hello\"", '"', '/');
    UTEST_ASSERT_STR_EQUALS(result, "\"say /\"hello/\"\"");
}

// Test no escape mode
UTEST_FUNC_DEF2(QuotedStr, NoEscapeMode) {
    std::string result = ustr::quoted_str("say \"hello\\world\"", '"', '\0');
    UTEST_ASSERT_STR_EQUALS(result, "\"say \"hello\\world\"\"");
}

// Test UTF-8 characters
UTEST_FUNC_DEF2(QuotedStr, UTF8Characters) {
    std::string result = ustr::quoted_str("Hello 世界! 🌍");
    UTEST_ASSERT_STR_EQUALS(result, "\"Hello 世界! 🌍\"");
}

UTEST_FUNC_DEF2(QuotedStr, UTF8WithDelimiters) {
    std::string result = ustr::quoted_str("说 \"你好\" 世界");
    UTEST_ASSERT_STR_EQUALS(result, "\"说 \\\"你好\\\" 世界\"");
}

// Test edge cases
UTEST_FUNC_DEF2(QuotedStr, OnlyDelimiters) {
    std::string result = ustr::quoted_str("\"\"\"");
    UTEST_ASSERT_STR_EQUALS(result, "\"\\\"\\\"\\\"\"");
}

UTEST_FUNC_DEF2(QuotedStr, OnlyEscapeChars) {
    std::string result = ustr::quoted_str("\\\\\\");
    UTEST_ASSERT_STR_EQUALS(result, "\"\\\\\\\\\\\\\"");
}

UTEST_FUNC_DEF2(QuotedStr, SingleDelimiter) {
    std::string result = ustr::quoted_str("\"");
    UTEST_ASSERT_STR_EQUALS(result, "\"\\\"\"");
}

UTEST_FUNC_DEF2(QuotedStr, SingleEscapeChar) {
    std::string result = ustr::quoted_str("\\");
    UTEST_ASSERT_STR_EQUALS(result, "\"\\\\\"");
}

// Performance test with large string
UTEST_FUNC_DEF2(QuotedStr, LargeString) {
    std::string large_input(1000, 'x');
    large_input += "\"test\"";
    large_input += std::string(1000, 'y');
    
    std::string result = ustr::quoted_str(large_input);
    
    // Check that it starts and ends with quotes
    UTEST_ASSERT_TRUE(result.front() == '"');
    UTEST_ASSERT_TRUE(result.back() == '"');
    
    // Check that the delimiters in the middle are escaped
    UTEST_ASSERT_TRUE(result.find("\\\"test\\\"") != std::string::npos);
}

// Test different delimiter and escape combinations
UTEST_FUNC_DEF2(QuotedStr, DifferentCombinations) {
    // Single quotes with backslash escape
    std::string result1 = ustr::quoted_str("It's a \"test\"", '\'', '\\');
    UTEST_ASSERT_STR_EQUALS(result1, "'It\\'s a \"test\"'");
    
    // Brackets with forward slash escape
    std::string result2 = ustr::quoted_str("test[bracket]", '[', '/');
    UTEST_ASSERT_STR_EQUALS(result2, "[test/[bracket][");
    
    // Pipe with caret escape
    std::string result3 = ustr::quoted_str("test|pipe^caret", '|', '^');
    UTEST_ASSERT_STR_EQUALS(result3, "|test^|pipe^^caret|");
}

int main() {
    UTEST_PROLOG();
    
    // Basic functionality tests
    UTEST_FUNC2(QuotedStr, EmptyString);
    UTEST_FUNC2(QuotedStr, SimpleString);
    UTEST_FUNC2(QuotedStr, StringWithoutSpecialChars);
    
    // Delimiter escaping tests
    UTEST_FUNC2(QuotedStr, StringWithDelimiter);
    UTEST_FUNC2(QuotedStr, StringWithMultipleDelimiters);
    
    // Escape character tests
    UTEST_FUNC2(QuotedStr, StringWithEscapeChar);
    UTEST_FUNC2(QuotedStr, StringWithMultipleEscapeChars);
    
    // Mixed escaping tests
    UTEST_FUNC2(QuotedStr, StringWithBothDelimAndEscape);
    
    // Custom delimiters and escape chars
    UTEST_FUNC2(QuotedStr, CustomDelimiter);
    UTEST_FUNC2(QuotedStr, CustomDelimiterWithEscaping);
    UTEST_FUNC2(QuotedStr, CustomEscapeChar);
    UTEST_FUNC2(QuotedStr, NoEscapeMode);
    
    // UTF-8 tests
    UTEST_FUNC2(QuotedStr, UTF8Characters);
    UTEST_FUNC2(QuotedStr, UTF8WithDelimiters);
    
    // Edge cases
    UTEST_FUNC2(QuotedStr, OnlyDelimiters);
    UTEST_FUNC2(QuotedStr, OnlyEscapeChars);
    UTEST_FUNC2(QuotedStr, SingleDelimiter);
    UTEST_FUNC2(QuotedStr, SingleEscapeChar);
    
    // Performance test
    UTEST_FUNC2(QuotedStr, LargeString);
    
    // Combination tests
    UTEST_FUNC2(QuotedStr, DifferentCombinations);
    
    UTEST_EPILOG();
}
