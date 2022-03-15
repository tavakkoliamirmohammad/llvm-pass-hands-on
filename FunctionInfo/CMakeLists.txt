cmake_minimum_required(VERSION 3.12.0)
project(FunctionInfo)

set(LLVM_VERSION "" CACHE STRING "LLVM Version")
set(LLVM_VERSION_SUFFIX "")

# Obtain the build flags from `llvm-config --cxxflags` and forward them to
# variable CMAKE_CXX_FLAGS.
execute_process(COMMAND llvm-config${LLVM_VERSION_SUFFIX} --cxxflags
                OUTPUT_VARIABLE LLVM_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${LLVM_CXXFLAGS} -Wall -Werror")
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

add_subdirectory(lib)  # Build the shared library libFunctionInfo.so.

file(GLOB_RECURSE SRC_FILEs "*.h" "*.cpp")
add_custom_target(format WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                         COMMAND clang-format${LLVM_VERSION_SUFFIX} -i ${SRC_FILEs})
