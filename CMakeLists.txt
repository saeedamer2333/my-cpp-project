cmake_minimum_required(VERSION 3.10)
project(MyCppProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add executable with all source files
add_executable(MyCppProject
    src/main.cpp
    src/ArrayBasedCollection.cpp
    src/LinkedListBasedCollection.cpp
    src/CSVParser.cpp
    src/Transaction.cpp
    src/DataStructureComparator.cpp
)

# Set output directory
set_target_properties(MyCppProject PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
