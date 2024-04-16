# Simple LLVM "getting started" project

This project is intended as (imo) the simplest possible way to get started with modern LLVM. 

## Features
* It uses LLVM 15, but can (probably) be adapted to use any (fairly recent) version of LLVM easily by modifying [`CMakeLists.txt`](CMakeLists.txt). 
* It uses the new pass manager.
* It is out-of-tree, which has the following advantages:
    * You can quickly change between LLVM versions and builds.
    * It does not require you to build LLVM yourself up front, which can be a large initial hurdle for beginners.
    * You get way more precise control over the passes that run before and after your own than is available via the default extension points, without having to modify the compiler sources.

I expect that this code can build against many LLVM versions, but it is most tested on LLVM 15.

## To build
You will likely have to download an official LLVM release. By default, this project uses LLVM 15, so for Ubuntu 20.04, you can grab that from [LLVM's apt repositories](https://apt.llvm.org/). General instructions like so:
```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 15
```

For such installations in the default systems folders, you can then simply run `cmake` from a build directory and the installed LLVM version 15 will be found. 
```bash
mkdir build
cd build
cmake ../
make -j
```
If you manually installed LLVM to a non-standard version, you can set the `LLVM_DIR` CMake variable to the `cmake/llvm` folder of your installation (containing LLVMConfig.cmake, `find <folder> -name "LLVMConfig.cmake"`).  

## To run
The project is set up to build both a shared library containing the passes, and a "runner" executable that has the passes linked in and acts as a driver to parse the LLVM bitcode, register the necessary analyses, and run the instrumentation pass. Then, it prints the resulting module (likely changed by the pass) to a specified file.

To run on some LLVM IR, use:
```
<build>/passrunner <.ll/.bc file> <output file>
```

The `libpass.so` file can also be run more "traditionally" as a plugin by clang/opt. I have not added the necessary boilerplate in `pass.cpp` to enable this for the new pass manager. If you have questions about that, feel free to make an issue.

## Code to run on
Since the passrunner runs on bitcode, you'll have to generate that from whatever language you're trying to analyze first. For clang, you can do this with the `-S -emit-llvm` options.

If you want to do whole-program analysis (WPA), I highly recommend [the gllvm project](https://github.com/SRI-CSL/gllvm) at the moment to generate complete LLVM bitcode for complex projects. It is a version of the more well-known [wllvm](https://github.com/travitch/whole-program-llvm) project that, among other things, sports faster compile times. An easy project to get started with is [MbedTLS](https://github.com/Mbed-TLS/mbedtls), since it builds statically by default and has some useful test programs (like [benchmark.c](https://github.com/Mbed-TLS/mbedtls/blob/development/programs/test/benchmark.c)) that are moderately sized but already contain many representative IR constructs.

If you want to run some passes before running your analysis, you can either do that with `opt`, or you can add them to `MPM` in [passrunner.cpp](passrunner.cpp) before `MyInstrumentationPass` if you want to run them every time. This gives very precise control over the layout of the IR before you run your own stuff.

Lastly, we could run these out-of-tree passes at link time using Clang's link-time optimization (LTO). The relevant boilerplate for that is in [LTOboilerplate.cpp](/LTOboilerplate.cpp)
