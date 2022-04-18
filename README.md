# Simple LLVM "getting started" project

This project is intended as (imo) the simplest possible way to get started with modern LLVM. 
## Features
* It uses LLVM 13, but can be adapted to use any (fairly recent) version of LLVM easily by modifying CMakeLists.txt. 
* It uses the new pass manager.
* It is out-of-tree, allowing you to quickly change between LLVM versions and builds.
* Because it is out-of-tree, it does not require you to build LLVM yourself up front, which can be a large initial hurdle for beginners.

## To build
You will likely have to download an official LLVM release from their GitHub. By default, this project uses LLVM 13, so for Ubuntu 20.04, you can grab that [here](https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz)

Then, simply set the LLVM_DIR cmake variable to the location of the installed LLVM's cmake/llvm folder (containing LLVMConfig.cmake) and you're good to go. Run the below common command from the (empty) folder where you wish to build this project:
```
cmake <path/to/llvm-getting-started> -DLLVM_DIR=<path/to/llvm-13.0.0>/lib/cmake/llvm/
```