#ifndef LIBVC_CPP
#define LIBVC_CPP
#include "libvc.hpp"

vc::compiler_ptr_t clang = nullptr;
std::filesystem::path
generateVersion(std::filesystem::path sourceCode,
                std::vector<std::string> functionNames,
                const std::list<vc::Option> &options = {
                    vc::Option("optimization for size flag", "-O", "s")}) {
  if (clang == nullptr)

  {
    clang = vc::make_compiler<vc::SystemCompilerOptimizer>(
        "llvm/clang", std::filesystem::u8path(CLANG_EXE_NAME),
        std::filesystem::u8path(OPT_EXE_FULLPATH), std::filesystem::u8path("."),
        std::filesystem::u8path("clang-compiler.log"),
        std::filesystem::u8path(LLVM_TOOLS_BINARY_DIR),
        std::filesystem::u8path("/"));
  }
  vc::Version::Builder builder;
  for (auto fn = functionNames.begin(); fn != functionNames.end(); ++fn) {
    builder._functionName.push_back(*fn);
  }
  builder._fileName_src.push_back(sourceCode);
  builder._compiler = clang;
  builder.options(options);
  builder._autoremoveFilesEnable = false;
  builder._genIROptionList=options;
  builder._optOptionList = {vc::Option("fp-contract", "-fp-contract=", "fast"),
                            vc::Option("inline", "-inline"),
                            vc::Option("mem2reg", "-mem2reg"),
                            vc::Option("pic","--relocation-model=pic")
                            };
  vc::version_ptr_t v = builder.build();
  bool ok = v->prepareIR();
  if (!ok) {
    if (!clang->hasIRSupport()) {
      std::cerr << "Error: something went wrong with the compiler."
                << std::endl;
    } else if (!v->hasGeneratedIR()) {
      std::cerr << "Error: generation of IR of " << sourceCode.c_str()
                << " failed." << std::endl;
    } else {
      std::cerr << "Error: optimization of IR of" << sourceCode.c_str()
                << " failed." << std::endl;
    }
    std::cerr << "\tPlease check the compiler/optimizer install path in "
                 "libvc.cpp source file"
              << std::endl;
  }
  ok = v->compile();
  if (!ok) { // something during the compilation went wrong
    if (!v->hasGeneratedBin()) {
      std::cerr << "Error: compilation of " << sourceCode.c_str() << " failed."
                << std::endl;
    } else {
      std::cerr << "Error: failure in loading symbols ";
      for (auto i : functionNames) {
        std::cerr << i << " ";
      }
      std::cerr << "not loaded" << std::endl;
    }
    exit(-1);
  }
  return v->getFileName_bin();
}

std::filesystem::path generateVersion(std::filesystem::path sourceCode,
                                      std::string functionName,
                                      const std::list<vc::Option> &options) {
  std::vector<std::string> functionNames = {functionName};
  return generateVersion(sourceCode, functionNames, options);
}

std::filesystem::path generateVersion(std::filesystem::path sourceCode,
                                      std::string functionName) {

  std::vector<std::string> functionNames = {functionName};
  return generateVersion(sourceCode, functionNames,
                         {vc::Option("optimization for size flag", "-O", "s")});
}
#endif // LIBVC_CPP