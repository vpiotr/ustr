#!/bin/bash

# USTR Test Runner Script
# ======================
# This script runs all tests for the USTR project

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
BUILD_DIR="$PROJECT_ROOT/build"

echo -e "${BLUE}USTR Test Runner${NC}"
echo -e "${BLUE}================${NC}"

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Build directory not found. Running build script...${NC}"
    if [ -x "$PROJECT_ROOT/rebuild.sh" ]; then
        "$PROJECT_ROOT/rebuild.sh"
    else
        echo -e "${RED}Build script not found or not executable: $PROJECT_ROOT/rebuild.sh${NC}"
        exit 1
    fi
fi

# Check if test binaries exist
CORE_TEST_BIN="$BUILD_DIR/bin/ustr_core_features_test"
CONTAINER_TEST_BIN="$BUILD_DIR/bin/ustr_container_test"
CUSTOM_CLASSES_TEST_BIN="$BUILD_DIR/bin/ustr_custom_classes_test"
ENUM_TEST_BIN="$BUILD_DIR/bin/ustr_enum_test"
FORMAT_CONTEXT_TEST_BIN="$BUILD_DIR/bin/ustr_format_context_test"
PAIR_TEST_BIN="$BUILD_DIR/bin/ustr_pair_test"
TUPLE_TEST_BIN="$BUILD_DIR/bin/ustr_tuple_test"
CUSTOM_SPECIALIZATION_TEST_BIN="$BUILD_DIR/bin/ustr_custom_specialization_test"
QUOTED_STR_TEST_BIN="$BUILD_DIR/bin/ustr_quoted_str_test"

if [ ! -x "$CORE_TEST_BIN" ] || [ ! -x "$CONTAINER_TEST_BIN" ] || [ ! -x "$CUSTOM_CLASSES_TEST_BIN" ] || [ ! -x "$ENUM_TEST_BIN" ] || [ ! -x "$FORMAT_CONTEXT_TEST_BIN" ] || [ ! -x "$PAIR_TEST_BIN" ] || [ ! -x "$TUPLE_TEST_BIN" ] || [ ! -x "$CUSTOM_SPECIALIZATION_TEST_BIN" ] || [ ! -x "$QUOTED_STR_TEST_BIN" ]; then
    echo -e "${RED}Test binaries not found or not executable:${NC}"
    [ ! -x "$CORE_TEST_BIN" ] && echo -e "${RED}- $CORE_TEST_BIN${NC}"
    [ ! -x "$CONTAINER_TEST_BIN" ] && echo -e "${RED}- $CONTAINER_TEST_BIN${NC}"
    [ ! -x "$CUSTOM_CLASSES_TEST_BIN" ] && echo -e "${RED}- $CUSTOM_CLASSES_TEST_BIN${NC}"
    [ ! -x "$ENUM_TEST_BIN" ] && echo -e "${RED}- $ENUM_TEST_BIN${NC}"
    [ ! -x "$FORMAT_CONTEXT_TEST_BIN" ] && echo -e "${RED}- $FORMAT_CONTEXT_TEST_BIN${NC}"
    [ ! -x "$PAIR_TEST_BIN" ] && echo -e "${RED}- $PAIR_TEST_BIN${NC}"
    [ ! -x "$TUPLE_TEST_BIN" ] && echo -e "${RED}- $TUPLE_TEST_BIN${NC}"
    [ ! -x "$CUSTOM_SPECIALIZATION_TEST_BIN" ] && echo -e "${RED}- $CUSTOM_SPECIALIZATION_TEST_BIN${NC}"
    [ ! -x "$QUOTED_STR_TEST_BIN" ] && echo -e "${RED}- $QUOTED_STR_TEST_BIN${NC}"
    echo -e "${YELLOW}Try running the build script first: ./rebuild.sh${NC}"
    exit 1
fi

# Run the tests
echo -e "${GREEN}Running USTR tests...${NC}"
echo "====================="
echo ""

# Execute the test binaries
echo -e "${BLUE}Running Core Features Tests:${NC}"
"$CORE_TEST_BIN"
core_exit_code=$?

echo ""
echo -e "${BLUE}Running Container Tests:${NC}"
"$CONTAINER_TEST_BIN"
container_exit_code=$?

echo ""
echo -e "${BLUE}Running Custom Classes Tests:${NC}"
"$CUSTOM_CLASSES_TEST_BIN"
custom_classes_exit_code=$?

echo ""
echo -e "${BLUE}Running Enum Tests:${NC}"
"$ENUM_TEST_BIN"
enum_exit_code=$?

echo ""
echo -e "${BLUE}Running Format Context Tests:${NC}"
"$FORMAT_CONTEXT_TEST_BIN"
format_context_exit_code=$?

echo ""
echo -e "${BLUE}Running Pair Tests:${NC}"
"$PAIR_TEST_BIN"
pair_exit_code=$?

echo ""
echo -e "${BLUE}Running Tuple Tests:${NC}"
"$TUPLE_TEST_BIN"
tuple_exit_code=$?

echo ""
echo -e "${BLUE}Running Custom Specialization Tests:${NC}"
"$CUSTOM_SPECIALIZATION_TEST_BIN"
custom_specialization_exit_code=$?

echo ""
echo -e "${BLUE}Running Quoted String Tests:${NC}"
"$QUOTED_STR_TEST_BIN"
quoted_str_exit_code=$?

# Check exit codes
if [ $core_exit_code -eq 0 ] && [ $container_exit_code -eq 0 ] && [ $custom_classes_exit_code -eq 0 ] && [ $enum_exit_code -eq 0 ] && [ $format_context_exit_code -eq 0 ] && [ $pair_exit_code -eq 0 ] && [ $tuple_exit_code -eq 0 ] && [ $custom_specialization_exit_code -eq 0 ] && [ $quoted_str_exit_code -eq 0 ]; then
    exit_code=0
else
    exit_code=1
fi

echo ""
if [ $exit_code -eq 0 ]; then
    echo -e "${GREEN}All tests passed successfully!${NC}"
else
    echo -e "${RED}Some tests failed (exit code: $exit_code)${NC}"
fi

exit $exit_code
