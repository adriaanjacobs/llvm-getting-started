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
You will likely have to download an official LLVM release. By default, this project uses LLVM 15. For Ubuntu you can grab pre-built libraries from [LLVM's apt repositories](https://apt.llvm.org/). General instructions like so:
```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 15
```

> **Recommended:** install the clang development files if you also want to invoke or extend the Clang frontend at some point.
> ```bash
> sudo apt install libclang-15-dev
> ```

You can then simply run `cmake` from a build directory and the installed LLVM version 15 will be found. 
```bash
mkdir build
cd build
cmake ../
mv compile_commands.json ../ # optionally expose the compilation database to help an IDE/language server
make -j
```
If you manually installed LLVM to a non-standard location, you can set the `LLVM_DIR` CMake variable to the `cmake/llvm` folder of your installation (containing LLVMConfig.cmake, `find <folder> -name "LLVMConfig.cmake"`).  

## To run
The project is set up to build:
1. A shared library containing the passes/analyses.
2. A "runner" executable that has the passes linked in and acts as a driver to parse the LLVM bitcode, register the necessary analyses, and run the instrumentation pass. Then, it prints the resulting module (possibly changed by the pass) to a specified file.
3. An LTO plugin that can be loaded into clang like so: `clang-15 -flto -fuse-ld=/usr/bin/ld.lld-15 -Xlinker --load-pass-plugin=<plugin path>`. You can find the boilerplate for that in [LTOboilerplate.cpp](/LTOboilerplate.cpp). 

> **NOTE:** For LLVM plugins to work, they need to match the ABI of the compiler they are loaded into. To ensure this is the case, build the project with 
> ```bash
> cmake ../ -DCMAKE_C_COMPILER=clang-15 -DCMAKE_CXX_COMPILER=clang++-15 
> ```

To simply run on some LLVM IR, use the runner executable as follows:
```
<build>/passrunner <.ll/.bc file> <output file>
```

The `libpass.so` file can also be run more "traditionally" as a non-LTO plugin by clang/opt. I have not added the necessary boilerplate in `pass.cpp` to enable this for the new pass manager. If you have questions about that, feel free to make an issue.

## Code to run on
Since the passrunner runs on bitcode, you'll have to generate that from whatever language you're trying to analyze first. For clang, you can do this with the `-S -emit-llvm` options.

If you want to do whole-program analysis (WPA), I highly recommend [the gllvm project](https://github.com/SRI-CSL/gllvm) at the moment to generate complete LLVM bitcode for complex projects. It is a version of the more well-known [wllvm](https://github.com/travitch/whole-program-llvm) project that, among other things, sports faster compile times. An easy project to get started with is [MbedTLS](https://github.com/Mbed-TLS/mbedtls), since it builds statically by default and has some useful test programs (like [benchmark.c](https://github.com/Mbed-TLS/mbedtls/blob/development/programs/test/benchmark.c)) that are moderately sized but already contain many representative IR constructs. 

> **TIP:** Always make sure to generate whole-program IR using the same compiler and version that you will use to analyze it!

If you want to run some passes before running your analysis, you can either do that with `opt`, or you can add them to `MPM` in [passrunner.cpp](passrunner.cpp) before `MyInstrumentationPass` if you want to run them every time. This gives very precise control over the layout of the IR before you run your own stuff.

