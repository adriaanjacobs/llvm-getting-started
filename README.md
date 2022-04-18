# Simple LLVM "getting started" project

This project is intended as (imo) the simplest possible way to get started with modern LLVM. 
## Features
* It uses LLVM 13, but can be adapted to use any (fairly recent) version of LLVM easily by modifying [`CMakeLists.txt`](CMakeLists.txt). 
* It uses the new pass manager.
* It is out-of-tree, allowing you to quickly change between LLVM versions and builds.
* Because it is out-of-tree, it does not require you to build LLVM yourself up front, which can be a large initial hurdle for beginners.

I expect that this code can even build against LLVM 12, which is available from the default Ubuntu 20.04 repositories and thus does not require you to download any additional LLVM release at all. I haven't tested this yet, though.

## To build
You will likely have to download an official LLVM release from their GitHub. By default, this project uses LLVM 13, so for Ubuntu 20.04, you can grab that [here](https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz)

Then, simply set the LLVM_DIR cmake variable to the location of the installed LLVM's cmake/llvm folder (containing LLVMConfig.cmake) and you're good to go. Run the below common command from the (empty) folder where you wish to build this project:
```
cmake <path/to/llvm-getting-started> -DLLVM_DIR=<path/to/llvm-13.0.0>/lib/cmake/llvm/
```

## To run
The project is set up to build both a shared library containing the passes, and a "runner" executable that has the passes linked in and acts as a driver to parse the LLVM bitcode, register the necessary analyses, and run the instrumentation pass. Then, it prints the resulting module (likely changed by the pass) to a specified file.

To run on some LLVM IR, use:
```
<build>/passrunner <.ll/.bc file> <output file>
```

The `libpass.so` file can also be run more "traditionally" as a dlopen'ed library by clang/opt. I have not added the necessary boilerplate in `pass.cpp` to enable this for the new pass manager. If you have questions about that, let me know at adriaan.jacobs7@gmail.com.

## Code to run on
Since the passrunner runs on bitcode, you'll have to generate that from whatever language you're trying to analyze first. For clang, you can do this with the `-S -emit-llvm` options.

If you want to do whole-program analysis (WPA), I highly recommend [the gllvm project](https://github.com/SRI-CSL/gllvm) at the moment to generate complete LLVM bitcode for complex projects. It is a version of the more well-known [wllvm](https://github.com/travitch/whole-program-llvm) project that, among other things, sports faster compile times. An easy project to get started with is [MbedTLS](https://github.com/Mbed-TLS/mbedtls), since it builds statically by default and has some useful test programs (like [benchmark.c](https://github.com/Mbed-TLS/mbedtls/blob/development/programs/test/benchmark.c)) that are moderately sized but still simple enough for most points-to analyses.

Lastly, ideally, we could run these out-of-tree passes at link time using Clang's link-time optimization (LTO). However, this does not support the new pass manager yet.