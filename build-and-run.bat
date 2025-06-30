@echo off
echo Building C++ project...
g++ -I include -std=c++17 src/main.cpp src/ArrayBasedCollection.cpp src/CSVParser.cpp src/Transaction.cpp src/LinkedListBasedCollection.cpp -o build/MyCppProject.exe
if %errorlevel% == 0 (
    echo Build successful! Running program...
    echo.
    .\build\MyCppProject.exe
) else (
    echo Build failed!
    pause
)
