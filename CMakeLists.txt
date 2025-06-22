cmake_minimum_required(VERSION 3.10)
project(MyCppProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(MyCppProject
    src/main.cpp
    src/ArrayBasedCollection.cpp
    src/LinkedListBasedCollection.cpp
    # Add more .cpp files here as needed
)