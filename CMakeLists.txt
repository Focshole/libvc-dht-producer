cmake_minimum_required(VERSION 3.20)
project(
  libVC_dht_producer
  VERSION 0.1
  DESCRIPTION "libVC dht code producer"
  LANGUAGES CXX C)
# target
set(EXE_NAME "libvc_dht_producer")
set(LIB_NAME "vc_dht_prod")
# look for dependencies
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# ----- Set the name of the Version Compiler library

find_package(LibVersioningCompiler REQUIRED)
include_directories("${LIBVC_INCLUDE_DIRS}" "${CMAKE_SOURCE_DIR}/include")
link_directories(${LIVC_LIB_DIR})

# Check that opendht is installed
find_package(GnuTLS REQUIRED)
include_directories(${GNUTLS_INCLUDE_DIR})
link_directories(${GNUTLS_LIBRARIES})
set(DEP_LIBS ${DEP_LIBS} "gnutls")
find_package(opendht REQUIRED)
if(NOT opendht_FOUND)
  message(FATAL_ERROR "opendht not found")
endif(NOT opendht_FOUND)
set(DEP_LIBS ${DEP_LIBS} "opendht")

find_package(libzmq REQUIRED)
if(NOT LIBZMQ_FOUND)
  message(FATAL_ERROR "libzmq not found")
endif(NOT LIBZMQ_FOUND)
include_directories(${LIBZMQ_INCLUDE_DIRS})
link_directories(${LIBZMQ_LIBRARIES})
set(DEP_LIBS ${DEP_LIBS} "-lzmq")

# finalize dependencies
set(DEP_LIBS ${DEP_LIBS} ${LIBVC_LIBRARIES} "-lzmq" "-lstdc++")

# sources
set(SRC_PROD
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/producer-args.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opendht-extra-utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/libvc.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/networking.cpp")
set(HDR_PROD
    "${CMAKE_CURRENT_SOURCE_DIR}/include/producer-args.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/libvc.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/opendht-extra-utils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/networking.hpp")
# compiler options
if(NOT CMAKE_BUILD_TYPE)
  message("No build type detected. Build type set to Release by default")
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
# Adjust paths for clang
add_definitions(-DLLVM_VERSION_MAJOR=${LLVM_VERSION_MAJOR})
add_definitions(-DCLANG_EXE_FULLPATH="${CLANG_EXE_FULLPATH}")
add_definitions(-DOPT_EXE_FULLPATH="${OPT_EXE_FULLPATH}")
add_definitions(-DCLANG_EXE_NAME="${CLANG_EXE_NAME}")
add_definitions(-DOPT_EXE_NAME="${OPT_EXE_NAME}")
add_definitions(-DLLVM_TOOLS_BINARY_DIR="${LLVM_TOOLS_BINARY_DIR}")
# finalize target
add_executable(${EXE_NAME} ${SRC_PROD} ${HDR_PROD})
target_link_libraries(${EXE_NAME} ${DEP_LIBS})

# install path
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif()
# Setup library to install
add_library(${LIB_NAME} STATIC ${SRC_PROD} ${HDR_PROD})
target_link_libraries(${LIB_NAME} ${DEP_LIBS})
install(TARGETS ${LIB_NAME} DESTINATION lib)
install(FILES ${HDR_PROD} DESTINATION include/libvc_dht_producer)
install(FILES ${FIND_LIBVC_OUTPUT} DESTINATION lib/cmake)
# Install binaries too
install(TARGETS ${EXE_NAME} DESTINATION bin)
