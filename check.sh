#!/bin/bash
# High-intensity check for every C++ file
for file in *.cpp; do
    echo "Checking $file..."
    g++ -std=c++20 -Wall -Wextra -Wpedantic -Werror -Wshadow -c "$file"
done