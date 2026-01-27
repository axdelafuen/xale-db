#!/bin/sh

# Clean
rm -rf build
rm -rf Logs

# Configure CMake
cmake -B ./build

# Build
cmake --build ./build

# Run
if [ $? -eq 0 ]; then
    if [ "$1" = "debug" ]; then
        ./build/xale-db-debug
    elif [ "$1" = "tests" ]; then
        ./build/xale-db-tests
    fi
else
    exit 1
fi
