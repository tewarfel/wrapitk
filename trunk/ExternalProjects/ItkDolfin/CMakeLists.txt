project(ItkDolfin)
include(CPack)
cmake_minimum_required(VERSION 2.4.2 FATAL_ERROR)

# TODO, for now we assume that FEniCS is installed
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
#find_package( FEniCS )
#include_directories( ${FENICS_DIR} )

# TODO this should be moved to findfenics
find_package(PkgConfig)
PKG_CHECK_MODULES(DOLFIN REQUIRED dolfin)
#link_directories(${DOLFIN_LIBRARY_DIRS})
#include_directories(${DOLFIN_INCLUDE_DIRS})
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${DOLFIN_CFLAGS})
set(LINK_FLAGS ${LINK_FLAGS} ${DOLFIN_LDFLAGS})
set(LINK_FLAGS_RELEASE ${LINK_FLAGS_RELEASE} ${DOLFIN_LDFLAGS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# wrapping
add_subdirectory(wrapping)
