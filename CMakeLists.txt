cmake_minimum_required(VERSION 3.4.3)
project(llvm-getting-started)

# # Add the location of LLVMConfig.cmake to CMake search paths (so that
# # find_package can locate it)
list(APPEND CMAKE_PREFIX_PATH 
            "${LLVM_DIR}/lib/cmake/llvm/" 
            # "${LLVM_DIR}/lib/cmake/clang/"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(MY_LLVM_VERSION 13)
message(STATUS "LLVM_DIR is ${LLVM_DIR}")
find_package(LLVM ${MY_LLVM_VERSION} REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
# find_package(Clang REQUIRED)
# message(STATUS "Found Clang ${Clang_PACKAGE_VERSION}")
# message(STATUS "Using ClangConfig.cmake in: ${Clang_DIR}")

# The pass includes headers from LLVM - update the include paths accordingly
# include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs Passes bitwriter core ipo irreader instcombine instrumentation target linker analysis scalaropts support transformutils)
# set(clang_libs clangAST clangBasic clangCodeGen clangLex clangParse clangSema clangAnalysis clangDriver clangFrontend clangAST clangEdit clangSerialization clangTooling clangToolingCore clangAST clangParse clangBasic clangSema clangAST)
set(COMMON_FLAGS -DMY_LLVM_VERSION=${MY_LLVM_VERSION} -Wno-comment -fno-rtti -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -ggdb -fPIC -Werror=return-type)

# The below part can be moved into its own subdirectory if you want (I typically do it like that)
add_library(pass SHARED pass.cpp)
target_compile_options(pass PRIVATE ${COMMON_FLAGS})
# target_link_libraries(pass)

add_executable(passrunner passrunner.cpp)
target_compile_options(passrunner PRIVATE ${COMMON_FLAGS})
target_link_libraries(passrunner pass ${llvm_libs})