# PowerShell build script for the C++ project

Write-Host "Building C++ project..." -ForegroundColor Green

# Create build directory if it doesn't exist
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Name "build"
}

# Compile all source files
Write-Host "Compiling source files..." -ForegroundColor Yellow
$result = & g++ -I include -std=c++17 `
    src/main.cpp `
    src/ArrayBasedCollection.cpp `
    src/CSVParser.cpp `
    src/Transaction.cpp `
    src/LinkedListBasedCollection.cpp `
    -o build/MyCppProject.exe 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful! Executable created at build/MyCppProject.exe" -ForegroundColor Green
    Write-Host "Run with: .\build\MyCppProject.exe" -ForegroundColor Cyan
}
else {
    Write-Host "Build failed!" -ForegroundColor Red
    Write-Host $result -ForegroundColor Red
    exit 1
}
