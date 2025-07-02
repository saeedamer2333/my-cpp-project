@echo off
echo Building C++ project...
if not exist build mkdir build
g++ -I include -std=c++17 src/main.cpp src/ArrayBasedCollection.cpp src/CSVParser.cpp src/Transaction.cpp src/LinkedListBasedCollection.cpp -o build/MyCppProject.exe
if %errorlevel% == 0 (
    echo Build successful!
    echo Run with: .\build\MyCppProject.exe
) else (
    echo Build failed!
)
