#ifndef LIBVC_HPP
#define LIBVC_HPP

#include "versioningCompiler/CompilerImpl/SystemCompilerOptimizer.hpp"
#include "versioningCompiler/Version.hpp"
#include <filesystem>
#include <iostream>

#ifndef CLANG_EXE_NAME
#define CLANG_EXE_NAME "clang"
#endif
#ifndef OPT_EXE_FULLPATH
#define OPT_EXE_FULLPATH "/bin/opt"
#endif
#ifndef LLVM_TOOLS_BINARY_DIR
#define LLVM_TOOLS_BINARY_DIR "/usr/bin"
#endif

// build a version of the source code and return its binary path
std::filesystem::path generateVersion(std::filesystem::path sourceCode,
                                      std::vector<std::string> functionNames,
                                      const std::list<vc::Option> &options);

std::filesystem::path generateVersion(std::filesystem::path sourceCode,
                                      std::string functionName,
                                      const std::list<vc::Option> &options);

std::filesystem::path generateVersion(std::filesystem::path sourceCode,
                                      std::string functionName); 
#endif // LIBVC_HPP