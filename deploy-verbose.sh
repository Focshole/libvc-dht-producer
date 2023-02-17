#!/bin/bash
rm -rf build/
rm -rf cmake/FindLibVersioningCompiler.cmake
rm -rf cmake/FindLibVersioningCompiler.cmake.in
mkdir cmake
cp libVC/config/FindLibVersioningCompiler.cmake cmake/
cmake -D CMAKE_VERBOSE_MAKEFILE=1 -D LIBCLANG_FIND_VERBOSE=1 -D LLVM_FIND_VERBOSE=1  -S . -B build 
cmake --build build
