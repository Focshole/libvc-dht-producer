cmake_minimum_required(VERSION 3.20)
project(
  libVersioningCompilerProducer
  VERSION 0.1
  DESCRIPTION "libVC dht code producer"
  LANGUAGES CXX C)
# ----- Set the name of the Version Compiler based library
set(EXE_NAME "libVersioningCompilerProducer")
set(LIB_NAME "VersioningCompilerProd")
# look for dependencies
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# Check that libVersioningCompiler is installed
find_package(LibVersioningCompiler REQUIRED)
include_directories("${LIBVC_INCLUDE_DIRS}" "${CMAKE_SOURCE_DIR}/include")
link_directories(${LIBVC_LIB_DIR})
# Check that gnuTLS is installed (required for importIdentity)
find_package(GnuTLS REQUIRED)
if(NOT GnuTLS_FOUND)
  message(FATAL_ERROR "GnuTLS not found")
endif(NOT GnuTLS_FOUND)
include_directories(${GNUTLS_INCLUDE_DIR})
link_directories(${GNUTLS_LIBRARIES})
set(LIBVC_PROD_DEP_LIBS ${LIBVC_PROD_DEP_LIBS} "gnutls")

# Check that opendht is installed
find_package(opendht REQUIRED)
if(NOT opendht_FOUND)
  message(FATAL_ERROR "opendht not found")
endif(NOT opendht_FOUND)
set(LIBVC_PROD_DEP_LIBS ${LIBVC_PROD_DEP_LIBS} "opendht")
# Check that zeromq is installed (custom find_package)
find_package(libzmq REQUIRED)
if(NOT LIBZMQ_FOUND)
  message(FATAL_ERROR "libzmq not found")
endif(NOT LIBZMQ_FOUND)
include_directories(${LIBZMQ_INCLUDE_DIRS})
link_directories(${LIBZMQ_LIBRARIES})
set(LIBVC_PROD_DEP_LIBS ${LIBVC_PROD_DEP_LIBS} "-lzmq")

# finalize dependencies
set(LIBVC_PROD_DEP_LIBS ${LIBVC_PROD_DEP_LIBS} ${LIBVC_LIBRARIES} "-lstdc++")

# sources
set(SRC_PROD
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/producer-args.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opendht-extra-utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/libvc.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/networking.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/producer-utils.cpp")
set(HDR_PROD
    "${CMAKE_CURRENT_SOURCE_DIR}/include/producer-args.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/libvc.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/opendht-extra-utils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/networking.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/producer-utils.hpp")
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

# configure
set(FIND_LIBVC_PROD_OUTPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/config/FindLibVersioningCompilerProd.cmake")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config/FindLibVersioningCompilerProd.in.cmake"
  "${FIND_LIBVC_PROD_OUTPUT}" @ONLY)
# finalize target
add_executable(${EXE_NAME} ${SRC_PROD} ${HDR_PROD})
target_link_libraries(${EXE_NAME} ${LIBVC_PROD_DEP_LIBS})

# install path
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif()
# Setup library to install
add_library(${LIB_NAME} STATIC ${SRC_PROD} ${HDR_PROD})
target_link_libraries(${LIB_NAME} ${LIBVC_PROD_DEP_LIBS})
install(TARGETS ${LIB_NAME} DESTINATION lib)
install(FILES ${HDR_PROD} DESTINATION include/versioningCompilerProd)
install(FILES ${FIND_LIBVC_PROD_OUTPUT} DESTINATION lib/cmake)
# Install binaries too
install(TARGETS ${EXE_NAME} DESTINATION bin)
