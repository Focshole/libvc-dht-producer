#!/bin/bash
rm -rf build/
rm -rf cmake/FindLibVersioningCompiler.cmake
rm -rf cmake/FindLibVersioningCompiler.cmake.in
mkdir cmake
cp libVC/config/FindLibVersioningCompiler.cmake cmake/
cmake -S . -B build 
cmake --build build
