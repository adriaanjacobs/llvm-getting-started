cmake_minimum_required(VERSION 3.4.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(llvm-getting-started)

find_package(LLVM 15 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# The below part can be moved into its own subdirectory if you want (I typically do it like that)
add_library(pass SHARED pass.cpp)
target_compile_options(pass PUBLIC 
                            -Wall -Wextra -Werror=return-type -Wimplicit-fallthrough -Wno-unused-parameter -Wno-unused-variable
                            -ggdb -fPIC -fno-rtti
                            -O2
)
target_include_directories(pass PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(pass LLVM)

# an executable that sets up LLVM's pass machinery and runs our pass on a provided bitcode file
add_executable(passrunner passrunner.cpp)
target_link_libraries(passrunner pass)

# a library that can be loaded into the llvm middle-end during LTO optimization at link time
add_library(LTOpass SHARED LTOboilerplate.cpp)
target_link_libraries(LTOpass pass)
