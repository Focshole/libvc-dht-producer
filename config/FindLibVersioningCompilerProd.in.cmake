set(LibVersioningCompilerProd_SOURCE_PATH @PROJECT_SOURCE_DIR@)
set(LibVersioningCompilerProd_BINARY_PATH @PROJECT_BINARY_DIR@)
set(LibVersioningCompilerProd_INSTALL_PATH @CMAKE_INSTALL_PREFIX@)
# check for headers
find_path(
  LibVersioningCompilerProd_INCLUDE_DIRS versioningCompilerProd/networking.hpp
  PATHS ${LibVersioningCompilerProd_INSTALL_PATH}/include ${LibVersioningCompilerProd_SOURCE_PATH}/include
        /usr/local/include /usr/include ${CMAKE_EXTRA_INCLUDES}
  NO_DEFAULT_PATH)

if(LibVersioningCompilerProd_INCLUDE_DIRS-NOTFOUND)
  set(LibVersioningCompilerProd_FOUND FALSE)
  if(LibVersioningCompilerProd_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find LibVersioningCompilerProd headers!")
  endif(LibVersioningCompiler_FIND_REQUIRED)
endif(LibVersioningCompilerProd_INCLUDE_DIRS-NOTFOUND)
# check for the libVersioningCompilerProd library
find_library(
  LibVersioningCompilerProd_LIBRARY VersioningCompilerProd
  PATHS ${LibVersioningCompilerProd_INSTALL_PATH}/lib ${LibVersioningCompilerProd_BINARY_PATH} /usr/local/lib /usr/lib
        /lib ${CMAKE_EXTRA_LIBRARIES}
  NO_DEFAULT_PATH)

if(LibVersioningCompilerProd_LIBRARY-NOTFOUND)
  set(LibVersioningCompilerProd_FOUND FALSE)
  if(LibVersioningCompilerProd_LIBRARY_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find LibVersioningCompilerProd library!")
  endif(LibVersioningCompilerProd_LIBRARY_FIND_REQUIRED)
endif(LibVersioningCompilerProd_LIBRARY-NOTFOUND)

get_filename_component(LibVersioningCompilerProd_LIBRARY_DIR ${LibVersioningCompilerProd_LIBRARY} PATH)

set(LibVersioningCompilerProd_LIBRARIES ${LibVersioningCompilerProd_LIBRARY} ${UUID_LIBRARY} ${CMAKE_DL_LIBS})

# Check for libVersioningCompiler
find_package(LibVersioningCompiler REQUIRED)
set(LibVersioningCompilerProd_INCLUDE_DIRS "${LibVersioningCompilerProd_INCLUDE_DIRS}" "${LIBVC_INCLUDE_DIRS}")
set(LibVersioningCompilerProd_LIBRARIES "${LibVersioningCompilerProd_LIBRARIES}" "${LIBVC_LIBRARIES}")

# Check that opendht is installed
find_package(GnuTLS REQUIRED)
if(NOT GNUTLS_FOUND)
  message(FATAL_ERROR "GnuTLS not found")
endif(NOT GNUTLS_FOUND)
set(LibVersioningCompilerProd_INCLUDE_DIRS "${LibVersioningCompilerProd_INCLUDE_DIRS}" "${GNUTLS_INCLUDE_DIR}")
set(LibVersioningCompilerProd_LIBRARIES "${LibVersioningCompilerProd_LIBRARIES}" "${GNUTLS_LIBRARIES}")
find_package(opendht REQUIRED)
if(NOT opendht_FOUND)
  message(FATAL_ERROR "opendht not found")
endif(NOT opendht_FOUND)
set(LibVersioningCompilerProd_LIBRARIES ${LibVersioningCompilerProd_LIBRARIES} "opendht")

find_package(libzmq REQUIRED)
if(NOT LIBZMQ_FOUND)
  message(FATAL_ERROR "libzmq not found")
endif(NOT LIBZMQ_FOUND)
set(LibVersioningCompilerProd_INCLUDE_DIRS "${LibVersioningCompilerProd_INCLUDE_DIRS}" "${LIBZMQ_INCLUDE_DIRS}")
set(LibVersioningCompilerProd_LIBRARIES "${LibVersioningCompilerProd_LIBRARIES}" "${LIBZMQ_LIBRARIES}")
set(LibVersioningCompilerProd_LIBRARIES ${LibVersioningCompilerProd_LIBRARIES} "-lzmq" )


get_filename_component(LibVersioningCompilerProd_LIBRARY_DIR ${LibVersioningCompilerProd_LIBRARY} PATH)


list(REMOVE_DUPLICATES LibVersioningCompilerProd_INCLUDE_DIRS)
list(REMOVE_DUPLICATES LibVersioningCompilerProd_LIBRARIES)


# set also standard cmake variable names
if(LibVersioningCompilerProd_FOUND)
  set(LibVersioningCompilerProd_INCLUDES ${LibVersioningCompilerProd_INCLUDE_DIRS})
  set(LibVersioningCompilerProd_LIBS ${LibVersioningCompilerProd_LIBRARIES})
endif(LibVersioningCompilerProd_FOUND)
