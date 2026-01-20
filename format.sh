#!/usr/bin/env bash
set -euo pipefail

CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"

if [[ $# -gt 0 ]]; then
    for file in "$@"; do
        echo "Formatting $file"
        "$CLANG_FORMAT" -i --style=file "$file"
    done
else
    # Safe way to iterate over git ls-files
    git ls-files '*.cpp' '*.hpp' '*.h' '*.c' '*.cc' '*.hh' \
        | while IFS= read -r file; do
            echo "Formatting $file"
            "$CLANG_FORMAT" -i --style=file "$file"
        done
fi

echo "Done!"
