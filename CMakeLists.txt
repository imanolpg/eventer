cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyCompilerProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)

# Print out the LLVM version
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include directories
include_directories(${LLVM_INCLUDE_DIRS} include)

# Use file(GLOB) to collect all .cpp files in src directory, excluding main.cpp
file(GLOB SRC_FILES "src/*.cpp")

# Add the main.cpp file explicitly
set(MAIN_FILE "main.cpp")

# Add executable target
add_executable(main ${MAIN_FILE} ${SRC_FILES})

# Use target_include_directories to specify include directories for this target
target_include_directories(main PRIVATE ${LLVM_INCLUDE_DIRS} include)

# Link against LLVM libraries
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(main ${llvm_libs})

# Additional options to suppress warnings from LLVM headers if necessary
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(main PRIVATE -Wno-gnu-zero-variadic-macro-arguments)
endif()
