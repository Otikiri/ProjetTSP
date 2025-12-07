#!/bin/bash
PROJECT_DIR="$(dirname "$(realpath "$0")")"
OUTPUT_DIR="$PROJECT_DIR/doc"

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Generate docs with overridden output directory
(cat "$PROJECT_DIR/Doxyfile"; echo "OUTPUT_DIRECTORY = $OUTPUT_DIR") | doxygen -

# Define the correct output file path
HTML_FILE="$OUTPUT_DIR/html/index.html"

# Check if the file exists
if [ -f "$HTML_FILE" ]; then
    explorer.exe "$(wslpath -w "$HTML_FILE")"
else
    echo "❌ Documentation not found at $HTML_FILE"
    echo "Check your Doxyfile's OUTPUT_DIRECTORY setting."
fi