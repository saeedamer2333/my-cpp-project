# My C++ Project

This project is a simple C++ application that demonstrates the structure and organization of a C++ project using CMake.

## Project Structure

```
my-cpp-project
├── src
│   └── main.cpp          # Entry point of the application
├── include
│   └── my_cpp_project.h  # Header file for function and class declarations
├── CMakeLists.txt        # CMake configuration file
└── README.md             # Project documentation
```

## Building the Project

To build the project, follow these steps:

1. Ensure you have CMake installed on your system.
2. Open a terminal and navigate to the project directory.
3. Create a build directory:
   ```
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Build the project:
   ```
   cmake --build .
   ```

## Running the Application

After building the project, you can run the application by executing the generated binary in the build directory.
