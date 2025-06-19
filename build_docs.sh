#!/bin/bash
# build_docs.sh - Generate HTML documentation using Doxygen

set -e

DOCS_DIR="$(dirname "$0")/docs"
BUILD_DIR="$DOCS_DIR/html"
DOXYFILE_IN="$DOCS_DIR/Doxyfile.in"
DOXYFILE="$DOCS_DIR/Doxyfile"
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"

# Check for Doxygen
if ! command -v doxygen &> /dev/null; then
    echo "Error: Doxygen is not installed. Please install it and try again."
    exit 1
fi

# Prepare output directory
mkdir -p "$BUILD_DIR"

# Always generate Doxyfile from template with hardcoded paths
sed \
    -e "s|@CMAKE_SOURCE_DIR@|$PROJECT_ROOT|g" \
    -e "s|@CMAKE_CURRENT_BINARY_DIR@|$DOCS_DIR|g" \
    -e "s|@PROJECT_VERSION@|dev|g" \
    "$DOXYFILE_IN" > "$DOXYFILE"

# Run Doxygen
cd "$DOCS_DIR"
echo "Generating documentation..."
doxygen "$DOXYFILE"
echo "Documentation generated in $BUILD_DIR"
