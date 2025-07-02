#!/bin/bash
# Simple build script for the C++ project

echo "Building C++ project..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

# Compile all source files
echo "Compiling source files..."
g++ -I include -std=c++17 \
    src/main.cpp \
    src/ArrayBasedCollection.cpp \
    src/CSVParser.cpp \
    src/Transaction.cpp \
    src/LinkedListBasedCollection.cpp \
    -o build/MyCppProject.exe

if [ $? -eq 0 ]; then
    echo "Build successful! Executable created at build/MyCppProject.exe"
    echo "Running program..."
    echo
    ./build/MyCppProject.exe
else
    echo "Build failed!"
    exit 1
fi
