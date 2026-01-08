#!/usr/bin/env bash
# format.sh - format files with clang-format

set -euo pipefail

# Path to clang-format (adjust if needed)
CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"

# If a file is passed as argument, format that file
if [[ $# -gt 0 ]]; then
    for file in "$@"; do
        echo "Formatting $file"
        "$CLANG_FORMAT" -i --style=file "$file"
    done
else
    # No arguments: format all tracked source files
    files=$(git ls-files '*.cpp' '*.hpp' '*.h' '*.c' '*.cc' '*.hh')
    for file in $files; do
        echo "Formatting $file"
        "$CLANG_FORMAT" -i --style=file "$file"
    done
fi

echo "Done!"

